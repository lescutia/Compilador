#include "CompilerPCH.h"

void fnInitCompiler( )
{
	g_lineNumber = 1;
	g_numberOfReadCharacters	= 0;
	g_numberOfIgnoredCharacters	= 0;
	g_numberOfComments			= 0;
	g_numberOfScannedSymbols	= 0;
}

int fnLeftShift( int iNumberToShift, int iShifting )
{
	return iNumberToShift << iShifting;
}

int fnRightShift( int iNumberToShift, int iShifting )
{
	return iNumberToShift >> iShifting;
}
