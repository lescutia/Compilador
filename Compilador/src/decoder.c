#include "../CompilerPCH.h"
#include "../includes/decoder.h"

void fnInitDecoder( )
{
	g_iOpCode		= 0;
	g_iRS			= 0;
	g_iRT			= 0;
	g_iRD			= 0;
	g_iImmediate	= 0;
	g_iFunction		= 0;
	g_iAddress		= 0;

	// el opcode más grande (utilizado) es 0x2B por lo que la memoria que se necesita es 43 + 1.
	g_OPCODES = (int*)malloc( 44 * sizeof(int) );
	*( g_OPCODES + OP_SPECIAL ) = ( int )"nop";
	*( g_OPCODES + OP_J ) = ( int )"j";
	*( g_OPCODES + OP_JAL ) = ( int )"jal";
	*( g_OPCODES + OP_BEQ ) = ( int )"beq";
	*( g_OPCODES + OP_BNE ) = ( int )"bne";
	*( g_OPCODES + OP_ADDIU ) = ( int )"addiu";
	*( g_OPCODES + OP_LW ) = ( int )"lw";
	*( g_OPCODES + OP_SW ) = ( int )"sw";
	
	// la funct más grande (utilizado) es 0x2A por lo que la memoria que se necesita es 42 + 1.
	g_FUNCTIONS = (int*) malloc( 44 * sizeof( int ) );
	*( g_FUNCTIONS + FCT_NOP ) = ( int )"nop";
	*( g_FUNCTIONS + FCT_JR ) = ( int )"jr";
	*( g_FUNCTIONS + FCT_SYSCALL ) = ( int )"syscall";
	*( g_FUNCTIONS + FCT_MFHI ) = ( int )"mfhi";
	*( g_FUNCTIONS + FCT_MFLO ) = ( int )"mflo";
	*( g_FUNCTIONS + FCT_MULTU ) = ( int )"multu";
	*( g_FUNCTIONS + FCT_DIVU ) = ( int )"divu";
	*( g_FUNCTIONS + FCT_ADDU ) = ( int )"addu";
	*( g_FUNCTIONS + FCT_SUBU ) = ( int )"subu";
	*( g_FUNCTIONS + FCT_SLT ) = ( int )"slt";
}

void fnUninitializeDecoder( )
{
	SAFE_RELEASE( g_OPCODES );
	SAFE_RELEASE( g_FUNCTIONS );
}

int fnDecodeOpcode		( int opcode ) { return fnRightShift( opcode, 26 ); }
int fnDecodeRS 			( int rs ) { return fnRightShift( fnLeftShift( rs, 6 ), 27 ); } // 6 bits a la izq y luego 21 + 6 que lo habiamos recorrido.
int fnDecodeRT			( int rt ) { return fnRightShift( fnLeftShift( rt, 11 ), 27 ); }
int fnDecodeRD 			( int rd ) { return fnRightShift( fnLeftShift( rd, 16 ), 27 ); }
int fnDecodeFunct 		( int funt ) { return fnRightShift( fnLeftShift( funt, 26 ), 26 ); }
int fnDecodeImmediate	( int IMM ) { return fnRightShift( fnLeftShift( IMM, 16 ), 16 ); }
int fnDecodeAddress		( int address ) { return fnRightShift( fnLeftShift( address, 6 ), 6 ); }

void fnDecodeRFormat( )
{
	g_iRS = fnDecodeRS( g_IR );
	g_iRT = fnDecodeRT( g_IR );
	g_iRD = fnDecodeRD( g_IR );
	g_iFunction = fnDecodeFunct( g_IR );
	g_iImmediate = 0;
	g_iAddress = 0;
}

void fnDecodeIFormat( )
{
	g_iRS = fnDecodeRS( g_IR );
	g_iRT = fnDecodeRT( g_IR );
	g_iRD = 0;
	g_iFunction = 0;
	g_iImmediate = fnDecodeImmediate( g_IR );
	g_iAddress = 0;
}

void fnDecodeJFormat( )
{
	g_iRS = 0;
	g_iRT = 0;
	g_iRD = 0;
	g_iFunction = 0;
	g_iImmediate = 0;
	g_iAddress = fnDecodeAddress( g_IR );
}


void fnDecodeNextInstruction( )
{
	//g_iOpCode = fnDecodeOpcode( ir );
	if( g_iOpCode == OP_SPECIAL )
		fnDecodeRFormat( );
	else if( g_iOpCode == OP_JAL )
		fnDecodeJFormat( );
	else if( g_iOpCode == OP_J )
		fnDecodeJFormat( );
	else
		fnDecodeIFormat( );
}