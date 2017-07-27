int fn( n );

int main( )
{
    int n;
    n = 10;

    return fn( n );
}

int fn( int n )
{
    int i;
    int iResult;

    if( n < 0 )
        return 0;

    i = 2;
    iResult = 1;

    while( i <= n)
    {
        iResult = iResult * i;
        i = i + 1;
    }

    return iResult;
}