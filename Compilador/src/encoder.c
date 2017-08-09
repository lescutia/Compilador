#include "../includes/encoder.h"
#include "../CompilerPCH.h"

int encodeRFormat( int opcode, int rs, int rt, int rd, int funct )
{
	return fnLeftShift( fnLeftShift( fnLeftShift( fnLeftShift( opcode, 5 ) + rs, 5 ) + rt, 5 ) + rd, 11 ) + funct;
	//return ( ( ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 5 ) + rd ) << 11 ) + function;
}

int encodeIFormat( int opcode, int rs, int rt, int IMM )
{
	// immediate debe estar en el rango de -2^16 a 2^16
	// si es negativo, se hace complemento a dos para quedarnos con los bits que nos interesan.
	if( IMM < 0 )
		IMM = IMM + fnLeftShift( 1, 16 );

	return fnLeftShift( fnLeftShift( fnLeftShift( opcode, 5 ) + rs, 5 ) + rt, 16 ) + IMM;
	//return ( ( ( ( ( opcode << 5 ) + rs ) << 5 ) + rt ) << 16 ) + immediate;
}

int encodeJFormat( int opcode, int instr_index )
{
	return fnLeftShift( opcode, 26 ) + instr_index;
	// return (opcode << 26) + instr_index
}
