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

/*******************************************************************************
********************************************************************************
*******************************************************************************/

sListNode* fnNewListNode( int iValue, int block )
{
	sListNode* newNode = (sListNode*)malloc(sizeof(sListNode));
	newNode->value = iValue;
	newNode->block = block;
	newNode->next = NULL;
	return newNode;
}
void fnInitializeList( sList* inList )
{
	sListNode* dummyNode = fnNewListNode( -1, -1 );
	inList->head = dummyNode;
	inList->tail = dummyNode;
	inList->iSize = 0;
}

sListNode* fnInsertNewElem( sList* inList, int inValue, int block )
{
	sListNode* newNode = fnNewListNode( inValue, block );
	sListNode* it = inList->head;
	while( it && it->value < inValue )
		it = it->next;

	inList->tail->next = newNode;
	inList->tail = newNode;
	return newNode;
}

void fnInsertNextToNode( sListNode* ActuaNode, int inValue, int block )
{
	sListNode* newNode = fnNewListNode( inValue, block );
	sListNode* tmpNode = ActuaNode->next;
	ActuaNode->next = newNode;
	newNode->next = tmpNode;
}

void fnClearList( sList* inList )
{
	sListNode* it = inList->head;
	sListNode* tmp = NULL;
	while( it )
	{
		tmp = it->next;
		SAFE_RELEASE( it );
		it = tmp;
	}
}

int fnInList( sList* inList, int inValue )
{
	sListNode* it = inList->head->next;
	while( it )
	{
		if( it->value == inValue )
			return 1;
		it = it->next;
	}
	return 0;
}

sListNode* fnGetNode( sList* inList, int inValue )
{
	sListNode* it = inList->head->next;
	while( it )
	{
		if( it->value == inValue )
			return it;
		it = it->next;
	}
	return NULL;
}
