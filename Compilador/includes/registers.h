/********************************************************************
	file:		registers.h
	date:		2017/08/09
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	
********************************************************************/

#pragma once

/************************************************************************/
/*							GLOBAL REGISTER CONSTANTS                   */
/************************************************************************/
#define NUMBEROFREGISTERS 32

#define REG_ZR 0
#define REG_AT 1
#define REG_V0 2
#define REG_V1 3
#define REG_A0 4
#define REG_A1 5
#define REG_A2 6
#define REG_A3 7
#define REG_T0 8
#define REG_T1 9
#define REG_T2 10
#define REG_T3 11
#define REG_T4 12
#define REG_T5 13
#define REG_T6 14
#define REG_T7 15
#define REG_S0 16
#define REG_S1 17
#define REG_S2 18
#define REG_S3 19
#define REG_S4 20
#define REG_S5 21
#define REG_S6 22
#define REG_S7 23
#define REG_T8 24
#define REG_T9 25
#define REG_K0 26
#define REG_K1 27
#define REG_GP 28
#define REG_SP 29
#define REG_FP 30
#define REG_RA 31

/************************************************************************/
/*							GLOBAL REGISTER VARIABLES                   */
/************************************************************************/
int* g_REGISTERS; // strings representing registers

/************************************************************************/
/*							GLOBAL REGISTER FUNCTIONS                   */
/************************************************************************/

void fnInitRegisters( );
void fnUninitializeRegisters( );
//void fnPrintRegister( int reg );