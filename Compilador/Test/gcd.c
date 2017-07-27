// GCD
int fnAbs( int x );
int fnMax( int a, int b );
int fnMin( int a, int b );
int fnGCD( int a, int b );

int main( )
{
    return fnGCD( 4 );
}

int fn( )
{
    return 0;
}

int fnAbs( int x )
{
    if ( x < 0 )
        return -x;

    return x;
}

int fnMax( int a, int b )
{
    if ( a > b )
        return a;

    return b;
}

int fnMin( int a, int b )
{
    if ( a < b )
        return a;

    return b;
}

int fnGCD( int a, int b )
{
    int iRemainder;
    int iMax;
    int iMin;

    iMax = fnMax( fnAbs( a ), fnAbs( b ) );
    iMin = fnMin( fnAbs( a ), fnAbs( b ) );

    //GCD(a, 0) = a
    if ( iMin == 0 )
        return iMax;

    iRemainder = iMax % iMin;

    // GCD( a, b ) = GCD( b, r  ),
    // donde, a > b y a = b * q + r
    while ( iRemainder != 0 )
    {
        iMax = iMin;
        iMin = iRemainder;

        iRemainder = iMax % iMin;
    }

    return iMin;
}