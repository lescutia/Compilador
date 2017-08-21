#pragma once

/*
*****************
*	D = DEFINED
*	NU = NOT USED
*	NF = NOT FOUND
*	ND = NOT DEFINED
*	M = MISSMATCH
*	R = REDEFINITION
*	FA= few arguments
*	MA= many arguments
*****************
*	P = PARAM
*	C = CAST
*****************
*	S = SEMICOLON
*****************
*	F = FUNCTION
*	V = VARIABLE

*/
/************************************************************************/
/*								WARNINGS                                */
/************************************************************************/

/* warning cast mistmatch */
#define WARNING_C_M		500
/* warning variable defined but not used */
#define WARNING_V_D_NU	501
/* warning param not defined */
#define WARNING_P_ND	502
/* warning function redefinition */
#define WARNING_F_R		503
/* warning variable redefinition */
#define WARNING_V_R		504
/* warning param redefinition */
//#define WARNING_P_R		503 //Nota: redefinicion de parametros entra como de variable

/************************************************************************/
/*								ERRORS									*/
/************************************************************************/
#define ERROR_V_ND		600

//#define ERROR_P_ND		601
//#define ERROR_P_NF		602
#define ERROR_P_M		603

#define ERROR_S_NF		603

#define ERROR_F_ND		604
#define ERROR_F_FA		605
#define ERROR_F_MA		606

/************************************************************************/
/*								FUNCTIONS								*/
/************************************************************************/
// if any param is equals to -1, then the param is not used.
// strName must be "" in case it dont have name.

void fnWarningMessage	( int iWarningType, int iTypeExpected, int iTypeFound, int iLineNO, char* strName );
void fnErrorMessage		( int iErrorType, int iTypeExpected, int iTypeFound, int iLineNO, char* strName );
