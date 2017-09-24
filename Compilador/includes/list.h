#pragma once

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#define MAX_LIST_SIZE 8

typedef struct
{
	int firstAvailable;
    char element[ MAX_LIST_SIZE ];
} stList;

/**
 * Desc: Crea una lista y la inicializa.
 * @Return		Regresa una puntero a la nueva lista.
 */
stList* fnInitList( );

/**
 * Desc: Libera la memoria ocupada por la lista L.
 * @Param L		Lista.
 */
void fnDestroyList( stList* L );

/**
 * Desc: Verifica si el elemento en la posici�n index, de la lista L, est� disponible.
 * @Param index		�ndice del elemento que se establecer� como ocupado.
 * @Param L			Lista.
 * @Return			Regresa 1 si el elemnto en la posici�n index, de la lista L, est�
 *					diponible; y 0 si no est� disponible.
 */
int fnIsAvailable( int index, stList * L );

/**
 * Desc: Establece el elemento en la posici�n index, de la lista L, como disponible.
 * @Param index		�ndice del elemento que se establecer� como disponible.
 * @Param L			Lista.
 */
void fnSetAvailable( int index, stList* L );

/**
 * Desc: Establece firstAvailable como el elemento en la posici�n index, de la lista L.
 * @Param index		�ndice del elemento que se establecer� como firstAvailable.
 * @Param L			Lista.
 */
void fnSetFirstAvailable( int index, stList * L );

/**
 * Desc: Establece el elemento en la posici�n index, de la lista L, como ocupado.
 * @Param index		�ndice del elemento que se establecer� como ocupado.
 * @Param L			Lista.
 */
void fnSetOccupied( int index, stList * L );

/**
 * Desc: Regresa la posici�n del primer elemento de la lista que est� disponible.
 * @Param L		Lista.
 * @Return		Posici�n del primer elemento, de la lista, que est� disponible, en
 *				caso de no haber ning�n elemento disponible regresa -1.
 */
int fnGetAvailable( stList* L );

/**
 * Desc: Busca el primer elemento, de la lista, disponible, a partir de index.
 * @Param index		�ndice a partir del cual se busca el primer elemento disponible.
 * @Param L			Lista.
 */
int fnSearchFirstAvailable( int index, stList * L );

#endif // LIST_H_INCLUDED


#ifndef LIST_STRUCT
#define LIST_STRUCT

typedef struct  
{
	int value;
	int block;
	struct sListNode* next;
} sListNode;

typedef struct 
{
	int iSize;
	sListNode* head;
	sListNode* tail;
} sList;

void fnInitializeList( sList* inList );
sListNode* fnInsertNewElem( sList* inList, int inValue, int block );
void fnInsertNextToNode( sListNode* ActuaNode, int inValue, int block );
void fnClearList( sList* inList );
int fnInList( sList* inList, int inValue );
sListNode* fnGetNode( sList* inList, int inValue );

#endif