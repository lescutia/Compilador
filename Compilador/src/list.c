#include "../CompilerPCH.h"
#include "../includes/list.h"

stList* fnInitList( )
{
	int i;

    stList *L = ( stList* ) malloc( sizeof( stList ) ) ;

	L->firstAvailable = -1;

	for ( i = 0; i < MAX_LIST_SIZE; i++ )
		L->element[ i ] = 0;

	return L;
}

void fnDestroyList( stList* L )
{
	free( L );
}

int fnIsValidIndex( int index )
{
	if ( index < 0 )
		return 0;

	if ( index >= MAX_LIST_SIZE )
		return 0;

	return 1;
}

int fnIsAvailable( int index, stList * L )
{
	return L->element[ index ];
}

void fnSetAvailable( int index, stList* L )
{
	L->element[ index ] = 1;

	if ( L->firstAvailable > index )
		L->firstAvailable = index;
}

void fnSetFirstAvailable( int index, stList * L )
{
	L->firstAvailable = index;
}

void fnSetOccupied( int index, stList * L )
{
	L->element[ index ] = 0;

	if ( L->firstAvailable == index )
		L->firstAvailable = fnSearchFirstAvailable( index, L );
}

int fnSearchFirstAvailable( int index, stList * L )
{
	int i;

	for ( i = index; i < MAX_LIST_SIZE; i++ )
		if ( fnIsAvailable( i, L ) )
			return i;
	
	return -1;
}

int fnGetAvailable( stList* L )
{
	return L->firstAvailable;
}
