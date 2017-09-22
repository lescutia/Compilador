#include "../CompilerPCH.h"

#pragma once

#ifndef PCODEREAD_H_INCLUDED
#define PCODEREAD_H_INCLUDED

#define INSTR_SIZE 3
#define MAX_ARG_SIZE 64

void fnClosePCode( );
void fnOpenPCodeToRead( char* strNameOfFile );

void fnGetChar( );

int fnHasInstr( );
int fnHasArg( );

void fnGetInstr( );
void fnGetArg( );

// se movió para ser compartida para la generación de código ASM y TACode
int fnInstrMatch( char * strInstr );

FILE *g_pPCodeFile;

extern char g_chCurrChar;

extern char g_strInstr[ INSTR_SIZE ];
extern char g_strArg[ MAX_ARG_SIZE ];

#endif // PCODEREAD_H_INCLUDED
