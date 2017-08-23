int fnFactorial( int n );

int main( )
{
    int n;
    prints("Factorial de 10: ");
    
    println();

    n = 10;

    n = fnFactorial( n );
    printi(n);
    return 0;
}

int fnFactorial( int n )
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