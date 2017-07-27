#include "CompilerPCH.h"

void initCompiler( )
{
	g_lineNumber = 1;
	g_numberOfReadCharacters	= 0;
	g_numberOfIgnoredCharacters	= 0;
	g_numberOfComments			= 0;
	g_numberOfScannedSymbols	= 0;
}
