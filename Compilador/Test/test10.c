// GCD
int fnMax( int a, int b );
int fnMin( int a, int b );
int fnGCD( int a, int b );

int main( )
{
    int gcd;
    gcd = fnGCD( 4, 10 );
    printi( gcd );

    return 0;
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

    iMax = fnMax( a, b );
    iMin = fnMin( a, b );

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
