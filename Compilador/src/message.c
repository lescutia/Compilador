#include "../CompilerPCH.h"
#include "../includes/message.h"

char* TypeToStrng(int iType)
{
	if (iType == INT_T)
		return "int";
	else if (iType == INTSTAR_T)
		return "int*";
	else if (iType == VOID_T)
		return "void";
	else if (iType == CHAR_T)
		return "char";
	else if (iType == CHARSTAR_T)
		return "char*";
	else
		return "";
}

void fnWarningMessage(int iWarningType, int iTypeExpected, int iTypeFound, int iLineNO, char* strName )
{
	printf("%s", "[WARNING]: ");

	if (iWarningType == WARNING_C_M)
		printf("cast mistmatch in line %d. Trying to convert %s to %s.\n", iLineNO, TypeToStrng(iTypeFound), TypeToStrng(iTypeExpected));
	else if (iWarningType == WARNING_V_D_NU)
		printf("variable %s defined in line %d but it is never used.\n", strName, iLineNO);
	else if (iWarningType == WARNING_F_R)
		printf("function %s redefinition in line %d.\n", strName, iLineNO);
	else if (iWarningType == WARNING_V_R)
		printf("variable %s redefinition in line %d.\n", strName, iLineNO);
	//else if (iWarningType == WARNING_P_ND)
		//printf("procedure %s used in line %d but is not defined.\n", strName, iLineNO);
	//else if (iWarningType == WARNING_P_R)
		//printf("procedure %s redefinition.\n", strName );
	else
		printf("warning with id %d is not defined.\n", iWarningType );
}


void fnErrorMessage(int iErrorType, int iTypeExpected, int iTypeFound, int iLineNO, char* strName)
{
	printf("[ERROR]:");
	if (iErrorType == ERROR_V_ND)
		printf("variable %s used in line %d but is not defined.\n", strName, iLineNO );
	//else if( iErrorType == ERROR_P_ND)
		//printf("param %s used in line %d but is not defined.\n", strName, iLineNO );
	//else if (iErrorType == ERROR_P_NF)
		//printf("param %s not found in line %d.\n", strName, iLineNO );
	else if (iErrorType == ERROR_P_M)
		printf("type mistmatch with param %s in line %d expected %s but found %s.\n", strName, iLineNO, TypeToStrng(iTypeExpected), TypeToStrng(iTypeFound) );
	else if (iErrorType == ERROR_S_NF)
		printf("semicolon not found. Expected in line %d.\n", iLineNO);
	else if( iErrorType == ERROR_F_ND)
		printf("procedure %s used in line %d but is not defined.\n", strName, iLineNO);
	else if( iErrorType == ERROR_F_FA)
		printf("few parameters with the function %s in line %d.\n", strName, iLineNO );
	else if( iErrorType == ERROR_F_MA)
		printf("many parameters with the function %s in line %d.\n", strName, iLineNO);
	else
		printf("error with id %d is not defined.\n", iErrorType );
	exit(iErrorType);
}