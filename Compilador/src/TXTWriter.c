#include "../includes/TXTWriter.h"
#include <stdio.h>
#include <stdlib.h>

FILE *g_pFile;
static int NO_LABEL = -1;

void fnOpenFileToWrite( char* strNameOfFile )
{
	// aqui debemos especificar donde queremos generar el codigo intermedio o al enviar el parametro?
	g_pFile = fopen( strNameOfFile, "w" );
	if( g_pFile == NULL )
	{
		printf( "Error: couldn't open the file '%s'\n", strNameOfFile );
		exit( 1 );
	}
}


void fnCloseFile( )
{
	fclose( g_pFile );
}


void fnWrite( char* instr, char* arg, int iLabel )
{
	fprintf( g_pFile, "%s", instr );
	fprintf( g_pFile, "%c", ' ' );
	fprintf( g_pFile, "%s", arg );

	if( iLabel != NO_LABEL )
		fprintf( g_pFile, "%d", iLabel );
}

