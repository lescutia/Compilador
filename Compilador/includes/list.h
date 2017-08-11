#pragma once

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#define MAX_LIST_SIZE 8
#define MAX_DATA_SIZE 4

typedef struct
{
    int iSize;
    char element[ MAX_LIST_SIZE ][ MAX_DATA_SIZE ];
} stList;

stList* fnInitList( );
void fnDestroyList( stList* L );

int fnIsEmptyList( stList* L );
int fnIsFullList( stList* L );

void fnInsertInPlace( stList* L, char* strData );

char* fnGetLast( stList* L );
void fnRemoveLast( stList *L );

#endif // LIST_H_INCLUDED
