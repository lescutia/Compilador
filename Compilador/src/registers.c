#include "../CompilerPCH.h"
#include "../includes/registers.h"

void fnInitRegisters( )
{
	g_REGISTERS = malloc( NUMBEROFREGISTERS * sizeof(int*) );

	*( g_REGISTERS + REG_ZR ) = ( int ) "$zero";
	*( g_REGISTERS + REG_AT ) = ( int ) "$at";
	*( g_REGISTERS + REG_V0 ) = ( int ) "$v0";
	*( g_REGISTERS + REG_V1 ) = ( int ) "$v1";
	*( g_REGISTERS + REG_A0 ) = ( int ) "$a0";
	*( g_REGISTERS + REG_A1 ) = ( int ) "$a1";
	*( g_REGISTERS + REG_A2 ) = ( int ) "$a2";
	*( g_REGISTERS + REG_A3 ) = ( int ) "$a3";
	*( g_REGISTERS + REG_T0 ) = ( int ) "$t0";
	*( g_REGISTERS + REG_T1 ) = ( int ) "$t1";
	*( g_REGISTERS + REG_T2 ) = ( int ) "$t2";
	*( g_REGISTERS + REG_T3 ) = ( int ) "$t3";
	*( g_REGISTERS + REG_T4 ) = ( int ) "$t4";
	*( g_REGISTERS + REG_T5 ) = ( int ) "$t5";
	*( g_REGISTERS + REG_T6 ) = ( int ) "$t6";
	*( g_REGISTERS + REG_T7 ) = ( int ) "$t7";
	*( g_REGISTERS + REG_S0 ) = ( int ) "$s0";
	*( g_REGISTERS + REG_S1 ) = ( int ) "$s1";
	*( g_REGISTERS + REG_S2 ) = ( int ) "$s2";
	*( g_REGISTERS + REG_S3 ) = ( int ) "$s3";
	*( g_REGISTERS + REG_S4 ) = ( int ) "$s4";
	*( g_REGISTERS + REG_S5 ) = ( int ) "$s5";
	*( g_REGISTERS + REG_S6 ) = ( int ) "$s6";
	*( g_REGISTERS + REG_S7 ) = ( int ) "$s7";
	*( g_REGISTERS + REG_T8 ) = ( int ) "$t8";
	*( g_REGISTERS + REG_T9 ) = ( int ) "$t9";
	*( g_REGISTERS + REG_K0 ) = ( int ) "$k0";
	*( g_REGISTERS + REG_K1 ) = ( int ) "$k1";
	*( g_REGISTERS + REG_GP ) = ( int ) "$gp";
	*( g_REGISTERS + REG_SP ) = ( int ) "$sp";
	*( g_REGISTERS + REG_FP ) = ( int ) "$fp";
	*( g_REGISTERS + REG_RA ) = ( int ) "$ra";
}

void fnUninitializeRegisters( )
{
	SAFE_RELEASE( g_REGISTERS );
}