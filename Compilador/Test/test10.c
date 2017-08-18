int fnSum( int n );
// void printi( int a );

int main( )
{
    int n;
    int sum;

    scani( n );

    sum = fnSum( n );

    printi( sum );
    // prints( a );
    return 0;
}

int fnSum( int an )
{
    if( n == 1 )
        return 1;

    return n + fnSum( n - 1);
}
