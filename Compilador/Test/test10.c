//FIBONACCI
int fnFibonacci( int n );

int main( )
{
    int i;
    int n;

    i = 1;
    n = 15;

    prints( "Primeros " );
    printi( n - 1 );
    prints( " numeros de la sucesion de fibonacci:" );
    println( );

    while( i < n )
    {
        printi( fnFibonacci( i ) );
        
        i = i + 1;

        if ( i < n )
            prints( ", " );
    }

     return 0;
}

int fnFibonacci( int n )
{
     if ( n == 0 )
          return 0;

     if ( n == 1 )
          return 1;

     return fnFibonacci( n - 1 ) + fnFibonacci( n - 2 );
}
