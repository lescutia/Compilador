#include "../includes/PCodeRead.h"

char g_chCurrChar = ' ';
char g_strInstr[ INSTR_SIZE ];
char g_strArg[ MAX_ARG_SIZE ];

void fnOpenPCodeToRead(char* strNameOfFile)
{
	g_pPCodeFile = fopen( strNameOfFile, "r" );

	if (g_pPCodeFile == NULL)
	{
		printf( "Error: Could not open the file '%s'", strNameOfFile );
		printf( "\n\nPress any key to exit..." );

		exit( 1 );
	}
}

void fnClosePCode( )
{
    fclose( g_pPCodeFile );
}

void fnGetChar( )
{
    g_chCurrChar = fgetc( g_pPCodeFile );
}

int fnHasInstr( )
{
    fnGetChar( );

    if ( g_chCurrChar != '\n' && g_chCurrChar != EOF )
        return 1;

    return 0;
}

int fnHasArg( )
{
    if ( g_chCurrChar == ' ' )
        return 1;

    return 0;
}

void fnGetInstr( )
{
    int i;

    for( i = 0; i < INSTR_SIZE; i++ )
    {
        if( g_chCurrChar == EOF )
        {
            printf( "\nError: Invalid instruction, reached EOF.\n" );
            exit( 1 );
        }

        g_strInstr[ i ] = g_chCurrChar;

        fnGetChar( );
    }
}

void fnGetArg( )
{
    int i = 0;

    fnGetChar( );

    while( g_chCurrChar != '\n' && i < MAX_ARG_SIZE )
    {
        if( g_chCurrChar == EOF )
        {
            printf( "\nErro: Invalid argument '%s', reached EOF.\n", g_strArg );
            exit( 1 );
        }

        g_strArg[ i ] = g_chCurrChar;

        fnGetChar( );
        i++;
    }

    g_strArg[ i ] = '\0';
}

int fnInstrMatch( char* strInstr )
{
	if (strcmp( g_strInstr, strInstr ) == 0)
		return 1;

	return 0;
}
