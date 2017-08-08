#include "../includes/encoder.h"
#include "../CompilerPCH.h"


// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+-----+-----+-----+
// |opcode|  rs |  rt |	 rd |00000|funct|
// +------+-----+-----+-----+-----+-----+
//    6      5     5     5    5		 6
// rs,rt y rd son representacion numericas para "source registers" y "destination registers"
// funct instrucciones que comparten un opcode

int encodeRFormat( int opcode, int rs, int rt, int rd, int funct )
{
	return fnLeftShift( fnLeftShift( fnLeftShift( fnLeftShift( opcode, 5 ) + rs, 5 ) + rt, 5 ) + rd, 11 ) + funct;
	//return ( ( ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 5 ) + rd ) << 11 ) + function;
}

// --------------------------------------------------------------
// 32 bit
//
// +------+-----+-----+----------------+
// |opcode| rs  |  rt |   	  IMM	   | <- se llama "immediate"
// +------+-----+-----+----------------+
//    6      5     5          16
// rs y rt registros fuente(source) y objetivo(target)
// IMM valor inmediato (immediate) usado como valor y en otros casos como complemento a dos.

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
// |opcode|	  pseudo-address	  | <- absolute addressing
// +------+-----------------------+
//    6     		26  
// 

int encodeJFormat( int opcode, int instr_index )
{
	return fnLeftShift( opcode, 26 ) + instr_index;
	// return (opcode << 26) + instr_index
}
