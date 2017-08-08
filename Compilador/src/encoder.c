#include "../includes/encoder.h"

#include "../includes/encoder.h"

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+-----+-----+--------+
// |opcode|  a  |  b  |	 c	|00000|function|
// +------+-----+-----+-----+-----+--------+
//    6      5     5     5    5		   6
// a,b y c son registros

int encodeRFormat( int opcode, int rs, int rt, int rd, int function )
{
	return fnLeftShift( fnLeftShift( fnLeftShift( fnLeftShift( opcode, 5 ) + rs, 5 ) + rt, 5 ) + rd, 11 ) + function;
	//return ( ( ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 5 ) + rd ) << 11 ) + function;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+----------------+
// |opcode| a   |  b  |   	  c		   | <- se llama "immediate"
// +------+-----+-----+----------------+
//    6      5     5          16
// a y b son registros, mientras que c es una constante o variable

int encodeIFormat( int opcode, int rs, int rt, int immediate )
{
	// immediate debe estar en el rango de -2^16 a 2^16
	// si es negativo, se hace complemento a dos para quedarnos con los bits que nos interesan.
	if( immediate < 0 )
		immediate = immediate + fnLeftShift( 1, 16 );

	return fnLeftShift( fnLeftShift( fnLeftShift( opcode, 5 ) + rs, 5 ) + rt, 16 ) + immediate;
	//return ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 16 ) + immediate;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----------------------+
// |opcode|  		  c  		  | <- absolute addressing
// +------+-----------------------+
//    6     		26  
// 

int encodeJFormat( int opcode, int instr_index )
{
	return fnLeftShift( opcode, 26 ) + instr_index;
	// return (opcode << 26) + instr_index
}
