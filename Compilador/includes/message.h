#pragma once

/*
********************
*	UNK = UNKNOWN
*	FA= few arguments
*	MA= many arguments
********************
*/

/************************************************************************/
/*								WARNINGS                                */
/************************************************************************/

#define WARNING_CAST_MISSTMATCH	500
#define WARNING_VAR_DEF_NUSED	501
#define WARNING_PARAM_NDEF		502
#define WARNING_FUNC_REDEF		503
#define WARNING_VAR_REDEF		504
/* warning param redefinition */
//#define WARNING_P_R		503 //Nota: redefinicion de parametros entra como de variable

/************************************************************************/
/*								ERRORS									*/
/************************************************************************/
#define ERROR_TYPE_UNK		600
#define ERROR_VAR_NDEF		601

//#define ERROR_P_ND		602
//#define ERROR_P_NF		603
#define ERROR_PARAM_MISSMATCH	604

#define ERROR_SEMICOLON_NFOUND	605

#define ERROR_FUNC_NDEF		606
#define ERROR_FUNC_FA		607
#define ERROR_FUNC_MA		608

/************************************************************************/
/*								FUNCTIONS								*/
/************************************************************************/
// if any symbol param is equals to -1, then the param is not used.
// strName must be "" in case it dont have name.

void fnWarningMessage	( int iWarningType, int iSymbolExpected, int iSymbolFound, int iLineNO, char* strName );

// NOTE: in the case of UNK iType Found can be another expected type.
void fnErrorMessage		( int iErrorType, int iSymbolExpected, int iSymbolFound, int iLineNO, char* strName );
