#pragma once
/************************************************
*			PARSER GLOBAL VARIABLES				*
*************************************************/



/************************************************
*				PARSER FUNCTIONS				*
*************************************************/
//int parserCStar();

void	fnParser			( );
int 	fnType				( );
int		fnExpression		( );
int		fnSimpleExpression	( );
int 	fnFactor			( );
int		fnTerm				( );
void	fnVariable			( );
int 	fnInitialization	( int iType );
void	fnStatement			( );
void	fnProcedure			( char* procedure, int type );
int 	fnCall				( char* procedure );
void	fnWhile				( );
void	fnIf				( );
void	fnReturn			( );
void	fnInclude			( );

int 	fnIsExpression	( );
int 	fnIsIntOrChar	( );
int 	fnIsComparison	( );
int 	fnIsPlusOrMinus	( );
int 	fnIsLiteral		( );
int 	isExpression	( );

int		fnIsAsteriskOrDivOrMod		( );
int		fnIsRbraceOrSemicolonOrEOF	( );
int		fnIsNotRbraceOrEOF			( );

void	fnSyntaxErrorSymbol		( int expected );
void	fnSyntaxErrorUnexpected	( );
void 	fnTypeWarning			( int expected, int found );

int		fnFindNextDataTypeOrHash( );
int		fnFindNextStatement		( );
int		fnFindNextFactor		( );
