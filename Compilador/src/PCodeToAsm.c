#include "../includes/symbolTable.h"
#include "../includes/PCodeRead.h"
#include "../includes/stack.h"
#include "../includes/list.h"
#include "../includes/PCodeToAsm.h"

#define REGISTER_TEMPORARY 0
#define REGISTER_ARGUMENT 1

void fnInitListOfTemporaries( );
void fnInitListOfArguments( );
void fnInsertRegister( char * strReg );
int fnRegisterType( char * strReg );
int fnGetNumberOfParameters( sEntry *  procedure );
int fnIsVoid( sEntry * procedure );
int fnIsParameter( sEntry * variable );

void fnGetTemporary( char * out_strReg );
void fnGetArgument( char * out_strReg );

int fnIsArgument( sEntry * entryProcedure, char * strArg );

void fnSaveTemporariesUsed( );
void fnSaveArgumentsUsed( int iNumberOfParameters );

int g_iNumParam;
int g_iTemporariesUsed = 0;

stList * g_listOfTemporaries, * g_listOfArguments;

void fnGenAsmCode( )
{
	int i, bMarkStack = 0, iNumberOfParameters = 0;
    char strReg0[ 4 ], strReg1[ 4 ], strReg2[ 4 ], strOpcode[ 5 ], strAuxReg[ 9 ];
    stStack * stackOfPMachine;
	sEntry * entry, * entryCurrProcedure;

    stackOfPMachine = fnInitStack( );
    g_listOfTemporaries = fnInitList( );
	g_listOfArguments = fnInitList( );

    fnInitListOfTemporaries( );
	fnInitListOfArguments( );

	// Sección de datos
	fnGenDirective( ".data" );
	entry = local_symbol_table;

	/* Generar código para todas las variables, que no sean parámetros
	 */
	while( entry )
	{
		if( !fnIsParameter( entry ) )
			fnGenVariable( fnGetString( entry ), fnGetType( entry ) );

		entry =  fnGetNextEntry( entry );
	}
	//

	// Sección de texto
	fnGenDirective( ".text" );
	fnGenDirective( ".globl main" );

	/* Consideramos que cada línea de código P es de la sigueinte forma
	 *                         instr arg
	 * donde,
	 *        instr  es la instrucción que se va a ejecutar; y
	 *        arg    es el argumento de la instrucción, el cual
	 *               puede ser "nulo".
	 */
    while( fnHasInstr( ) )
    {
        fnGetInstr( );

        if ( fnInstrMatch( "ent" ) )
        {
            fnGetArg( );
			fnGenLabel( g_strArg );

			entryCurrProcedure = fnSearchSymbolTable( global_symbol_table, g_strArg, PROCEDURE, 0 );
			iNumberOfParameters = fnGetNumberOfParameters( entryCurrProcedure );
        }
        else if ( fnInstrMatch( "lda" ) )
        {
            fnGetArg( );
			fnGetTemporary( strReg0 );

            fnGenPseudoInstr( "la", strReg0, g_strArg, "" );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "ldc" ) )
        {
            fnGetArg( );
			fnGetTemporary( strReg0 );
			
            fnGenPseudoInstr( "li", strReg0, "", g_strArg );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "lod" ) )
        {
            fnGetArg( );

			// TODO: Considerar más de 4 parámetros
			if ( fnIsArgument( entryCurrProcedure, g_strArg ) )
				sprintf( strReg0, "$a%d", g_iNumParam );
			else
			{
				fnGetTemporary( strReg0 );
				fnGenIFormat( "lw", strReg0, g_strArg, "" );
			}

            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "stn" ) )
        {
            strcpy( strReg0, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg0 );

			strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 );

			// TODO: REVISAR
			// De acuerdo al código P siempre asignamos un valor de esta forma.
			sprintf( strAuxReg, "0(%s)", strReg1 );
            fnGenIFormat( "sw", strReg0, strAuxReg, "" );
        }
        else if ( fnInstrMatch( "adi" ) )
        {
			strcpy( strReg2, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg2 );
			
            strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 ); 

			fnGetTemporary( strReg0 );

            fnGenRFormat( "add", strReg0, strReg1, strReg2, "" );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "sbi" ) )
        {
            strcpy( strReg2, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg2 );

            strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 );

			fnGetTemporary( strReg0 );

            fnGenRFormat( "sub", strReg0, strReg1, strReg2, "" );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "mpi" ) )
        {
            strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg0 );

			fnGenRFormat( "mult", "", strReg0, strReg1, "" );
			
			fnGetTemporary( strReg0 );

			fnGenRFormat( "mflo", strReg0, "", "", "" );
			fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "dvi" ) )
        {
            strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg0 );

			fnGenRFormat( "div", "", strReg0, strReg1, "" );

			fnGetTemporary( strReg0 );

			fnGenRFormat( "mflo", strReg0, "", "", "" );
			fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "mod" ) )
        {
            strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg0 );

			fnGenRFormat( "div", "", strReg0, strReg1, "" );

			fnGetTemporary( strReg0 );

			fnGenRFormat( "mfhi", strReg0, "", "", "" );
			fnPush( stackOfPMachine, strReg0 );
        }
		else if ( fnInstrMatch( "ngi" ) )
		{
			strcpy( strReg0, fnPop( stackOfPMachine ) );
			fnGenRFormat( "sub", strReg0, "$zero", strReg0, "" );
			fnPush( stackOfPMachine, strReg0 );
		}
        else if ( fnInstrMatch( "les" ) )
        {
            fnPush( stackOfPMachine, "bge" );
        }
        else if ( fnInstrMatch( "leq" ) )
        {
            fnPush( stackOfPMachine, "bgt" );
        }
        else if ( fnInstrMatch( "equ" ) )
        {
            fnPush( stackOfPMachine, "bne" );
        }
        else if ( fnInstrMatch( "grt" ) )
        {
            fnPush( stackOfPMachine, "ble" );
        }
        else if ( fnInstrMatch( "geq" ) )
        {
            fnPush( stackOfPMachine, "blt" );
        }
        else if ( fnInstrMatch( "neq" ) )
        {
            fnPush( stackOfPMachine, "beq" );
        }
        else if ( fnInstrMatch( "fjp" ) )
        {
            fnGetArg( );
            strcpy( strOpcode, fnPop( stackOfPMachine ) );

            strcpy( strReg1, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
			fnInsertRegister( strReg0 );

           if ( strcmp( strOpcode, "beq" ) == 0 || strcmp( strOpcode, "bne" ) == 0 )
				fnGenIFormat( strOpcode, strReg0, strReg1, g_strArg );
			else
				fnGenPseudoInstr( strOpcode, strReg0, strReg1, g_strArg );
        }
        else if ( fnInstrMatch( "ujp" ) )
        {
            fnGetArg( );
			fnGenJFormat( "j", g_strArg );
        }
        else if ( fnInstrMatch( "lab" ) )
        {
            fnGetArg( );
			fnGenLabel( g_strArg );
        }
		else if ( fnInstrMatch( "mst" ) )
		{
			// Ahora está en "ent"
			// iNumberOfParameters = fnGetNumberOfParameters( entryCurrProcedure );

			// Se cambió a cup y csp
			/* sprintf( strAuxReg, "-%d", 4 * ( g_iTemporariesUsed + iNumberOfParameters + 1 ) );
			fnGenIFormat( "addi", "$sp", "$sp", strAuxReg ); */

			// TODO: Liberar los registros temporales, para que puedan ser usados.
			// Consirera que al liberlos el valor de g_iTemporariesUsed cambia.
			//
			// Salvar los valores de los registros temporales que han sido usados.
			/* for ( i = 0; i < g_iTemporariesUsed; i++ )
			{
				sprintf( strReg0, "$t%d", i );
				sprintf( strAuxReg, "%d($sp)", 4 * i );

				// fnInsertRegister( strReg0 );
				fnGenIFormat( "sw", strReg0, strAuxReg, "" );
			} */

			// Se cambió a cup y csp
			// fnSaveTemporariesUsed( );
			
			// Se salvan los valores de los argumentos, del procedimiento actual.
			// Y se liberan para que se puedan volver a utilizar.
			/* for ( i = 0; i < iNumberOfParameters; i++ )
			{
				sprintf( strReg0, "$a%d", i);
				sprintf( strAuxReg, "%d($sp)", 4 * ( i + g_iTemporariesUsed ) );

				fnInsertRegister( strReg0 );
				fnGenIFormat( "sw", strReg0, strAuxReg, "" );
			} */

			// Se cambió a cup y csp
			/* fnSaveArgumentsUsed( iNumberOfParameters );

			sprintf( strAuxReg, "%d($sp)", 4 * ( g_iTemporariesUsed + iNumberOfParameters ) );
			fnGenIFormat( "sw", "$ra", strAuxReg, "" ); */

			fnPush( stackOfPMachine, "mst" );
			bMarkStack = 1;
		}
		else if ( fnInstrMatch( "cup" ) )
		{
			fnGetArg( );

			// Se movió de mst
			// Salvamos los valores de los argumentos, si hay
			if ( iNumberOfParameters > 0 )
			{
				sprintf( strAuxReg, "-%d", 4 * iNumberOfParameters );
				fnGenIFormat( "addi", "$sp", "$sp", strAuxReg );

				fnSaveArgumentsUsed( iNumberOfParameters );
			}
			//

			entry = fnSearchSymbolTable( global_symbol_table, g_strArg, PROCEDURE, 0 );
			// Se calcula el número de parámetros del procedimiento que se va a llamar.
			i = fnGetNumberOfParameters( entry ) - 1;

			strcpy( strReg1, fnPop( stackOfPMachine ) );
			
			/* Guardar los valores de los parámetros
			 */
			while ( strcmp( strReg1, "mst" ) != 0 )
			{
				// Obtenemos un registro para guardar el argumento
				sprintf( strReg0, "$a%d", i );
				fnSetOccupied( i--, g_listOfArguments );
				//

				// TODO: REVISAR, POSIBLES ERRORES
				// No mover el valor de un registro al mismo registro
				if ( strcmp( strReg0, strReg1 ) != 0 )
					fnGenPseudoInstr( "move", strReg0, strReg1, "" );

				fnInsertRegister( strReg1 );
				strcpy( strReg1, fnPop( stackOfPMachine ) );
			}

			// TODO: Modificar
			fnInitListOfArguments( );
			//
			// Se movió de mst
			// Salvamos los temporales y $ra
			sprintf( strAuxReg, "-%d", 4 * ( g_iTemporariesUsed + 1 ) );
			fnGenIFormat( "addi", "$sp", "$sp", strAuxReg );

			sprintf( strAuxReg, "%d($sp)", 0 );
			fnGenIFormat( "sw", "$ra", strAuxReg, "" );

			fnSaveTemporariesUsed( );
			//
			fnGenJFormat( "jal", g_strArg );

			// Restauramos primero $ra
			sprintf( strAuxReg, "%d($sp)", 0 );
			fnGenIFormat( "lw", "$ra", strAuxReg, "" );

			// TODO: Si se liberan los registros, se tiene que volver a 
			// establecer cómo ocupados.
			//
			// Restaurar los valores de los registros temporales, si hay.
			for (i = 0; i < g_iTemporariesUsed; i++)
			{
				sprintf( strReg0, "$t%d", i );
				sprintf( strAuxReg, "%d($sp)", 4 * ( i + 1 ) );

				fnGenIFormat( "lw", strReg0, strAuxReg, "" );
				// fnSetOcupied( i, g_listOfTemporaries );
			}

			// Restaurar los valores argumentos, si hay
			for ( i = 0; i < iNumberOfParameters; i++ )
			{
				sprintf( strReg0, "$a%d", i );
				sprintf( strAuxReg, "%d($sp)", 4 * ( i + g_iTemporariesUsed + 1 ) );
				
				fnGenIFormat( "lw", strReg0, strAuxReg, "" );
				fnSetOccupied( i, g_listOfArguments );
			}

			// sprintf( strAuxReg, "%d($sp)", 4 * ( g_iTemporariesUsed + iNumberOfParameters ) );
			// fnGenIFormat( "lw", "$ra", strAuxReg, "" );

			sprintf( strAuxReg, "%d", 4 * ( g_iTemporariesUsed + iNumberOfParameters + 1 ) );
			fnGenIFormat( "addi", "$sp", "$sp", strAuxReg );

			// Se busca, en la tabla de símbolos, el procedimiento que se va a llamar
			entry = fnSearchSymbolTable( global_symbol_table, g_strArg, PROCEDURE, 0 );
			
			// Si regresa algo se guarda en un registro temporal
			if ( !fnIsVoid( entry ) )
			{
				fnGetTemporary( strReg0 );

				fnGenPseudoInstr( "move", strReg0, "$v0", "" );
				fnPush( stackOfPMachine, strReg0 );
			}

			bMarkStack = 0;
		}
		else if ( fnInstrMatch( "csp" ) )
		{
			fnGetArg();

			if ( strcmp( g_strArg, "printi" ) == 0 )
			{
				strcpy( strReg1, fnPop( stackOfPMachine ) );

				if ( iNumberOfParameters > 0 )
				{
					fnGetTemporary( strReg0 );
					fnGenPseudoInstr( "move", strReg0, "$a0", "" );
				}

				fnGenPseudoInstr( "move", "$a0", strReg1, "" );

				fnGenIFormat( "li", "$v0", "", "1" );
				fnGenRFormat( "syscall", "", "", "", "" );

				if ( iNumberOfParameters > 0 )
				{
					fnGenPseudoInstr( "move", "$a0", strReg0, "" );
					fnInsertRegister( strReg0 );
				}
			}
			else if ( strcmp( g_strArg, "printc" ) == 0 )
			{
				strcpy( strReg1, fnPop( stackOfPMachine ) );

				if ( iNumberOfParameters > 0 )
				{
					fnGetTemporary( strReg0 );
					fnGenPseudoInstr( "move", strReg0, "$a0", "" );
				}

				fnGenPseudoInstr( "move", "$a0", strReg1, "" );
				fnGenIFormat( "li", "$v0", "", "11" );
				fnGenRFormat( "syscall", "", "", "", "" );

				if ( iNumberOfParameters > 0 )
				{
					fnGenPseudoInstr( "move", "$a0", strReg0, "" );
					fnInsertRegister( strReg0 );
				}
			}
			/* else if ( strcmp( g_strArg, "scani" ) == 0 )
			{
				fnGenIFormat( "li", "$v0", "", "5" );
				fnGenRFormat( "syscall", "", "", "", "" );
				
				//fnInsertRegister( fnPop( stackOfPMachine ) );
			} */
		}
		else if ( fnInstrMatch( "ret" ) )
        {
			if ( !fnIsVoid( entryCurrProcedure ) )
			{
				strcpy( strReg0, fnPop( stackOfPMachine ) );
				fnInsertRegister( strReg0 );

				fnGenPseudoInstr( "move", "$v0", strReg0, "" );
			}

			fnGenRFormat( "jr", "", "$ra", "", "" );
        }
    }
	//

    fnDestroyList( g_listOfTemporaries );
	fnDestroyList( g_listOfArguments );
    fnDestroyStack( stackOfPMachine );
}

void fnGenLabel( char* label )
{
	printf( "%s:\n", label );
}

void fnGenDirective( char* directive )
{
	printf( "     %s\n", directive );
}

void fnGenVariable( char* var, int type )
{
	printf( "%s: ", var );

	if ( type == INT_T )
		printf( ".word 0" );
	else if ( type == CHAR_T )
		printf( ".byte 0" );

	printf( "\n" );
}

void fnGenRFormat( char* opcode, char* rd, char* rs, char* rt, char* funct )
{
	int bComma = 0;

	printf( "     %s", opcode );

	if ( strcmp( rd, "" ) != 0 )
	{
		printf( " %s", rd );
		bComma = 1;
	}

	if ( strcmp( rs, "" ) != 0 )
	{ 
		if ( bComma )
			printf( "," );

		printf( " %s", rs );
	}

	if ( strcmp( rt, "" ) != 0 )
		printf( ", %s", rt );

	if ( strcmp( funct, "" ) != 0 )
		printf( ", %s", funct );

	printf( "\n" );
}

void fnGenIFormat( char* opcode, char* rt, char* rs, char* imm )
{
	printf( "     %s %s", opcode, rt );

	if( strcmp( rs, "" ) != 0 )
		printf( ", %s", rs );

	if ( strcmp( imm, "" ) != 0 )
		printf( ", %s", imm );

	printf( "\n" );
}

void fnGenJFormat( char* opcode, char* addr )
{
	printf( "     %s %s\n", opcode, addr );
}

void fnGenPseudoInstr( char * opcode, char * rx, char * ry, char * imm )
{
	printf( "     %s", opcode );

	if ( strcmp( rx, "" ) != 0 )
		printf( " %s", rx );

	if ( strcmp( ry, "" ) != 0 )
		printf( ", %s", ry );

	if ( strcmp( imm, "" ) != 0 )
		printf( ", %s", imm );

	printf( "\n" );
}

int fnInstrMatch( char* strInstr )
{
	if ( strcmp( g_strInstr, strInstr ) == 0 )
		return 1;

	return 0;
}

void fnGetTemporary( char * out_strReg )
{
	int regAvailable = fnGetAvailable( g_listOfTemporaries );

	if ( regAvailable == -1 )
	{
		printf( "\nError: No hay registros temporales disponibles.\n" );
		getch( );

		exit( 1 );
	}

	sprintf( out_strReg, "$t%d", regAvailable );
	fnSetOccupied( regAvailable, g_listOfTemporaries );
	g_iTemporariesUsed++;
}

void fnGetArgument( char * out_strReg )
{
	int regAvailable = fnGetAvailable( g_listOfArguments );

	if ( regAvailable == -1 )
	{
		printf( "\nError: No hay registros para argumentos disponibles.\n" );
		getch();

		exit( 1 );
	}

	sprintf( out_strReg, "$a%d", regAvailable );
	fnSetOccupied( regAvailable, g_listOfArguments );
}

int fnIsVoid( sEntry * procedure )
{
	if (  fnGetType( procedure ) == VOID_T )
		return 1;

	return 0;
}

int fnIsParameter( sEntry * variable )
{
	char * strParent;
	sEntry * entryProcedure;
	sParameter * tmpParam;

	strParent = fnGetParent( variable );

	entryProcedure = fnSearchSymbolTable( global_symbol_table, strParent, PROCEDURE, 0 );

	if ( entryProcedure )
	{
		tmpParam = entryProcedure->parameter;

		while( tmpParam )
		{
			if ( strcmp( fnGetIdParameter( tmpParam ), fnGetString( variable ) ) == 0 )
				return 1;

			tmpParam =  fnGetNextParameter( tmpParam );
		}
	}

	return 0;
}

int fnGetNumberOfParameters( sEntry *  procedure )
{
	int count = 0;
	sParameter* tmpParam;

	if ( procedure->parameter )
	{
		tmpParam = procedure->parameter;

		while ( tmpParam != NULL )
		{
			tmpParam = tmpParam->next;
			count++;
		}
	}

	return count;
}

int fnRegisterType( char *strReg )
{
	if ( strReg[ 1 ] == 't' )
		return REGISTER_TEMPORARY;

	if (strReg[ 1 ] == 'a')
		return REGISTER_ARGUMENT;

	return -1;
}

// TODO: MEJORAR
int fnGetNumberOfRegister( char * strReg )
{
	return (int)strReg[ 2 ] - 48;
}

void fnInsertRegister( char *strReg )
{
	int regType = fnRegisterType( strReg );

	if ( regType == REGISTER_TEMPORARY )
	{
		fnSetAvailable( fnGetNumberOfRegister( strReg ), g_listOfTemporaries );
		g_iTemporariesUsed--;
	}
	else if ( regType == REGISTER_ARGUMENT )
		fnSetAvailable( fnGetNumberOfRegister( strReg ), g_listOfArguments );
}

void fnSaveTemporariesUsed( )
{
	int i;
	char strReg0[ 4 ], strAuxReg[ 9 ];

	for ( i = 0; i < g_iTemporariesUsed; i++ )
	{
		sprintf( strReg0, "$t%d", i );
		sprintf( strAuxReg, "%d($sp)", 4 * ( i + 1 ) );

		// fnInsertRegister( strReg0 );
		fnGenIFormat( "sw", strReg0, strAuxReg, "" );
	}
}

void fnSaveArgumentsUsed( int iNumberOfParameters )
{
	int i;
	char strReg0[ 4 ], strAuxReg[ 9 ];

	for ( i = 0; i < iNumberOfParameters; i++ )
	{
		sprintf( strReg0, "$a%d", i );
		sprintf( strAuxReg, "%d($sp)", 4 * i );

		fnInsertRegister( strReg0 );
		fnGenIFormat( "sw", strReg0, strAuxReg, "" );
	}
}

int fnIsArgument( sEntry *entryProcedure, char * strArg )
{
	sParameter* tmpParam;

	if ( entryProcedure->parameter )
	{
		tmpParam = entryProcedure->parameter;
		g_iNumParam = 0;

		while( tmpParam != NULL )
		{
			if ( strcmp( tmpParam->id, strArg ) == 0 )
				return 1;

			tmpParam = tmpParam->next;
			g_iNumParam++;
		}
	}

	return 0;
}

void fnInitListOfRegisters( stList * L, int numOfElements )
{
	int i;

	for ( i = 0; i < numOfElements; i++ )
		fnSetAvailable( i, L );

	fnSetFirstAvailable( 0, L );
}

void fnInitListOfTemporaries( )
{
	fnInitListOfRegisters( g_listOfTemporaries, 8 );
}

void fnInitListOfArguments( )
{
	fnInitListOfRegisters( g_listOfArguments, 4 );
}
