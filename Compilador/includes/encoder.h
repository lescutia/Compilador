/********************************************************************
	file:		encoder.h
	date:		2017/08/06
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	Codificar instrucciones.
********************************************************************/

#pragma once
/** 
*	Desc: función para codificar una instrucción usando registros.
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param rs es el registro donde se guardara el valor.
*	@Param rt carga el valor correspondiente al registro enviado.
*	@Param rd carga el valor correspondiente al registro enviado.
*	@Param function la función a realizar con el operador
*	@Return regresa la instrucción a realizar codificada en un entero.
*/
int encodeRFormat( int opcode, int rs, int rt, int rd, int function );

/**
*	Desc: función para codificar una instrucción.
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param rs es el registro donde se guardara el valor.
*	@Param rt carga el valor correspondiente al registro enviado.
*	@Param immediate carga el valor estatico enviado.
*	@Return regresa la instrucción a realizar codificada en un entero.
*/
int encodeIFormat( int opcode, int rs, int rt, int immediate );

// TODO: para que es el instr_index?
/**
*	Desc: función para codificar una instrucción.
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param instr_index 
*	@Return regresa la instrucción a realizar codificada en un entero.
*/
int encodeJFormat( int opcode, int instr_index );