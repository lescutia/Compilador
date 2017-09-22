#include "../CompilerPCH.h"
#include "../includes/symbolTable.h"

// ------------------------- INITIALIZATION ------------------------

sEntry* fnGetNextEntry	( sEntry* entry )	{ return	entry->next; }
char* fnGetString	( sEntry* entry )		{ return	entry->string; }
int  fnGetLineNumber( sEntry* entry )		{ return	entry->line; }
int  fnGetClass		( sEntry* entry )		{ return	entry->class; }
int  fnGetType		( sEntry* entry )		{ return	entry->type; }
int  fnGetValue		( sEntry* entry )		{ return	entry->value; }
int  fnGetAddress	( sEntry* entry )		{ return	entry->address; }
int  fnGetScope 	( sEntry* entry )		{ return	entry->scope; }
int  fnIsDefined	( sEntry* entry )		{ return	entry->defined; }
char* fnGetParent 	( sEntry* entry )		{ return	entry->parent; }
void fnSetNextEntry	( sEntry* entry, sEntry* next )    	{ entry->next = next; }
void fnSetString	( sEntry* entry, char* identifier ) { entry->string = identifier; }
void fnSetLineNumber( sEntry* entry, int line )    		{ entry->line = line; }
void fnSetClass		( sEntry* entry, int class )        { entry->class = class; }
void fnSetType		( sEntry* entry, int type )         { entry->type = type; }
void fnSetValue		( sEntry* entry, int value )        { entry->value = value; }
void fnSetAddress	( sEntry* entry, int address )    	{ entry->address = address; }
void fnSetScope		( sEntry* entry, int scope )        { entry->scope = scope; }
void fnSetDefined	( sEntry* entry, int defined )		{ entry->defined = defined; }
void fnSetParent	( sEntry* entry, char* parent ) 	{ entry->parent = parent; }

int fnIsVoid( sEntry * procedure )
{
	if ( fnGetType( procedure ) == VOID_T )
		return 1;

	return 0;
}

int fnAddParameter	( sEntry* entry, char* varname, int type )
{
	sParameter* newParam = ( sParameter* )malloc( sizeof( sParameter ) );

	newParam->id = (char*)malloc( strlen( varname ) + 1 );
	strcpy( newParam->id, varname );
	newParam->type = type;
	newParam->next = NULL;

	sParameter* prevParam = entry->parameter;

	if ( prevParam )
	{
		//while (prevParam->next && strcmp( prevParam->id, varname ) != 0)
		while (prevParam->next)
			prevParam = prevParam->next;

		/*
		if (prevParam->next)
			return 1;
		else
		//*/
		prevParam->next = newParam;
	}
	else
		entry->parameter = newParam;
	return 0;
}

// PARA VERIFICAR EL NÚMERO DE PARÁMETROS
sParameter* fnGetNextParameter( sParameter* parameter ) { return parameter->next; }
int fnGetTypeParameter( sParameter* parameter )			{ return parameter->type; }
char* fnGetIdParameter( sParameter* parameter )			{ return parameter->id; }

// TODO: ORDENAR
void fnCreateParameter( sEntry* entry, int type, char* id )
{
	sParameter* newParameter;
	sParameter* ptrParameter;

	newParameter = (sParameter*)malloc( sizeof( sParameter ) );
	// +1 es para incluir el caracter final '\0'
	newParameter->id = (char*)malloc( strlen( id ) + 1 );

	newParameter->next = NULL;
	newParameter->type = type;
	strcpy ( newParameter->id, id );

	// Si la lista está vacía
	if( entry->parameter == NULL )
		entry->parameter = newParameter;
	else
	{
		ptrParameter = entry->parameter;

		while( ptrParameter->next != NULL )
			ptrParameter = ptrParameter->next;

		ptrParameter->next = newParameter;
	}
}
//

void fnInitializeSymbolTables( )
{
	sEntry * entry;

	global_symbol_table  = 0;
	local_symbol_table   = 0;
	library_symbol_table = 0;

	numberOfGlobalVariables = 0;
	numberOfProcedures      = 0;
	numberOfStrings         = 0;

	entry = fnCreateSymbolTableEntry( LIBRARY_TABLE, "printi", -1, PROCEDURE, VOID_T, 0, -1, 1, 0 );
	fnAddParameter( entry, "n", INT_T );

	entry = fnCreateSymbolTableEntry( LIBRARY_TABLE, "printc", -1, PROCEDURE, VOID_T, 0, -1, 1, 0 );
	fnAddParameter( entry, "c", CHAR_T );

	entry = fnCreateSymbolTableEntry( LIBRARY_TABLE, "prints", -1, PROCEDURE, VOID_T, 0, -1, 1, 0 );
	fnAddParameter( entry, "s", CHARSTAR_T );

	entry = fnCreateSymbolTableEntry( LIBRARY_TABLE, "println", -1, PROCEDURE, VOID_T, 0, -1, 1, 0 );

	fnCreateSymbolTableEntry( GLOBAL_TABLE, "string_buffer", -1, VARIABLE, CHARSTAR_T, -1, -1, 1, 0 );
	/*
	fnCreateSymbolTableEntry( LIBRARY_TABLE, "scani", -1, PROCEDURE, VOID_T, 0, -1, 1, 0 );
	fnCreateSymbolTableEntry( LIBRARY_TABLE, "scans", -1, PROCEDURE, VOID_T, 0, -1, 1, 0 ); */

}

//BORRAR - FUNCIONES AXUXILIARES, PARA IMPRIMIR
// Ya se está usando fnPrintType en los mensajes de error
// de tipo.
void fnPrintType( int type )
{
	if( type == INT_T )
		printf( "INT" );
	else if( type == INTSTAR_T )
		printf( "INTSTAR" );
	else if( type == VOID_T )
		printf( "VOID" );
	else if( type == CHAR_T )
		printf( "CHAR" );
	else if( type == CHARSTAR_T )
		printf( "CHARSTAR" );
	else
		printf( "UNKNOWN" );

	printf( "\n" );
}

void fnPrintClass( int class )
{
	if( class == VARIABLE )
		printf( "VARIABLE" );
	else if( class == PROCEDURE )
		printf( "PROCEDURE" );
	else if( class == STRING )
		printf( "STRING" );
	else
		printf( "UNKNOWN" );

	printf( "\n" );
}
//

void fnPrintTable( int whichTable )
{
	sEntry* it = NULL;

	if( whichTable == GLOBAL_TABLE )
	{
		printf( "\n/****************************************\n" );
		printf( "*		GLOBAL TABLE 		*\n" );
		printf( "/****************************************" );

		it = global_symbol_table;
	}
	else if( whichTable == LOCAL_TABLE )
	{
		printf( "\n/****************************************\n" );
		printf( "*		LOCAL TABLE		*\n" );
		printf( "/****************************************" );

		it = local_symbol_table;
	}

	while( it != 0 )
	{
		printf( "\n symbol:  %s\n", it->string );
		printf( " line:    %d\n", it->line );

		if( it->parent )
			printf( " parent:  %s\n", it->parent );
		
		if ( it->parameter )
		{
			sParameter* tmpParam = it -> parameter;
			printf( " param:   " );

			do
			{
				printf( "%s", tmpParam -> id );
				tmpParam = tmpParam -> next;

				if ( tmpParam )
					printf( ", " );
			} while ( tmpParam );

			printf( "\n" );
		}

		printf( " class:   " );
		fnPrintClass( it->class );

		printf( " type:    " );
		fnPrintType( it->type );

		//printf(" value:	 %d\n",   it->value);
		//printf(" address: %d\n",   it->address);
		//printf(" scope:	 %d\n",   it->scope);
		printf( " defined: %d\n", it->defined );

		it = it->next;
	}
}

void fnResetSymbolTables( )
{
	while( global_symbol_table != 0 )
	{
		sEntry* next = fnGetNextEntry( global_symbol_table );
		sParameter* it = global_symbol_table->parameter;
		while( global_symbol_table->parameter )
		{
			it = it->next;
			SAFE_RELEASE( global_symbol_table->parameter->id );
			SAFE_RELEASE( global_symbol_table->parameter );
			global_symbol_table->parameter = it;
		}
		//SAFE_RELEASE( global_symbol_table->parent );
		SAFE_RELEASE( global_symbol_table->string );
		SAFE_RELEASE( global_symbol_table );
		
		global_symbol_table = next;
	}

	while( local_symbol_table != 0 )
	{
		sEntry* next = fnGetNextEntry( local_symbol_table );
		SAFE_RELEASE( local_symbol_table->parent );
		SAFE_RELEASE( local_symbol_table->string );
		SAFE_RELEASE( local_symbol_table );
		local_symbol_table = next;
	}


	while ( library_symbol_table != 0 )
	{
		sEntry* next = fnGetNextEntry( library_symbol_table );
		SAFE_RELEASE( library_symbol_table->string );
		
		sParameter* it = library_symbol_table->parameter;
		while (library_symbol_table->parameter)
		{
			it = it->next;
			SAFE_RELEASE( library_symbol_table->parameter->id );
			SAFE_RELEASE( library_symbol_table->parameter );
			library_symbol_table->parameter = it;
		}

		SAFE_RELEASE( library_symbol_table );
		library_symbol_table = next;
	}
	
	global_symbol_table  = 0;
	local_symbol_table   = 0;
	library_symbol_table = 0;

	numberOfGlobalVariables = 0;
	numberOfProcedures      = 0;
	numberOfStrings         = 0;
}

sEntry * fnCreateSymbolTableEntry( int whichTable, char* string, int line, int class, int type, int value, int address, int defined, char* parent )
{
	sEntry* newEntry;
	newEntry = (sEntry*)malloc( sizeof( sEntry ) );

	//newEntry->string = malloc( sizeof(string) );
	//sizeof(string) no es la longitud de la cadena
	// +1 es para incluir el caracter final '\0'
	newEntry->string = (char*)malloc( strlen( string ) + 1 );

	//memcpy ( newEntry->string, string, sizeof(string) );
	strcpy ( newEntry->string, string );

	// TODO: ORDENAR
	newEntry->parameter = NULL;

	//fnSetString		( newEntry, string );
	fnSetLineNumber	( newEntry, line );
	fnSetClass		( newEntry, class );
	fnSetType		( newEntry, type );
	fnSetValue		( newEntry, value );
	fnSetAddress	( newEntry, address );
	fnSetDefined	( newEntry, defined );

	if( parent )
	{
		// +1 es para incluir el caracter final '\0'
		newEntry->parent = (char*)malloc( strlen( parent ) + 1 );
		strcpy( newEntry->parent, parent );
	}
	else
		fnSetParent( newEntry, 0 );

	if( whichTable == GLOBAL_TABLE )
	{
		//fnSetScope( newEntry, REG_GP );
		fnSetNextEntry( newEntry, global_symbol_table );
		global_symbol_table = newEntry;

		if( class == VARIABLE )
			numberOfGlobalVariables = numberOfGlobalVariables + 1;
		else if( class == PROCEDURE )
			numberOfProcedures = numberOfProcedures + 1;
		else if( class == STRING )
			numberOfStrings = numberOfStrings + 1;
	}
	else if( whichTable == LOCAL_TABLE )
	{
		//fnSetScope( newEntry, REG_GP );
		fnSetNextEntry( newEntry, local_symbol_table );
		local_symbol_table = newEntry;
	}
	else
	{
		fnSetNextEntry( newEntry, library_symbol_table );
		library_symbol_table = newEntry;
		//printf( "Error: no existen funciones de libreria.\n" );
	}

	return newEntry;
}

sEntry* fnSearchSymbolTable( sEntry* entry, char* string, int class, char* actualProc )
{
	while( entry != 0 )
	{
		//if( string == fnGetString( entry ) )
		/*Porque se cambió la definición de variableOrProcedureName, para
		 *evitar problemas con la función fnGetSymbol que cambiaba el
		 *su valor, cuando era un char*.
		 */
		if( !strcmp( string, fnGetString( entry ) ) )
			if( class == fnGetClass( entry ) )
			{
				if( !fnGetParent( entry ) )
				{
					if( !actualProc )
						return entry;
				}
				else if( !strcmp( fnGetParent( entry ), actualProc ) )
					return entry;
			}
		entry = fnGetNextEntry( entry );
	}

	return 0; // NULL
}

sParameter * fnSearchParameter( sEntry * entryProcedure, char * strParameter )
{
	sParameter* tmpParam;

	if ( entryProcedure->parameter )
	{
		tmpParam = entryProcedure->parameter;

		while( tmpParam != NULL )
		{
			if ( strcmp( tmpParam->id, strParameter ) == 0 )
				return tmpParam;

			tmpParam = tmpParam->next;
		}
	}

	return NULL;
}

sEntry* fnGetScopedSymbolTableEntry( char* string, int class, char* actualProc )
{
	sEntry* entry = 0;

	if( class == VARIABLE )
		entry = fnSearchSymbolTable( local_symbol_table, string, VARIABLE, actualProc );
	else
		entry = fnSearchSymbolTable( library_symbol_table, string, PROCEDURE, 0 );

	// Si es una variable global o una función
	if( entry == 0 )
		entry = fnSearchSymbolTable( global_symbol_table, string, class, 0 );

	return entry;
}
