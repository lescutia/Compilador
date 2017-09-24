#include "../CompilerPCH.h"
#include "../includes/ThreeAddressCode.h"
#include <ctype.h>

/* void fnNewBlock( stGraph * G )
{
	fnSetNumOfBlocks( G, G->numOfBlocks );
	fnSetCurrBlock( G, G->currBlock + 1 );
	fnInitBlock( &G->block[ G->currBlock ] );
} */

void fnInitBlock( stBlock * B )
{
	B->numOfInstr = 0;
	B->next = -1;
	B->jump = -1;
}

void fnInitGraph( stGraph * G )
{
	G->numOfBlocks = 0;
}

/* stGraph * fnInitGraph( )
{
	stGraph * G = ( stGraph* ) malloc( sizeof( stGraph ) );
	G->numOfBlocks = 0;

	return G;
} */

/* void fnDestroyBlock( stBlock * B )
{
	free( (int*) B );
}

void fnDestroyGraph( stGraph * G )
{
	free( (int*)G );
} */

void fnAddInstr( stBlock * B, char * strOp, char * strOperand1, char * strOperand2, char * strResult, int kind )
{
	strcpy( B->instr[ B->numOfInstr ].Op, strOp );
	strcpy( B->instr[ B->numOfInstr ].operand1, strOperand1 );
	strcpy( B->instr[ B->numOfInstr ].operand2, strOperand2 );
	strcpy( B->instr[ B->numOfInstr ].tmp, strResult );

	B->instr[ B->numOfInstr ].kind = kind;

	B->numOfInstr++;
}

void fnRemoveInstr( int pos, stBlock * B )
{
	int i;

	for ( i = pos; i < B->numOfInstr - 1; i++ )
	{
		strcpy( B->instr[ pos ].Op, B->instr[ pos + 1 ].Op);
		strcpy( B->instr[ pos ].operand1, B->instr[ pos + 1 ].operand1 );
		strcpy( B->instr[ pos ].operand2, B->instr[ pos + 1 ].operand2 );
		strcpy( B->instr[ pos ].tmp, B->instr[ pos + 1 ].tmp );

		B->instr[ pos ].kind = B->instr[ pos + 1 ].kind;
	}

	B->numOfInstr--;
}

void fnSetNextBlock( stBlock * B, int next )
{
	B->next = next;
}

void fnSetJumpToBlock( stBlock * B, int jump )
{
	B->jump = jump;
}

int fnKindOfLastInstr( stBlock * B )
{
	return B->instr[ B->numOfInstr - 1 ].kind;
}

void fnPrintBlock( stBlock * B )
{
	printf( "\n instr\t opr\t opd1\t opd2\t result\n" );

	for ( int i = 0; i < B->numOfInstr; i++ )
		printf( " (%d)\t %s\t %s\t %s\t %s\n", i + 1, B->instr[ i ].Op, B->instr[i].operand1, B->instr[i].operand2, B->instr[i].tmp );
}

void fnPrintFlowGraphs( stGraph * G, int numOfGraphs )
{
	for ( int i = 0; i < numOfGraphs; i++ )
	{
		printf( "\n- Graph %d -", i + 1 );
		fnPrintGraph( &G[ i ] );
	}
}

void fnPrintGraph( stGraph * G )
{
	for ( int i = 0; i <= G->numOfBlocks; i++ )
	{
		printf( "\n Block %d; next: %d; jump: %d\n", i, G->block[ i ].next, G->block[ i ].jump );
		/* printf( "\n Block %d; next: %d; jump: %d\n", i, G->block[i].next, G->block[i].jump );
		fnPrintBlock( &G->block[ i ] ); */

		//printf( "\nBlock %d\n", i + 1 );
		fnBlockToTACode( &G->block[ i ] );
	}
}

void fnBlockToTACode( stBlock * B )
{
	for ( int i = 0; i < B->numOfInstr; i++ )
	{
		if ( strcmp( B->instr[ i ].Op, "entry" ) == 0 )
			printf( " entry %s\n", B->instr[ i ].operand1 );
		else if ( strcmp( B->instr[ i ].Op, "begin_args" ) == 0 )
			printf( " begin_args\n" );
		else if ( strcmp( B->instr[ i ].Op, "arg" ) == 0 )
			printf( " args %s\n", B->instr[ i ].operand1 );
		else if ( strcmp( B->instr[ i ].Op, "call" ) == 0 )
			printf( " call %s\n", B->instr[ i ].operand1 );
		else if ( strcmp( B->instr[ i ].Op, "return" ) == 0 )
			printf( " return %s\n", B->instr[ i ].operand1 );
		else if ( strcmp( B->instr[ i ].Op, "if_f" ) == 0 )
			printf( " if_false %s goto %s\n", B->instr[ i ].operand1, B->instr[ i ].operand2 );
		else if ( strcmp( B->instr[ i ].Op, "label" ) == 0 )
			printf( " label %s\n", B->instr[ i ].operand1 );
		else if ( strcmp( B->instr[ i ].Op, "goto" ) == 0 )
			printf( " goto %s\n", B->instr[ i ].operand1 );
		else if ( strcmp( B->instr[ i ].Op, "=" ) == 0 )
			printf( " %s %s %s\n", B->instr[ i ].tmp, B->instr[ i ].Op, B->instr[ i ].operand1 );
		else
			printf( " %s = %s %s %s\n", B->instr[ i ].tmp, B->instr[ i ].operand1, B->instr[ i ].Op, B->instr[ i ].operand2 );
	}
}

void fnGraphToTACode( stGraph * G )
{
	for ( int i = 0; i <= G->numOfBlocks; i++ )
		fnBlockToTACode( &G->block[ i ] );
}

/*******************************************************************/
/*                      LOCAL COPY PROPAGATION                     */
/*******************************************************************/

int fnHasLeft( stInstr instr )
{
	if ( instr.kind == UNARY_EXPRESSION )
		return 1;

	if ( instr.kind == BINARY_EXPRESSION )
		return 1;

	return 0;
}

// los pares son (x, y), significan que x = y
// y con bloques
//       x sería B->instr[ i ].tmp
//       y sería B->instr[ i ].operand1
char * fnCopyValue( char * strOperand, stBlock * B )
{
	int i;

	for ( i = 0; i < B->numOfInstr; i++ )
		// Si el operando es igual a x, se regresa y
		if ( strcmp( strOperand, B->instr[ i ].tmp ) == 0 )
			return B->instr[i].operand1;

	return strOperand;
}

// No es tan necesaria,se puede hacer dentro de fnLocalCopyProp
// void fnRemoveACP( char * left );

void fnLocalCopyProp( stBlock * B )
{
	int i, j;
	// cambiar por un conjunto de pares ordenados
	stBlock auxBlock;

	fnInitBlock( &auxBlock );

	for ( i = 0; i < B->numOfInstr; i++ )
	{
		if ( B->instr[ i ].kind == BINARY_EXPRESSION )
		{
			strcpy( B->instr[ i ].operand1, fnCopyValue( B->instr[i].operand1, &auxBlock ) );
			strcpy( B->instr[ i ].operand2, fnCopyValue( B->instr[i].operand2, &auxBlock ) );
		}
		else if ( B->instr[ i ].kind == UNARY_EXPRESSION )
			strcpy( B->instr[i].operand1, fnCopyValue( B->instr[i].operand1, &auxBlock ) );

		if ( fnHasLeft( B->instr[ i ] ) )
		{
			// TODO: CAMBIAR POR Remove_ACP
			for ( j = 0; j < auxBlock.numOfInstr; j++ )
			{
				if ( strcmp( B->instr[ i ].tmp, auxBlock.instr[ j ].tmp ) == 0  )
				{
					fnRemoveInstr( j, &auxBlock );
					j--;
				}
				else if ( strcmp( B->instr[ i ].tmp, auxBlock.instr[ j ].operand1 ) == 0  )
				{
					fnRemoveInstr( j, &auxBlock );
					j--;
				}
			}
			//
		}

		// Que sea una asignación, que se asigne una variable y que no sea de
		// la forma x = x
		if (strcmp( B->instr[i].Op, "=" ) == 0)
			fnAddInstr( &auxBlock, B->instr[i].Op, B->instr[i].operand1, B->instr[i].operand2, B->instr[i].tmp, B->instr[i].kind ); // cambiar
	}
}

/*******************************************************************/
/*             LOCAL COMMON - SUBEXPRESSION ELIMINATION            */
/*******************************************************************/
int fnIsCommutative( char * strOp )
{
	if (strcmp( strOp, "+" ))
		return 1;

	if (strcmp( strOp, "*" ))
		return 1;

	return 0;
}

int fnMatchBinaryInstr( stInstr instr1, stInstr instr2 )
{
	if (strcmp( instr1.Op, instr2.Op ) == 0)
	{
		if (strcmp( instr1.operand1, instr2.operand1 ) == 0)
			if (strcmp( instr1.operand2, instr2.operand2 ) == 0)
				return 1;

		if (fnIsCommutative( instr1.Op ))
		{
			if (strcmp( instr1.operand1, instr2.operand2 ) == 0)
				if (strcmp( instr1.operand2, instr2.operand1 ) == 0)
					return 1;
		}
	}

	return 0;
}

void fnLocalCSE( stBlock * B )
{
	int i, j, bFound;
	stBlock auxBlock;

	fnInitBlock( &auxBlock );

	for (i = 0; i < B->numOfInstr; i++)
	{
		bFound = 0;

		if (B->instr[i].kind == BINARY_EXPRESSION)
		{
			// TODO: CABIAR POR UN WHILE
			for (j = 0; j < auxBlock.numOfInstr; j++)
			{
				if (fnMatchBinaryInstr( B->instr[i], auxBlock.instr[j] ))
				{
					bFound = 1;

					strcpy( B->instr[i].Op, "=" );
					strcpy( B->instr[i].operand1, auxBlock.instr[j].tmp );
					strcpy( B->instr[i].operand2, "" );

					// Se convierte en una asignación
					B->instr[i].kind = UNARY_EXPRESSION;

					// TODO: REVISAR
					// Ya no es necesario seguir comparando con las demás instrucciones de auxBlock
					break;
				}
			}

			if (!bFound)
				fnAddInstr( &auxBlock, B->instr[i].Op, B->instr[i].operand1, B->instr[i].operand2, B->instr[i].tmp, B->instr[i].kind );
		}

		for (j = 0; j < auxBlock.numOfInstr; j++)
		{
			if (strcmp( B->instr[i].tmp, auxBlock.instr[j].operand1 ) == 0)
			{
				fnRemoveInstr( j, &auxBlock );
				j--; // Para no saltar un elemento
			}
			else if (strcmp( B->instr[i].tmp, auxBlock.instr[j].operand2 ) == 0)
			{
				fnRemoveInstr( j, &auxBlock );
				j--; // Para no saltar un elemento
			}
		}
	}
}

/*******************************************************************/
/*        CONSTANT-EXPRESSION EVALUATION (CONSTANT FOLDING)        */
/*******************************************************************/

int fnIsConstant( char * operand )
{
	int i = 0;

	while ( operand[ i ] != '\0' )
	{
		if ( !isdigit( operand[ i ] )  )
			return 0;

		i++;
	}

	return 1;
}

int fnGetValueOperand( char * operand )
{
	int i = 0, value = 0, relativeValue = 1, digit;

	while ( operand[ i ] != '\0' )
	{
		digit = operand[ i ] - '0';
		value += digit * relativeValue;

		relativeValue *= 10;
		i++;
	}

	return value;
}

void fnPerformBinryOperation( char * operand1, char * op, char * operand2, char * result )
{
	int opd1 = fnGetValueOperand( operand1 ),
		opd2 = fnGetValueOperand( operand2 );

	if ( strcmp( op, "+" ) == 0 )
		sprintf( result, "%d", opd1 + opd2 );
	else if ( strcmp( op, "-" ) == 0 )
		sprintf( result, "%d", opd1 - opd2 );
	else if ( strcmp( op, "*" ) == 0 )
		sprintf( result, "%d", opd1 * opd2 );
	else if ( strcmp( op, "/" ) == 0 )
		sprintf( result, "%d", opd1 / opd2 );
	else
		sprintf( result, "error" );
}

void fnConstEval( stInstr * instr )
{
	char result[ 9 ];

	if ( instr->kind == BINARY_EXPRESSION )
	{
		if ( fnIsConstant( instr->operand1 ) && fnIsConstant( instr->operand2 ) )
		{
			fnPerformBinryOperation( instr->operand1, instr->Op, instr->operand2, result );

			strcpy( instr->Op, "=" );
			strcpy( instr->operand1, result );
			strcpy( instr->operand2, "" );

			instr->kind = UNARY_EXPRESSION;
		}
	}
}
