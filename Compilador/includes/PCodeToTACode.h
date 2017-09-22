#pragma once

#ifndef PCODETOTACODE_H_INCLUDED
#define PCODETOTACODE_H_INCLUDED

void fnGenTACode( );
void fnNewTemp( );

/**
 * @Param strName	Nombre del procedimiento
 */
void fnTACGenEntry( char * strName );

void fnTACGenBeginArgs( );

/**
 * @Param strArg	Nombre del argumento
 */
void fnTACGenArg( char * strArg );

/**
 * @Param strProcedure	Nombre del procedimiento
 */
void fnTACGenCall( char * strProcedure );

/**
 * @Param strValue	Valor de retorno
 */
void fnTACGenReturn( char * strValue );

/**
 * @Param strValue
 * @Param strLabel
 */
void fnTACGenIfFalse( char * strValue, char * strLabel );

/**
 * @Param strLabel	Nombre de la etiqueta
 */
void fnTACGenLabel( char * strLabel );

/**
 * @Param strLabel	Etiqueta a la que se va a dar el salto
 */
void fnTACGenGoto( char * strLabel );

/**
 * @Param strOp			Operador
 * @Param strOperand1	Operando izquierdo
 * @Param strOperand2	Operando derecho
 * @Param strResult		Resultado
 */
void fnTACGenOperation( char * strOp, char * strOperand1, char * strOperand2, char * strResult );

#endif // PCODETOTACODE_H_INCLUDED
