/********************************************************************
	file:		decoder.h
	date:		2017/08/07
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	
********************************************************************/
/*
Tabla de operaciones
+------+------------+-------------------------------------------+-------+---------+-------+
| Used |  Mnemonic 	|				Meaning						| Type	|  Opcode | Funct |
+------+------------+-------------------------------------------+-------+---------+-------+
|  1   |	add		|	Add										|	R	|	0x00  |	0x20  |
|  1   |	addi	|	Add Immediate							|	I	|	0x08  |	NA    |
|      |	addiu	|	Add Unsigned Immediate					|	I	|	0x09  |	NA    |
|  1   |	addu	|	Add Unsigned							|	R	|	0x00  |	0x21  |
|      |	and		|	Bitwise AND								|	R	|	0x00  |	0x24  |
|      |	andi	|	Bitwise AND Immediate					|	I	|	0x0C  |	NA    |
|  1   |	beq		|	Branch if Equal							|	I	|	0x04  |	NA    |
|  1   |	bne		|	Branch if Not Equal						|	I	|	0x05  |	NA    |
|  1   |	div		|	Divide									|	R	|	0x00  |	0x1A  |
|  1   |	divu	|	Unsigned Divide							|	R	|	0x00  |	0x1B  |
|  1   |	j		|	Jump to Address							|	J	|	0x02  |	NA    |
|  1   |	jal		|	Jump and Link							|	J	|	0x03  |	NA    |
|  1   |	jr		|	Jump to Address in Register				|	R	|	0x00  |	0x08  |
|      |	lbu		|	Load Byte Unsigned						|	I	|	0x24  |	NA    |
|      |	lhu		|	Load Halfword Unsigned					|	I	|	0x25  |	NA    |
|      |	lui		|	Load Upper Immediate					|	I	|	0x0F  |	NA    |
|  1   |	lw		|	Load Word								|	I	|	0x23  |	NA    |
|  1   |	mfhi	|	Move from HI Register					|	R	|	0x00  |	0x10  |
|  1   |	mflo	|	Move from LO Register					|	R	|	0x00  |	0x12  |
|      |	mfc0	|	Move from Coprocessor 0					|	R	|	0x10  |	NA    |
|  1   |	mult	|	Multiply								|	R	|	0x00  |	0x18  |
|  1   |	multu	|	Unsigned Multiply						|	R	|	0x00  |	0x19  |
|      |	nor		|	Bitwise NOR (NOT-OR)					|	R	|	0x00  |	0x27  |
|      |	xor		|	Bitwise XOR (Exclusive-OR)				|	R	|	0x00  |	0x26  |
|      |	or		|	Bitwise OR								|	R	|	0x00  |	0x25  |
|      |	ori		|	Bitwise OR Immediate					|	I	|	0x0D  |	NA    |
|      |	sb		|	Store Byte								|	I	|	0x28  |	NA    |
|      |	sh		|	Store Halfword							|	I	|	0x29  |	NA    |
|  1   |	slt		|	Set to 1 if Less Than					|	R	|	0x00  |	0x2A  |
|      |	slti	|	Set to 1 if Less Than Immediate			|	I	|	0x0A  |	NA    |
|      |	sltiu	|	Set to 1 if Less Than Unsigned Immediate|	I	|	0x0B  |	NA    |
|      |	sltu	|	Set to 1 if Less Than Unsigned			|	R	|	0x00  |	0x2B  |
|      |	sll		|	Logical Shift Left						|	R	|	0x00  |	0x00  |
|      |	srl		|	Logical Shift Right (0-extended)		|	R	|	0x00  |	0x02  |
|      |	sra		|	Arithmetic Shift Right (sign-extended)	|	R	|	0x00  |	0x03  |
|  1   |	sub		|	Subtract								|	R	|	0x00  |	0x22  |
|  1   |	subu	|	Unsigned Subtract						|	R	|	0x00  |	0x23  |
|  1   |	sw		|	Store Word								|	I	|	0x2B  |	NA    |
+------+------------+-------------------------------------------+-------+---------+-------+
//*/
#pragma once
/************************************************************************/
/*						GLOBAL OPCODE CONSTANTS                         */
/************************************************************************/

#define OP_SPECIAL	0 // 0x00
#define OP_J		2 // 0x02
#define OP_JAL		3 // 0x03
#define OP_BEQ		4 // 0x04
#define OP_BNE		5 // 0x05
#define OP_ADDIU	9 // 0x09
#define OP_LW		35// 0x23
#define OP_SW		43// 0x2B

int* g_OPCODES; // strings representing MIPS opcodes

/************************************************************************/
/*						GLOBAL FUNCTION CONSTANTS						*/
/************************************************************************/

#define FCT_NOP		0
#define FCT_JR		8
#define FCT_SYSCALL	12
#define FCT_MFHI	16
#define FCT_MFLO	18
#define FCT_MULTU	25
#define FCT_DIVU	27
#define FCT_ADDU	33
#define FCT_SUBU	35
#define FCT_SLT		42

int* g_FUNCTIONS; // strings representing MIPS functions

/************************************************************************/
/*						GLOBAL DECODER VARIABLES                        */
/************************************************************************/

/** mnemonic para la instrucción a realizar. */
int g_iOpCode;
/** Source register o registro fuente. */
int g_iRS;
/** Source register o registro fuente. */
int g_iRT;
/** Destination register o registro destino. */
int g_iRD;
/** Valor inmediato no mayor a 2^16, el bit más significativo es el signo. */
int g_iImmediate;
/** Para diferencias los OpCode identicos. */
int g_iFunction;
/** Dirección destino acortada de 26bits, los 2 LSBits y 4 MSBits son removidos. */
int g_iAddress;


/************************************************************************/
/*						GLOBAL DECODER FUNCTIONS                        */
/************************************************************************/
void fnInitDecoder( );
void fnUninitializeDecoder( );
void fnDecodeNextInstruction( );