/********************************************************************
	file:		CompilerPCH.h
	date:		2017/07/26
	author: 	Luis Escutia, Miguel Corona
	Contact:	escutialuis93@gmail.com
				fmiguelcorona@gmail.com

	Purpose:	
********************************************************************/

#pragma once
/************************************************
*					STD INCLUDES				*
*************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <vld.h>

/************************************************
*				SYMBOL DEFINES					*
*************************************************/

#define SYM_EOF          -1	// end of file
#define SYM_IDENTIFIER   0	// identifier
#define SYM_INTEGER      1	// integer
#define SYM_VOID         2	// VOID
#define SYM_INT          3	// INT
#define SYM_SEMICOLON    4	// ;
#define SYM_IF           5	// IF
#define SYM_ELSE         6	// ELSE
#define SYM_PLUS         7	// +
#define SYM_MINUS        8	// -
#define SYM_ASTERISK     9	// *
#define SYM_DIV          10	// /
#define SYM_EQUALITY     11	// ==
#define SYM_ASSIGN       12	// =
#define SYM_LPARENTHESIS 13	// (
#define SYM_RPARENTHESIS 14	// )
#define SYM_LBRACE       15	// {
#define SYM_RBRACE       16	// }
#define SYM_WHILE        17	// WHILE
#define SYM_RETURN       18	// RETURN
#define SYM_COMMA        19	// ,
#define SYM_LT           20	// <
#define SYM_LEQ          21	// <=
#define SYM_GT           22	// >
#define SYM_GEQ          23	// >=
#define SYM_NOTEQ        24	// !=
#define SYM_MOD          25	// %
#define SYM_CHARACTER    26	// character
#define SYM_STRING       27	// string

// No es parte de C*
#define SYM_CHAR         28	// char

// No es parte de C*
#define SYM_OCTOTHORPE	 29	// #

// No es parte de C*
#define SYM_INCLUDE		 30	// include
/************************************************
*					CHAR DEFINES				*
*************************************************/

#define CHAR_EOF          -1 // end of file
#define CHAR_TAB          9  // ASCII code 9  = tabulator
#define CHAR_LF           10 // ASCII code 10 = line feed
#define CHAR_CR           13 // ASCII code 13 = carriage return
#define CHAR_SPACE        ' '
#define CHAR_SEMICOLON    ';'
#define CHAR_PLUS         '+'
#define CHAR_DASH         '-'
#define CHAR_ASTERISK     '*'
#define CHAR_SLASH        '/'
#define CHAR_UNDERSCORE   '_'
#define CHAR_EQUAL        '='
#define CHAR_LPARENTHESIS '('
#define CHAR_RPARENTHESIS ')'
#define CHAR_LBRACE       '{'
#define CHAR_RBRACE       '}'
#define CHAR_COMMA        ','
#define CHAR_LT           '<'
#define CHAR_GT           '>'
#define CHAR_EXCLAMATION  '!'
#define CHAR_PERCENTAGE   '%'
#define CHAR_SINGLEQUOTE  39 // ASCII code 39 = '
#define CHAR_DOUBLEQUOTE  '"'

// No es parte de C*
#define CHAR_OCTOTHORPE   '#'

/************************************************
*				GLOBAL VARIABLES				*
*************************************************/
char** g_SYMBOLS; // strings representing g_symbols
//char* g_character_buffer;
char g_currCharacter;  //most recently read character
int g_symbol; // most recently recognized symbol

FILE* g_sourceFD;

#define MaxIdentifierLength 64 // maximum number of characters in an identifier
#define MaxIntegerLength    10 // maximum number of characters in an integer
#define MaxStringLength		128 // maximum number of characters in a string

#define SAFE_RELEASE( x ) if( x ){ free( x ); }

char g_identifier	[ MaxIdentifierLength ];
char g_integer		[ MaxIntegerLength ];
char g_string		[ MaxStringLength ];

char g_literal[ 1 ]; // stores value of scanned character

int g_lineNumber;

int g_numberOfReadCharacters;
int g_numberOfIgnoredCharacters;
int g_numberOfComments;
int g_numberOfScannedSymbols;

/************************************************
*				LIBRARY INCLUDES				*
*************************************************/
void initCompiler( );
#include "includes/intermediateCode.h"
#include "includes/symbolTable.h"
#include "includes/scanner.h"
#include "includes/parser.h"
