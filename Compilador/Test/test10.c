int g = -2;

int fnAdd( int a, int b );

int main( )
{
	int result;

	result = -3 + fnAdd( 3, 5 );

	return result > 3;
}

int fnAdd( int a, int b )
{
	return a + b;
}
