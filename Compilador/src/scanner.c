#include "../includes/CompilerPCH.h"

void fnUninitializeScanner( )
{
	SAFE_RELEASE( g_SYMBOLS );
}

void fnInitScanner( )
{
	g_SYMBOLS = (char**) malloc ( 31 * sizeof( char* ) );

	//
	*( g_SYMBOLS + SYM_IDENTIFIER )   = "identifier";
	*( g_SYMBOLS + SYM_INTEGER )      = "integer";
	*( g_SYMBOLS + SYM_CHARACTER )    = "character";
	*( g_SYMBOLS + SYM_STRING )       = "string";

	//Palabras reservadas
	*( g_SYMBOLS + SYM_INCLUDE )      = "include"; // No es parte de C*

	*( g_SYMBOLS + SYM_VOID )         = "void";
	*( g_SYMBOLS + SYM_INT )          = "int";

	*( g_SYMBOLS + SYM_CHAR )         = "char";   // No es parte de C*

	*( g_SYMBOLS + SYM_WHILE )        = "while";
	*( g_SYMBOLS + SYM_RETURN )       = "return";
	*( g_SYMBOLS + SYM_IF )           = "if";
	*( g_SYMBOLS + SYM_ELSE )         = "else";

	//Delimitadores
	*( g_SYMBOLS + SYM_LPARENTHESIS ) = "(";
	*( g_SYMBOLS + SYM_RPARENTHESIS ) = ")";
	*( g_SYMBOLS + SYM_LBRACE )       = "{";
	*( g_SYMBOLS + SYM_RBRACE )       = "}";
	*( g_SYMBOLS + SYM_SEMICOLON )    = ";";
	*( g_SYMBOLS + SYM_COMMA )        = ",";

	//Operadores aritméticos
	*( g_SYMBOLS + SYM_PLUS )         = "+";
	*( g_SYMBOLS + SYM_MINUS )        = "-";
	*( g_SYMBOLS + SYM_ASTERISK )     = "*";
	*( g_SYMBOLS + SYM_DIV )          = "/";
	*( g_SYMBOLS + SYM_MOD )          = "%";

	//Operador de asignación
	*( g_SYMBOLS + SYM_ASSIGN )       = "=";

	//Operadores relacionales
	*( g_SYMBOLS + SYM_EQUALITY )     = "==";
	*( g_SYMBOLS + SYM_NOTEQ )        = "!=";
	*( g_SYMBOLS + SYM_LT )           = "<";
	*( g_SYMBOLS + SYM_LEQ )          = "<=";
	*( g_SYMBOLS + SYM_GT )           = ">";
	*( g_SYMBOLS + SYM_GEQ )          = ">=";

	//Gato, hash, pund symbol, numeral symb
	*( g_SYMBOLS + SYM_OCTOTHORPE )   = "#";

	g_currCharacter = CHAR_EOF;
	g_symbol        = SYM_EOF;
}

void fnReadCharacter( )
{
	int result = fread( &g_currCharacter, 1, 1, g_sourceFD );

	if( result == 0 )
		g_currCharacter = CHAR_EOF;
	else
		g_numberOfReadCharacters = g_numberOfReadCharacters + 1;
}

int fnGetSymbol( )
{
	int i;

	g_symbol = SYM_EOF;

	if( fnFindNextCharacter( ) != CHAR_EOF )
	{
		if( g_symbol != SYM_DIV )
		{

			if( fnIsCharacterLetter( ) ) // Verificar si es un idenitificador
			{
				i = 0;
				// resetear g_identifier
				memset( g_identifier, 0, MaxIdentifierLength );

				while( fnIsCharacterLetterOrDigitOrUnderscore( ) )
				{
					g_identifier[ i ] = g_currCharacter;
					i = i + 1;

					fnReadCharacter( );
				}

				g_symbol = fnIdentifierOrKeyword( );
			}
			else if( fnIsCharacterDigit( ) ) // Verificar si es un entero
			{
				i = 0;
				// resetear g_integer
				memset( g_integer, 0, MaxIntegerLength );

				while( fnIsCharacterDigit( ) )
				{
					g_integer[ i ] = g_currCharacter;
					i = i + 1;

					fnReadCharacter( );
				}

				// Verificar algunas condiciones...

				g_symbol = SYM_INTEGER;

			}
			else if( g_currCharacter == CHAR_SINGLEQUOTE ) // Verificar si es un carácter
			{
				fnReadCharacter( );

				if( g_currCharacter == CHAR_EOF )
				{
					fnSyntaxErrorMessage( "reached end of file looking for a character literal.\n" );

					return 0;
				}

				else
					g_literal[ 0 ] = g_currCharacter; // CAMBIAR

				fnReadCharacter( );

				if( g_currCharacter == CHAR_SINGLEQUOTE )
				{
					// REVISAR
					fnReadCharacter( );
				}
				else if( g_currCharacter == CHAR_EOF )
				{
					fnsyntaxErrorCharacter( CHAR_SINGLEQUOTE );

					return 0;
				}
				else
				{
					// REVISAR
					fnsyntaxErrorCharacter( CHAR_SINGLEQUOTE );

					return 0;
				}

				g_symbol =SYM_CHARACTER;
			}
			else if( g_currCharacter == CHAR_DOUBLEQUOTE ) // Verificar si es una cadena
			{
				fnReadCharacter( );

				i = 0;
				// resetear g_string
				memset( g_string, 0, MaxStringLength );

				while( fnISCharacterNotDoubleQuoteOrNewLineOrEOF( ) )
				{
					// CONDICIONES

					g_string[ i ] = g_currCharacter;
					i = i + 1;

					fnReadCharacter( );
				}

				if( g_currCharacter == CHAR_DOUBLEQUOTE )
					fnReadCharacter( );
				else
				{
					fnsyntaxErrorCharacter( CHAR_DOUBLEQUOTE );
					return 0;
				}

				g_symbol = SYM_STRING;
			}
			else if( g_currCharacter == CHAR_SEMICOLON ) // Verificar si es ';'
			{
				fnReadCharacter( );

				g_symbol = SYM_SEMICOLON;
			}
			else if( g_currCharacter == CHAR_PLUS ) // Verificar si es '+'
			{
				fnReadCharacter( );

				g_symbol = SYM_PLUS;
			}
			else if( g_currCharacter == CHAR_DASH ) // Verificar si es '-'
			{
				fnReadCharacter( );

				g_symbol = SYM_MINUS;
			}
			else if( g_currCharacter == CHAR_ASTERISK ) // Verificar si es '*'
			{
				fnReadCharacter( );

				g_symbol = SYM_ASTERISK;
			}
			else if( g_currCharacter == CHAR_EQUAL ) // Verificar si es '==' o '='
			{
				fnReadCharacter( );

				if( g_currCharacter == CHAR_EQUAL )
				{
					fnReadCharacter( );

					g_symbol = SYM_EQUALITY;
				}
				else
				{
					g_symbol = SYM_ASSIGN;
				}
			}
			else if( g_currCharacter == CHAR_LPARENTHESIS ) // Verificar si es '('
			{
				fnReadCharacter( );

				g_symbol = SYM_LPARENTHESIS;
			}
			else if( g_currCharacter == CHAR_RPARENTHESIS ) // Verificar si es ')'
			{
				fnReadCharacter( );

				g_symbol = SYM_RPARENTHESIS;
			}
			else if( g_currCharacter == CHAR_LBRACE ) // Verificar si es '{'
			{
				fnReadCharacter( );

				g_symbol = SYM_LBRACE;
			}
			else if( g_currCharacter == CHAR_RBRACE ) // Verificar si es '}'
			{
				fnReadCharacter( );

				g_symbol = SYM_RBRACE;
			}
			else if( g_currCharacter == CHAR_COMMA ) // Verificar si es ','
			{
				fnReadCharacter( );

				g_symbol = SYM_COMMA;
			}
			else if( g_currCharacter == CHAR_LT ) // Verificar si es '<' o '<='
			{
				fnReadCharacter( );

				if( g_currCharacter == CHAR_EQUAL )
				{
					fnReadCharacter( );

					g_symbol = SYM_LEQ;
				}
				else
				{
					g_symbol = SYM_LT;
				}
			}
			else if( g_currCharacter == CHAR_GT ) // Verificar si es '>' 0 '>='
			{
				fnReadCharacter( );

				if( g_currCharacter == CHAR_EQUAL )
				{
					fnReadCharacter( );

					g_symbol = SYM_GEQ;
				}
				else
				{
					g_symbol = SYM_GT;
				}
			}
			else if( g_currCharacter == CHAR_EXCLAMATION ) // Verificar si es '!='
			{
				fnReadCharacter( );

				if( g_currCharacter == CHAR_EQUAL )
					fnReadCharacter( );
				else
				{
					fnsyntaxErrorCharacter( CHAR_EQUAL );
					return 0;
				}

				g_symbol = SYM_NOTEQ;
			}
			else if( g_currCharacter == CHAR_PERCENTAGE ) // Verificar si es '%'
			{
				fnReadCharacter( );

				g_symbol = SYM_MOD;
			}
			else if( g_currCharacter == CHAR_OCTOTHORPE ) // Verificar si es '#'
			{
				fnReadCharacter( );

				g_symbol = SYM_OCTOTHORPE;
			}
			else // En caso de que un carácter no se reconozca
			{
				fnSyntaxErrorMessage( "found unknown character.\n" );

				return 0;
			}
		}

		g_numberOfScannedSymbols = g_numberOfScannedSymbols + 1;
	}

	return 1;
}

char fnFindNextCharacter( )
{
	int inComment = 0;

	while( 1 )
	{
		if( inComment )
		{
			fnReadCharacter( );

			if( fnIsCharacterNewLine( ) )
			{
				inComment = 0;
			}
			else if( g_currCharacter == CHAR_EOF )
			{
				return g_currCharacter;
			}
			else
			{
				g_numberOfReadCharacters = g_numberOfReadCharacters + 1;
			}
		}
		else if( fnIsCharacterWhitespace( ) )
		{
			if( g_currCharacter == CHAR_LF )
			{
				g_lineNumber = g_lineNumber + 1;
			}

			g_numberOfIgnoredCharacters = g_numberOfIgnoredCharacters + 1;

			fnReadCharacter( );
		}
		else if( g_currCharacter == CHAR_SLASH )
		{
			fnReadCharacter( );

			if( g_currCharacter == CHAR_SLASH )
			{
				inComment = 1;

				g_numberOfIgnoredCharacters = g_numberOfIgnoredCharacters + 2;
				g_numberOfComments = g_numberOfComments + 1;
			}
			else
			{
				g_symbol = SYM_DIV;

				return g_currCharacter;
			}
		}
		else
		{
			return g_currCharacter;
		}
	}
}

int fnIsCharacterNewLine( )
{
	if( g_currCharacter == CHAR_LF || g_currCharacter == CHAR_CR )
		return 1;
	return 0;
}

int fnIsCharacterWhitespace( )
{
	if( g_currCharacter == CHAR_SPACE || g_currCharacter == CHAR_TAB )
		return 1;

	return fnIsCharacterNewLine( );
}

int fnIsCharacterLetter( )
{
	if( g_currCharacter >= 'a' )
		if( g_currCharacter <= 'z' )
			return 1;

	if( g_currCharacter >= 'A' )
		if( g_currCharacter <= 'Z' )
			return 1;

	return 0;
}

int fnIsCharacterDigit( )
{
	if( g_currCharacter >= '0' )
		if( g_currCharacter <= '9' )
			return 1;

	return 0;
}

int fnIsCharacterLetterOrDigitOrUnderscore( )
{
	if( fnIsCharacterLetter( ) )
		return 1;
	else if( fnIsCharacterDigit( ) )
		return 1;
	else if( g_currCharacter == CHAR_UNDERSCORE )
		return 1;

	return 0;
}

int fnISCharacterNotDoubleQuoteOrNewLineOrEOF( )
{
	if( g_currCharacter == CHAR_DOUBLEQUOTE )
		return 0;
	else if( fnIsCharacterNewLine( ) )
		return 0;
	else if( g_currCharacter == CHAR_EOF )
		return 0;

	return 1;
}

int fnIdentifierStringMatch( char* keyword )
{
	// La función strcp regresa 0 si g_idenntifier es igual a keyword
	if( strcmp( g_identifier, keyword ) )
		return 0;

	return 1;
}

int fnIdentifierOrKeyword( )
{
	if( fnIdentifierStringMatch( "while" ) )
		return SYM_WHILE;

	if( fnIdentifierStringMatch( "if" ) )
		return SYM_IF;

	if( fnIdentifierStringMatch( "int" ) )
		return SYM_INT;

	if( fnIdentifierStringMatch( "else" ) )
		return SYM_ELSE;

	if( fnIdentifierStringMatch( "return" ) )
		return SYM_RETURN;

	if( fnIdentifierStringMatch( "void" ) )
		return SYM_VOID;

	if( fnIdentifierStringMatch( "char" ) )
		return SYM_CHAR;

	if( fnIdentifierStringMatch( "include" ) )
		return SYM_INCLUDE;

	return SYM_IDENTIFIER;
}

void fnSyntaxErrorMessage( char* message )
{
	printf( "\n Error: line %d, ", g_lineNumber );
	//printf(message);
}

void fnsyntaxErrorCharacter( char expected )
{
	printf( "\n Error: line %d, ", g_lineNumber );
	printf( "'%c' expected but ", expected );

	if( g_currCharacter == EOF )
		printf( "EOF" );
	else if( g_currCharacter == '\n' )
		printf( "'\\n'" );
	else if( g_currCharacter == '\t' )
		printf( "'\\t'" );
	else
		printf( "'%c'", g_currCharacter );

	printf( " found.\n" );
}

void fnLog( )
{
	printf( "\n Number of read characters: %d", g_numberOfReadCharacters );
	printf( "\n Number of ignored characters: %d", g_numberOfIgnoredCharacters );
	printf( "\n Number of comments: %d", g_numberOfComments );
	printf( "\n Number of scanned symbols: %d\n", g_numberOfScannedSymbols );
}
