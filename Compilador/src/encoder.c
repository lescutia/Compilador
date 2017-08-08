#include "../includes/encoder.h"

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+-----+-----+--------+
// |opcode|  a  |  b  |	 c	|00000|function|<- register addressing
// +------+-----+-----+-----+-----+--------+
//    6      5     5     5    5		   6

int encodeRFormat( int opcode, int rs, int rt, int rd, int function )
{
	return ( ( ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 5 ) + rd ) << 11 ) + function;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+----------------+
// |opcode| a   |  b  |   	  c		   | <- se llama "immediate"
// +------+-----+-----+----------------+
//    6      5     5          16
int encodeIFormat( int opcode, int rs, int rt, int immediate )
{
	// immediate debe estar en el rango de -2^16 a 2^16
	if( immediate < 0 )
		immediate = immediate + 1 << 16;

	return ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 16 ) + immediate;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----------------------+
// |opcode|  		  c  		  | <- absolute addressing
// +------+-----------------------+
//    6     		26  
int encodeJFormat( int opcode, int instr_index )
{
	return ( opcode << 26 ) + instr_index;
}
