#include "./CompilerPCH.h"
#include <conio.h>

int main( int argc, char** argv )
{
	fnInitCompiler( );
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

	/* fnPrintTable( GLOBAL_TABLE );
	fnPrintTable( LOCAL_TABLE ); */

	fnOpenPCodeToRead( "codegen/testfile.txt" );

	fnOpenFileToWrite( "tacode.txt" ); // Three Address Code
	fnGenTACode();
	fnCloseFile();

	fnClosePCode();

	fnOpenPCodeToRead( "codegen/testfile.txt" );

	fnOpenFileToWrite( "testfile.s" ); // ASM
	fnGenAsmCode( );
	fnCloseFile( );

	fnClosePCode();
	
	fnUninitializeCompiler( );
	fnResetSymbolTables( );

	/* printf( "\n lines: %d\n", g_lineNumber );
	printf( " comments: %d\n", g_numberOfComments );
	printf( " characters read: %d\n", g_numberOfReadCharacters );
	printf( " characters ignored: %d\n", g_numberOfIgnoredCharacters );
	printf( " scanned symbols: %d\n", g_numberOfScannedSymbols );
	printf( " while: %d\n", g_iNumberOfWhile );
	printf( " if: %d\n", g_iNumberOfIf );
	printf( " return: %d\n", g_iNumberOfReturn );
	printf( " calls: %d\n", g_iNumberOfCalls );
	printf( " assignments: %d\n", g_iNumberOfAssignments ); */

	printf( "\n Press any key to exit..." );
	_getch( );

	return 0;
}
