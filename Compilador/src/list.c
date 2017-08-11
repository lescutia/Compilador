#include "../CompilerPCH.h"
#include "../includes/list.h"

stList* fnInitList( )
{
    stList *L = ( stList* ) malloc( sizeof( stList ) ) ;
	L -> iSize = 0;

	return L;
}

void fnDestroyList( stList* L )
{
    free( L );
}

int fnIsEmptyList( stList* L )
{
    return L -> iSize == 0;
}

int fnIsFullList( stList* L )
{
    return L -> iSize == MAX_LIST_SIZE;
}

// De mayor a menor
void fnInsertInPlace( stList* L, char* strData )
{
    int iIndex;

    if ( !fnIsFullList( L ) )
    {
        iIndex = L -> iSize;

        while ( iIndex > 0 )
        {
            if ( strcmp( strData, L -> element[ iIndex - 1 ]) < 0 )
                break;

            strcpy( L -> element[ iIndex ], L -> element[ iIndex - 1 ] );
            iIndex--;
        }

        strcpy( L -> element[ iIndex ], strData );
        L -> iSize ++;
    }
    else
        printf( "\nError: There is not enough space on the list.\n" );
}

char* fnGetLast( stList* L )
{
    if ( !fnIsEmptyList( L ) )
    {
        return L -> element[ L -> iSize - 1 ];
    }
    else
        printf( "\nError: The last item cannot be obtained, because the list is empty.\n" );

    return "ERR";
}

void fnRemoveLast( stList *L )
{
    if ( !fnIsEmptyList( L ) )
    {
        L -> iSize--;
    }
    else
        printf( "\nError: The last item cannot be removed, because the list is empty.\n" );

}
