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
*	+------+-----+-----+-----+-----+-----+
*	|opcode|  rs |  rt |  rd |00000|funct|
*	+------+-----+-----+-----+-----+-----+
*		6      5     5     5    5	 6
*/
int encodeRFormat( int opcode, int rs, int rt, int rd, int funct );

/**
*	Desc: usada cuando una instrucción debe operar con un valor inmediato y en un registro.
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param rs es el registro donde se guardara el valor.
*	@Param rt carga el valor correspondiente al registro enviado.
*	@Param IMM carga el valor estatico enviado en caso de ser negativo, se le hace complemento a dos.
*	@Return regresa la instrucción a realizar codificada en un entero.
*	+------+-----+-----+----------------+
*	|opcode| rs  |  rt |   		IMM	    | <- se llama "immediate"
*	+------+-----+-----+----------------+
*		6      5     5          16
*/
int encodeIFormat( int opcode, int rs, int rt, int IMM );

/**
*	Desc: usada cuando se necesita hacer un brinco(jump)
*	@Param opcode es un entero para identificar el operador utilizado.
*	@Param address dirección de 26 bits acortada, ya que los 2 LSBits y los 4 MSBits son removidos y se asume que es igual a la dirección actual.
*	@Return regresa la instrucción a realizar codificada en un entero.
*	+------+------------------------+
*	|opcode|	  pseudo-address	| <- absolute addressing
*	+------+------------------------+
*		6     			26
*/
int encodeJFormat( int opcode, int address );

/*
* Nota: instrucciones FR y FI no se utilizan ya que son reservados para operaciones de coma flotante.
*/