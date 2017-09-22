#pragma once

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#define MAX_STACK_SIZE 100//500
#define MAX_DATA_SIZE 128

typedef struct
{
	int iTop;
	char data[ MAX_STACK_SIZE ][ MAX_DATA_SIZE ];
} stStack;

stStack* fnInitStack( );
void fnDestroyStack( stStack* S );

int fnIsEmpty( stStack* S );
int fnIsFull( stStack* S );

int fnPush( stStack* S, char* strData );
char* fnPop( stStack* S );
char* fnPeek( stStack* S );

#endif // STACK_H_INCLUDED
