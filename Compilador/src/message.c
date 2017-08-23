#include "../CompilerPCH.h"
#include "../includes/message.h"

char* fnSymbolToString( int iSymbol )
{
	return *(g_SYMBOLS + iSymbol);
}

char* fnTypeToString( int iType )
{
	if (iType == VOID_T)
		return "void";

	if (iType == INT_T)
		return "int";

	if (iType == INTSTAR_T)
		return "int*";

	if (iType == CHAR_T)
		return "char";

	if (iType == CHARSTAR_T)
		return "char*";

	return "type UNK";
}

void fnWarningMessage(int iWarningType, char* SymbolExpected, char* SymbolFound, int iLineNO, char* strName )
{
	printf("[WARNING]: ");
	switch (iWarningType)
	{
	case WARNING_CAST_MISTMATCH:
		printf( "cast mismatch in line %d. Trying to convert '%s' to '%s'.", iLineNO, SymbolFound, SymbolExpected );
		break;
	case WARNING_VAR_DEF_NUSED:
		printf( "variable '%s' defined in line %d but it is never used.", strName, iLineNO );
		break;
	case WARNING_PARAM_NDEF:
		printf("");
		break;
	case WARNING_FUNC_REDEF:
		printf( "function '%s' redefinition in line %d.", strName, iLineNO );
		break;
	case WARNING_VAR_REDEF:
		printf( "variable '%s' redefinition in line %d.", strName, iLineNO );
		break;
	default:
		printf( "type warning %d is not defined.", iWarningType );
	}

	printf( "\n" );
}


void fnErrorMessage(int iErrorType, char* iTypeExpected, char* iTypeFound, int iLineNO, char* strName)
{
	printf("[ERROR]:");
	switch (iErrorType)
	{
	case ERROR_SYNTAX_SYMBOL:
		printf( "%s expected, but %s found in line %d.", iTypeExpected, iTypeFound, iLineNO );
		break;
	case ERROR_SYNTAX_UNEXPECTED:
		printf( "unexpected symbol %s found in line %d.", iTypeFound, iLineNO );
		break;
	case ERROR_TYPE_INCOMPATIBLE:
		printf("incompatible types, expected %s but %s found in line %d", iTypeExpected, iTypeFound, iLineNO);
		break;
	case ERROR_TYPE_UNK: 
		printf( "data type unknown in line %d", iLineNO );
		if ( strcmp( iTypeExpected,"") != 0 && strcmp( iTypeFound, "" ) != 0 )
			printf( ", expexted '%s' or '%s'.", iTypeExpected, iTypeFound );
		else if (strcmp( iTypeExpected, "" ) != 0 )
			printf( ", expexted '%s'.", iTypeExpected );
		break;
	case ERROR_VAR_NDEF: 
		printf( "variable '%s' used in line %d but is not defined.", strName, iLineNO );
		break;
	case ERROR_PARAM_MISMATCH: 
		printf( "conflicting types for '%s' in line %d expected '%s' but '%s' found.", strName, iLineNO, iTypeExpected, iTypeFound );
		break;
	case ERROR_PARAM_REDEF:
		printf( "redefinition parameter '%s' from '%s' in line %d", iTypeFound, strName, iLineNO );
		break;
	case ERROR_SEMICOLON_NFOUND:
		printf( "semicolon not found. Expected in line %d.", iLineNO );
		break;
	case ERROR_FUNC_NDEF:
			printf( "procedure '%s' used in line %d but is not defined.", strName, iLineNO );
		break;
	case ERROR_FUNC_FA:
			printf( "few parameters with the function '%s' in line %d.", strName, iLineNO );
		break;
	case ERROR_FUNC_MA: 
			printf( "many parameters with the function '%s' in line %d.", strName, iLineNO );
		break;
	case ERROR_FUNC_MISMATCH:
		printf( "number of arguments does not match prototype '%s' in line %d.", strName, iLineNO );
		break;
	default:
		printf( "type error %d is not defined.", iErrorType );
	}
	printf("\n");
	getch();
	exit(iErrorType);
}
