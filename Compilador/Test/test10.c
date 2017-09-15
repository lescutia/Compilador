int fnSum( int n );

int main( )
{
  int sum;

  if( fnSum( 4 ) > 10 )
  {
      return 1;
  }  

  return 0;
}

int fnSum( int n )
{
  if( n == 1 )
    return 1;

  return n + fnSum( n - 1);
}
