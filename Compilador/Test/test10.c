int fnSum( int n );

int main( )
{
    int sum;
    sum = fnSum( 5 );

    return 0;
}

int fnSum( int n )
{
    if( n == 1 )
        return 1;

    return n + fnSum( n - 1);
}
