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

#define WARNING_CAST_MISTMATCH	500
#define WARNING_VAR_DEF_NUSED	501
#define WARNING_PARAM_NDEF		502
#define WARNING_FUNC_REDEF		503
#define WARNING_VAR_REDEF		504
#define WARNING_FUNC_NRETURN 505
#define WARNING_FUNC_VOID_RETURN 506
//#define WARNING_TYPE_MISMATCH 507

/* warning param redefinition */
//#define WARNING_P_R		503 //Nota: redefinicion de parametros entra como de variable

/************************************************************************/
/*								ERRORS									*/
/************************************************************************/
#define ERROR_SYNTAX_SYMBOL		600
#define ERROR_SYNTAX_UNEXPECTED	( ERROR_SYNTAX_SYMBOL + 1 )

#define ERROR_TYPE_MISMATCH ( ERROR_SYNTAX_UNEXPECTED + 1 )
#define ERROR_TYPE_INCOMPATIBLE	( ERROR_TYPE_MISMATCH + 1 )
#define ERROR_TYPE_UNK			( ERROR_TYPE_INCOMPATIBLE + 1 )
#define ERROR_VAR_NDEF			( ERROR_TYPE_UNK + 1 )

//#define ERROR_P_ND		602
//#define ERROR_P_NF		603
#define ERROR_PARAM_MISMATCH	(ERROR_VAR_NDEF + 1)
#define ERROR_PARAM_REDEF		(ERROR_PARAM_MISMATCH +1)

#define ERROR_SEMICOLON_NFOUND	(ERROR_PARAM_REDEF + 1)

#define ERROR_FUNC_NDEF		(ERROR_SEMICOLON_NFOUND + 1)
#define ERROR_FUNC_FA		(ERROR_FUNC_NDEF + 1)
#define ERROR_FUNC_MA		(ERROR_FUNC_FA	+ 1)
#define ERROR_FUNC_MISMATCH (ERROR_FUNC_MA + 1)
#define ERROR_FUNC_TYPE_MISMATCH (ERROR_FUNC_MISMATCH + 1)
/************************************************************************/
/*								FUNCTIONS								*/
/************************************************************************/
// if any symbol param is equals to -1, then the param is not used.
// strName must be "" in case it dont have name.

void fnWarningMessage	( int iWarningType, char* SymbolExpected, char* SymbolFound, int iLineNO, char* strName );

// NOTE: in the case of UNK iSymbolFound can be another expected type.
void fnErrorMessage		( int iErrorType, char* iSymbolExpected, char* iSymbolFound, int iLineNO, char* strName );


char* fnTypeToString( int iType );
char* fnSymbolToString( int iSymbol );