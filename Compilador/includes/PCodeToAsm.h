#pragma once

#ifndef PCODETOASM_H_INCLUDED
#define PCODETOASM_H_INCLUDED

void fnGenAsmCode( );
void fnGenLabel( char * label );
void fnGenDirective( char * directive );
void fnGenVariable( char * var, int type );
void fnGenRFormat( char * opcode, char * rs, char * rt, char * rd, char * funct );
void fnGenIFormat( char * opcode, char * rs, char * rt, char * imm );
void fnGenJFormat( char * opcode, char * addr );
void fnGenPseudoInstr( char * opcode, char * rx, char * ry, char * imm );
// Se movió a PCodeRead.h
// int fnInstrMatch( char * strInstr );

#endif // PCODETOASM_H_INCLUDED
