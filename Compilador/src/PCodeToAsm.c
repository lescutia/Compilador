#include "../includes/symbolTable.h"
#include "../includes/PCodeRead.h"
#include "../includes/stack.h"
#include "../includes/list.h"
#include "../includes/PCodeToAsm.h"

void fnInitListOfTemporaries( stList* L );

int g_bReturn = 0;
int g_bMarkStack = 0;

void fnGenAsmCode( )
{
    char strReg0[ 4 ], strReg1[ 4 ], strReg2[ 4 ], strBranchAddr[ 4 ];
    stStack* stackOfPMachine;
    stList* listOfTemporaries;
	// SYMBOLTABLE
	sEntry* entry;
	//

    stackOfPMachine = fnInitStack( );
    listOfTemporaries = fnInitList( );

    fnInitListOfTemporaries( listOfTemporaries );

    while( fnHasInstr( ) )
    {
        fnGetInstr( );

        if ( fnInstrMatch( "ent" ) )
        {
            fnGetArg( );
            printf( "%s:\n", g_strArg );

			// SYMBOLTABLE
			entry = fnSearchSymbolTable( global_symbol_table, g_strArg, PROCEDURE, 0 );

			if( entry != NULL && fnGetType( entry ) != VOID_T )
					g_bReturn = 1;
			//
        }
        else if ( fnInstrMatch( "lda" ) )
        {
            fnGetArg( );

            strcpy( strReg0, fnGetLast( listOfTemporaries ) );
            fnRemoveLast( listOfTemporaries );

            printf( " la %s, %s\n", strReg0, g_strArg );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "ldc" ) )
        {
            fnGetArg( );

            strcpy( strReg0, fnGetLast( listOfTemporaries ) );
            fnRemoveLast( listOfTemporaries );
			
            printf( " li %s, %s\n", strReg0, g_strArg );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "lod" ) )
        {
            fnGetArg( );

            strcpy( strReg0, fnGetLast( listOfTemporaries ) );
            fnRemoveLast( listOfTemporaries );

            printf( " lw %s, %s\n", strReg0, g_strArg );
            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "stn" ) )
        {
            if ( strcmp( fnPeek( stackOfPMachine ), "Hi" ) == 0 )
            {
                strcpy( strReg0, fnGetLast( listOfTemporaries ) );
                //fnRemoveLast( listOfTemporaries );

                fnPop( stackOfPMachine ); // Pop Hi

                printf( " mfhi %s\n", strReg0 );

                strcpy( strReg1, fnPop( stackOfPMachine ) );
                fnInsertInPlace( listOfTemporaries, strReg1 );

                printf( " sw %s, 0(%s)\n", strReg0, strReg1 );
            }
            else if( strcmp( fnPeek( stackOfPMachine ), "Lo" ) == 0 )
            {
                strcpy( strReg0, fnGetLast( listOfTemporaries ) );
                //fnRemoveLast( listOfTemporaries );

                fnPop( stackOfPMachine ); // Pop Lo

                printf( " mflo %s\n", strReg0 );

                strcpy( strReg1, fnPop( stackOfPMachine ) );
                fnInsertInPlace( listOfTemporaries, strReg1 );

                printf( " sw %s, 0(%s)\n", strReg0, strReg1 );
            }
            else
            {
                strcpy( strReg0, fnPop( stackOfPMachine ) );
				fnInsertInPlace( listOfTemporaries, strReg0 );

                strcpy( strReg1, fnPop( stackOfPMachine ) );
                fnInsertInPlace( listOfTemporaries, strReg1 );

                printf( " sw %s, 0(%s)\n", strReg0, strReg1 );
            }
        }
        else if ( fnInstrMatch( "adi" ) )
        {
            strcpy( strReg2, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg2 );

            strcpy( strReg1, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg1 );

            strcpy( strReg0, fnGetLast( listOfTemporaries ) );

            fnRemoveLast( listOfTemporaries );

            printf( " add %s, %s, %s\n", strReg0, strReg1, strReg2 );

            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "sbi" ) )
        {
            strcpy( strReg2, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg2 );

            strcpy( strReg1, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg1 );

            strcpy( strReg0, fnGetLast( listOfTemporaries ) );
            fnRemoveLast( listOfTemporaries );

            printf( " sub %s, %s, %s\n", strReg0, strReg1, strReg2 );

            fnPush( stackOfPMachine, strReg0 );
        }
        else if ( fnInstrMatch( "mpi" ) )
        {
            strcpy( strReg0, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg0 );

            strcpy( strReg1, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg1 );

            printf( " mult %s, %s\n", strReg0, strReg1 );

            fnPush( stackOfPMachine, "Hi" );
        }
        else if ( fnInstrMatch( "dvi" ) )
        {
            strcpy( strReg1, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg0 );

            printf( " div %s, %s\n", strReg0, strReg1 );

            fnPush( stackOfPMachine, "Lo" );
        }
        else if ( fnInstrMatch( "mod" ) )
        {
            strcpy( strReg1, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg0 );

            printf( " div %s, %s\n", strReg0, strReg1 );

            fnPush( stackOfPMachine, "Hi" );
        }
        else if ( fnInstrMatch( "les" ) )
        {
            fnPush( stackOfPMachine, "bge" );
        }
        else if ( fnInstrMatch( "leq" ) )
        {
            fnPush( stackOfPMachine, "bgt" );
        }
        else if ( fnInstrMatch( "equ" ) )
        {
            fnPush( stackOfPMachine, "bne" );
        }
        else if ( fnInstrMatch( "grt" ) )
        {
            fnPush( stackOfPMachine, "ble" );
        }
        else if ( fnInstrMatch( "geq" ) )
        {
            fnPush( stackOfPMachine, "blt" );
        }
        else if ( fnInstrMatch( "neq" ) )
        {
            fnPush( stackOfPMachine, "beq" );
        }
        else if ( fnInstrMatch( "fjp" ) )
        {
            fnGetArg( );
            strcpy( strBranchAddr, fnPop( stackOfPMachine ) );

            strcpy( strReg1, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg1 );

            strcpy( strReg0, fnPop( stackOfPMachine ) );
            fnInsertInPlace( listOfTemporaries, strReg0 );

            printf( " %s %s, %s, %s\n", strBranchAddr, strReg0, strReg1, g_strArg );
        }
        else if ( fnInstrMatch( "ujp" ) )
        {
            fnGetArg( );
            printf( " j %s\n", g_strArg );
        }
        else if ( fnInstrMatch( "lab" ) )
        {
            fnGetArg( );
            printf( " %s:\n", g_strArg );
        }
		else if ( fnInstrMatch( "mst" ) )
		{
			printf( " addi $sp, $sp, -4\n" );
			printf( " sw $ra, 0($sp)\n" );

			g_bMarkStack = 1;
		}
		else if ( fnInstrMatch( "cup" ) )
		{
			fnGetArg( );

			printf( " jal %s\n", g_strArg );

			printf( " lw $ra, 0($sp)\n" );
			printf( " addi $sp, $sp, 4\n" );

		}
		else if( fnInstrMatch( "ret" ) )
        {
			if( g_bReturn )
			{
				strcpy( strReg0, fnPop( stackOfPMachine ) );
				fnInsertInPlace( listOfTemporaries, strReg0 );

				printf( " move $v0, %s\n", strReg0 );

				g_bReturn = 0;
			}

            printf( " jr $ra\n" );
        }
    }

    fnDestroyList( listOfTemporaries );
    fnDestroyStack( stackOfPMachine );
}

int fnInstrMatch( char* strInstr )
{
    if( strcmp( g_strInstr, strInstr ) == 0 )
        return 1;

    return 0;
}

void fnInitListOfTemporaries( stList* L )
{
    fnInsertInPlace( L, "$t7" );
    fnInsertInPlace( L, "$t6" );
    fnInsertInPlace( L, "$t5" );
    fnInsertInPlace( L, "$t4" );
    fnInsertInPlace( L, "$t3" );
    fnInsertInPlace( L, "$t2" );
    fnInsertInPlace( L, "$t1" );
    fnInsertInPlace( L, "$t0" );
}
