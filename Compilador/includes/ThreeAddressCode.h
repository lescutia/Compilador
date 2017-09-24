#pragma once

#define MAX_INSTR 50
#define MAX_BLOCKS 50
#define MAX_GRAPHS 10

#define NO_EXPRESSION -1
#define UNARY_EXPRESSION 0
#define BINARY_EXPRESSION 1
#define INST_RETURN 2

typedef struct _instr
{
	char operand1[ 124 ],
		 Op[ 10 ],
		 operand2[ 124 ],
		 tmp[ 124 ];
	int kind;
} stInstr;

typedef struct _block
{
	int numOfInstr;
	stInstr instr[ MAX_INSTR ];
	int next, jump;
} stBlock;

typedef struct _graph
{
	int numOfBlocks;
	stBlock block[ MAX_BLOCKS ];
} stGraph;

// void fnNewBlock( stGraph * G );

/**
 * @Brief Inicializa un bloque.
 */
// stBlock * fnInitBlock( );
void fnInitBlock( stBlock * B );

/**
 * @Brief Inicializa un grafo.
 */
// stGraph * fnInitGraph( );
void fnInitGraph( stGraph * G );

/**
 * @Brief Libera la memoria que fue solicitada por fnInitBlock.
 * @Param B		Puntero al bloque que va a ser destruido.
 */
// void fnDestroyBlock( stBlock * B );

/**
 * @Brief Libera la memoria que fue solicitada por fnInitGraph.
 * @Param G		Puntero al grafo que va a ser destruido.
 * @See fnInitGraph.
 */
// void fnDestroyGraph( stGraph * G );

/**
 * @Brief Agrega una instrucción a un bloque.
 * @Param B				Puntero al bloque.
 * @Param strOp			Operador
 * @Param strOperand1	Primer operando
 * @Param strOperand2	Segundo operando
 * @Param strResult		Resultado
 */
void fnAddInstr( stBlock * B, char * strOp, char * strOperand1, char * strOperand2, char * strResult, int kind );

/**
 * @Param pos		Posición en el cual se encuentra la instrucción.
 * @Param B			Puntero al bloqueB.
 */
void fnRemoveInstr( int pos, stBlock * B );

/**
 * @Param B
 * @Param next
 */
void fnSetNextBlock( stBlock * B, int next );

/**
 * @Param B
 * @Param jump
 */
void fnSetJumpToBlock( stBlock * B, int jump );

/*
 * @Param B		Puntero al bloque del cual se desea saber el tipo
 *				de la última instrucción.
 */
int fnKindOfLastInstr( stBlock * B );

/**
 * @Param B		Puntero al bloque que se va a imprimir.
 */
void fnPrintBlock( stBlock * B );

/**
 * @Param G				Puntero al  primer grafo.
 * @Param numOfGraphs	Número de grafos que se van a imprimir.
 */
void fnPrintFlowGraphs( stGraph * G, int numOfGraphs );

/**
 * @Param G		Puntero al grafo que se va a imprimir.
 */
void fnPrintGraph( stGraph * G );

/**
 * @Param B		Puntero al bloque que se va a imprimir.
 */
void fnBlockToTACode( stBlock * B );

/**
 * @Param G		Grafo que se va a pasar a código de tres direcciones.
 */
void fnGraphToTACode( stGraph * G );

/**
 * @Param instr	Instrucción.
 */
int fnHasLeft( stInstr instr );

/**
 * @Param strOperand	Operando.
 * @Param B				Bloque de instrucciones.
 */
char * fnCopyValue( char * strOperand, stBlock * B );

/**
 * @Param B		Puntero al bloque, en el cual se va a realizar la propagación de copias.
 */
void fnLocalCopyProp( stBlock * B );

/**
 * @Param strOp		Operador.
 */
int fnIsCommutative( char * strOp );

/**
 * @Brief Compara si las dos expresiones, binarias, coinciden.
 * @Param instr1	Primera instrucción.
 * @Param instr2	Segunda instrucción.
 */
int fnMatchBinaryInstr( stInstr instr1, stInstr instr2 );

/**
 * @Brief Local Common-Subexpression Elimination
 * @Param B		Bloque en el cual se quieren eliminar las subexpresiones comunes locales.
 */
void fnLocalCSE( stBlock * B );

/**
 * @Param operand		Operando del cual se desea saber si es constante.
 * @Return 1 si el operando es constante; y 0 en caso contrario.
 */
int fnIsConstant( char * operand );

/**
 * @Param operand		Operando del cual se desea saber su valor numérico.
 * @Return valor numérico del operando.
 */
int fnGetValueOperand( char * operand );

/**
 * @Param operand1		Operando 1
 * @Param op			Operador
 * @Param operand2		Operando 2
 * @Param result		Puntero a la cadena en la cual se guardará el resultado de realizar
 *						la operación.
 */
void fnPerformBinryOperation( char * operand1, char * op, char * operand2, char * result );

/**
 * @Brief Constant-Expression Evaluation (Constant Folding)
 * @Param I		Instrucción que se quiere evalaur.
 */
void fnConstEval( stInstr * I );
