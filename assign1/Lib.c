#include	<stdio.h>
#include	<stdlib.h>

#include	"Prototypes.h"
#include	"Declare.h"

void	InitMain( void)
{
	int	i;

	printf( "... Check the size of int: %d bytes\n", i = sizeof( int));
	if (i < 4)
	{
		printf( "Size of int must be >= 4 bytes\a\n");
		exit( 1);
	}

	RadixBits = 14;
	Radix     = 1 << RadixBits;	// 2^RadixBits: 2^14 = 0x4000 = 16384
	n         = 12;	// 5 > { number of digits for 2^14} > 4;
					// 12 * 4 = 48, is about 50 digits.
	FixedLength = 4*n;
/*
RadixBits   = 14;
Radix       = 1 << RadixBits;
n           = 4;
FixedLength = 4*n;
 */

	if (FixedLength > MaxLength)
	{
		printf( "Length of array is too short (FixedLength > MaxLength)!\a\n");
		exit( 1);
	}
	printf( "\nArray's radix is %d, and length of N is %d elements\n", \
		Radix, 2*n);

	InitArray( Zero, 0, 0, FixedLength-1);
	InitArray( One, 0, 0, FixedLength-1); One[0] = 1;
	InitArray( Two, 0, 0, FixedLength-1); Two[0] = 2;
	InitArray( MinusOne, 0, 0, FixedLength-1); MinusOne[0] = -1;

	InitArray( P, 0, 0, FixedLength-1); InitArray( Q, 0, 0, FixedLength-1);
	InitArray( N, 0, 0, FixedLength-1); InitArray( Fai, 0, 0, FixedLength-1);
	InitArray( D, 0, 0, FixedLength-1); InitArray( E, 0, 0, FixedLength-1);

	BitMove = (RadixBits+1) / 2; // In case RadixBits is odd, so plus one
	for ( Mask = 1, i = 1; i < BitMove; i++)
	{
		Mask |= (1 << i);
	}
	Pad = ((Mask << BitMove) | Mask) % Radix;
}

/* Note, start and end are the index of array!
 */
void	InitArray( int * array, int value, int start, int end)
{
	int i;

	for (i = start; i <= end; i++)
	{
		array[i] = value;
	}
}

/* Copy a[] to b[] with the specified length
 */
void	ArrayA2B( int a[], int b[], int length)
{
	int i;

	for (i = 0; i < length; i++)
	{
		b[i] = a[i];
	}
}

/* Set a[] to -1 * a[]
 */
void	NegateIntArray( int a[], int length)
{
	int i;

	for (i = 0; i < length; i++)
	{
		a[i] = -a[i];
	}
}

/* If the int array[] represents an odd num, return 1; otherwise even,
 * return 0. This works, only if radix Radix is an even num
 */
int	OddOrEven( int * array, int length)
{
	return *array & 1;
}

/*
 * p[] is the container for the desired prime, and has been initialised to an
 * odd num already before being called
 */
void	GetPrime( int p[], int length)
{
	int	primality, i, j, x, count = 0;
	int	a[MaxLength], temp[MaxLength], temp1[MaxLength];

	do
	{
		primality = 1;	// Suppose the array is prime tentatively
		count++;
		for (i = 1; i <= 10; i++)
		{
			InitArray( a, 0, 0, FixedLength-1);
			for (j = 0; j < length; j++)
				a[j] = random( ) % Radix;
			JaqcobiMod( a, p, length);

			ArrayA2B( a, temp, FixedLength);
			ArrayA2B( p, temp1, FixedLength);
//printf( "temp[] = %lld\n", IntArrayValue( temp, Radix, length+1));
//printf( "temp1[] = %lld\n", IntArrayValue( temp1, Radix, length+1));
			x = Jaqcobi( temp, temp1, length);
//printf("x = %d\n", x);
			if (x != 0)
			{
				Subtract( p, One, temp, length);
				DivideBy2( temp, 2, length);
				Exp( a, temp, temp1, p, length);
//printf( "temp1[] = %lld\n", IntArrayValue( temp1, Radix, length+1));

				Subtract( p, One, temp, length);
				if (IntArrayCmp( temp1, temp, length) == 0) // if (y == n-1)
					ArrayA2B( MinusOne, temp1, FixedLength);
//printf( "temp1'[] = %lld\n", IntArrayValue( temp1, Radix, length+1));

				InitArray( temp, 0, 0, FixedLength-1); temp[0] = x;
				if (IntArrayCmp( temp, temp1, length) != 0)
				{
					primality = 0;
					break;
				}
			}
		}
		InitArray( temp, 0, 0, FixedLength-1);
		Add( p, Two, temp, length);	// Be careful of overflow
		ArrayA2B( temp, p, FixedLength);
//printf( "New p[] = %lld\n", IntArrayValue( p, Radix, length+1));
	} while (primality == 0);
//printf( "GetPrime( ): count = %d\n", count-1);

	Subtract( p, Two, temp, length);
	ArrayA2B( temp, p, FixedLength);
}

/*
 * a[] + b[] => c[]
 * Both a[] and b[] are the same positive or negative; or one is 0, the other
 * doesn't matter. 'length' is the effective length of each array.
 * To avoid overflow, the max of a [] & b[]'s int value should be shorter than
 * 'length' at least by one. a[] & b[] aren't changed in the func. Be careful, c[]
 * need be initialised to 0 before this function is called.
 *
 * Actually, if a[] & b[] are different sign, then the result c[] might be
 * funny (e.g. c[2]=-3 c[1]=2 c[0]=-7), but still correct, after making the
 * the sum of all the elements of c[].
 */
void	PureAdd( int a[], int b[], int c[], int length)
{
	int i;

	for (i = 0; i < length; i++)
	{
		c[i+1] = (c[i] + a[i] + b[i]) / Radix;
		c[i]   = (c[i] + a[i] + b[i]) % Radix;
	}
}

/*
 * a[] - b[] => c[]. 'length' is the effective length of the array.
 * The precondition: a[] >= b[]. And a[], b[] can be different signed, if it's
 * this case, 'length' should be longer than the the max of a[] & b[]'s int value at
 * least by one, otherwise it might cause overflow. For example,
 * a[3]={79, 79, 79}, b[3]={-79, -79, -79}, radix=80, length = 3; then a[] - b[]
 * will result in overflow. Further, because we pre-use a[i+1], so the
 * absolute length of a[] should be longer than 'length' at least by one. a[] & b[]
 * aren't changed in the func
 */
void	PureSubtract( int a[], int b[], int c[], int length)
{
	int	temp[ MaxLength], i;

/* This will change a[]'s value, not good!
	for (i = 0; i < length; i++)
	{
		a[i+1]--;
		c[i]    = (a[i] + Radix - b[i]) % Radix;
		a[i+1] += (a[i] + Radix - b[i]) / Radix;
	}
 */
	ArrayA2B( a, temp, FixedLength);
	for (i = 0; i < length; i++)
	{
		temp[i+1]--;
		c[i]     = (temp[i] + Radix - b[i]) % Radix;
		temp[i+1] += (temp[i] + Radix - b[i]) / Radix;
	}
}

/*
 * a[] + b[] => c[]
 * c[] need be initialised to 0 before this function is called.
 */
void	Add( int a[], int b[], int c[], int length)
{
	int	i, aSign=0, bSign=0, temp[MaxLength];

	aSign = JudgeSign( a, length); bSign = JudgeSign( b, length);
	if (aSign * bSign >= 0)	// a[], b[] are same signed. Be careful of overflow
		PureAdd( a, b, c, length);
	else if (aSign > 0)	// And bSign < 0
	{
		ArrayA2B( b, temp, FixedLength); NegateIntArray( temp, length);
		if (IntArrayCmp( a, temp, length) >= 0)
			PureSubtract( a, temp, c, length);
		else
		{
			PureSubtract( temp, a, c, length);
			NegateIntArray( c, length);
		}
	}
	else	// aSign < 0 and bSign > 0
	{
		ArrayA2B( a, temp, FixedLength); NegateIntArray( temp, length);
		if (IntArrayCmp( b, temp, length) >= 0)
			PureSubtract( b, temp, c, length);
		else
		{
			PureSubtract( temp, b, c, length);
			NegateIntArray( c, length);
		}
	}
}

/*
 * a[] - b[] => c[]
 */
void	Subtract( int a[], int b[], int c[], int length)
{
	if (IntArrayCmp( a, b, length) >= 0)
		PureSubtract( a, b, c, length);
	else
	{
		PureSubtract( b, a, c, length);
		NegateIntArray( c, length);
	}
}

/*
 * If a[] > 0, then return >0; if a[] < 0, then return <0;
 * if a[] is 0, then return 0
 */
int	JudgeSign(int a[], int length)
{
	int	i;

	for (i = length-1; i >= 0 && a[i] == 0; i--)
		;
	return i == -1 ? 0 : a[i];
}

/*
 * Compare a[] & b[], return a[] - b[]
 */
int	IntArrayCmp( int a[], int b[], int length)
{
	int i;

	for (i = length-1; i >= 0 && a[i] == b[i]; i--)
		;

	return i == -1 ? 0 : (a[i] - b[i]);
}

void	PrintIntArray( int * array, int length)
{
	int i;

	for (i = length; i > 0; i--)
	{
		printf( " $[%d]=%d", i-1, array[i-1]);
	}
	printf( "\n");
}

/*
 * a[] * b[] => c[]. a[] & b[] can be any signed (positive or negative).
 * a[] & b[]'s values aren't changed in this function
 */
void	Multiply( int a[], int b[], int c[], int n)
{
	int	i, j;
	for (i = 0; i <= 2 * n - 1; i++)
		c[i] = 0;

	for (i = 0; i <= n-1; i++)
	{
		for (j = 0; j <= n-1; j++)
		{
			c[i+j+1] = c[i+j+1] + (c[i+j] + a[i] * b[j])/Radix;
			c[i+j]   = (c[i+j] + a[i] * b[j]) % Radix;
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
 * after calling this Divide( ), b[]'s value CHANGES to be as 2^return times
 * as its original. c[] need be initialised to 0 before calling.
 *                                         - Changed by XuZF
 */
// int	Divide( int a[], int b[], int c[], int d[], int n, int m)
int	Divide( int a[], int b[], int c[], int n, int m)
{
	int	i, j, q, x;
	int	a1[MaxLength];
	int	times, counter4Remainder;	/* Added by XuZF: Counter for remainder */

	while (b[m-1] == 0)
	{
		m--;
	}

	a[n] = 0;
	n++;

	times = 1; counter4Remainder = 0;
	while (b[m-1] < (Radix/2))
	{
		times *= 2; counter4Remainder++;

		for (i=0; i <= m-1; i++)
		{
			b[i] = 2 * b[i];
		}
		for (i=0; i <= m-1; i++)
		{
			b[i+1] = b[i+1] + b[i]/Radix;
			b[i]   = b[i] % Radix;
		}

		for (j=0; j <= n-1; j++)
		{
			a[j] = 2 * a[j];
		}
		for (j=0; j <= n-1; j++)
		{
			a[j+1] = a[j+1] + a[j]/Radix;
			a[j]   = a[j] % Radix;
		}
	}

	for (j = n-m-1; j >= 0; j--)
	{
		x = Radix * a[j+m] + a[j+m-1];
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
					a1[j+i+1] = a1[j+i+1] - (Radix-1-a1[i+j])/Radix;
					a1[j+i]   = a1[j+i] + ((Radix-1-a1[i+j])/Radix)*Radix;
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
		a[j-1] += (a[j] % times) * Radix;
		a[j]   /= times;
	}
	a[0] /= times;

	return counter4Remainder;
}

/*
 * Computes x^e mod c => res[]. This makes use of the fact that the radix is
 * 2^RadixBits (e.g. 2^14); otherwise this function can't work properly.
 * res[] has been initialised in Exp( ).
 * NOTE: The value of 'length' is important. I suggests it should be the length of N
 * or plus one (N is P * Q)
 */
void    Exp( int x[], int e[], int res[], int c[], int length)
{
    int y[MaxLength], mask, i, j, temp[MaxLength];

    InitArray( res, 0, 0, FixedLength-1);    res[0] = 1;
    ArrayA2B( res, y, FixedLength);
//  for (i = 2 * length - 1; i >= 0 && e[i] == 0; i--)
    for (i = length - 1; i >= 0 && e[i] == 0; i--)
        ;
    if (i == -1)
        return; // Because e[] = 0, so res[] should be 1
    for ( ; i >= 0; i--)
    {
        mask  = 1 << (RadixBits - 1);
        for (j = RadixBits - 1; j >= 0; j--)
        {
            Multiply( res, res, y, length);

			InitArray( res, 0, 0, FixedLength-1);    // Computing res % c
			ArrayA2B( c, temp, FixedLength);	// Important
			Divide( y, temp, res, 2*length, length);
			ArrayA2B( y, res, FixedLength);
//printf( "1. res[]:"); PrintIntArray( res, 2*length);
//printf( "1. res[] = %lld\n", IntArrayValue( res, Radix, 2*length));

            if (e[i] & mask)
            {
                Multiply( res, x, y, length);
//printf( "2. y[] = %lld\n", IntArrayValue( y, Radix, 2*length));

                InitArray( res, 0, 0, FixedLength-1);    // Computing res % c
			    ArrayA2B( c, temp, FixedLength);
                Divide( y, temp, res, 2*length, length);
				ArrayA2B( y, res, FixedLength);
//printf( "2. res[]:"); PrintIntArray( res, 2*length);
//printf( "2. res[] = %lld\n", IntArrayValue( res, Radix, 2*length));
            }
            mask >>= 1;
        }
    }
}

/*
 * Jaqcobi func. a[] & b[] must be positive, because of DivideBy2( ); and both
 * a[] & b[] might be changed in this func. And, I think, in Jaqcobi func,
 * a[] & b[] always >= 0.
 * The method of minimising the local variables in this recursive func to
 * avoid overuse the system resource by assigning they into to other func,
 * ExpMinusOne1( ), ExpMinusOne2( ) & JaqcobiMod( ) is good; but other recursive
 * funcs might not be able to apply this way, because Jaqcobi func has its own
 * characteristics.
 */
int	Jaqcobi( int a[], int b[], int length)
{
	int	i;
//int j, k;

	if (IntArrayCmp( a, Zero, length) == 0)
		return 0;

	if (IntArrayCmp( a, One, length) == 0)
		return 1;

	if (OddOrEven( a, length) == 0)
	{
		DivideBy2( a, 2, length);
//printf( "Jaqcobi.1 a=%lld  b=%lld  ", IntArrayValue(a,Radix,length), IntArrayValue(b,Radix,length));
//printf( "EO1: %d\n",  ExpMinusOne1( b, length));
		/* Jaqcobi( ) must follow ExpMinusOne1( ), because b is global and
		 * might be change in recursion; so if the order is reverse,
		 * ExpMinusOne1( )'s b will be changed unexpectedly! */
		return  ExpMinusOne1( b, length) * Jaqcobi( a, b, length);
//j = (k = ExpMinusOne1( b, length)) * Jaqcobi( a, b, length);
//printf( "  j = %d  k = %d\n", j, k);
//return j;
	}
	else
	{
		i = ExpMinusOne2( a, b, length);
		JaqcobiMod( b, a, length);
//printf( "Jaqcobi.2 b=%lld  a=%lld  ", IntArrayValue(b,Radix,length), IntArrayValue(a,Radix,length));
//printf( "EO2 i: %d\n",  i);
		return Jaqcobi( b, a, length) * i;
//j = Jaqcobi( b, a, length) * i;
//printf( "  j = %d  i = %d\n", j, i);
//return j;
	}
}

/*
 * Used in Jaqcobi( ) to minimise the usage of local variables in recursion.
 * a[] mod b[] => a[], b[] isn't changed in this function, and must be
 * positive (Refer to Divide( )'s description)
 */
void	JaqcobiMod( int a[], int b[], int length)
{
	int	temp[MaxLength], temp1[MaxLength];

	ArrayA2B( b, temp, FixedLength);
	InitArray( temp1, 0, 0, FixedLength-1);
	Divide( a, temp, temp1, length, length);
}

/*
 * Only used in Jaqcobi( ) as exp( -1, (b*b-1) / 8), to minimise the usage of local
 * variables in recursion and make Jaqcobi( ) simpler. b[] isn't changed in this
 * function
 */
int	ExpMinusOne1( int b[], int length)
{
	int	one[MaxLength], temp[MaxLength], temp1[MaxLength], isNegative;

	Multiply( b, b, temp, length);

	InitArray( one, 0, 0, FixedLength-1); one[0] = 1;
	InitArray( temp1, 0, 0, FixedLength-1);
	Subtract( temp, one, temp1, 2*length);

	isNegative = 0;
	if (JudgeSign( temp1, 2*length) < 0)
	{
		NegateIntArray( temp1, 2*length);
		isNegative = 1;
	}
	DivideBy2( temp1, 8, 2*length);
	if (isNegative == 1)
		NegateIntArray( temp1, 2*length);	// Not necessary

	return OddOrEven( temp1, 2*length) ? -1: 1;
}

/*
 * Only used in Jaqcobi( ) as exp( -1, (a-1)*(b-1) / 4), to minimise the usage of
 * local variables in recursion and make Jaqcobi( ) simpler. a[] & b[] aren't
 * changed in this function
 */
int	ExpMinusOne2( int a[], int b[], int length)
{
	int	temp[MaxLength], temp1[MaxLength], temp2[MaxLength], isNegative;

	InitArray( temp, 0, 0, FixedLength-1); temp[0] = 1;
	InitArray( temp1, 0, 0, FixedLength-1);
	InitArray( temp2, 0, 0, FixedLength-1);
	Subtract( a, temp, temp1, length);
	Subtract( b, temp, temp2, length);
	Multiply( temp1, temp2, temp, length);

	isNegative = 0;
	if (JudgeSign( temp, 2*length) < 0)
	{
		NegateIntArray( temp, 2*length);
		isNegative = 1;
	}
	DivideBy2( temp, 4, 2*length);
	if (isNegative == 1)
	{
		NegateIntArray( temp, 2*length);
	}
	return OddOrEven( temp, 2*length) ? -1: 1;
}

/*
 * a[] / b => a[]
 * This divides a[] by 2^bits (b) and store the result to a[], by using bitwise-shift,
 * e.g. a/2, a/4, a/8 => a>>1, a>>2, a>>3. This makes use of the fact the radix in my
 * program is 2^RadixBits, so b must be 2^bits; and a[] must be positive, or
 * result is pointless
 */
void	DivideBy2( int a[], int b, int length)
{
	int	bits, bitsOfB, i, mask;

	for (bitsOfB = 0; b >>= 1; bitsOfB++)
		;

	for ( ; bitsOfB > 0; bitsOfB -= RadixBits) {
		bits = (bitsOfB > RadixBits) ? RadixBits : bitsOfB;

		for ( mask = 1, i = 1; i < bits; i++) {
			mask |= (1 << i);
		}
		for (i = 0; i < length; i++) {
			a[i] >>= bits;
			if (i == length - 1)	break;
			a[i] |= ((a[i+1] & mask) << (RadixBits - bits));
		}
	}
}

/*
 * (P-1)*(Q-1) => Fai; p is P, q is Q, fai is Fai
 */
void	GetFai( int p[], int q[], int fai[], int length)
{
	int	temp[MaxLength], temp1[MaxLength];

	Subtract( p, One, temp, length);
	Subtract( q, One, temp1, length);

	Multiply( temp, temp1, fai, length);
}

/*
 * Use Euclid's algorithm for gcd( ) to compute E. E * D = 1 (mod Fai). Here
 * Fai is definitely larger than D, so 'length' should be determined by Fai
 */
void	GetE( int Fai[], int D[], int E[], int length)
{
	int	r0[MaxLength], r1[MaxLength], r2[MaxLength], q1[MaxLength];
	int	a0[MaxLength], a1[MaxLength], a2[MaxLength];
	int	b0[MaxLength], b1[MaxLength], b2[MaxLength];
	int	i, t[MaxLength];

	ArrayA2B( Fai, r0, FixedLength); ArrayA2B( D, r1, FixedLength);
	ArrayA2B( Zero, a0, FixedLength); ArrayA2B( One, a1, FixedLength);
	ArrayA2B( One, b0, FixedLength); ArrayA2B( Zero, b1, FixedLength);

	while (1)
	{
//printf( "\nGetE( ): New round ...\n");
//printf( "r0[]:"); PrintIntArray( r0, length);
//printf( "r0[] = %lld\n", IntArrayValue( r0, Radix, length));
//printf( "r1[]:"); PrintIntArray( r1, length);
//printf( "r1[] = %lld\n", IntArrayValue( r1, Radix, length));
		InitArray( q1, 0, 0, FixedLength-1);
		InitArray( t, 0, 0, FixedLength-1);
		ArrayA2B( r1, t, FixedLength);
		Divide( r0, t, q1, length, length);
		ArrayA2B( r0, r2, FixedLength);
//printf( "q1[]:"); PrintIntArray( q1, length);
//printf( "q1[] = %lld\n", IntArrayValue( q1, Radix, length));
//printf( "r2[]:"); PrintIntArray( r2, length);
//printf( "r2[] = %lld\n", IntArrayValue( r2, Radix, length));

		Multiply( q1, a1, t, length);
		InitArray( a2, 0, 0, FixedLength-1);
		Subtract( a0, t, a2, 2*length);
//printf( "a2[]:"); PrintIntArray( a2, length);
//printf( "a2[] = %lld\n", IntArrayValue( a2, Radix, length));

		Multiply( q1, b1, t, length);
		InitArray( b2, 0, 0, FixedLength-1);
		Subtract( b0, t, b2, 2*length);
//printf( "b2[]:"); PrintIntArray( b2, length);
//printf( "b2[] = %lld\n", IntArrayValue( b2, Radix, length));

		if (IntArrayCmp( r2, Zero, length) == 0)
		{
//printf( "* a1[]:"); PrintIntArray( a1, length);
//printf( "* a1[] = %lld\n", IntArrayValue( a1, Radix, length));
			if (IntArrayCmp( a1, Zero, 2*length) < 0)
			{
				InitArray( E, 0, 0, FixedLength-1);
				Add( Fai, a1, E, 2*length);
			}
			else
			{
				ArrayA2B( a1, E, FixedLength);
			}
//printf( "* E[]:"); PrintIntArray( E, length);
//printf( "* E[] = %lld\n", IntArrayValue( E, Radix, length));

			break;
		}

		ArrayA2B( r1, r0, FixedLength);
		ArrayA2B( r2, r1, FixedLength);

		ArrayA2B( a1, a0, FixedLength);
		ArrayA2B( a2, a1, FixedLength);
		ArrayA2B( b1, b0, FixedLength);
		ArrayA2B( b2, b1, FixedLength);
	}
}

void	StoreKey( int Key[], int length, char * KeyFile)
{
	FILE	* fp;
	int	i;

	if ((fp = fopen( KeyFile, "w")) == NULL)
	{
		printf( "ERROR: [%s] cannot be opened!\n", KeyFile);
		exit( 1);
	}

	fprintf( fp, "%d", Key[0]);
	for (i = 1; i < length; i++)
	{
		fprintf( fp, ":%d", Key[i]);
	}

	fclose( fp);
}

void	RetrieveKey( int Key[], int length, char * KeyFile)
{
	FILE	* fp;
	char	str[50], *pch, ch;
	int	end = 0, i;

	if ((fp = fopen( KeyFile, "Radix")) == NULL)
	{
		printf( "ERROR: [%s] cannot be opened!\n", KeyFile);
		exit( 1);
	}

	for (i = 0; end == 0; i++)
	{
		pch = str;
		while (1)
		{
			if ((ch = fgetc( fp)) == ':')
			{
				break;
			}
			else if (ch == EOF)
			{
				end = 1;
				break;
			}
			*pch++ = ch;
		}

		if (i >= length)
		{
			printf( "ERROR: The retrieved key is too long\n");
			exit( 1);
		}

		*pch = '\0';

//printf( "RetrieveKey( ): str = [%s]\n", str);
		Key[i] = atoi( str);
	}

	InitArray( Key, 0, i, FixedLength-1);

	fclose( fp);
}

/*
 * Put single char into one element of int array to encrypt, so this
 * can deal with like Chinese language, etc, unlike BlockEncrypt2( )
 */
void	BlockEncrypt( FILE *FClear, FILE *FCipher)
{
	char	ch, buf[2*MaxLength];
	int	i, Clear[MaxLength], Cipher[MaxLength];

	while (1)
	{
		/*
		 * Encryption block shuld be one shorter than NLength, to ensure Clear
		 * is smaller than N (the modulus in Clear^E Mod N)
		 */
		for (i = 0; i < NLength-1; i++)
		{
			if ((ch = fgetc( FClear)) == EOF)
			{
				break;
			}
			Clear[i] = (unsigned char)ch;
		}

		if (i != 0)	// Some chars have been read
		{
			InitArray( Clear, 0, i, FixedLength-1);
			if (i != NLength-1)
			{
//				Mistake, because it may cause Clear >= N!
//				InitArray( Clear, Pad, i, NLength-1);
				InitArray( Clear, Pad, i, NLength-2);
			}
			Exp( Clear, E, Cipher, N, NLength);
			for (i = 0; i < NLength; i++)
			{
				buf[2*i]   = Cipher[i] & Mask;
				buf[2*i+1] = (Cipher[i] >> BitMove) & Mask;
			}
			fwrite( buf, 2 * NLength * sizeof( char), 1, FCipher);
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
	int	len, i, Clear[MaxLength], Cipher[MaxLength];

	while (1)
	{
		for (len = 2*(NLength-1), i = 0; i < len; i++)
		{
			if ((ch = fgetc( FClear)) == EOF)
			{
				break;
			}
			else if (ch < 0)	// -128 <= ch <= 127
			{
				printf( "Encrypting failed!\n"
					"The file to be encrypted must be TXT file\n"
					"Each char's ASCII value inside should be 1~127\n");
				exit( 1);
			}
			// So, RadixBits need be even
//			Clear[i/2] = (i & 1) ? ((Clear[i/2] << RadixBits/2) + ch) : ch;
			Clear[i/2] = (i & 1) ? ((Clear[i/2] << RadixBits/2) + \
				(unsigned char)ch) : (unsigned char)ch;
		}

		if (i != 0)	// No ch has been read
		{
			InitArray( Clear, 0, (i+1)/2, FixedLength-1);

			Exp( Clear, E, Cipher, N, NLength);
printf( "BlockEncrypt2( ): Cipher is:"); PrintIntArray( Cipher, NLength+1);
			fwrite( Cipher, NLength * sizeof( int), 1, FCipher);
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
	int	Clear[MaxLength], Cipher[MaxLength], ClearLen, BitMove, i;
	char	buf[2*MaxLength];

	BitMove = (RadixBits+1) / 2; // In case RadixBits is odd, so plus one

	while (1)
	{
		fread( buf, 2 * NLength * sizeof( char), 1, FCipher);
		if (feof( FCipher))
		{
			break;
		}
		if (ferror( FCipher))
		{
			printf( "ERROR: Reading CipherTxt file failed!\n");
			exit( 1);
		}
		for (i = 0; i < NLength; i++)
		{
			Cipher[i] = buf[2*i+1];
//			Cipher[i] = (Cipher[i] << BitMove) + buf[2*i];
			Cipher[i] = (Cipher[i] << BitMove) | buf[2*i];
		}

		InitArray( Cipher, 0, NLength, FixedLength-1);

		Exp( Cipher, D, Clear, N, NLength);
//		ClearLen = ArrayEffectiveLength( Clear, 0, FixedLength-1); // bug
		ClearLen = ArrayEffectiveLength( Clear, Pad, NLength-1);
		for (i = 0; i < ClearLen; i++)
		{
			if (fputc( Clear[i], FClear) == EOF)
			{
				printf( "ERROR: Writing ClearTxt file failed!\n");
				exit( 1);
			}
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
 * Computing the length of the effective value in a[]. prefix's value
 * means, in the specified 'length' of the array, these prefix should be excluded.
 * If a = {12, 30, 7, 0, 0, 0, 6}, for prefix = 0, length = 5, return 3
 */
int	ArrayEffectiveLength( int a[], int prefix, int length)
{
	int	i;

	for (i = length-1; i >= 0 && a[i] == prefix; i--)
		;
	return i+1;
}

/*-------------------- Debugging helper functions --------------------*/
/*
 * Calculating base^exponent. Basically it's for debugging
 */
long long	Power( long base, long exponent)
{
	long	l;
	long long result;

	for (l = 0, result = 1; l < exponent; l++)
	{
		result *= base;
	}

	return result;
}

/*
 * Calculating the int array's int value. It's for debugging
 */
long long	IntArrayValue( int * array, int radix, int length)
{
	int	i;
	long long	result;

	for (i = 0, result = 0; i <length; i++)
	{
		result += array[i] * Power( radix, i);
	}

	return result;
}
