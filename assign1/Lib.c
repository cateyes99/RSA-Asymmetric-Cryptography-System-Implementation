#include	<stdio.h>
#include	<stdlib.h>

#include	"Prototypes.h"
#include	"Declare.h"

void	InitMain( void)
{
	int	ii;

	printf( "... Check the size of int: %d bytes\n", ii = sizeof( int));
	if (ii < 4)
	{
		printf( "Size of int must be >= 4 bytes\a\n");
		exit( 1);
	}

	r         = 0x4000;	// 2^RadixBits = 2^14 = 0x4000 = 16384
	RadixBits = 14;
	n      = 12;	// 5 > {digits of 2^14} > 4; 12 * 4 = 48 is about 50 digits
	FixLen = 4*n;
/*
r         = 0x4000;
RadixBits = 14;
n      = 4;
FixLen = 4*n;
 */

	if (FixLen > MaxElem)
	{
		printf( "Length of array is too short (FixLen > MaxElem)!\a\n");
		exit( 1);
	}
	printf( "\nArray's radix is %d, and length of N is %d elements\n", \
		r, 2*n);

	InitArray( Zero, 0, 0, FixLen-1);
	InitArray( One, 0, 0, FixLen-1); One[0] = 1;
	InitArray( Two, 0, 0, FixLen-1); Two[0] = 2;
	InitArray( MinusOne, 0, 0, FixLen-1); MinusOne[0] = -1;

	InitArray( P, 0, 0, FixLen-1); InitArray( Q, 0, 0, FixLen-1);
	InitArray( N, 0, 0, FixLen-1); InitArray( Fai, 0, 0, FixLen-1);
	InitArray( D, 0, 0, FixLen-1); InitArray( E, 0, 0, FixLen-1);

	BitMove = (RadixBits+1) / 2; // In case RadixBits is odd, so plus one
	for ( Mask = 1, ii = 1; ii < BitMove; ii++)
		Mask |= (1 << ii);
	Pad = ((Mask << BitMove) | Mask) % r;
}

/* Note, n1 and n2 are the index of array!
 */
void	InitArray( int * array, int value, int n1, int n2)
{
	int ii;

	for (ii = n1; ii <= n2; ii++)
		array[ii] = value;
}

/* Copy a[] to b[] with the length of nn
 */
void	ArrayA2B( int a[], int b[], int nn)
{
	int ii;

	for (ii = 0; ii < nn; ii++)
		b[ii] = a[ii];
}

/* Set a[] to -1 * a[]
 */
void	NegateIntArray( int a[], int nn)
{
	int ii;

	for (ii = 0; ii < nn; ii++)
		a[ii] = -a[ii];
}

/* If the int array[ ] represents an odd num, return 1; otherwise even,
 * return 0. This works, only if radix r is an even num
 */
int	OddOrEven( int * array, int nn)
{
	return *array & 1;
}

/*
 * pp[ ] is the container for the desired prime, and has been initialised to an
 * odd num already before being called
 */
void	GetPrime( int pp[], int nn)
{
	int	primality, ii, jj, xx, count = 0;
	int	aa[MaxElem], tt[MaxElem], t1[MaxElem];

	do
	{
		primality = 1;	// Suppose the array is prime tentatively
		count++;
		for (ii = 1; ii <= 10; ii++)
		{
			InitArray( aa, 0, 0, FixLen-1);
			for (jj = 0; jj < nn; jj++)
				aa[jj] = random( ) % r;
			J1_Mod( aa, pp, nn);

			ArrayA2B( aa, tt, FixLen);
			ArrayA2B( pp, t1, FixLen);
//printf( "tt[ ] = %lld\n", IntArrayValue( tt, r, nn+1));
//printf( "t1[ ] = %lld\n", IntArrayValue( t1, r, nn+1));
			xx = J1( tt, t1, nn);
//printf("xx = %d\n", xx);
			if (xx != 0)
			{
				Subtract( pp, One, tt, nn);
				DivideBy2( tt, 2, nn);
				exp1( aa, tt, t1, pp, nn);
//printf( "t1[ ] = %lld\n", IntArrayValue( t1, r, nn+1));

				Subtract( pp, One, tt, nn);
				if (IntArrayCmp( t1, tt, nn) == 0) // if (y == n-1)
					ArrayA2B( MinusOne, t1, FixLen);
//printf( "t1'[ ] = %lld\n", IntArrayValue( t1, r, nn+1));

				InitArray( tt, 0, 0, FixLen-1); tt[0] = xx;
				if (IntArrayCmp( tt, t1, nn) != 0)
				{
					primality = 0; break;
				}
			}
		}
		InitArray( tt, 0, 0, FixLen-1);
		Add( pp, Two, tt, nn);	// Be careful of overflow
		ArrayA2B( tt, pp, FixLen);
//printf( "New pp[ ] = %lld\n", IntArrayValue( pp, r, nn+1));
	} while (primality == 0);
//printf( "GetPrime( ): count = %d\n", count-1);

	Subtract( pp, Two, tt, nn);
	ArrayA2B( tt, pp, FixLen);
}

/*
 * a[] + b[] => c[]
 * Both a[] and b[] are the same positive or negative; or one is 0, the other
 * doesn't matter. nn is the effective length of each array.
 * To avoid overflow, the max of a [] & b[]'s int value should be shorter than
 * nn at least by one. a[] & b[] aren't changed in the func. Be careful, c[]
 * need be initialised to 0 before this function is called.
 *
 * Actually, if a[] & b[] are different sign, then the result c[] might be
 * funny (e.g. c[2]=-3 c[1]=2 c[0]=-7), but still correct, after makeing the
 * the sum of all the elements of c[].
 */
void	PureAdd( int a[], int b[], int c[], int nn)
{
	int ii;

	for (ii = 0; ii < nn; ii++)
	{
		c[ii+1] = (c[ii] + a[ii] + b[ii]) / r;
		c[ii]   = (c[ii] + a[ii] + b[ii]) % r;
	}
}

/*
 * a[] - b[] => c[]. nn is the effective length of the array.
 * The precondition: a[] >= b[]. And a[], b[] can be different signed, if it's
 * this case, nn should be longer than the the max of a[] & b[]'s int value at
 * least by one, otherwise it might cause overflow. For example,
 * a[3]={79, 79, 79}, b[3]={-79, -79, -79}, radix=80, nn = 3; then a[] - b[]
 * will result in overflow. Further, because we pre-use a[ii+1], so the
 * absolute length of a[] should be longer than nn at least by one. a[] & b[]
 * aren't changed in the func
 */
void	PureSubtract( int a[], int b[], int c[], int nn)
{
	int	tt[ MaxElem], ii;

/* This will change a[]'s value, not good!
	for (ii = 0; ii < nn; ii++)
	{
		a[ii+1]--;
		c[ii]    = (a[ii] + r - b[ii]) % r;
		a[ii+1] += (a[ii] + r - b[ii]) / r;
	}
 */
	ArrayA2B( a, tt, FixLen);
	for (ii = 0; ii < nn; ii++)
	{
		tt[ii+1]--;
		c[ii]     = (tt[ii] + r - b[ii]) % r;
		tt[ii+1] += (tt[ii] + r - b[ii]) / r;
	}
}

/*
 * a[] + b[] => c[]
 * c[] need be initialised to 0 before this function is called.
 */
void	Add( int a[], int b[], int c[], int nn)
{
	int	ii, aa=0, bb=0, temp[MaxElem];

	aa = JudgeSign( a, nn); bb = JudgeSign( b, nn);
	if (aa * bb >= 0)	// a[], b[] are same signed. Be careful of overflow
		PureAdd( a, b, c, nn);
	else if (aa > 0)	// And bb < 0
	{
		ArrayA2B( b, temp, FixLen); NegateIntArray( temp, nn);
		if (IntArrayCmp( a, temp, nn) >= 0)
			PureSubtract( a, temp, c, nn);
		else
		{
			PureSubtract( temp, a, c, nn);
			NegateIntArray( c, nn);
		}
	}
	else	// aa < 0 and b > 0
	{
		ArrayA2B( a, temp, FixLen); NegateIntArray( temp, nn);
		if (IntArrayCmp( b, temp, nn) >= 0)
			PureSubtract( b, temp, c, nn);
		else
		{
			PureSubtract( temp, b, c, nn);
			NegateIntArray( c, nn);
		}
	}
}

/*
 * a[] - b[] => c[]
 */
void	Subtract( int a[], int b[], int c[], int nn)
{
	if (IntArrayCmp( a, b, nn) >= 0)
		PureSubtract( a, b, c, nn);
	else
	{
		PureSubtract( b, a, c, nn);
		NegateIntArray( c, nn);
	}
}

/*
 * If a[] > 0, then return >0; if a[] < 0, then return <0;
 * if a[] is 0, then return 0
 */
int	JudgeSign(int a[], int nn)
{
	int	ii;

	for (ii = nn-1; ii >= 0 && a[ii] == 0; ii--)
		;
	return ii == -1 ? 0 : a[ii];
}

/*
 * Compare a[] & b[], return a[] - b[]
 */
int	IntArrayCmp( int a[], int b[], int nn)
{
	int ii;

	for (ii = nn-1; ii >= 0 && a[ii] == b[ii]; ii--)
		;

	return ii == -1 ? 0 : (a[ii] - b[ii]);
}

void	PrintIntArray( int * array, int nn)
{
	int ii;

	for (ii = nn; ii > 0; ii--)
		printf( " $[%d]=%d", ii-1, array[ii-1]);
	printf( "\n");
}

/*
 * a[] * b[] => c[]. a[] & b[] can be any signed (positive or negative).
 * a[] & b[]'s values aren't changed in this function
 */
void	multiply1( int a[], int b[], int c[], int n)
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
 * a[] / b[] => c[], and a[] % b[] => a[]. b[] must be positive, a[] doesn't
 * matter. If a[] is negative, then the result is quite interesting: for
 * example, a[] / b[] = -13 / 4, c[] = -4 (not -3), and the remainder a[] = 3
 * (not -1), because -13 = -3 * 4 - 1 = -4 * 4 + 3.
 *
 * Return the number of how many times it iterates the loop. In each iteration,
 * the divisor b[] and the remainder container a[] will be multiplied by 2; so
 * after calling this divide1( ), b[]'s value CHANGES to be as 2^return times
 * as its original. c[] need be initialised to 0 before calling.
 *                                         - Changed by XuZF
 */
// int	divide1( int a[], int b[], int c[], int d[], int n, int m)
int	divide1( int a[], int b[], int c[], int n, int m)
{
	int	i, j, q, x;
	int	a1[MaxElem];
	int	times, CountRe;	/* Added by XuZF: Counter for remainder */

	while (b[m-1] == 0)
		m--;

	a[n] = 0;
	n++;

	times = 1; CountRe = 0;
	while (b[m-1] < (r/2))
	{
		times *= 2; CountRe++;

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

	for (j = n-m-1; j >= 0; j--)
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

			for (i = m-1; i >= 0; i--)
				a1[j+i] = a1[j+i] - q * b[i];
			for (i = 0; i <= m-2; i++)
			{
				if (a1[j+i] < 0)
				{
					a1[j+i+1] = a1[j+i+1] - (r-1-a1[i+j])/r;
					a1[j+i]   = a1[j+i] + ((r-1-a1[i+j])/r)*r;
				}
			}
		} while (a1[j+m-1] < 0);

		c[j] = q;
		for (i = m-1; i >= 0; i--)
			a[j+i] = a1[j+i];
	}

	/*
	 * Here, (a[] % times) must be 0!, otherwise it's wrong
	 */
//	for (j = n-m; j >= 1; j--)	Bug!
	for (j = n-1; j >= 1; j--)
	{
		a[j-1] += (a[j] % times) * r;
		a[j]   /= times;
	}
	a[0] /= times;

	return CountRe;
}

/*
 * Computes x^e mod c => res[]. This makes use of the fact that the radix is
 * 2^RadixBits (e.g. 2^14); otherwise this function can't work properly.
 * res[] has been initialised in exp1( ).
 * NOTE: The value of nn is important. I suggests it should be the length of N
 * or plus one (N is P * Q)
 */
void    exp1( int x[], int e[], int res[], int c[], int nn)
{
    int y[MaxElem], mask, ii, jj, tt[MaxElem];

    InitArray( res, 0, 0, FixLen-1);    res[0] = 1;
    ArrayA2B( res, y, FixLen);
//  for (ii = 2 * nn - 1; ii >= 0 && e[ii] == 0; ii--)
    for (ii = nn - 1; ii >= 0 && e[ii] == 0; ii--)
        ;
    if (ii == -1)
        return; // Because e[] = 0, so res[] should be 1
    for ( ; ii >= 0; ii--)
    {
        mask  = 1 << (RadixBits - 1);
        for (jj = RadixBits - 1; jj >= 0; jj--)
        {
            multiply1( res, res, y, nn);

			InitArray( res, 0, 0, FixLen-1);    // Computing res % c
			ArrayA2B( c, tt, FixLen);	// Important
			divide1( y, tt, res, 2*nn, nn);
			ArrayA2B( y, res, FixLen);
//printf( "1. res[ ]:"); PrintIntArray( res, 2*nn);
//printf( "1. res[ ] = %lld\n", IntArrayValue( res, r, 2*nn));

            if (e[ii] & mask)
            {
                multiply1( res, x, y, nn);
//printf( "2. y[ ] = %lld\n", IntArrayValue( y, r, 2*nn));

                InitArray( res, 0, 0, FixLen-1);    // Computing res % c
			    ArrayA2B( c, tt, FixLen);
                divide1( y, tt, res, 2*nn, nn);
				ArrayA2B( y, res, FixLen);
//printf( "2. res[ ]:"); PrintIntArray( res, 2*nn);
//printf( "2. res[ ] = %lld\n", IntArrayValue( res, r, 2*nn));
            }
            mask >>= 1;
        }
    }
}

/*
 * Jaqcobi func. a[] & b[] must be positive, because of DivideBy2( ); and both
 * a[] & b[] might be changed in this func. And, I think, in Japcobi func,
 * a[] & b[] always >= 0.
 * The method of minimising the local variables in this recursive func to
 * avoid overuse the system source by assigning they into to other func,
 * ExpMinusOne1( ), ExpMinusOne2( ) & J1_Mod( ) is good; but other recursive
 * funcs might not be able to apply this way, because Japcobi func has its own
 * characteristics.
 */
int	J1( int a[], int b[], int nn)
{
	int	ii;
//int jj, kk;

	if (IntArrayCmp( a, Zero, nn) == 0)
		return 0;

	if (IntArrayCmp( a, One, nn) == 0)
		return 1;

	if (OddOrEven( a, nn) == 0)
	{
		DivideBy2( a, 2, nn);
//printf( "J1.1 a=%lld  b=%lld  ", IntArrayValue(a,r,nn), IntArrayValue(b,r,nn));
//printf( "EO1: %d\n",  ExpMinusOne1( b, nn));
		/* J1( ) must follow ExpMinusOne1( ), because b is global and
		 * might be change in recursion; so if the oreder is reverse,
		 * ExpMinusOne1( )'s b will be changed unexpectedly! */
		return  ExpMinusOne1( b, nn) * J1( a, b, nn);
//jj = (kk = ExpMinusOne1( b, nn)) * J1( a, b, nn);
//printf( "  jj = %d  kk = %d\n", jj, kk);
//return jj;
	}
	else
	{
		ii = ExpMinusOne2( a, b, nn);
		J1_Mod( b, a, nn);
//printf( "J1.2 b=%lld  a=%lld  ", IntArrayValue(b,r,nn), IntArrayValue(a,r,nn));
//printf( "EO2 ii: %d\n",  ii);
		return J1( b, a, nn) * ii;
//jj = J1( b, a, nn) * ii;
//printf( "  jj = %d  ii = %d\n", jj, ii);
//return jj;
	}
}

/*
 * Used in J1( ) to minimise the usage of local vairables in recursion.
 * a[] mod b[] => a[], b[] isn't changed in this function, and must be
 * positive (Refer to divide1( )'s description)
 */
void	J1_Mod( int a[], int b[], int nn)
{
	int	tt[MaxElem], t1[MaxElem];

	ArrayA2B( b, tt, FixLen);
	InitArray( t1, 0, 0, FixLen-1);
	divide1( a, tt, t1, nn, nn);
}

/*
 * Only used in J1( ) as exp( -1, (n*n-1) / 8), to minimise the usage of local
 * vairables in recursion and make J1( ) simpler. b[] isn't changed in this
 * function
 */
int	ExpMinusOne1( int b[], int nn)
{
	int	tt[MaxElem], tt1[MaxElem], tt2[MaxElem], ii;

	multiply1( b, b, tt, nn);

	InitArray( tt1, 0, 0, FixLen-1); tt1[0] = 1;
	InitArray( tt2, 0, 0, FixLen-1);
	Subtract( tt, tt1, tt2, 2*nn);

	ii = 0;
	if (JudgeSign( tt2, 2*nn) < 0)
	{
		NegateIntArray( tt2, 2*nn);
		ii = 1;
	}
	DivideBy2( tt2, 8, 2*nn);
	if (ii == 1)
		NegateIntArray( tt2, 2*nn);	// Not necessary

	return OddOrEven( tt2, 2*nn) ? -1: 1;
}

/*
 * Only used in J1( ) as exp( -1, (a-1)*(n-1) / 4), to minimise the usage of
 * local vairables in recursion and make J1( ) simpler. a[] & b[] aren't
 * changed in this function
 */
int	ExpMinusOne2( int a[], int b[], int nn)
{
	int	tt[MaxElem], tt1[MaxElem], tt2[MaxElem], ii;

	InitArray( tt, 0, 0, FixLen-1); tt[0] = 1;
	InitArray( tt1, 0, 0, FixLen-1);
	InitArray( tt2, 0, 0, FixLen-1);
	Subtract( a, tt, tt1, nn);
	Subtract( b, tt, tt2, nn);
	multiply1( tt1, tt2, tt, nn);
	ii = 0;
	if (JudgeSign( tt, 2*nn) < 0)
	{
		NegateIntArray( tt, 2*nn);
		ii = 1;
	}
	DivideBy2( tt, 4, 2*nn);
	if (ii == 1)
		NegateIntArray( tt, 2*nn);

	return OddOrEven( tt, 2*nn) ? -1: 1;
}

/*
 * a[] / bb => a[]
 * This divides a[] by 2^bits (bb) to a[], by using bitwise-shift, e.g. a/2,
 * a/4, a/8 => a>>1, a>>2, a>>3. This makes use of the fact the radix in my
 * program is 2^RadixBits, so bb must be 2^bits; and a[] must be positive, or
 * result is pointless
 */
void	DivideBy2( int a[], int bb, int nn)
{
	int	bits, bits1, ii, mask;

	for (bits1 = 0; bb >>= 1; bits1++)
		;

	for ( ; bits1 > 0; bits1 -= RadixBits)
	{
		bits = (bits1 > RadixBits) ? RadixBits : bits1;

		for ( mask = 1, ii = 1; ii < bits; ii++)
			mask |= (1 << ii);

		for (ii = 0; ii < nn; ii++)
		{
			a[ii] >>= bits;
			if (ii == nn - 1)	break;
			a[ii] |= ((a[ii+1] & mask) << (RadixBits - bits));
		}
	}
}

/*
 * (P-1)*(Q-1) => Fai; P = pp, Q = qq, Fai = ff
 */
void	GetFai( int pp[], int qq[], int ff[], int nn)
{
	int	tt[MaxElem], t1[MaxElem];

	Subtract( pp, One, tt, nn);
	Subtract( qq, One, t1, nn);

	multiply1( tt, t1, ff, nn);
}

/*
 * Use Euclid's algorithm for gcd( ) to compute E. E * D = 1 (mod Fai). Here
 * Fai is definitely larger than D, so nn should be determined by Fai
 */
void	GetE( int Fai[], int D[], int E[], int nn)
{
	int	r0[MaxElem], r1[MaxElem], r2[MaxElem], q1[MaxElem];
	int	a0[MaxElem], a1[MaxElem], a2[MaxElem];
	int	b0[MaxElem], b1[MaxElem], b2[MaxElem];
	int	ii, tt[MaxElem];

	ArrayA2B( Fai, r0, FixLen); ArrayA2B( D, r1, FixLen);
	ArrayA2B( Zero, a0, FixLen); ArrayA2B( One, a1, FixLen);
	ArrayA2B( One, b0, FixLen); ArrayA2B( Zero, b1, FixLen);

	while (1)
	{
//printf( "\nGetE( ): New round ...\n");
//printf( "r0[ ]:"); PrintIntArray( r0, nn);
//printf( "r0[ ] = %lld\n", IntArrayValue( r0, r, nn));
//printf( "r1[ ]:"); PrintIntArray( r1, nn);
//printf( "r1[ ] = %lld\n", IntArrayValue( r1, r, nn));
		InitArray( q1, 0, 0, FixLen-1);
		InitArray( tt, 0, 0, FixLen-1);
		ArrayA2B( r1, tt, FixLen);
		divide1( r0, tt, q1, nn, nn);
		ArrayA2B( r0, r2, FixLen);
//printf( "q1[ ]:"); PrintIntArray( q1, nn);
//printf( "q1[ ] = %lld\n", IntArrayValue( q1, r, nn));
//printf( "r2[ ]:"); PrintIntArray( r2, nn);
//printf( "r2[ ] = %lld\n", IntArrayValue( r2, r, nn));

		multiply1( q1, a1, tt, nn);
		InitArray( a2, 0, 0, FixLen-1);
		Subtract( a0, tt, a2, 2*nn);
//printf( "a2[ ]:"); PrintIntArray( a2, nn);
//printf( "a2[ ] = %lld\n", IntArrayValue( a2, r, nn));

		multiply1( q1, b1, tt, nn);
		InitArray( b2, 0, 0, FixLen-1);
		Subtract( b0, tt, b2, 2*nn);
//printf( "b2[ ]:"); PrintIntArray( b2, nn);
//printf( "b2[ ] = %lld\n", IntArrayValue( b2, r, nn));

		if (IntArrayCmp( r2, Zero, nn) == 0)
		{
//printf( "* a1[ ]:"); PrintIntArray( a1, nn);
//printf( "* a1[ ] = %lld\n", IntArrayValue( a1, r, nn));
			if (IntArrayCmp( a1, Zero, 2*nn) < 0)
			{
				InitArray( E, 0, 0, FixLen-1);
				Add( Fai, a1, E, 2*nn);
			}
			else
				ArrayA2B( a1, E, FixLen);
//printf( "* E[ ]:"); PrintIntArray( E, nn);
//printf( "* E[ ] = %lld\n", IntArrayValue( E, r, nn));

			break;
		}

		ArrayA2B( r1, r0, FixLen);
		ArrayA2B( r2, r1, FixLen);

		ArrayA2B( a1, a0, FixLen);
		ArrayA2B( a2, a1, FixLen);
		ArrayA2B( b1, b0, FixLen);
		ArrayA2B( b2, b1, FixLen);
	}
}

void	StoreKey( int Key[], int nn, char * KeyFile)
{
	FILE	* fp;
	int	ii;

	if ((fp = fopen( KeyFile, "w")) == NULL)
	{
		printf( "ERROR: [%s] cannot be opened!\n", KeyFile);
		exit( 1);
	}

	fprintf( fp, "%d", Key[0]);
	for (ii = 1; ii < nn; ii++)
		fprintf( fp, ":%d", Key[ii]);

	fclose( fp);
}

void	RetrieveKey( int Key[], int nn, char * KeyFile)
{
	FILE	* fp;
	char	str[50], *pch, ch;
	int	end = 0, ii;

	if ((fp = fopen( KeyFile, "r")) == NULL)
	{
		printf( "ERROR: [%s] cannot be opened!\n", KeyFile);
		exit( 1);
	}

	for (ii = 0; end == 0; ii++)
	{
		pch = str;
		while (1)
		{
			if ((ch = fgetc( fp)) == ':')
				break;
			else if (ch == EOF)
			{
				end = 1;
				break;
			}
			*pch++ = ch;
		}

		if (ii >= nn)
		{
			printf( "ERROR: The retrieved key is too long\n");
			exit( 1);
		}

		*pch = '\0';

//printf( "RetrieveKey( ): str = [%s]\n", str);
		Key[ii] = atoi( str);
	}

	InitArray( Key, 0, ii, FixLen-1);

	fclose( fp);
}

/*
 * Put single char into one element of int array to encrypt, so this
 * can deal with like Chinese language, etc, unlike BlockEncrypt2( )
 */
void	BlockEncrypt( FILE *FClear, FILE *FCipher)
{
	char	ch, buf[2*MaxElem];
	int	ii, Clear[MaxElem], Cipher[MaxElem];

	while (1)
	{
		/*
		 * Encryption block shuld be one shorter than NLen, to ensure Clear
		 * is smaller than N (the modulus in Clear^E Mod N)
		 */
		for (ii = 0; ii < NLen-1; ii++)
		{
			if ((ch = fgetc( FClear)) == EOF)
				break;
			Clear[ii] = (unsigned char)ch;
		}

		if (ii != 0)	// Some chars have been read
		{
			InitArray( Clear, 0, ii, FixLen-1);
			if (ii != NLen-1)
//				Mistake, because it may cause Clear >= N!
//				InitArray( Clear, Pad, ii, NLen-1);
				InitArray( Clear, Pad, ii, NLen-2);

			exp1( Clear, E, Cipher, N, NLen);
			for (ii = 0; ii < NLen; ii++)
			{
				buf[2*ii]   = Cipher[ii] & Mask;
				buf[2*ii+1] = (Cipher[ii] >> BitMove) & Mask;
			}
			fwrite( buf, 2 * NLen * sizeof( char), 1, FCipher);
			if (ferror( FCipher))
			{
				printf( "ERROR: Writing to CipherTxt file failed!\n");
				exit( 1);
			}
		}

		if (ch == EOF)	break;
	}
}

// BlockEncrypt2( ) hasn't been tested yet
/*
 * Put two chars into one element of int array to encrypt, so the enryption
 * speed will be doubled
 */
void	BlockEncrypt2( FILE *FClear, FILE *FCipher)
{
	char	ch;
	int	len, ii, Clear[MaxElem], Cipher[MaxElem];

	while (1)
	{
		for (len = 2*(NLen-1), ii = 0; ii < len; ii++)
		{
			if ((ch = fgetc( FClear)) == EOF)
				break;
			else if (ch < 0)	// -128 <= ch <= 127
			{
				printf( "Encrypting failed!\n"
					"The file to be encrypted must be TXT file\n"
					"Each char's ASCII value inside should be 1~127\n");
				exit( 1);
			}
			// So, RadixBits need be even
//			Clear[ii/2] = (ii & 1) ? ((Clear[ii/2] << RadixBits/2) + ch) : ch;
			Clear[ii/2] = (ii & 1) ? ((Clear[ii/2] << RadixBits/2) + \
				(unsigned char)ch) : (unsigned char)ch;
		}

		if (ii != 0)	// No ch has been read
		{
			InitArray( Clear, 0, (ii+1)/2, FixLen-1);

			exp1( Clear, E, Cipher, N, NLen);
printf( "BlockEncrypt2( ): Cipher is:"); PrintIntArray( Cipher, NLen+1);
			fwrite( Cipher, NLen * sizeof( int), 1, FCipher);
			if (ferror( FCipher))
			{
				printf( "ERROR: Writing to CipherTxt file failed!\n");
				exit( 1);
			}
		}

		if (ch == EOF)	break;
	}
}

/*
 * Put single char into one element of int array to encrypt, so this
 * can deal with like Chinese language, etc, unlike BlockEncrypt2( )
 */
void	BlockDecrypt( FILE *FCipher, FILE *FClear)
{
	int	Clear[MaxElem], Cipher[MaxElem], ClearLen, BitMove, ii;
	char	buf[2*MaxElem];

	BitMove = (RadixBits+1) / 2; // In case RadixBits is odd, so plus one

	while (1)
	{
		fread( buf, 2 * NLen * sizeof( char), 1, FCipher);
		if (feof( FCipher))
			break;
		if (ferror( FCipher))
		{
			printf( "ERROR: Reading CipherTxt file failed!\n");
			exit( 1);
		}
		for (ii = 0; ii < NLen; ii++)
		{
			Cipher[ii] = buf[2*ii+1];
//			Cipher[ii] = (Cipher[ii] << BitMove) + buf[2*ii];
			Cipher[ii] = (Cipher[ii] << BitMove) | buf[2*ii];
		}

		InitArray( Cipher, 0, NLen, FixLen-1);

		exp1( Cipher, D, Clear, N, NLen);
//		ClearLen = ArrayValueLen( Clear, 0, FixLen-1); // bug
		ClearLen = ArrayValueLen( Clear, Pad, NLen-1);
		for (ii = 0; ii < ClearLen; ii++)
			if (fputc( Clear[ii], FClear) == EOF)
			{
				printf( "ERROR: Writing ClearTxt file failed!\n");
				exit( 1);
			}
	}
}

/* 
 * Work together with BlockEncrypt2( )
 */
void	BlockDecrypt2( FILE *FCipher, FILE *FClear)
{
}

/*
 * Computing the length of the effective value in aa[]. prefix's value
 * means, in the length nn of the array, these prefix should be excluded.
 * If aa = {12, 30, 7, 0, 0, 0, 6}, for prefix = 0, nn = 5, return 3
 */
int	ArrayValueLen( int aa[], int prefix, int nn)
{
	int	ii;

	for (ii = nn-1; ii >= 0 && aa[ii] == prefix; ii--)
		;
	return ii+1;
}

/*-------------------- Debugging functions --------------------*/
/*
 * Calculating x^y. Basically it's for debugging
 */
long long	Power( long x, long y)
{
	long	ll;
	long long tt;

	for (ll = 0, tt = 1; ll < y; ll++)
		tt *= x;

	return tt;
}

/*
 * Calculating the int array's int value. It's for debugging
 */
long long	IntArrayValue( int * array, int r, int nn)
{
	int	ii;
	long long	ll;

	for (ii = 0, ll = 0; ii <nn; ii++)
		ll += array[ii] * Power( r, ii);

	return ll;
}
