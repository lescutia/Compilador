/********************************************************************
	file:		interpreter.h
	date:		2017/08/07
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	
********************************************************************/

#pragma once
/************************************************************************/
/*							FUNCTIONS                                   */
/************************************************************************/

/** Inicializa variables globales que va a utilizar. */
void fnInitInterpreter( );

/** Ejectuta interprete. */
void fnRunInterpreter( );

/** Si existe alguna interrupción se encargara de manejarla y temrinar el programa. */
//void fnInterrupt( );


/************************************************************************/
/*							VARIABLES                                   */
/************************************************************************/

/** program counter */
int g_PC;

/** instruction register */
int g_IR;