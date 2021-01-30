/*
 * COSC413 lecturer Tadao Takaoka's single precision verion. 
 * This program finds a prime number by using Euler's criterion 10 times
 */
#include	<stdio.h>

int	main( void);
int	exp( int x, int e);
long long	expLong( int x, int e);
int	J( int a, int n);

int	a, i, n, x, y, w;
int	primality, count = 0;

/*
 *  This computes x^e. x can be negative, but e must be positive
 */
int	exp( int x, int e)
{
	int	y, i, m, b[100];

	for (i = 0; e > 0; i++)
	{
		if ((e/2)*2 == e)
			b[i] = 0;
		else
			b[i] = 1;
		e = e / 2;
	}

	m = i - 1; y = 1;
	for (i = m; i >= 0; i--)
	{
		y = y * y; y = y % n;
		if (b[i] == 1)
		{
			y = y * x; y = y % n;	// y is int, be careful of overflow here
		}
	}

	return y;
}

/* Long long int version, added by XuZF */
long long	expLong( int x, int e)
{
	long long	y;
	int	i, m, b[100];

	for (i = 0; e > 0; i++)
	{
		if ((e/2)*2 == e)
			b[i] = 0;
		else
			b[i] = 1;
		e = e / 2;
	}

	m = i - 1; y = 1;
	for (i = m; i >= 0; i--)
	{
		y = y * y; y = y % n;
		if (b[i] == 1)
		{
			y = y * x;
			y = y % n;
		}
	}

	return y;
}

/*
 * Jaqcobi function
 */
int	J( int a, int n)
{
	if (a == 0)
		return 0;
	else if (a == 1)
		return 1;
	else if ((a/2)*2 == a)
		return J( a/2, n) * exp( -1, (n*n-1)/8);
	else
		return J(n%a, a) * exp( -1, (a-1)*(n-1)/4);

/* For debugging
int ii;
J1_0++;
printf( "0. a = %d  n = %d\n", a, n);
	if (a == 0)
		return 0;
	else if (a == 1)
		return 1;
	else if ((a/2)*2 == a)
	{
J1_1++;
printf( "1. a/2 = %d\t", a/2);
printf( "n=%d  (n*n-1)/8=%d\t", n, (n*n-1)/8);
		ii = exp( -1, (n*n-1)/8);
printf( "exp( -1) = %d\n", ii);
		return J( a/2, n) * ii;
	}
	else
	{
J1_2++;
printf( "2. n%%a = %d  a = %d\t", n%a, a);
		ii = exp( -1, (a-1)*(n-1)/4);
printf( "exp( -1) = %d\n", ii);
		return J(n%a, a) * ii;
	}
 */
}

int	main( void)
{
	n = random( ) % 16000;
	if ((n/2)*2 == n)	n++;
	printf( "n: %d", n); getchar( );

	do
	{
		primality = 1;
		count++;
		for (i = 1; i <= 10; i++)
		{
			a = random( ) % n;
			x = J( a, n);
			if (x != 0)
			{
				y = exp( a, (n-1)/2);
				if (y == n-1)
					y = -1;
				if (x != y)
				{
					primality = 0; break;
				}
			}
		}
		n = n + 2;
	} while (primality == 0);

	printf( "n: %d  count: %d\n", n-2, count);

	return 0;
}
