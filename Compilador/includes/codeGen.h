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

/**
 * @Param strName	Nombre del procedimiento
 */
void fnTACEntry( char * strName );

/**
 */
void fnTACBeginArgs( );

/**
 * @Param strArg	Nombre del argumento
 */
void fnTACArg( char * strArg );

/**
 * @Param strProcedure	Nombre del procedimiento
 */
void fnTACCall( char * strProcedure );

/**
 * @Param strValue	Valor de retorno
 */
void fnTACReturn( char * strValue );

/**
 * @Param strValue
 * @Param strLabel
 */
void fnTACIfFalse( char * strValue, char * strLabel );

/**
 * @Param strLabel	Nombre de la etiqueta
 */
void fnTACLabel( char * strLabel );

/**
* @Param strLabel	Etiqueta a la que se va a dar el salto
*/
void fnTACGoto( char * strLabel );

/**
 * @Param strOp			Operador
 * @Param strOperand1	Operando izquierdo
 * @Param strOperand2	Operando derecho
 * @Param strResult		Resultado
 */
void fnTACOperation( char * strOp, char * strOperand1, char * strOperand2, char * strResult );
