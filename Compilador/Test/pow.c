int fnBaseOrExpAreNeg( int iBase, int iExp);
int fnPow( int iBase, int iExp );

int main( )
{
	return fnPow(2, 8);
}

int fnBaseOrExpAreNeg( int iBase, int iExp)
{
	if ( iBase < 0 )
		return 1;

	if ( iExp < 0 )
		return 1;

	return 0;
}

int fnPow( int iBase, int iExp )
{
	int i, iResult;

	if ( fnBaseOrExpAreNeg( iBase, iExp ) )
		return 0;

	i = 2;
	iResult = iBase;

	if ( iBase > 0 )
	{
		if ( iExp == 0 )
			return 1;

		while ( i < iExp )
		{
			iResult = iResult * iResult;
		}
	}

	return iResult;
}
