#include "../includes/codeGen.h"
#include "../includes/symbolTable.h"
#include "../includes/PCodeRead.h"
#include "../includes/stack.h"
#include "../includes/PCodeToTACode.h"
#include "../includes/ThreeAddressCode.h"
#include "../includes/list.h"

#define DEBUG_TACODE 0

int g_iTemp = 0;
int g_iCurrBlock = 0;
int g_iCurrGraph = -1;

// TACBLOCK
stGraph graph[ MAX_GRAPHS ];
sList g_lstLabels;
sList g_lstJumps;

//
void fnMapLabelsAndJumps( )
{
	sListNode* jumpIt = g_lstJumps.head;
	sListNode* labelIt = g_lstLabels.head;

	if( !jumpIt || !labelIt )
		return;

	jumpIt = jumpIt->next;
	while( jumpIt )
	{
		sListNode* tmp = fnGetNode( &g_lstLabels, jumpIt->value );
		if( tmp )
			graph[ g_iCurrGraph ].block[ jumpIt->block ].jump = tmp->block;
		jumpIt = jumpIt->next;
	}
}

void fnGenTACode( )
{
	int bMarkStack = 0;
	char strOpcode[ 3 ] = "",
		 strOperand1[ 128 ] = "",
		 strOperand2[ 128 ] = "",
		 strResult[ 128 ] = "",
		 strTemp[ 10 ] = "";
	stStack * stackOfPMachine;
	stStack * stackAux;
	sEntry * entry, * entryCurrProcedure;

    stackOfPMachine = fnInitStack( );
	stackAux = fnInitStack( );

	while( fnHasInstr( ) )
    {
        fnGetInstr( );

        if ( fnInstrMatch( "ent" ) )
        {
            fnGetArg( );
			fnTACGenEntry( g_strArg );

			entryCurrProcedure = fnSearchSymbolTable( global_symbol_table, g_strArg, PROCEDURE, 0 );
        }
        else if ( fnInstrMatch( "lda" ) )
        {
            fnGetArg( );
			fnPush( stackOfPMachine, g_strArg );
        }
        else if ( fnInstrMatch( "ldc" ) )
        {
            fnGetArg( );
			fnPush( stackOfPMachine, g_strArg );
        }
		else if( fnInstrMatch( "lds" ) )
		{
			fnGetArg( );
			// TODO: ADAPTAR
			fnPush( stackOfPMachine, g_strArg );
		}
        else if ( fnInstrMatch( "lod" ) )
        {
            fnGetArg( );
			fnPush( stackOfPMachine, g_strArg );
        }
        else if ( fnInstrMatch( "stn" ) )
        {
			strcpy( strOperand1, fnPop( stackOfPMachine ) );
			strcpy( strResult, fnPop( stackOfPMachine ) );

			fnTACGenOperation( "", strOperand1, "", strResult );
        }
        else if ( fnInstrMatch( "adi" ) )
        {
			fnNewTemp();

			strcpy( strOperand2, fnPop( stackOfPMachine ) );
			strcpy( strOperand1, fnPop( stackOfPMachine ) );

			sprintf( strResult, "t%d", g_iTemp );
			fnTACGenOperation( "+", strOperand1, strOperand2, strResult );

			fnPush( stackOfPMachine, strResult );
        }
        else if ( fnInstrMatch( "sbi" ) )
        {
			fnNewTemp();

			strcpy( strOperand2, fnPop( stackOfPMachine ) );
			strcpy( strOperand1, fnPop( stackOfPMachine ) );

			sprintf( strResult, "t%d", g_iTemp );
			fnTACGenOperation( "-", strOperand1, strOperand2, strResult );

			fnPush( stackOfPMachine, strResult );
        }
        else if ( fnInstrMatch( "mpi" ) )
        {
			fnNewTemp();

			strcpy( strOperand2, fnPop( stackOfPMachine ) );
			strcpy( strOperand1, fnPop( stackOfPMachine ) );

			sprintf( strResult, "t%d", g_iTemp );
			fnTACGenOperation( "*", strOperand1, strOperand2, strResult );

			fnPush( stackOfPMachine, strResult );
        }
        else if ( fnInstrMatch( "dvi" ) )
        {
			fnNewTemp();

			strcpy( strOperand2, fnPop( stackOfPMachine ) );
			strcpy( strOperand1, fnPop( stackOfPMachine ) );

			sprintf( strResult, "t%d", g_iTemp );
			fnTACGenOperation( "/", strOperand1, strOperand2, strResult );

			fnPush( stackOfPMachine, strResult );
        }
        else if ( fnInstrMatch( "mod" ) )
        {
			fnNewTemp();

			strcpy( strOperand2, fnPop( stackOfPMachine ) );
			strcpy( strOperand1, fnPop( stackOfPMachine ) );

			sprintf( strResult, "t%d", g_iTemp );
			fnTACGenOperation( "%", strOperand1, strOperand2, strResult );

			fnPush( stackOfPMachine, strResult );
        }
		else if ( fnInstrMatch( "ngi" ) )
		{
			fnNewTemp( );

			sprintf( strResult, "t%d", g_iTemp );
			sprintf( strOperand1, "-%s", fnPop( stackOfPMachine ) );
			fnTACGenOperation( "", strOperand1, "", strResult );

			fnPush( stackOfPMachine, strResult );
		}
        else if ( fnInstrMatch( "les" ) )
        {
			fnPush( stackOfPMachine, "<" );
        }
        else if ( fnInstrMatch( "leq" ) )
        {
			fnPush( stackOfPMachine, "<=" );
        }
        else if ( fnInstrMatch( "equ" ) )
        {
			fnPush( stackOfPMachine, "==" );
        }
        else if ( fnInstrMatch( "grt" ) )
        {
			fnPush( stackOfPMachine, ">" );
        }
        else if ( fnInstrMatch( "geq" ) )
        {
			fnPush( stackOfPMachine, ">=" );
        }
        else if ( fnInstrMatch( "neq" ) )
        {
			fnPush( stackOfPMachine, "!=" );
        }
        else if ( fnInstrMatch( "fjp" ) )
        {
            fnGetArg( );

			strcpy( strOpcode, fnPop( stackOfPMachine ) );
			strcpy( strOperand2, fnPop( stackOfPMachine ) );
			strcpy( strOperand1, fnPop( stackOfPMachine ) );

			fnNewTemp();
			sprintf( strTemp, "t%d", g_iTemp );

			fnTACGenOperation( strOpcode, strOperand1, strOperand2, strTemp );

			fnTACGenIfFalse( strTemp, g_strArg );
        }
        else if ( fnInstrMatch( "ujp" ) )
        {
            fnGetArg( );
			fnTACGenGoto( g_strArg );
        }
        else if ( fnInstrMatch( "lab" ) )
        {
            fnGetArg( );
			fnTACGenLabel( g_strArg );
        }
		else if ( fnInstrMatch( "mst" ) )
		{
			fnPush( stackOfPMachine, "mst" );
			fnTACGenBeginArgs( );

			bMarkStack = 1;
		}
		else if ( fnInstrMatch( "cup" ) )
		{
			fnGetArg( );			
			strcpy( strTemp, fnPop( stackOfPMachine ) );

			entry = fnSearchSymbolTable( global_symbol_table, g_strArg, PROCEDURE, 0 );

			while ( strcmp( strTemp, "mst" ) != 0 )
			{
				// TODO: Cambiar el tamaño de strTemp
				fnPush( stackAux, strTemp );				
				strcpy( strTemp, fnPop( stackOfPMachine ) );
			}

			while ( !fnIsEmpty( stackAux ) )
			{
				strcpy( strTemp, fnPop( stackAux ) );
				fnTACGenArg( strTemp );
			}

			fnTACGenCall( g_strArg );

			if ( !fnIsVoid( entry ) )
			{
				fnNewTemp( );

				sprintf( strResult, "t%d", g_iTemp );
				fnTACGenOperation( "", g_strArg, "", strResult );

				fnPush( stackOfPMachine, strResult );
			}

			bMarkStack = 0;
		}
		else if ( fnInstrMatch( "csp" ) )
		{
			fnGetArg( );

			if ( strcmp( g_strArg, "println" ) != 0 )
				fnTACGenArg( fnPop( stackOfPMachine ) );
			
			fnTACGenCall( g_strArg );
		}
		else if ( fnInstrMatch( "ret" ) )
        {
			if ( !fnIsVoid( entryCurrProcedure ) )
				fnTACGenReturn( fnPop( stackOfPMachine ) );
			else
				fnTACGenReturn( "" );
        }
    }

	// TACBLOCK
	fnMapLabelsAndJumps( );
	fnPrintFlowGraphs( graph, g_iCurrGraph + 1 );
	// fnLocalCopyProp( &graph[ g_iCurrGraph ].block[ 1 ] );
	// fnGraphToTACode( graph );

	// fnLocalCSE( &graph[ g_iCurrGraph ].block[ 1 ] );
	// fnGraphToTACode( graph );

	// fnDestroyGraph( graph );
	//
	fnDestroyStack( stackAux );
    fnDestroyStack( stackOfPMachine );
}

void fnNewTemp( ) { g_iTemp++; }

void fnTACGenEntry( char * strName )
{
	fnMapLabelsAndJumps( );
	fnClearList( &g_lstLabels );
	fnInitializeList( &g_lstLabels );

	fnClearList( &g_lstJumps );
	fnInitializeList( &g_lstJumps );

	fnTACEntry( strName );
	// TACBLOCK
	g_iCurrGraph++;
	g_iCurrBlock = 0;

	fnInitGraph( &graph[g_iCurrGraph] );
	fnInitBlock( &graph[g_iCurrGraph].block[g_iCurrBlock] );

	fnAddInstr( &graph[ g_iCurrGraph ].block[g_iCurrBlock], "entry", strName, "", "", NO_EXPRESSION );

	fnSetNextBlock( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], g_iCurrBlock + 1 );
	fnInitBlock( &graph[ g_iCurrGraph ].block[ ++g_iCurrBlock ] );
	graph[ g_iCurrGraph ].numOfBlocks++;
	//
	if ( DEBUG_TACODE )
		printf( " entry %s\n", strName );
}

void fnTACGenBeginArgs( )
{
	fnTACBeginArgs( );
	// TACBLOCK
	fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "begin_args", "", "", "", NO_EXPRESSION );
	//
	if ( DEBUG_TACODE )
		printf( " begin_args\n" );
}

void fnTACGenArg( char * strArg )
{
	fnTACArg( strArg );
	// TACBLOCK
	fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "arg", strArg, "", "", NO_EXPRESSION );
	//
	if ( DEBUG_TACODE )
		printf( " arg %s\n", strArg );
}

void fnTACGenCall( char * strProcedure )
{
	fnTACCall( strProcedure );
	// TACBLOCK
	fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "call", strProcedure, "", "", NO_EXPRESSION );
	//
	if ( DEBUG_TACODE )
		printf( " call %s\n", strProcedure );
}

void fnTACGenReturn( char * strValue )
{
	fnTACReturn( strValue );
	// TACBLOCK
	if ( strcmp( strValue, "" ) != 0 )
		fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "return", strValue, "", "", INST_RETURN );
	else
		fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "return", "", "", "", INST_RETURN );
	//
	if ( DEBUG_TACODE )
	{
		printf( " return" );

		if ( strcmp( strValue, "" ) != 0 )
			printf( " %s", strValue );

		printf( "\n" );
	}
}

void fnTACGenIfFalse( char * strValue, char * strLabel )
{
	fnTACIfFalse( strValue, strLabel );
	// TACBLOCK
	fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "if_f", strValue, strLabel, "", NO_EXPRESSION );
	
	fnInsertNewElem( &g_lstJumps, strLabel[ 1 ] - '0', g_iCurrBlock );

	fnSetNextBlock( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], g_iCurrBlock + 1 );
	fnInitBlock( &graph[ g_iCurrGraph ].block[ ++g_iCurrBlock ] );
	graph[ g_iCurrGraph ].numOfBlocks++;
	//
	if ( DEBUG_TACODE )
		printf( " if_false %s goto %s\n", strValue, strLabel );
}

void fnTACGenLabel( char * strLabel )
{
	fnTACLabel( strLabel );
	// TACBLOCK
	/* Para evitar blques como
	 *                     label Li
	 */
	if ( graph[ g_iCurrGraph ].block[ g_iCurrBlock ].numOfInstr != 0 )
	{
		fnSetNextBlock( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], g_iCurrBlock + 1 );
		fnInitBlock( &graph[ g_iCurrGraph ].block[ ++g_iCurrBlock ] );
		graph[ g_iCurrGraph ].numOfBlocks++;
	}

	fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "label", strLabel, "", "", NO_EXPRESSION );
	fnInsertNewElem( &g_lstLabels, strLabel[ 1 ] - '0', g_iCurrBlock );
	//
	if ( DEBUG_TACODE )
		printf( " label %s\n", strLabel );
}

void fnTACGenGoto( char * strLabel )
{
	fnTACGoto( strLabel );
	// TACBLOCK
	/* Para evitar en los if else tener un goto Li, después de
	 * un return
	 */
	// if ( fnKindOfLastInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ] ) != INST_RETURN )
	// {
		fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "goto", strLabel, "", "", NO_EXPRESSION );
		fnInsertNewElem( &g_lstJumps, strLabel[ 1 ] - '0', g_iCurrBlock );
		fnSetNextBlock( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], g_iCurrBlock + 1 );
		fnInitBlock( &graph[ g_iCurrGraph ].block[ ++g_iCurrBlock ] );
		graph[ g_iCurrGraph ].numOfBlocks++;
	//}
	//
	if ( DEBUG_TACODE )
		printf( " goto %s\n", strLabel );
}

void fnTACGenOperation( char * strOp, char * strOperand1, char * strOperand2, char * strResult )
{
	fnTACOperation( strOp, strOperand1, strOperand2, strResult );
	// TACBLOCK
	if ( strcmp( strOp, "" ) != 0 )
		fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], strOp, strOperand1, strOperand2, strResult, BINARY_EXPRESSION );
	else
		fnAddInstr( &graph[ g_iCurrGraph ].block[ g_iCurrBlock ], "=", strOperand1, strOperand2, strResult, UNARY_EXPRESSION );
	//
	if ( DEBUG_TACODE )
	{
		printf( " %s = %s", strResult, strOperand1 );

		if ( strcmp( strOp, ""  ) != 0 )
			printf( " %s %s", strOp, strOperand2 );

		printf( "\n" );
	}	
}
