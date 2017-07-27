/********************************************************************
	file:		intermediateCode.h
	date:		2017/07/26
	author: 	Luis Escutia
	Contact:	escutialuis93@gmail.com

	Purpose:	
********************************************************************/

#pragma once

/*
	Desc: Funcion para abrir el archivo en el cual se desea escribir.
	@Param strNameOfFile es el nombre del archivo en el cual se desea escribir.
*/
void fnOpenFileToWrite( char* strNameOfFile );

/*
	Desc: Funcion para cerrar el archivo.
*/
void fnCloseFile(  );

/*
	Desc: funcion para escribir en el archivo especificado
	@Param instr
	@Param arg
	@Param iLabel	
*/
void fnPCode( char* instr, char* arg, int iLabel );
