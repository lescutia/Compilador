/********************************************************************
	file:		parser.c
	date:		2017/07/26
	author: 	Luis Escutia, Miguel Corona
	Contact:	escutialuis93@gmail.com
				fmiguelcorona@gmail.com

	Purpose:		
********************************************************************/

#include "../CompilerPCH.h"
#include "../includes/parser.h"
//classes, types y symbol tables se movieron a compilerPCH.h
// classes
//int VARIABLE  = 1;
//int PROCEDURE = 2;
//int STRING    = 3;

// types
//int INT_T      = 1;
//int INTSTAR_T  = 2;
//int VOID_T     = 3;

//Tipos  no soportados por C*
//int CHAR_T     = 4;
//int CHARSTAR_T = 5;

// symbol tables
//int GLOBAL_TABLE  = 1;
//int LOCAL_TABLE   = 2;
//int LIBRARY_TABLE = 3;

char* g_lastProc = 0;
int g_iNumberOfAssignments  = 0;
int g_iNumberOfWhile        = 0;
int g_iNumberOfIf           = 0;
int g_iNumberOfReturn       = 0;
int g_iNumberOfCalls        = 0;

int g_iAllocatedTemporaries = 0;

int g_iReturnTypeProcedure  = 0;

// return type of currently parsed procedure
int g_iReturnType = 0;

// La utiliza la función fnIsDeclared
int g_iTypeCurrIdentifier = 0;

// TODO: MEJORAR COMENTARIO...
/* Se usa para saber si una función de tipo
 * no void no regresa ningún valor.
 */
int g_bHasReturn = 0;

/* Se usa para saber cuando una variable se considera como
 * un parámetro.
 */
int g_bSearchParameters = 0;

// Label actual
int g_iLabel = 0;

// Etiquetas para el if
int g_iL1If = 0;
int g_iL2If = 0;

// Etiquetas para el while
int g_iL1While = 0;
int g_iL2While = 0;

// Para saber en fnDebugCodeGen se va a imprimir una etiqueta
static int NO_LABEL = -1;

// TODO: MEJORAR
// Para no eviarle entry a fnVariable, desde fnProcedure
sEntry* g_entry;

/******* MOVER AL LUGAR APROPIADO *******/
static int DEBUG_PARSER  = 0;
static int DEBUG_CODEGEN = 1;

void fnDebugParser( char* strMessage )
{
	if( DEBUG_PARSER )
		printf( "%s ", strMessage );
}

void fnDebugCodeGen( char* strInstr, char* strArg, int iLabel )
{
	// Mandamos a escribir en el archivo txt para código intermedio.
	fnPCode( strInstr, strArg, iLabel );

	if( DEBUG_CODEGEN )
	{
		printf( " %s",strInstr );
		printf( " " );
		printf( "%s", strArg );			

		if( iLabel != NO_LABEL )
			printf( "%d", iLabel );

		printf( "\n" );
	}
}
/****************************************/

// Función para generar las etiquetas para los saltos
int fnNewLabel( ) { g_iLabel++; }

void fnParser( )
{
	int iType;
	int iCurrentLineNumber;
	int iInitialValue;
	int bIsDefined;
	//char* variableOrProcedureName;
	/*Si lo dejamos como apuntadora a cadena, cuando hacemos fnGetSymbol()
	 *el valor de variableOrProcedureName cambia, ya que fnGetSymbol()
	 *cambia el valor de la cadena a la cual está apuntado
	 *variableOrProcedureName
	*/
	char variableOrProcedureName[ MaxStringLength ];
	sEntry* entry;

	bIsDefined = 0;

	/*Mietras no se llegue al fin de archivo (EOF) analizamos el
	 *flujo de tokens, que nos entrega el scanner.
	 */
	while( g_symbol != SYM_EOF )
	{
		/*Se recorre la cadena de tokens hasta encontar un tipo
		 *de dato (int, char o void) O EOF (fin de archivo)
		 *teniendo en cuenta que las variables globales o funciones
		 *siempre están asociadas a un tipo de dato. En caso de
		 *encontrar tokens que no correspondan a tipos de datos se
		 *imprime un mensaje de error y se continua la busqueda.
		 */
		 //REVISAR SI SE TIENE QUE SALIR DEL PROGRAMA CON LA CADENA VACÍA
		 //fnFindNextDataType(); 
		 //Encontrar el siguiente tipo de dato o '#'
		fnFindNextDataTypeOrHash( );

		/*Si encontramos un tipo de dato se analiza si se tiene:
		 *  type identifier (';' | '=' literal | procedure )
		 *donde,
		 *       literal ::= integer | character
		 */

		 /*Si el tipo de dato es void, es decir
		  *void ...
		  *identifier
		  */
		if( g_symbol == SYM_VOID )
		{
			iType = VOID_T;

			fnDebugParser( "void" );
			fnGetSymbol( );
			/*void identifier ...
			 *procedure declaration o definition
			 */
			if( g_symbol == SYM_IDENTIFIER )
			{
				//variableOrProcedureName = g_identifier;
				strcpy( variableOrProcedureName, g_identifier );
				fnDebugParser( "id" );

				g_lastProc = variableOrProcedureName;

				/*Antes de llamar a fnProcedure() se obtiene
				 *el siguiente token.
				 */
				fnGetSymbol( );
				fnProcedure( variableOrProcedureName, iType );
			}
			else
			{
				fnSyntaxErrorSymbol( SYM_IDENTIFIER );
				/*Una razón para no obtener el siguiente token, es que el
				 *actual puede ser un tipo de dato,y con ese token se
				 *comenzaria el análisis (test5.txt).
				 */
				 //fnGetSymbol();
			}
		}
		/* '#' ...
		 */
		else if( g_symbol == SYM_OCTOTHORPE )
		{
			fnDebugParser( "#" );

			/*En fnInclude se comienza a analizar desde el token
			 *que obtuvimos anteriormente.
			 */
			fnInclude( );
		}
		/*(int | char) ['*'] ...
		 */
		else
		{
			/*Se obtiene el siguiente token.
			 *
			 *No nos interesa el tipo si no consideramos la tabla
			 *de símbolos.
			*/
			//fnGetSymbol();

			/*Mandamos llamar a fnType() para saber que tipo de dato
			 *estamos analizando.
			 *
			 * fnType ya obtiene el siguiente simbolo internamente, por
			 * lo que se omite fnGetSymbol().
			 */
			iType = fnType( );

			/*type identifier ...
			 */
			if( g_symbol == SYM_IDENTIFIER )
			{
				/*Se guarda el valor de g_identifier para no perderlo
				 *cuando se llama a fnGetSymbol(), si encuentra otro
				 *identificador...
				 */
				 //variableOrProcedureName = g_identifier;
				strcpy( variableOrProcedureName, g_identifier );

				//printf("\n g_identifier: %s\n", variableOrProcedureName);

				fnDebugParser( "id" );
				fnGetSymbol( );

				/*type identifier '(' ...
				 *Se ve si es una declaración de una función o definición
				 */
				if( g_symbol == SYM_LPARENTHESIS )
				{

					g_lastProc = variableOrProcedureName;

					/*procedure declaration o definition
					 *En fnProcedure se comienza a analizar desde el token
					 *que obtuvimos anteriormente.
					 */
					fnProcedure( variableOrProcedureName, iType );
				}
				/*type identifier ...
				 *Se ve si es una declaración de una variable global o una
				 *definición global (type identifier '=' literal;).
				 */
				else
				{
					iCurrentLineNumber = g_lineNumber;
					g_lastProc = 0;

					/*type identifier ';'
					 *declaración de una variable global.
					 */
					if( g_symbol == SYM_SEMICOLON )
					{
						bIsDefined = 0; // No se ha definido la variable

						fnDebugParser( ";" );
						fnGetSymbol( );

						//no debería inicializarse initialValue en 0
						//automaticamente.
						//¿POR QUÉ?

						/*Si encuentra ';', entonces ha visto:
						 *              type indetifier ';'
						 *e "interpreta" que ha visto una variable globlal
						 *que no ha sido inicializada.
						 */
						iInitialValue = 0;
					}
					/*type identifier '=' ...
					 *definicíón de una variable global.
					 */
					else
					{
						bIsDefined = 1;

						//MODIFICAR fnInitialization( )
						// TODO: CONTINUAR
						iInitialValue = fnInitialization( iType );
					}

					/*Busca si el símbolo almacenado en variableOrProcedureName
					 *de la clase VARIABLE, está en la tabla de símbolos global.
					 */
					entry = fnSearchSymbolTable( global_symbol_table, variableOrProcedureName, VARIABLE, 0 );

					//Si no está se crea una entrada en la tabla de símbolos.
					if( entry == 0 )
					{
						//allocatedMemory = allocatedMemory + WORDSIZE
						//TODO: cambiar -1 por -allocatedMemory

						/*Crea una entrada en la tabla de símbolos global
						 *(GLOBAL_TABLE) para el símbolo (string) que está almacenado
						 *en variableOrProcedureName, en la línea (line)
						 *currentLineNumber, de clase (class) VARIABLE, de
						 *tipo type, con valor (value) initialValue, dirección
						 *(address) -1 y el valor 1 para indicar que está definido
						 *(defined)
						 */
						fnCreateSymbolTableEntry( GLOBAL_TABLE, variableOrProcedureName, iCurrentLineNumber, VARIABLE, iType, iInitialValue, -1, bIsDefined, 0 );
					}
					/*La variable global ya está declarada o definida;
					 *imprimimos un mensaje indicando esa situación.
					 */
					else
					{
						printf( "\n Warning, line %d, ", iCurrentLineNumber );
						printf( "redefinition of global variable " );
						printf( "'%s' ", variableOrProcedureName );
						printf( "ignored.\n" );
					}
				}
			}
			/*No es una declaración válida.
			 */
			else
			{
				fnSyntaxErrorSymbol( SYM_IDENTIFIER );
				/*No es necesario obtener el siguiente token, ya que se
				 *obtiene cuando se está buscando un tipo de dato, con la
				 *función fnFindNextDataType().
				 *
				 *Una razón para no obtener el siguiente token, es que el
				 *actual puede ser un tipo de dato,y con ese token se
				 *comenzaria el análisis (test6.txt).
				 */
				 //fnGetSymbol();
			}
		}
	}
}

int fnType( )
{
	int iType;

	/* int ...
	 */
	if( g_symbol == SYM_INT )
	{
		iType = INT_T;

		fnDebugParser( "int" );
		fnGetSymbol( );

		/* int* ...
		 */
		if( g_symbol == SYM_ASTERISK )
		{
			iType = INTSTAR_T;

			fnDebugParser( "*" );
			fnGetSymbol( );
		}
	}
	/* char ...
	 */
	else if( g_symbol == SYM_CHAR )
	{
		iType = CHAR_T;

		fnDebugParser( "char" );
		fnGetSymbol( );

		/* char*
		 */
		if( g_symbol == SYM_ASTERISK )
		{
			iType = CHARSTAR_T;

			fnDebugParser( "*" );
			fnGetSymbol( );
		}
	}
	/*Tipo de dato desconocido
	*/
	else
	{
		printf( "\n Error: line %d, ", g_lineNumber );
		printf( "'%s' or '%s' expected but ", *( g_SYMBOLS + SYM_INT ), *( g_SYMBOLS + SYM_CHAR ) );

		if( g_symbol == SYM_EOF )
			printf( "EOF" );
		else
			printf( "'%s' found.", *( g_SYMBOLS + g_symbol ) );

		printf( "\n\n Press any key to exit..." );
		_getch( );

		exit( 1 );
	}

	return iType;
}

char* fnGetInstPCode( int iOp )
{
	if ( iOp == SYM_PLUS )
		return "adi";
	
	if ( iOp == SYM_MINUS )
		return "sbi";
	
	if ( iOp == SYM_ASTERISK )
		return "mpi";
	
	if ( iOp == SYM_DIV )
		return "dvi";
	
	if ( iOp == SYM_MOD )
		return "mod";
	
	if ( iOp == SYM_LT )
		return "les";
	
	if ( iOp == SYM_LEQ )
		return "leq";
	
	if ( iOp == SYM_EQUALITY )
		return "equ";
	
	if ( iOp == SYM_GT )
		return "grt";
	
	if ( iOp == SYM_GEQ )
		return "geq";
	
	if ( iOp == SYM_NOTEQ )
		return "neq";

	return "unknown";
}

int fnExpression( )
{
	int iLType; // Tipo del lado izquierdo de la expresión
	int iRType; // Tipo del lado derecho de la expresión
	// CODEGEN
	int iOperatorSymbol;
	//
	/* simplexpr
	 */
	iLType = fnSimpleExpression( );

	/* simplexpr [comp] ...
	 */
	if( fnIsComparison( ) )
	{
		// CODEGEN
		iOperatorSymbol = g_symbol;
		//
		fnGetSymbol( );
		
		/* simplexpr [comp simpexpr] ...
		 */
		iRType = fnSimpleExpression( );
		// CODEGEN
		fnDebugCodeGen( fnGetInstPCode( iOperatorSymbol ), "", NO_LABEL );
		//

		if( iLType != iRType )
			fnTypeWarning( iLType, iRType );
	}

	return iLType;
}

// TODO: COMPLETAR ALGUNOS DETALLES
int fnSimpleExpression( )
{
	// No tomamos en cuenta el signo
	int iLType;
	int iRType;
	int bSign;
	int iOperatorSymbol;

	bSign = 0;

	/*Signo opcional: '-' ...
	 */
	if( g_symbol == SYM_MINUS )
	{
		bSign = 1;
		fnGetSymbol( );
	}
	/*else
		bSign = 0*/

		/* ['-'] term
		 */
	iLType = fnTerm( );

	if( bSign )
	{
		if( iLType != INT_T )
		{
			// TODO: MEJORAR EL MENSAJE
			fnTypeWarning( INT_T, iLType );

			// TODO: VER POSIBLES PROBLEMAS DE
			// DARLE INT_T a iLTipe POR DEFAULT.
			// CONSIDERAR SUMA DE CARACTERES
			iLType = INT_T;
		}
		else
			// CODEGEN
			fnDebugCodeGen( "ngi", "", NO_LABEL );
			//
	}

	/* ['-'] term ('+' | '-') ...
	 */
	while( fnIsPlusOrMinus( ) )
	{
		iOperatorSymbol = g_symbol;

		fnGetSymbol( );

		/* '-' term ('+' | '-') term ...
		 */
		iRType = fnTerm( );

		// CODEGEN
		fnDebugCodeGen( fnGetInstPCode( iOperatorSymbol ), "", NO_LABEL );
		//
		if( iOperatorSymbol == SYM_PLUS )
		{
			// if ( iLType == INTSTAR_T )
			// {
			//     // Aritmética de punteros
			//     // if ( iRType == INT_T )
			// }
			// else if ( iRType == INTSTAR_T )
			/*if ( iRType == INTSTAR_T )*/
			if( iLType != iRType )
				// fnTypeWarning( iLType, iRType );
				// TODO: CONSIDERAR DIFERENTES CASOS
				printf( "\n Warning: line %d, incompatible types.\n", g_lineNumber );
		}
		else if( iOperatorSymbol == SYM_MINUS )
		{
			if( iLType != iRType )
				// fnTypeWarning( iLType, iRType );
				// TODO: MEJORAR
				printf( "\n Warning: line %d, incompatible types.\n", g_lineNumber );
		}
	}

	return iLType;
}

int fnTerm( )
{
	int iLType;
	int iRType;
	// CODEGEN
	int iOperatorSymbol;
	//
	/* factor
	 */
	iLType = fnFactor( );

	/* factor ('*' | '\' | %)
	 */
	while( fnIsAsteriskOrDivOrMod( ) )
	{
		// CODEGEN
		iOperatorSymbol = g_symbol;
		//
		fnGetSymbol( );

		/* factor ('*' | '\' | %) factor
		 */
		iRType = fnFactor( );

		// CODEGEN
		fnDebugCodeGen( fnGetInstPCode( iOperatorSymbol ), "", NO_LABEL );
		//
		if( iLType != iRType )
			//fnTypeWarning( iLType, iRType );
			// TODO: MEJORAR
			printf( "\n Warning: line %d, incompatible types.\n", g_lineNumber );
	}

	return iLType;
}

int fnIsFactor( )
{
	if( g_symbol == SYM_LPARENTHESIS )
		return 1;

	if( g_symbol == SYM_ASTERISK )
		return 1;

	if( g_symbol == SYM_IDENTIFIER )
		return 1;

	if( g_symbol == SYM_INTEGER )
		return 1;

	if( g_symbol == SYM_CHARACTER )
		return 1;

	if( g_symbol == SYM_STRING )
		return 1;

	return 0;
}

int fnFindNextFactor( )
{
	while( !fnIsFactor( ) )
	{
		/*Se imprime un error informando que no se encontro un
		 *factor, sino un símbolo inesperado.
		 */
		fnSyntaxErrorUnexpected( );

		/*Si llegamos al fin de archivo (EOF) salimos del programa
		 */
		if( g_symbol == SYM_EOF )
			//exit( 1 );
			return 0;
		/*En caso contrario, pedimos el sigunete token
		 */
		else
			fnGetSymbol( );
	}

	return 1;
}

int fnIsDeclared( char* id )
{
	sEntry* entry;

	entry = fnGetScopedSymbolTableEntry( id, VARIABLE, g_lastProc );

	if( entry == 0 ) // entry == NULL
		return 0;

	g_iTypeCurrIdentifier = fnGetType( entry );

	return 1;
}

int fnFactor( )
{
	int bHasCast;
	// Lo usa para asignar el tipo de dato del cast
	// debido a que el cast es opcional y se puede
	// cambiar el valor de iType.
	int iCast;
	int iType;
	// char* variableOrProcedureName;
	// Para evitar problemas
	char variableOrProcedureName[ 64 ];

	bHasCast = 0;

	fnFindNextFactor( );

	// Cast opcional
	/*  '('
	 */
	if( g_symbol == SYM_LPARENTHESIS )
	{
		fnDebugParser( "(" );
		fnGetSymbol( );

		/*Es un cast:
		 * '(' (int | char) ...
		 */
		if( fnIsIntOrChar( ) )
		{
			/* '(' type ...
			 *
			 *donde,
			 *       void ::= int | int* | char | char*
			 */
			bHasCast = 1;
			iCast = fnType( );

			/* '(' type ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
		}
		/* No es un cast
		 */
		else
		{
			/* '(' expression ...
			 */
			iType = fnExpression( );

			/* '(' expression ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );

			// Para evitar problemas con '(' expression ')', por ejemplo
			// (5 + 6) 7
			return iType;
		}
	}

	/* Es una dereferencia:
	 * '*' ...
	 */
	if( g_symbol == SYM_ASTERISK )
	{
		fnDebugParser( "*" );
		fnGetSymbol( );

		/* '*' identifier ...
		 */
		if( g_symbol == SYM_IDENTIFIER )
		{
			fnDebugParser( "id" );

			// TODO: REVISAR (MEJORAR) Y HACER UNA FUNCIÓN QUE LO HAGA
			/************************************************/
			/* Si el identificador está definido se guarda su
			 * tipo en la variable global g_iTypeCurrIdentifier.
			 */
			if( fnIsDeclared ( g_identifier ) )
				iType = g_iTypeCurrIdentifier;
			/* Si el identificador no se ha definido, entonces
			 * imprimimos un mensaje de erro y salimos del
			 * compilador.
			 */
			else
			{
				printf( "\n Error: line %d, ", g_lineNumber );
				printf( "'%s' undeclared.", g_identifier );
				
				printf( "\n\n Press any key to exit..." );
				_getch( );

				/* TODO: CREAR UNA FUNCIÓN fnExitCompiler() EN
				 * LA CUAL LIBEREMOS TODA LA MEMORIA QUE HEMOS
				 * SOLICITADO.
				 */
				exit( 1 );
			}
			/************************************************/

			fnGetSymbol( );
		}
		/* '*' '(' ...
		 */
		else if( g_symbol == SYM_LPARENTHESIS )
		{
			fnDebugParser( "(" );
			fnGetSymbol( );

			/* '*' '(' expression ...
			 */
			iType = fnExpression( );

			/* '*' '(' expression ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
		}
		else
			fnSyntaxErrorUnexpected( );

		// TODO: COMPROBAR
		/************* COMPROBAR EL TIPO ***********/
		if( iType == INTSTAR_T )
			iType = INT_T;
		else if( iType == CHARSTAR_T )
			iType = CHAR_T;
		else
		{
			printf( "\n Warning: line %d, ", g_lineNumber );
			printf( "type mismatch, a pointer data type " );
			// TODO: REVISAR
			printf( "expected but '%s' found.\n", *( g_SYMBOLS + g_symbol ) );
			// TODO: VER SI ES NECESARIO SALIR DEL PROGRAMA O CONTINUAR
		}
		/*******************************************/
	}
	/* identifier ...
	 */
	else if( g_symbol == SYM_IDENTIFIER )
	{
		// variableOrProcedureName = g_identifier;
		strcpy( variableOrProcedureName, g_identifier );

		fnDebugParser( "id" );
		fnGetSymbol( );

		/* identifier '(' ...
		 */
		if( g_symbol == SYM_LPARENTHESIS )
		{
			fnDebugParser( "(" );
			fnGetSymbol( );

			/* identifier '(' call ...
			 * identifier '(' [expression ',' ...] ')' ...
			 */
			iType = fnCall( variableOrProcedureName );
		}
		else
		{
			// TODO: VER SI ES EL LUGAR CORRECTO
			// CODEGEN
			fnDebugCodeGen( "lod", g_identifier, NO_LABEL );
			//
			// TODO: REVISAR (MEJORAR) Y HACER UNA FUNCIÓN QUE LO HAGA
			/************************************************/
			/* Si el identificador está definido se guarda su
			 * tipo en la variable global g_iTypeCurrIdentifier.
			 */
			if( fnIsDeclared( g_identifier ) )
				iType = g_iTypeCurrIdentifier;
			/* Si el identificador no se ha definido, entonces
			 * imprimimos un mensaje de erro y salimos del
			 * compilador.
			 */
			else
			{
				printf( "\n Error: line %d, ", g_lineNumber );
				printf( "'%s' undeclared.", g_identifier );
				
				printf( "\n\n Press any key to exit..." );
				_getch( );

				/* TODO: CREAR UNA FUNCIÓN fnExitCompiler() EN
				 * LA CUAL LIBEREMOS TODA LA MEMORIA QUE HEMOS
				 * SOLICITADO.
				 */
				exit( 1 );
			}
			/************************************************/
		}
	}
	/* integer ...
	 */
	else if( g_symbol == SYM_INTEGER )
	{
		fnDebugParser( "integer" );
		fnGetSymbol( );

		// TODO: REVISAR SI ESTÁ EN EL LUGAR CORRECTO
		// CODEGEN
		fnDebugCodeGen( "ldc", g_integer, NO_LABEL );
		//
		iType = INT_T;
	}
	/* character ...
	 */
	else if( g_symbol == SYM_CHARACTER )
	{
		fnDebugParser( "character" );
		fnGetSymbol( );

		iType = CHAR_T;
	}
	/* string ...
	 */
	else if( g_symbol == SYM_STRING )
	{
		fnDebugParser( "string" );
		fnGetSymbol( );

		iType = CHARSTAR_T;
	}
	/* '(' ...
	 */
	else if( g_symbol == SYM_LPARENTHESIS )
	{
		fnDebugParser( "(" );
		fnGetSymbol( );

		/* '(' expression ...
		 */
		iType = fnExpression( );

		/* '(' expression ')' ...
		 */
		if( g_symbol == SYM_RPARENTHESIS )
		{
			fnDebugParser( ")" );
			fnGetSymbol( );
		}
		else
			fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
	}
	/*Factor desconocido, imprimir que se ha encontrado un error.
	 */
	else
		fnSyntaxErrorUnexpected( );

	if( bHasCast )
		return iCast;

	return iType;
}

void fnVariable( )
{
	int iType;
	sEntry* entry;

	/* type ...
	 *
	 *donde,
	 *       type ::= int | int*| char | char*
	 */
	iType = fnType( );

	/*type identifier ...
	 */
	if( g_symbol == SYM_IDENTIFIER )
	{
		fnDebugParser( "id" );

		// SÓLO EN LA TABLA LOCAL
		entry = fnSearchSymbolTable( local_symbol_table, g_identifier, VARIABLE, g_lastProc );
		
		if( entry == 0 )
		{
			fnCreateSymbolTableEntry( LOCAL_TABLE, g_identifier, g_lineNumber, VARIABLE, iType, 0, -1, 0, g_lastProc );

			if ( g_bSearchParameters )
			{				
				entry = fnSearchSymbolTable( global_symbol_table, g_lastProc, PROCEDURE, 0 );
				fnAddParameter( entry, g_identifier, iType );

				// Se busca el parámetro g_identifier en la tabla local y se establece como definido.
				entry = fnSearchSymbolTable( local_symbol_table, g_identifier, VARIABLE, g_lastProc );
				fnSetDefined( entry, 1 );
			}
		}
		// TODO: redefinition variable
		/* Se comentó porque detecta los parámetros de un prototipo
		 * de función y los mismos parámteros en la definición de la
		 * función, y los detecta como redefinidos.
		 */
		 /*else // TODO: REVISAR SI ES NECESARIO
		 {
			 printf("\n Warning, line %d, ", g_lineNumber);
			 printf("redefinition of local variable ");
			 printf("'%s' ", g_identifier);
			 printf("ignored.\n");
		 }*/

		fnGetSymbol( );
	}
	else // TODO: TRATAR EL CASO CUANDO NO ES UN IDENTIFICADOR
	{
		fnSyntaxErrorSymbol( SYM_IDENTIFIER );
		//createSymbolTableEntry( LOCAL_TABLE, "missing variable name", lineNumber, VARIABLE, type, 0, offset, 0 );

		// TODO: VER SI ES NECESARIO CREAR ESTA ENTRADA EN LA TABLA
		//fnCreateSymbolTableEntry( LOCAL_TABLE, "missing variable name", g_lineNumber, VARIABLE, type, 0, -1, 0 );
	}
}

// HASTA EL MOMENTO SÓLO SE CONSIDERAN LOS TIPOS INT Y CHAR
int fnInitialization( int iType )
{
	int iAssignedType;
	int initialValue;
	int hasCast;
	int cast;
	int sign;

	initialValue = 0; //Quitar
	hasCast = 0;
	sign = 0;

	/* identifier '=' ...
	 */
	if( g_symbol == SYM_ASSIGN )
	{
		// CODEGEN
		fnDebugCodeGen( "lda", g_identifier, NO_LABEL );
		//
		fnDebugParser( "=" );
		fnGetSymbol( );

		// optional cast: [ cast ]
		/*identifier '=' '(' ...
		 */
		if( g_symbol == SYM_LPARENTHESIS )
		{
			hasCast = 1;

			fnDebugParser( "(" );
			fnGetSymbol( );

			/*identifier '=' '(' type ...
			 */
			cast = fnType( );
			// iType = fnType(); // No estamos usando el cast

			/*identifier '=' '(' type ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
		}

		// optional: -
		/*identifier '=' ['(' type ')] '-' ...
		 */
		if( g_symbol == SYM_MINUS )
		{
			sign = 1;

			// TODO: HACER ALGO CON ESTO

			fnDebugParser( "-" );
			fnGetSymbol( );
		}
		//else
		//    sign = 0; // ya se había inicializado con 0

		/* identifier '=' ['(' type ')] ['-'] literal ...
		 */
		if( fnIsLiteral( ) )
		{
			// Por el momento no es de nuestro interés
			// initialValue
			initialValue = g_literal[ 0 ];

			if( g_symbol == SYM_INTEGER )
			{
				// CODEGEN
				fnDebugCodeGen( "ldc", g_integer, NO_LABEL );
				//
				iAssignedType = INT_T;
			}
			else if( g_symbol == SYM_CHARACTER )
			{
				// CODEGEN
				fnDebugCodeGen( "ldc", g_literal, NO_LABEL );
				//
				iAssignedType = CHAR_T;
			}

			if ( sign )
			{
				initialValue = -initialValue;
				// CODEGEN
				fnDebugCodeGen("ngi", "", NO_LABEL);
				//
			}
			// CODEGEN
			fnDebugCodeGen("stn ", "", NO_LABEL);
			//
			fnGetSymbol();
		}
		else
			fnSyntaxErrorUnexpected( );

		/*identifier '=' ['(' type ')] ['-'] literal ';' ...
		 */
		if( g_symbol == SYM_SEMICOLON )
		{
			fnDebugParser( ";" );
			fnGetSymbol( );
		}
		else
			fnSyntaxErrorSymbol( SYM_SEMICOLON );
	}
	else
		fnSyntaxErrorSymbol( SYM_ASSIGN );

	// TODO: REVISAR SI NO CAUSA PROBLEMAS NO INICIALIZAR
	// iAssignedType CUANDO SE LLEGA HASTA ESTE PUNTO
	// MEDIANTE "PUROS ERRORES".
	if( hasCast )
	{
		if( iType != cast )
			fnTypeWarning( iType, cast );
	}
	else if( iType != iAssignedType )
		fnTypeWarning( iType, iAssignedType );

	//TODO: checar que el cast corresponde al tipo correcto
	return initialValue;
}

int fnIsStatement( )
{
	if( g_symbol == SYM_ASTERISK )
		return 1;

	if( g_symbol == SYM_IDENTIFIER )
		return 1;

	if( g_symbol == SYM_WHILE )
		return 1;

	if( g_symbol == SYM_IF )
		return 1;

	if( g_symbol == SYM_RETURN )
		return 1;

	return 0;
}

int fnFindNextStatement( )
{
	while( !fnIsStatement( ) )
	{
		/*Se imprime un error informando que no se encontro un
		 *statement, sino un símbolo inesperado.
		 */
		fnSyntaxErrorUnexpected( );

		/*Si llegamos al fin de archivo (EOF) salimos del programa
		 */
		if( g_symbol == SYM_EOF )
			//exit( 1 );
			return 0;
		/*En caso contrario, pedimos el sigunete token
		 */
		else
			fnGetSymbol( );
	}

	return 1;
}

// TODO: MOVER
int fnIsPointer( int  iType )
{
	if( iType == INTSTAR_T )
		return 1;
	else if( iType == CHARSTAR_T )
		return 1;

	return 0;
}

void fnStatement( )
{
	int iLType;
	int iRType;
	// char* variableOrProcedureName;
	char variableOrProcedureName[ 64 ];
	sEntry* entry;

	/*Se recorre la cadena de tokens hasta encontar un statement
	 *('*' identifier, while, if o return) O EOF (fin de archivo).
	 *En caso de encontrar tokens que no correspondan a statements
	 *se imprime un mensaje de error y se continua la busqueda.
	 */
	fnFindNextStatement( );

	/* '*'
	 */
	if( g_symbol == SYM_ASTERISK )
	{
		fnDebugParser( "*" );
		fnGetSymbol( );

		/* '*' identifier
		 */
		if( g_symbol == SYM_IDENTIFIER )
		{
			// TODO: REVISAR (MEJORAR) Y HACER UNA FUNCIÓN QUE LO HAGA
			/************************************************/
			/* Si el identificador está definido se guarda su
			 * tipo en la variable global g_iTypeCurrIdentifier.
			 */
			if( fnIsDeclared ( g_identifier ) )
				iLType = g_iTypeCurrIdentifier;
			/* Si el identificador no se ha definido, entonces
			 * imprimimos un mensaje de erro y salimos del
			 * compilador.
			 */
			else // Mensaje de error y salir del programa
			{
				printf( "\n Error: line %d, ", g_lineNumber );
				printf( "'%s' undeclared.", g_identifier );
				
				printf( "\n\n Press any key to exit..." );
				_getch( );

				/* TODO: CREAR UNA FUNCIÓN fnExitCompiler() EN
				 * LA CUAL LIBEREMOS TODA LA MEMORIA QUE HEMOS
				 * SOLICITADO.
				 */
				exit( 1 );
			}
			/************************************************/

			// TODO: CHECAR SI ES CORRECTO
			// Por el '*' el identificador tiene que ser
			// un puntero a un tipo de dato.
			if( !fnIsPointer( iLType ) )
			{
				printf( "\n Se esperaba un puntero a tipo de dato " );
				printf( "pero se encontro " );
				fnPrintType( iLType );
				printf( ".\n" );

				// if ( iLType != INTSTAR_T )
				//     fnTypeWarning( INTSTAR_T, iLType );
				// else if ( iLType != CHARSTAR_T )
				//     fnTypeWarning( CHARSTAR_T, iLType );
			}

			// TODO: VER SI ES NECESARIO IMPRIMIR ANTES
			fnDebugParser( "id" );
			fnGetSymbol( );

			/* '*' identifier '='
			 */
			if( g_symbol == SYM_ASSIGN )
			{
				fnDebugParser( "=" );
				fnGetSymbol( );

				/* '*' identifier '=' expression
				 */
				iRType = fnExpression( );

				// TODO: NO SE CONSIDERAN STRINGS
				// Se le tiene que asignar el tipo al que
				// apunta.
				if( iLType == INTSTAR_T )
				{
					if( iRType != INT_T )
						fnTypeWarning( INT_T, iRType );
				}
				else if( iLType == CHARSTAR_T )
				{
					// TODO: CONSIDERAR EL CASO PARA EL
					// CUAL SE ASIGNAN STRINGS.
					// VER SI NO HAY ERRORES AL HACER ESTO.
					if( iRType != CHAR_T )
						fnTypeWarning( CHAR_T, iRType );
				}

				g_iNumberOfAssignments = g_iNumberOfAssignments + 1;
			}
			else
				fnSyntaxErrorSymbol( SYM_ASSIGN );

			/* '*' identifier ['=' expression] ';'
			 */
			if( g_symbol == SYM_SEMICOLON )
			{
				fnDebugParser( ";" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_SEMICOLON );
		}
		/* '*' '(' ...
		 */
		else if( g_symbol == SYM_LPARENTHESIS )
		{
			fnDebugParser( "(" );
			fnGetSymbol( );

			/* '*' '(' expression ...
			 */
			iLType = fnExpression( );

			if( !fnIsPointer( iLType ) )
			{
				// TODO: PROBAR Y VER SI IMPRIME MÁS DE LOS NECESARIO
				// CREO QUE SE DEBERÍA IMPRIMIR:
				printf( "\n Se esperaba un puntero a tipo de dato " );
				printf( "pero se encontro " );
				fnPrintType( iLType );
				printf( ".\n" );
				// if ( iLType != INTSTAR_T )
				//     fnTypeWarning( INTSTAR_T, iLType );
				// else if ( iLType != CHARSTAR_T )
				//     fnTypeWarning( CHARSTAR_T, iLType );
			}

			/* '*' '(' expression ')' ...
			*/
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );

				/* '*' '(' expression ')' '='
				 */
				if( g_symbol == SYM_ASSIGN )
				{
					fnDebugParser( "=" );
					fnGetSymbol( );

					/* '*' '(' expression ')' '=' expression
					 */
					iRType = fnExpression( );

					if( iLType == INTSTAR_T )
					{
						if( iRType != INT_T )
							fnTypeWarning( INT_T, iRType );
					}
					else if( iLType == CHARSTAR_T )
					{
						// TODO: CONSIDERAR EL CASO PAR EN EL
						// CUAL SE ASIGNAN STRINGS.
						// VER SI NO HAY ERRORES AL HACER ESTO.
						if( iRType != CHAR_T )
							fnTypeWarning( CHAR_T, iRType );
					}

					g_iNumberOfAssignments = g_iNumberOfAssignments + 1;
				}
				else
					fnSyntaxErrorSymbol( SYM_ASSIGN );

				/* '*' '(' expression ')' ['=' expression] ';'
				 */
				if( g_symbol == SYM_SEMICOLON )
				{
					fnDebugParser( ";" );
					fnGetSymbol( );
				}
				else
					fnSyntaxErrorSymbol( SYM_SEMICOLON );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
		}
		else
			fnSyntaxErrorSymbol( SYM_LPARENTHESIS );
	}
	/* identifier
	 */
	else if( g_symbol == SYM_IDENTIFIER )
	{
		strcpy( variableOrProcedureName, g_identifier );

		fnDebugParser( "id" );
		fnGetSymbol( );

		/* identifier '('
		 */
		if( g_symbol == SYM_LPARENTHESIS )
		{
			fnDebugParser( "(" );
			fnGetSymbol( );

			/* identifier '('[type indentifier ',' ...] ')'
			 * Llamada a procedimiento
			 */
			fnCall( variableOrProcedureName );

			/* identifier '('[type indentifier ',' ...] ')' ';'
			 */
			if( g_symbol == SYM_SEMICOLON )
			{
				fnDebugParser( ";" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_SEMICOLON );
		}
		/* identifier '='
		 */
		else if( g_symbol == SYM_ASSIGN )
		{
			entry = fnGetScopedSymbolTableEntry( g_identifier, VARIABLE, g_lastProc );

			if( entry == 0 )
			{
				printf( "\n Error: line %d, ", g_lineNumber );
				printf( "'%s' undeclared.", g_identifier );
				
				printf( "\n\n Press any key to exit..." );
				_getch( );

				exit( 1 );
			}

			iLType = fnGetType( entry );

			// CODEGEN
			fnDebugCodeGen( "lda", g_identifier, NO_LABEL );
			//
			fnDebugParser( "=" );
			fnGetSymbol( );

			/* identifier '=' expression
			*/
			iRType = fnExpression( );

			// CODEGEN
			fnDebugCodeGen( "stn", "", NO_LABEL );
			//
			if( iLType != iRType )
				fnTypeWarning( iLType, iRType );
			else if( !fnIsDefined( entry ) )
				fnSetDefined( entry, 1 );

			g_iNumberOfAssignments = g_iNumberOfAssignments + 1;

			/* identifier '=' expression ';'
			 */
			if( g_symbol == SYM_SEMICOLON )
			{
				fnDebugParser( ";" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_SEMICOLON );
		}
		else
			fnSyntaxErrorUnexpected( );
	}
	/* while
	 */
	else if( g_symbol == SYM_WHILE )
	{
		fnDebugParser( "while" );
		fnWhile( );
	}
	/* if
	 */
	else if( g_symbol == SYM_IF )
	{
		fnDebugParser( "if" );
		fnIf( );
	}
	/* return
	 */
	else if( g_symbol == SYM_RETURN )
	{
		fnDebugParser( "return" );

		/* return expresion
		 */
		fnReturn( );

		/* return expresion ';'
		 */
		if( g_symbol == SYM_SEMICOLON )
		{
			fnDebugParser( ";" );
			fnGetSymbol( );
		}
		else
			fnSyntaxErrorSymbol( SYM_SEMICOLON );
	}
}

/*Antes de llamar a fnProcedure( ) se debe de haber obtenido un token.
 */
// COMPLETAR ALGUNOS DETALLES
void fnProcedure( char* procedure, int type )
{
	int bIsUndefined;
	int iNumberOfParameters;
	int iParameters;
	int iLocalVariables;
	sEntry* entry;

	bIsUndefined = 1; // No ha sido usado
	iNumberOfParameters = 0;

	g_bHasReturn = 0;

	/* Se movio aquí para asegurar que coincidan los parámetros
	 * de las funciones que ya se han definido.
	 */
	entry = fnSearchSymbolTable( global_symbol_table, procedure, PROCEDURE, 0 );

	if ( entry == 0 ) //NULL
	{
		//Se crea una entrada en la tabla para el procedimiento.
		fnCreateSymbolTableEntry( GLOBAL_TABLE, procedure, g_lineNumber, PROCEDURE, type, 0, -1, 0, 0 );
	}

	// try parsing formal parameters
	/* type identifier '('...
	 */
	if( g_symbol == SYM_LPARENTHESIS )
	{
		fnDebugParser( "(" );
		fnGetSymbol( );

		/*
		 */
		if( g_symbol != SYM_RPARENTHESIS )
		{
			// TODO: REVISAR
			// Para verificar que los parámetros coincidan o
			// crear la lista de parámetros de la función
			g_bSearchParameters = 1;
			g_entry = entry;
			//

			/* type identifier '(' type identifier ...
			 */
			fnVariable( );
			iNumberOfParameters = 1;

			/* type identifier '(' type identifier ',' ...
			 */
			while( g_symbol == SYM_COMMA )
			{
				fnDebugParser( "," );

				fnGetSymbol( );
				fnVariable( );

				iNumberOfParameters = iNumberOfParameters + 1;
			}

			// Faltan algunas cosas aquí para los parámetros

			/* type identifier '(' type identifier ',' ... ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );

			// TODO: REVISAR
			g_bSearchParameters = 0;
		}
		/* Si g_symbol == SYM_RPARENTHESIS
		 * type identifier '(' ')'
		 */
		else
		{
			fnDebugParser( ")" );
			fnGetSymbol( );
		}
	}
	else
		fnSyntaxErrorSymbol( SYM_LPARENTHESIS );

	/* NOTA: Lo moví al inicio para ver que los parámetros de una
	 * función que ya ha sido declarada coincidan.
	 *
	 * Bucamos en la tabla global el procedimiento
	 */
	 // entry = fnSearchSymbolTable( global_symbol_table, procedure, PROCEDURE );

	 /* type identifier '(' type identifier ',' ... ')' ';'
	  * Declaración de una función (prototipo de una función).
	  */
	if( g_symbol == SYM_SEMICOLON )
	{
		fnDebugParser( ";" );

		//Si entry es NULL (0), entonces es una declaración de
		//procedimiento.
		if ( entry != 0 ) //NULL
		{
			printf( "\n Warning: line %d, previous declaration of '%s'.", g_lineNumber, procedure );

			if ( fnGetType( entry ) != type )
				printf( "\n Warning: line %d, conflicting types for '%s'.", g_lineNumber, procedure );

			printf( "\n" );
		}

		fnGetSymbol( );
	}
	/* type identifier '(' type identifier ',' ... ')' '{'
	 * Definición de una función.
	 */
	else if( g_symbol == SYM_LBRACE )
	{
		// CODEGEN
		fnDebugCodeGen( "ent", procedure, NO_LABEL );
		//
		fnDebugParser( "{" );

		if( entry == 0 )
		{
			// procedure never called nor declared nor defined
			// createSymbolTableEntry( GLOBAL_TABLE, procedure, lineNumber, PROCEDURE, type, 0, binaryLength );
			
			// TODO: poner procedure en definido
			entry = fnSearchSymbolTable( global_symbol_table, procedure, PROCEDURE, 0) ;
			fnSetDefined( entry, 1 );
			//fnCreateSymbolTableEntry( GLOBAL_TABLE, procedure, g_lineNumber, PROCEDURE, type, 0, -1, 1, 0 );
		}
		// TODO: COMPLETAR
		else
		{
			if( fnGetType( entry ) != type )
				printf( "\n Warning: line %d, conflicting types for '%s'.", g_lineNumber, procedure );

			if( !fnIsDefined( entry ) )
				fnSetDefined( entry, 1 );
			else
			{
				printf( "\n Warning: line %d, ", g_lineNumber );
				printf( "redefinition of procedure '%s'.", procedure );
				// printf( "ignored.\n" );
			}
		}

		fnGetSymbol( );

		iLocalVariables = 0;

		/* type identifier '(' type identifier ',' ... ')'
		 * '{'
		 *      type identifier [= literal] ';'
		 *      ...
		 */
		while( fnIsIntOrChar( ) )
		{
			// Se cuenta el número de variables locales
			iLocalVariables = iLocalVariables + 1;

			fnVariable( );

			if( g_symbol == SYM_SEMICOLON )
			{
				fnDebugParser( ";" );
				fnGetSymbol( );
			}
			/* Para que acepte la declaración e inicialiazción de una
			 * variable local, al mismo tiempo, descomentar el else if.
			 */
			 /*else if( g_symbol == SYM_ASSIGN )
				 fnInitialization();*/
			else
				fnSyntaxErrorSymbol( SYM_SEMICOLON );
		}

		// help_procedure_prologue(localVariables);
		// create a fixup chain for return statements

		// returnBranches = 0;

		// TODO: REVISAR
		g_iReturnType = type;

		/* type identifier '(' type identifier ',' ... ')'
		 * '{'
		 *      type identifier [= literal] ';'
		 *      ...
		 *      statement ...
		 */
		while( fnIsNotRbraceOrEOF( ) )
			fnStatement( );

		// TODO: REVISAR
		g_iReturnType = 0;

		/* type identifier '(' type identifier ',' ... ')'
		 * '{'
		 *      type identifier [= literal] ';'
		 *      ...
		 *      statement
		 * '}' ...
		 */
		if( g_symbol == SYM_RBRACE )
		{
			fnDebugParser( "}" );

			/********************/
			if( !g_bHasReturn )
			{
				// CODEGEN
				fnDebugCodeGen ( "ret", "", NO_LABEL );
				//
				if( type != VOID_T )
				{
					printf( "\n Warning: in function '%s', a value of type '", procedure );
					fnPrintType( type );
					printf( "' should be returned.\n" );
				}
			}
			/********************/

			fnGetSymbol( );
		}
		else
		{
			fnSyntaxErrorSymbol( SYM_RBRACE );

			printf( "\n Press any key to exit..." );
			_getch( );

			exit( 1 );
		}
	}
	else
		fnSyntaxErrorUnexpected( );

	// ¿PARA QUÉ ES?
	// local_symbol_table = (int*) 0;    
}

// FALTA COMPLETAR ALGUNOS DETALLES
int fnCall( char* procedure )
{
	int iType;
	// NO LO NECECITAMOS, POR EL MOMENTO
	int iNumberOfTemporaries;
	sEntry* entry;

	entry = fnGetScopedSymbolTableEntry( procedure, PROCEDURE, g_lastProc );

	iNumberOfTemporaries = g_iAllocatedTemporaries;

	/* identifier '(' expression ...
	 */
	if( fnIsExpression( ) )
	{
		// CODEGEN
		fnDebugCodeGen( "mst", "", NO_LABEL );
		//
		fnExpression( );

		// TODO: check if types/number of parameters is correct

		/* identifier '(' expression ',' ...
		 */
		while( g_symbol == SYM_COMMA )
		{
			fnDebugParser( "," );
			fnGetSymbol( );

			/* identifier '(' expression ',' expression ...
			 */
			fnExpression( );
		}

		/* identifier '(' expression [, expression ',' ...] ')' ...
		 */
		if( g_symbol == SYM_RPARENTHESIS )
		{
			// CODEGEN
			fnDebugCodeGen( "cup", procedure, NO_LABEL );
			//
			fnDebugParser( ")" );
			fnGetSymbol( );

			// El procedimiento nunca ha sido llamado,
			// declarado ni definido.
			if( entry == 0 )
			{
				// TODO: REVISAR
				// Suponemos que todo procedimiento que sea
				// llamado tiene que haber sido declarado o
				// definido.
				printf( "\n Error: line %d, procedure '%s' ", g_lineNumber, g_identifier );
				printf( "has never been declared or defined." );
				
				printf( "\n\n Press any key to exit..." );
				_getch( );

				exit( 1 );
			}
			else
				iType = fnGetType( entry );
		}
		else
		{
			fnSyntaxErrorSymbol( SYM_RPARENTHESIS );

			// TODO: VER POR QUÉ SE DA POR DEFAULT
			// iType = INT_T;
		}
	}
	/* identifier '(' ')' ...
	 */
	else if( g_symbol == SYM_RPARENTHESIS )
	{
		// CODEGEN
		fnDebugCodeGen( "cup", g_identifier, NO_LABEL );
		//
		fnDebugParser( ")" );
		fnGetSymbol( );

		// El procedimiento nunca ha sido llamado,
		// declarado ni definido.
		if( entry == 0 )
		{
			// TODO: REVISAR
			// Suponemos que todo procedimiento que sea
			// llamado tiene que haber sido declarado o
			// definido.
			printf( "\n Error: line %d, procedure '%s' ", g_lineNumber, g_identifier );
			printf( "has never been declared or defined." );
			
			printf( "\n\n Press any key to exit..." );
			_getch( );

			exit( 1 );
		}
		else
			iType = fnGetType( entry );
	}
	else
	{
		fnSyntaxErrorSymbol( SYM_RPARENTHESIS );

		// TODO: VER POR QUÉ SE DA POR DEFAULT
		// iType = INT_T;
	}

	g_iNumberOfCalls = g_iNumberOfCalls + 1;

	return iType;
}

void fnWhile( )
{
	/* while ...
	 */
	if( g_symbol == SYM_WHILE )
	{
		// CODEGEN
		fnNewLabel( );
		g_iL1While = g_iLabel;

		fnDebugCodeGen( "lab", "L", g_iLabel );
		//
		fnGetSymbol( );

		/* while '(' ...
		 */
		if( g_symbol == SYM_LPARENTHESIS )
		{
			fnDebugParser( "(" );
			fnGetSymbol( );

			/* while '(' expression ...
			 */
			fnExpression( );

			// CODEGEN
			fnNewLabel( ); // ¿Se genera aquí?
			g_iL2While = g_iLabel;

			fnDebugCodeGen( "fjp", "L", g_iLabel );
			//
			/* while '(' expression ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );

				// zero or more statements: { statement }
				/* while '(' expression ')'
				 * '{' ...
				 */
				if( g_symbol == SYM_LBRACE )
				{
					fnDebugParser( "{" );
					fnGetSymbol( );

					/* while '(' expression ')'
					 * '{'
					 *      statement
					 *      ...
					 */
					while( fnIsNotRbraceOrEOF( ) )
						fnStatement( );

					/* while '(' expression ')'
					 * '{'
					 *      statement
					 *      ...
					 * '}'
					 */
					if( g_symbol == SYM_RBRACE )
					{
						fnDebugParser( "}" );
						fnGetSymbol( );
					}
					else
					{
						fnSyntaxErrorSymbol( SYM_RBRACE );

						printf( "\n Press any key to exit..." );
						_getch( );

						exit( 1 );
					}
				}
				// only one statement without '{' '}'
				/* while '(' expression ')'
				 *    statement ...
				 */
				else
					fnStatement( );
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
		}
		else
			fnSyntaxErrorSymbol( SYM_LPARENTHESIS );
	}
	else
		fnSyntaxErrorSymbol( SYM_WHILE );

	// TODO: VER SI ES EL LUGAR CORRECTO
	// CODEGEN
	fnDebugCodeGen( "ujp", "L", g_iL1While );
	fnDebugCodeGen( "lab", "L", g_iL2While );
	//
	g_iNumberOfWhile = g_iNumberOfWhile + 1;
}

void fnIf( )
{
	/* if ...
	 */
	if( g_symbol == SYM_IF )
	{
		fnGetSymbol( );

		/* if '(' ...
		 */
		if( g_symbol == SYM_LPARENTHESIS )
		{
			fnDebugParser( "(" );
			fnGetSymbol( );

			// CODEGEN
			fnNewLabel( );
			g_iL1If = g_iLabel;
			//
			/* if '(' expression ...
			 */
			fnExpression( );

			/* if '(' expression ')' ...
			 */
			if( g_symbol == SYM_RPARENTHESIS )
			{
				fnDebugParser( ")" );
				fnGetSymbol( );

				// CODEGEN
				fnDebugCodeGen( "fjp", "L", g_iL1If );
				//
				// zero or more statements: { statement }
				/* if '(' expression ')'
				 * '{' ...
				 */
				if( g_symbol == SYM_LBRACE )
				{
					fnDebugParser( "{" );
					fnGetSymbol( );

					/* if '(' expression ')'
					 * '{'
					 *      statement
					 *      ...
					 */
					while( fnIsNotRbraceOrEOF( ) )
						fnStatement( );

					/* if '(' expression ')'
					 * '{'
					 *      statement
					 *      ...
					 * '}'
					 */
					if( g_symbol == SYM_RBRACE )
					{
						fnDebugParser( "}" );
						fnGetSymbol( );
					}
					else
					{
						fnSyntaxErrorSymbol( SYM_RBRACE );

						printf( "\n Press any key to exit..." );
						_getch( );

						exit( 1 );
					}
				}
				// only one statement without '{' '}'
				/* if '(' expression ')'
				 *      statement ...
				 */
				else
					fnStatement( );

				//optional: else
				/* if '(' expression ')'
				 *  ...
				 * else ...
				 */
				if( g_symbol == SYM_ELSE )
				{
					fnDebugParser( "else" );
					fnGetSymbol( );

					// CODEGEN: 
					fnNewLabel( ); // ¿Se genera aquí?
					g_iL2If = g_iLabel;

					fnDebugCodeGen( "ujp", "L", g_iL2If );
					fnDebugCodeGen( "lab", "L", g_iL1If );
					//
					// zero or more statements: { statement }
					/* if '(' expression ')'
					 *  ...
					 * else
					 * '{' ...
					 */
					if( g_symbol == SYM_LBRACE )
					{
						fnDebugParser( "{" );
						fnGetSymbol( );

						/* if '(' expression ')'
						 *  ...
						 * else
						 * '{'
						 *      statement
						 *      ...
						 */
						while( fnIsNotRbraceOrEOF( ) )
							fnStatement( );

						/* if '(' expression ')'
						 *  ...
						 * else
						 * '{'
						 *      statement
						 *      ...
						 * '}' ...
						 */
						if( g_symbol == SYM_RBRACE )
						{
							fnDebugParser( "}" );
							fnGetSymbol( );

							// CODEGEN
							// fnDebugCodeGen( "lab", "L", g_iL2If );
							//
						}
						else
						{
							fnSyntaxErrorSymbol( SYM_RBRACE );

							printf( "\n Press any key to exit..." );
							_getch( );

							exit( 1 );
						}
						// only one statement without {}
						/* if '(' expression ')'
						 *  ...
						 * else
						 *    statement ...
						 */
					}
					else
					{
						fnStatement( );

						// CODEGEN
						// fnDebugCodeGen( "lab", "L", g_iL2If );
						//
					}

					// CODEGEN
					fnDebugCodeGen( "lab", "L", g_iL2If );
					//
				}
				else// No tiene else  
					// CODEGEN
					fnDebugCodeGen( "lab", "L", g_iL1If );
					//
			}
			else
				fnSyntaxErrorSymbol( SYM_RPARENTHESIS );
		}
		else
			fnSyntaxErrorSymbol( SYM_LPARENTHESIS );
	}
	else
		fnSyntaxErrorSymbol( SYM_IF );

	g_iNumberOfIf = g_iNumberOfIf + 1;
}

void fnReturn( )
{
	int iType;

	/* return ...
	 */
	if( g_symbol == SYM_RETURN )
		fnGetSymbol( );
	else
		fnSyntaxErrorSymbol( SYM_RETURN );

	// optional: expression
	if( g_symbol != SYM_SEMICOLON )
	{
		/* return expression ...
		 */
		iType = fnExpression( );

		// CODEGEN
		fnDebugCodeGen( "ret", "", NO_LABEL );

		if ( strcmp( g_lastProc, "main" ) == 0 )
			fnDebugCodeGen( "stp", "", NO_LABEL );
		//
		if( iType != g_iReturnType )
			fnTypeWarning( g_iReturnType, iType );
	}
	/* Es posible que en una función de tipo void se tenga: return ;
	 */
	else if( g_iReturnType != VOID_T )
	{
		printf( "\n Warning: line %d, 'return' with ", g_lineNumber );
		printf( "no value, in function returning non-void.\n" );
	}

	g_bHasReturn = 1;
	g_iNumberOfReturn = g_iNumberOfReturn + 1;
}

void fnInclude( )
{
	/* '#' ...
	 */
	if( g_symbol == SYM_OCTOTHORPE )
	{
		fnGetSymbol( );

		/* '#' include ...
		 */
		if( g_symbol == SYM_INCLUDE )
		{
			fnDebugParser( "include" );
			fnGetSymbol( );

			/* '#' include string ...
			 */
			if( g_symbol == SYM_STRING )
			{
				fnDebugParser( "string" );
				fnGetSymbol( );
			}
			else
				fnSyntaxErrorSymbol( SYM_STRING );
		}
		else
			fnSyntaxErrorSymbol( SYM_INCLUDE );
	}
	else
		fnSyntaxErrorSymbol( SYM_OCTOTHORPE );
}

//int fnIsType()
int fnIsTypeOrHash( )
{
	if( g_symbol == SYM_VOID )
		return 1;

	if( g_symbol == SYM_INT )
		return 1;

	if( g_symbol == SYM_CHAR )
		return 1;

	if( g_symbol == SYM_OCTOTHORPE )
		return 1;

	return 0;
}

/*Si regresa 0, llego al fin de archivo y no encontó ningún type o '#', en
 *caso de regresar 1, encontró un token del tipo type.
 */
 //int fnFindNextDataType()
int fnFindNextDataTypeOrHash( )
{
	while( !fnIsTypeOrHash( ) )
	{
		/*Se imprime un error informando que no se encontro un
		 *tipo de dato, sino un símbolo inesperado.
		 */
		fnSyntaxErrorUnexpected( );

		/*Si llegamos al fin de archivo (EOF) salimos del programa
		 */
		if( g_symbol == SYM_EOF )
			return 0;
		/*En caso contrario, pedimos el sigunete token
		 */
		else
			fnGetSymbol( );
	}

	return 1;
}

int fnIsIntOrChar( )
{
	if( g_symbol == SYM_INT )
		return 1;

	if( g_symbol == SYM_CHAR )
		return 1;

	return 0;
}

int fnIsPlusOrMinus( )
{
	if( g_symbol == SYM_PLUS )
	{
		fnDebugParser( "PLUS" );
		return 1;
	}

	if( g_symbol == SYM_MINUS )
	{
		fnDebugParser( "MINUS" );
		return 1;
	}

	return 0;
}

int fnIsExpression( )
{
	if( g_symbol == SYM_MINUS )
		return 1;

	if( g_symbol == SYM_LPARENTHESIS )
		return 1;

	if( g_symbol == SYM_IDENTIFIER )
		return 1;

	if( g_symbol == SYM_INTEGER )
		return 1;

	if( g_symbol == SYM_ASTERISK )
		return 1;

	if( g_symbol == SYM_STRING )
		return 1;

	if( g_symbol == SYM_CHARACTER )
		return 1;

	return 0;
}

int fnIsAsteriskOrDivOrMod( )
{
	if( g_symbol == SYM_ASTERISK )
	{
		fnDebugParser( "TIMES" );
		return 1;
	}

	if( g_symbol == SYM_DIV )
	{
		fnDebugParser( "DIV" );
		return 1;
	}

	if( g_symbol == SYM_MOD )
	{
		fnDebugParser( "MOD" );
		return 1;
	}

	return 0;
}

int fnIsComparison( )
{
	if( g_symbol == SYM_EQUALITY )
	{
		fnDebugParser( "==" );
		return 1;
	}

	if( g_symbol == SYM_NOTEQ )
	{
		fnDebugParser( "!=" );
		return 1;
	}

	if( g_symbol == SYM_LT )
	{
		fnDebugParser( "<" );
		return 1;
	}

	if( g_symbol == SYM_GT )
	{
		fnDebugParser( ">" );
		return 1;
	}

	if( g_symbol == SYM_LEQ )
	{
		fnDebugParser( "<=" );
		return 1;
	}

	if( g_symbol == SYM_GEQ )
	{
		fnDebugParser( ">=" );
		return 1;
	}

	return 0;
}

int fnIsLiteral( )
{
	if( g_symbol == SYM_INTEGER )
	{
		fnDebugParser( "integer" );
		return 1;
	}

	if( g_symbol == SYM_CHARACTER )
	{
		fnDebugParser( "character" );
		return 1;
	}

	return 0;
}

int fnIsRbraceOrSemicolonOrEOF( )
{
	if( g_symbol == SYM_RBRACE )
		return 1;

	if( g_symbol == SYM_SEMICOLON )
		return 1;

	if( g_symbol == SYM_EOF )
		return 1;

	return 0;
}

int fnIsNotRbraceOrEOF( )
{
	if( g_symbol == SYM_RBRACE )
		return 0;

	if( g_symbol == SYM_EOF )
		return 0;

	return 1;
}

void fnSyntaxErrorSymbol( int expected )
{
	printf( "\n Error: line %d, ", g_lineNumber );
	printf( "'%s' expected but ", *( g_SYMBOLS + expected ) );

	if( g_symbol == SYM_EOF )
		printf( "EOF" );
	else
		printf( "'%s'", *( g_SYMBOLS + g_symbol ) );

	printf( " found.\n" );
}

void fnSyntaxErrorUnexpected( )
{
	printf( "\n Error: line %d, ", g_lineNumber );
	printf( "unexpected symbol " );

	if( g_symbol == SYM_EOF )
		printf( "EOF" );
	else
		printf( "'%s'", *( g_SYMBOLS + g_symbol ) );

	printf( " found.\n" );
}

void fnTypeWarning( int expected, int found )
{
	printf( "\n Warning: line %d, ", g_lineNumber );

	printf( "type mismatch, '" );
	fnPrintType( expected );

	printf( "' expected but '" );
	fnPrintType( found );

	printf( "' found.\n" );
}
