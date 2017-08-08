/********************************************************************
	file:		MIPS_Globals.h
	date:		2017/08/07
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	
********************************************************************/

#pragma once
/************************************************************************/
/*							GLOBAL CONSTANTS                            */
/************************************************************************/

#define OP_SPECIAL	0;
#define OP_J		2;
#define OP_JAL		3;
#define OP_BEQ		4;
#define OP_BNE		5;
#define OP_ADDIU	9;
#define OP_LW		35;
#define OP_SW		43;

int* OPCODES; // strings representing MIPS opcodes

#define FCT_NOP		0;
#define FCT_JR		8;
#define FCT_SYSCALL	12;
#define FCT_MFHI	16;
#define FCT_MFLO	18;
#define FCT_MULTU	25;
#define FCT_DIVU	27;
#define FCT_ADDU	33;
#define FCT_SUBU	35;
#define FCT_SLT		42;

int* FUNCTIONS; // strings representing MIPS functions

/************************************************************************/
/*							GLOBAL VARIABLES                            */
/************************************************************************/

/** Variable para almacenar el tipo de operador utilizado. */
int g_iOpCode;
/** Registro donde se guarda el resultado de una operación. */
int g_iRS;
/**  */
int g_iRT;
int g_iRD;
int g_iImmediate;
int g_iFunction;
int g_iInstrIndex;
