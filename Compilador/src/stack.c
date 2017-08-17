#include "../CompilerPCH.h"
#include "../includes/stack.h"

stStack* fnInitStack( )
{
    stStack *S = ( stStack* ) malloc( sizeof( stStack ) ) ;
	S -> iTop = -1;

	return S;
}

void fnDestroyStack( stStack* S )
{
    free( S );
}

int fnIsEmpty( stStack* S )
{
    return S -> iTop == -1;
}

int fnIsFull( stStack* S )
{
    return S -> iTop == MAX_STACK_SIZE - 1;
}

int fnPush( stStack* S, char* strData )
{
    if ( !fnIsFull( S ) )
        strcpy( S -> data[ ++( S -> iTop ) ], strData );
    else
    {
        printf( "\nError: Stack overflow.\n" );
        return 0;
    }

    return 1;
}

char* fnPop( stStack* S )
{
    char* iTemp = NULL;

    if ( !fnIsEmpty( S ) )
    {
        iTemp = S -> data[ S -> iTop ];
        ( S -> iTop )--;
    }
	else
	{
		printf( "\nError: Stack underflow.\n" );
		getch(  );

		exit( 1 );
	}   

    return iTemp;
}

char* fnPeek( stStack* S )
{
    if( !fnIsEmpty( S ) )
        return S -> data[ S -> iTop ];

    printf( "\nError: Stask is empty.\n" );

    return " ";
}
