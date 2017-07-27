#include "./includes/CompilerPCH.h"
#include <conio.h>

int fnTest()
{
	printf( "Imprime algo...\n" );
}

int main(int argc, char** argv)
{
	initCompiler( );
	char* filename;
	/**
	if( argc > 0 )
	{
		for ( int i = 0; i < argc; ++i )
    		printf( "%s\n", argv[ i ] );
		return 0;
	}
	*/
	//Example... a.exe "Test\test7.txt" > out.txt
	if( argc > 1 )
		filename = argv[ 1 ];
	else
	 filename = "Test/testCodeGen.c";

	g_sourceFD = fopen ( filename, "r" );

	if ( g_sourceFD != NULL )
	{
		fnTest( );

	    fnInitScanner( );    // Incializar el scanner
        fnReadCharacter( );  // Leer el primer carácter
        fnGetSymbol( );      // Obtener el primer token
        fnParser( );         // Iniciar el parser

        fclose( g_sourceFD ); // Cerrar el archivo
	}
	else
	{
		printf( "Could not open input file: " );
		printf( "\"%s\"\n", filename );
		exit( 1 );
	}
	printf( "\n" );

	/*fnPrintTable( GLOBAL_TABLE );
	fnPrintTable( LOCAL_TABLE );*/

	fnResetSymbolTables( );
	_getch();
	return 0;
}
