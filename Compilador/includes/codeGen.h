/********************************************************************
	file:		codeGen.h
	date:		2017/07/26
	author: 	Luis Escutia, Miguel Corona
	Contact:	escutialuis93@gmail.com, fmiguelcorona@gmail.com

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

/**
 * Desc: función para escribir en el archivo especificado
 * @Param label
 */
void fnTargetCodeLabel( char* label );

/**
 * Desc: función para escribir en el archivo especificado
 * @Param directive
 */
void fnTargetCodeDirective( char* directive );

/**
 * Desc: función para escribir en el archivo especificado
 * @Param var
 * @Param type
 */
void fnTargetCodeVariable( char* var, int type );

/**
 * Desc: función para escribir en el archivo especificado
 * @Param opcode
 * @Param rd
 * @Param rs
 * @Param rt
 * @Param funct
 */
void fnTargetCodeRFormat( char* opcode, char* rd, char* rs, char* rt, char* funct );

/**
 * Desc: función para escribir en el archivo especificado
 * @Param opcode
 * @Param rt
 * @Param rs
 * @Param imm
 */
void fnTargetCodeIFormat( char* opcode, char* rt, char* rs, char* imm );

/**
 * Desc: función para escribir en el archivo especificado
 * @Param opcode
 * @Param addr
 */
void fnTargetCodeJFormat( char* opcode, char* addr );

/**
 * Desc: función para escribir en el archivo especificado
 * @Param opcode
 * @Param rx
 * @Param ry
 * @Param imm
 */
void fnTargetCodePseudoInstr( char * opcode, char * rx, char * ry, char * imm );
