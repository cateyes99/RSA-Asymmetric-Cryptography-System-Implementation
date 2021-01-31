/* COSC413 lecturer Tadao Takaoka's multiple precision verion */
#include	<stdio.h>
#include	"Prototypes.h"
#include	"Global.h"

int	main( void);
void	multiply( int a[], int b[], int c[]);
int	divide( int a[], int b[], int c[], int n, int m);

int	b[1000], c[1000], temp[1000];
int	a[1000];
char	s[1000], t[1000];
int	i, j, k, m, n, n1, flag, count, r;

/*
 * a[] * b[] => c[]. a[] & b[] can be any signed (positive or negative).
 * a[] & b[]'s values aren't changed after being called
 */
void	multiply( int a[], int b[], int c[])
{
	int	i, j;
	for (i = 0; i <= 2 * n - 1; i++)
		c[i] = 0;

	for (i = 0; i <= n-1; i++)
	{
		for (j = 0; j <= n-1; j++)
		{
			c[i+j+1] = c[i+j+1] + (c[i+j] + a[i] * b[j])/r;
			c[i+j]   = (c[i+j] + a[i] * b[j]) % r;
		}
	}
}

/*
 * a[] / b[] => c[], and a[] % b[] => a[], d[] is set to 2^return at the end.
 * b[] must be positive, a[] doesn't matter. If a[] is negative, then the
 * result is quite interesting: for example, a[] / b[] = -13 / 4, c[] = -4
 * (not -3), and the remaider a[] = 3 * 2^return (not -1 * 2^return), because
 * -13 = -3 * 4 - 1 = -4 * 4 + 3.
 *
 * Return the number of how many times it iterates the loop for calculating
 * remainder. In each iteration, the divisor b[] and the remainder container
 * a[] will be multiplied by 2; so after calling this divide1( ), b[]'s value
 * is changed by times 2^return, and this is the same case for the remainder
 * container a[]. As a result, the remainder should be a[]/2^return
 * (i.e., a[]/d[]). c[] & d[] need be initialised to 0 before calling.
 *                                         - Changed by XuZF
 */
int	divide( int a[], int b[], int c[], int n, int m)
{
	int	i, j, q, x;
	int	a1[100];
	int	CountRe;	/* Added by XuZF: Counter for remainder */

	while (b[m-1] == 0)
		m = m - 1;

	a[n] = 0;
	n = n + 1;

	CountRe = 0;
	while (b[m-1] < (r/2))
	{
		CountRe++;

		for (i=0; i <= m-1; i++)
			b[i] = 2 * b[i];
		for (i=0; i <= m-1; i++)
		{
			b[i+1] = b[i+1] + b[i]/r;
			b[i]   = b[i] % r;
		}

		for (j=0; j <= n-1; j++)
			a[j] = 2 * a[j];
		for (j=0; j <= n-1; j++)
		{
			a[j+1] = a[j+1] + a[j]/r;
			a[j]   = a[j] % r;
		}
	}
printf( "Divide( ) - The counter for remainder: %d\n", CountRe);

	for (j = n-m-1; j >=0; j--)
	{
		x = r * a[j+m] + a[j+m-1];
		a[j+m-1] = x;
		a[j+m]   = 0;
		q = x/b[m-1] + 1;	// Guess q
		do
		{
			for (i = m-1; i >= 0; i--)
				a1[j+i] = a[j+i];

			q = q - 1;
//			count++;	commented by XuZF

			for (i = m-1; i >=0; i--)
				a1[j+i] = a1[j+i] - q * b[i];
			for (i = 0; i <= m-2; i++)
			{
				if (a1[j+i] < 0)
				{
					a1[j+i+1] = a1[j+i+1] - (r-1-a1[i+j])/r;	// debit from left
					a1[j+i]   = a1[j+i] + ((r-1-a1[i+j])/r)*r;	// remaining at i+j
				}
			}
		} while (a1[j+m-1] < 0);

		c[j] = q;
		for (i = m-1; i >= 0; i--)
			a[j+i] = a1[j+i];
	}

	return CountRe;
}

int	main( void)
{
	int	ii, jj;
	int	aa1[100], d[100];
	long long int	ll = 999999, lla, llb, llc;

	printf( "Size of (int): %d\n", sizeof( int));

	count = 0;

	r = 25; n = 3;
	a[0] = -99; a[1] = -89; a[2] = -99;
	b[0] = 22; b[1] = 11; b[2] = 3;
	c[0] = c[1] = c[2] = c[3] = c[4] = c[5] = 0;

	multiply1( a, b, c, n);
	for (ii = 2*n; ii > 0; ii--)
		printf( "c[%d]=%d ", ii-1, c[ii-1]);
	printf( "\n");
	printf( "c = %lld\n", ((long long)c[5]*r*r*r*r*r +
		(long long)c[4]*r*r*r*r + c[3]*r*r*r + c[2]*r*r + c[1]*r + c[0]));

	printf( "a[]:"); PrintIntArray( a, 3);
	printf( "b[]:"); PrintIntArray( b, 3);
	printf( "a = %lld\n", lla = a[2]*r*r + a[1]*r + a[0]);
	printf( "b = %lld\n", llb = b[2]*r*r + b[1]*r + b[0]);
	printf( "a*b = %lld\n", lla*llb);
	printf( "ll^2 = %lld\n", ll*ll);
	printf( "\n");


	printf( "Divide ...\n");
	r = 17; n = 3;
	c[0] = -14; c[1] = -11; c[2] = -15; c[3] = -8; c[4] = -2; c[5] = -3;
	a[0] = 13; a[1] = 15; a[2] = 16;
	InitArray( b, 0, 0, 2*n-1);
	InitArray( d, 0, 0, 2*n-1);
	printf( "c[]:"); PrintIntArray( c, 2*n);
	printf( "a[]:"); PrintIntArray( a, n);
	printf( "IntArrayValue( ): c = %lld\n", llc=IntArrayValue( c, r, 2*n));
	printf( "IntArrayValue( ): a = %lld\n", lla=IntArrayValue( a, r, n));
	printf( "llc / lla = %lld	llc %% lla = %lld\n", llc/lla, llc%lla);
//	jj = divide( c, a, b, 2*n, n);
//	jj = divide1( c, a, b, d, 2*n, n);
	jj = divide1( c, a, b, 2*n, n);
	printf( "After divide( ) ...\n");
	printf( "The counter for remainder: %d\n", jj);
	printf( "a[]:"); PrintIntArray( a, n);
	printf( "b[]:"); PrintIntArray( b, 2*n);
	printf( "c[]:"); PrintIntArray( c, 2*n);
	printf( "IntArrayValue( ): a = %lld\n", IntArrayValue( a, r, n));
	printf( "IntArrayValue( ): b = %lld\n", IntArrayValue( b, r, 2*n));
	printf( "IntArrayValue( ): c = %lld\n", IntArrayValue( c, r, 2*n));
exit( 0);

	printf( "Calculating remainder ...\n");
	/* Calculate 2^jj */
/*
	aa1[0] = 2; aa1[1] = aa1[2] = 0;
	a[0] = 1; a[1] = a[2] = 0;
	b[0] = b[1] = b[2] = b[3] = 0;
	for (ii = 0; ii < jj; ii++)
	{
		multiply1( a, aa1, b, n);
		a[0] = b[0]; a[1] = b[1]; a[2] = b[2];
	}
	printf( "IntArrayValue( ): a = %lld\n", IntArrayValue( a, r, n));
	printf( "IntArrayValue( ): b = %lld\n", IntArrayValue( b, r, 2*n));
	printf( "IntArrayValue( ): c = %lld\n", IntArrayValue( c, r, 2*n));
	printf( "IntArrayValue( ): d = %lld\n", IntArrayValue( d, r, 2*n));
	b[0] = b[1] = b[2] = b[3] = b[4] = b[5] = 0;
	InitArray( a, 0, 0, 2*n-1);
//	divide( c, a, b, 2*n, n);
	divide1( c, d, b, a, 2*n, n);
	printf( "1. After divide( ) ...\n");
	printf( "a[]:"); PrintIntArray( a, n);
	printf( "b[]:"); PrintIntArray( b, 2*n);
	printf( "c[]:"); PrintIntArray( c, 2*n);
	printf( "d[]:"); PrintIntArray( d, 2*n);
	printf( "IntArrayValue( ): a = %lld\n", IntArrayValue( a, r, n));
	printf( "IntArrayValue( ): b = %lld\n", IntArrayValue( b, r, 2*n));
	printf( "IntArrayValue( ): c = %lld\n", IntArrayValue( c, r, 2*n));
	printf( "IntArrayValue( ): d = %lld\n", IntArrayValue( d, r, 2*n));
 */

	return 0;
}
