#include "../CompilerPCH.h"
#include "../includes/codeGen.h"

FILE *g_pFile;
static int NO_LABEL = -1;

void fnOpenFileToWrite( char* strNameOfFile )
{
	char tmpBuffer[ MaxStringLength ] = { 0 };
	
	mkdir( "codegen" );

	strcpy( tmpBuffer, "codegen/" );
	strcat( tmpBuffer, strNameOfFile );

	g_pFile = fopen( tmpBuffer, "w" );

	if( g_pFile == NULL )
	{
		printf( "Error: Could not open the file '%s'", strNameOfFile );
		printf( "\n\nPress any key to exit..." );
		_getch( );

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

	if ( strcmp( strArg, "" ) )
	{
		fprintf( g_pFile, "%c", ' ' );
		fprintf( g_pFile, "%s", strArg );
	}

	if( iLabel != NO_LABEL )
		fprintf( g_pFile, "%d", iLabel );

	fprintf( g_pFile, "\n" );
}

void fnTargetCodeLabel( char* label )
{
	fprintf( g_pFile, "%s:\n", label );
}

void fnTargetCodeDirective( char* directive )
{
	fprintf( g_pFile, "     .%s\n", directive );
}

void fnTargetCodeVariable( char* var, int type )
{
	fprintf( g_pFile, "%s: ", var );

	if (type == INT_T)
		fprintf( g_pFile, ".word 0" );
	else if (type == CHAR_T)
		fprintf( g_pFile, ".byte 0" );

	fprintf( g_pFile, "\n" );
}

void fnTargetCodeRFormat( char* opcode, char* rd, char* rs, char* rt, char* funct )
{
	int bComma = 0;

	fprintf( g_pFile, "     %s", opcode );

	if ( strcmp( rd, "" ) != 0 )
	{
		fprintf( g_pFile, " %s", rd );
		bComma = 1;
	}

	if ( strcmp( rs, "" ) != 0 )
	{
		if ( bComma )
			fprintf( g_pFile, "," );

		fprintf( g_pFile, " %s", rs );
	}

	if ( strcmp( rt, "" ) != 0 )
		fprintf( g_pFile, ", %s", rt );

	if ( strcmp( funct, "" ) != 0 )
		fprintf( g_pFile, ", %s", funct );

	fprintf( g_pFile, "\n" );
}

void fnTargetCodeIFormat( char* opcode, char* rt, char* rs, char* imm )
{
	fprintf( g_pFile, "     %s %s", opcode, rt );

	if ( strcmp( rs, "" ) != 0 )
		fprintf( g_pFile, ", %s", rs );

	if ( strcmp( imm, "" ) != 0 )
		fprintf( g_pFile, ", %s", imm );

	fprintf( g_pFile, "\n" );
}

void fnTargetCodeJFormat( char* opcode, char* addr )
{
	fprintf( g_pFile, "     %s %s\n", opcode, addr );
}

void fnTargetCodePseudoInstr( char * opcode, char * rx, char * ry, char * imm )
{
	fprintf( g_pFile, "     %s", opcode );

	if ( strcmp( rx, "" ) != 0 )
		fprintf( g_pFile, " %s", rx );

	if ( strcmp( ry, "" ) != 0 )
		fprintf( g_pFile, ", %s", ry );

	if ( strcmp( imm, "" ) != 0 )
		fprintf( g_pFile, ", %s", imm );

	fprintf( g_pFile, "\n" );
}

void fnTACEntry( char * strName )
{
	fprintf( g_pFile, "entry %s\n", strName );
}

void fnTACBeginArgs( )
{
	fprintf( g_pFile, "begin_args\n" );
}

void fnTACArg( char * strArg )
{
	fprintf( g_pFile, "arg %s\n", strArg );
}

void fnTACCall( char * strProcedure )
{
	fprintf( g_pFile, "call %s\n", strProcedure );
}

void fnTACReturn( char * strValue )
{
	fprintf( g_pFile, "return" );

	if ( strcmp( strValue, "" ) != 0 )
		fprintf( g_pFile, " %s", strValue );

	fprintf( g_pFile, "\n" );
}

void fnTACIfFalse( char * strValue, char * strLabel )
{
	fprintf( g_pFile, "if_false %s goto %s\n", strValue, strLabel );
}

void fnTACLabel( char * strLabel )
{
	fprintf( g_pFile, "label %s\n", strLabel );
}

void fnTACGoto( char * strLabel )
{
	fprintf( g_pFile, "goto %s\n", strLabel );
}

void fnTACOperation( char * strOp, char * strOperand1, char * strOperand2, char * strResult )
{
	fprintf( g_pFile, "%s = %s", strResult, strOperand1 );

	if ( strcmp( strOp, "" ) != 0 )
		fprintf( g_pFile, " %s %s", strOp, strOperand2 );

	fprintf( g_pFile, "\n" );
}
