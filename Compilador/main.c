#include "CompilerPCH.h"
#include <conio.h>

int main( int argc, char** argv )
{
	initCompiler( );
	char* strFilename;
	
	//Example... a.exe "Test\test7.txt" > out.txt
	if( argc > 1 )
		strFilename = argv[ 1 ];
	else
		strFilename = "Test/test10.c";

	g_sourceFD = fopen(strFilename, "r" );
	
	if( g_sourceFD != NULL )
	{
		fnOpenFileToWrite( "testfile.txt" );

		fnInitScanner( );    // Incializar el scanner
		fnReadCharacter( );  // Leer el primer carácter
		fnGetSymbol( );      // Obtener el primer token
		fnParser( );         // Iniciar el parser
		
		fnCloseFile( );
		fclose( g_sourceFD ); // Cerrar el archivo
	}
	else
	{
		printf( "Could not open input file: '%s'", strFilename );
		printf( "\n\n Press any key to exit..." );
		_getch();

		exit( 1 );
	}

	/*fnPrintTable( GLOBAL_TABLE );
	fnPrintTable( LOCAL_TABLE );*/
	
	fnUninitializeScanner( );
	fnResetSymbolTables( );

	printf( "\n Press any key to exit..." );
	_getch( );

	return 0;
}
