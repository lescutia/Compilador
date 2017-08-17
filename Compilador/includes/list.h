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
 * Desc: Verifica si el elemento en la posición index, de la lista L, está disponible.
 * @Param index		Índice del elemento que se establecerá como ocupado.
 * @Param L			Lista.
 * @Return			Regresa 1 si el elemnto en la posición index, de la lista L, está
 *					diponible; y 0 si no está disponible.
 */
int fnIsAvailable( int index, stList * L );

/**
 * Desc: Establece el elemento en la posición index, de la lista L, como disponible.
 * @Param index		Índice del elemento que se establecerá como disponible.
 * @Param L			Lista.
 */
void fnSetAvailable( int index, stList* L );

/**
 * Desc: Establece firstAvailable como el elemento en la posición index, de la lista L.
 * @Param index		Índice del elemento que se establecerá como firstAvailable.
 * @Param L			Lista.
 */
void fnSetFirstAvailable( int index, stList * L );

/**
 * Desc: Establece el elemento en la posición index, de la lista L, como ocupado.
 * @Param index		Índice del elemento que se establecerá como ocupado.
 * @Param L			Lista.
 */
void fnSetOcupied( int index, stList * L );

/**
 * Desc: Regresa la posición del primer elemento de la lista que esté disponible.
 * @Param L		Lista.
 * @Return		Posición del primer elemento, de la lista, que esté disponible, en
 *				caso de no haber ningún elemento disponible regresa -1.
 */
int fnGetAvailable( stList* L );

/**
 * Desc: Busca el primer elemento, de la lista, disponible, a partir de index.
 * @Param index		Índice a partir del cual se busca el primer elemento disponible.
 * @Param L			Lista.
 */
int fnSearchFirstAvailable( int index, stList * L );

#endif // LIST_H_INCLUDED
