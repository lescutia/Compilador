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

// TODO: cambiar todo por defines de este archivo
/************************************************
*				SYMBOL DEFINES					*
*************************************************/

static int SYM_EOF          = -1; // end of file
static int SYM_IDENTIFIER   = 0;  // identifier
static int SYM_INTEGER      = 1;  // integer
static int SYM_VOID         = 2;  // VOID
static int SYM_INT          = 3;  // INT
static int SYM_SEMICOLON    = 4;  // ;
static int SYM_IF           = 5;  // IF
static int SYM_ELSE         = 6;  // ELSE
static int SYM_PLUS         = 7;  // +
static int SYM_MINUS        = 8;  // -
static int SYM_ASTERISK     = 9;  // *
static int SYM_DIV          = 10; // /
static int SYM_EQUALITY     = 11; // ==
static int SYM_ASSIGN       = 12; // =
static int SYM_LPARENTHESIS = 13; // (
static int SYM_RPARENTHESIS = 14; // )
static int SYM_LBRACE       = 15; // {
static int SYM_RBRACE       = 16; // }
static int SYM_WHILE        = 17; // WHILE
static int SYM_RETURN       = 18; // RETURN
static int SYM_COMMA        = 19; // ,
static int SYM_LT           = 20; // <
static int SYM_LEQ          = 21; // <=
static int SYM_GT           = 22; // >
static int SYM_GEQ          = 23; // >=
static int SYM_NOTEQ        = 24; // !=
static int SYM_MOD          = 25; // %
static int SYM_CHARACTER    = 26; // character
static int SYM_STRING       = 27; // string

// No es parte de C*
static int SYM_CHAR         = 28; // char

// No es parte de C*
static int SYM_OCTOTHORPE   = 29; // #

// No es parte de C*
static int SYM_INCLUDE      = 30; // include
/************************************************
*					CHAR DEFINES				*
*************************************************/

static int CHAR_EOF          = -1; // end of file
static int CHAR_TAB          = 9;  // ASCII code 9  = tabulator
static int CHAR_LF           = 10; // ASCII code 10 = line feed
static int CHAR_CR           = 13; // ASCII code 13 = carriage return
static int CHAR_SPACE        = ' ';
static int CHAR_SEMICOLON    = ';';
static int CHAR_PLUS         = '+';
static int CHAR_DASH         = '-';
static int CHAR_ASTERISK     = '*';
static int CHAR_SLASH        = '/';
static int CHAR_UNDERSCORE   = '_';
static int CHAR_EQUAL        = '=';
static int CHAR_LPARENTHESIS = '(';
static int CHAR_RPARENTHESIS = ')';
static int CHAR_LBRACE       = '{';
static int CHAR_RBRACE       = '}';
static int CHAR_COMMA        = ',';
static int CHAR_LT           = '<';
static int CHAR_GT           = '>';
static int CHAR_EXCLAMATION  = '!';
static int CHAR_PERCENTAGE   = '%';
static int CHAR_SINGLEQUOTE  = 39; // ASCII code 39 = '
static int CHAR_DOUBLEQUOTE  = '"';

// No es parte de C*
static int CHAR_OCTOTHORPE   = '#';

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

char g_identifier	[ MaxIdentifierLength ];
char g_integer		[ MaxIntegerLength ];
char g_string		[ MaxStringLength ];

char g_literal[ 1 ]; // stores value of scanned character

///*
int g_lineNumber;

int g_numberOfReadCharacters;
int g_numberOfIgnoredCharacters;
int g_numberOfComments;
int g_numberOfScannedSymbols;

/************************************************
*				LIBRARY INCLUDES				*
*************************************************/
void initCompiler( );
#include "symbolTable.h"
#include "scanner.h"
#include "parser.h"
