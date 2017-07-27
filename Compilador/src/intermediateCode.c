#include "../includes/intermediateCode.h"
#include <stdio.h>
#include <stdlib.h>

FILE *g_pFile;
static int NO_LABEL = -1;

void fnOpenFileToWrite( char* strNameOfFile )
{
	// Aquí debemos especificar dónde queremos generar el código
	// intermedio o al enviar el parámetro?
	g_pFile = fopen( strNameOfFile, "w" );

	if( g_pFile == NULL )
	{
		printf( "Error: Could not open the file '%s'", strNameOfFile );
		printf("\n\nPress any key to exit...");
		_getch();

		exit( 1 );
	}
}


void fnCloseFile( )
{
	fclose( g_pFile );
}


void fnPCode( char* strInstr, char* strArg, int iLabel )
{
	fprintf( g_pFile, "%s", strInstr );
	fprintf( g_pFile, "%c", ' ' );
	fprintf( g_pFile, "%s", strArg );

	if( iLabel != NO_LABEL )
		fprintf( g_pFile, "%d", iLabel );

	fprintf(g_pFile, "\n");
}

