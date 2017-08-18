/********************************************************************
	file:		symbolTable.h
	date:		2017/07/26
	author: 	Luis Escutia, Miguel Corona
	Contact:	escutialuis93@gmail.com
				fmiguelcorona@gmail.com

	Purpose:	
********************************************************************/

#pragma once
typedef struct sParameter
{
	struct sParameter* next;
	char* id;
	int type;
} sParameter;

typedef struct sEntry
{
	struct sEntry*	next; 	// pointer to next entry
	char*	string;			// identifier string, string literal
	char*	parent;
	int 	line;			// source line number
	int 	class;			// VARIABLE, PROCEDURE, STRING
	int 	type;			// INT_T, INTSTAR_T, VOID_T, CHAR_T, CHARSTAR_T
	int 	value;			// VARIABLE: initial value
	int 	address;		// VARIABLE: offset, PROCEDURE: address, STRING: offset
	int 	scope;			// REG_GP, REG_FP pertenecen a los registros
	int 	defined;		// flag to know if the entry is defined

	struct sParameter* parameter;// 
} sEntry;

// classes
static int VARIABLE  = 1;
static int PROCEDURE = 2;
static int STRING    = 3;

// types
static int INT_T      = 1;
static int INTSTAR_T  = 2;
static int VOID_T     = 3;

//Tipos  no soportados por C*
static int CHAR_T     = 4;
static int CHARSTAR_T = 5;

static int GLOBAL_TABLE  = 1;
static int LOCAL_TABLE   = 2;
static int LIBRARY_TABLE = 3;

// ------------------------ GLOBAL VARIABLES -----------------------

// table pointers
sEntry* global_symbol_table;
sEntry* local_symbol_table;
sEntry* library_symbol_table;

int numberOfGlobalVariables;
int numberOfProcedures;
int numberOfStrings;

/************************************************
*					FUNCTIONS					*
*************************************************/

/*
*	@Param whichTable 	es la tabla en la que se desea insertar
*	@Param string 		es el nombre del identificador
*	@Param line 		es el numero de la linea
*	@Param class 		es la clase a la que corresponde, puede ser procedimiento, variable o cadena
*	@Param type 		tipo al que corresponde, puede ser int,int*,char,char*,void
*	@Param value 		valor inicial
*	@Param address 		es la direccion de memoria en caso de procedimiento, de otro modo es un offset
*	@Param defined 		es una bandera para indicar si se esta definiendo el valor
*/
void fnCreateSymbolTableEntry( int whichTable, char* string, int line, int class, int type, int value, int address, int defined, char* parent );
void fnResetSymbolTables( );
void fnInitializeSymbolTables( );
void fnPrintTable( int whichTable );
sEntry* fnSearchSymbolTable( sEntry* entry, char* string, int class, char* actualProc );
sEntry* fnGetScopedSymbolTableEntry( char* string, int class, char* actualProc );
//funciones para buscar procedimientos de la libreria de kirsch
//int fnReportUndefinedProcedures();
//int fnIsUndefinedProcedure(int* entry);

/*
*	@Param entry 	es la entrada de la cual se desea obtener la siguiente.
*	@Return 		regresa una estructura a la siguiente entrada
*/
sEntry* fnGetNextEntry	( sEntry* entry );
/*
*	@Param entry 	es la entrada de la cual se desea obtener la cadena
*	@Return 		regresa una cadena del identificador o una literal
*/
char* fnGetString	( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener el numero de linea
*	@Return regresa el numero de linea donde fue definido
*/
int  fnGetLineNumber( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener su clase
*	@Return regresa una clase el cual puede ser una variable, un procedimiento o una cadena
*/
int  fnGetClass		( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener su tipo
*	@Return regresa si es un int, int*, char, char*, void
*/
int  fnGetType		( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener su valor
*	@Return en caso de ser una variable regresa su valor inicial
*/
int  fnGetValue		( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener su direccion de memoria
*	@Return regresa en caso de ser variable u string un offset y si es un procedimiento su direccion
*/
int  fnGetAddress	( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener su alcance
*	@Return regresa REG_GP, REG_FP los cuales pertenecen a los registros (global pointer, frame pointer)
*/
int  fnGetScope 	( sEntry* entry );
/*
*	@Param entry es la entrada de la cual se desea obtener una bandera de si esta definido el valor o no
*	@Return regresa
*/
int  fnIsDefined	( sEntry* entry );
char* fnGetParent 	( sEntry* entry );

void fnSetNextEntry	( sEntry* entry, sEntry* next );
void fnSetString	( sEntry* entry, char* identifier );
void fnSetLineNumber( sEntry* entry, int line );
void fnSetClass		( sEntry* entry, int class );
void fnSetType		( sEntry* entry, int type );
void fnSetValue		( sEntry* entry, int value );
void fnSetAddress	( sEntry* entry, int address );
void fnSetScope		( sEntry* entry, int scope );
void fnSetDefined	( sEntry* entry, int defined );
void fnSetParent	( sEntry* entry, char* parent );
int fnAddParameter	( sEntry* entry, char* varname, int type );
void fnPrintType( int type );

// PARA VERIFICAR EL NÚMERO DE PARÁMETROS
sParameter* fnGetNextParameter( sParameter* parameter );
char* fnGetIdParameter( sParameter* parameter );
int fnGetTypeParameter( sParameter* parameter );
void fnCreateParameter( sEntry* entry, int type, char* id );
//

// ------------------------ GLOBAL CONSTANTS -----------------------
