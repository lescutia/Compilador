/********************************************************************
	file:		encoder.h
	date:		2017/08/06
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	Codificar instrucciones.
********************************************************************/
// referencia de los formatos
// https://en.wikibooks.org/wiki/MIPS_Assembly/Instruction_Formats
// http://www-id.imag.fr/~briat/perso.html/NACHOS/NACHOS_DOC/CA225b.html#Load%20and%20store

#pragma once
/** 
*	Desc: usada cuando todos los valores usados estan alojados en registros.
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param rs es el registro donde se guardara el valor.
*	@Param rt carga el valor correspondiente al registro enviado.
*	@Param rd carga el valor correspondiente al registro enviado.
*	@Param function la función a realizar con el operador
*	@Return regresa la instrucción a realizar codificada en un entero.
*/
int encodeRFormat( int opcode, int rs, int rt, int rd, int funct );

/**
*	Desc: usada cuando una instrucción debe operar en un valor inmediato y en un registro.
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param rs es el registro donde se guardara el valor.
*	@Param rt carga el valor correspondiente al registro enviado.
*	@Param immediate carga el valor estatico enviado.
*	@Return regresa la instrucción a realizar codificada en un entero.
*/
int encodeIFormat( int opcode, int rs, int rt, int immediate );

/**
*	Desc: usada cuando se necesita hacer un brinco(jump)
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param address dirección de 26 bits acortada, los 2 LSBits y los 4 MSBits son removidos y se asume que es igual a la dirección actual.
*	@Return regresa la instrucción a realizar codificada en un entero.
*/
int encodeJFormat( int opcode, int address );

/*
* Nota: instrucciones FR y FI no se utilizan ya que son reservados para operaciones de coma flotante.
*/