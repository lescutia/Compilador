#pragma once

/*
*****************
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
#define WARNING_C_M		500
#define WARNING_ND		501
#define WARNING_P_R		502


/************************************************************************/
/*								ERRORS									*/
/************************************************************************/
#define ERROR_ND		600
#define ERROR_S_NF		601
#define ERROR_S_M		602
#define ERROR_P_NF		603
#define ERROR_F_R		604
#define ERROR_V_R		605
#define ERROR_F_ND		606
#define ERROR_V_ND		607
#define ERROR_F_FA		608
#define ERROR_F_MA		609

/************************************************************************/
/*								FUNCTIONS								*/
/************************************************************************/
// if any param is equals to -1, then the param is not used.

void fnWarningMessage	( int iWarningType, int iTypeExpected, int iTypeFound, int iLineNO );
void fnErrorMessage		( int iErrorType, int iTypeExpected, int iTypeFound, int iLineNO );
