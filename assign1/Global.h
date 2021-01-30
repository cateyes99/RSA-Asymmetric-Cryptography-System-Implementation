#ifndef Global_h
#define Global_h

int	Zero[MaxElem], One[MaxElem], Two[MaxElem], MinusOne[MaxElem];

int	r;	/* Radix  of each int array. For sizeof( int) = 4, max is 2^15,
		 * to avoid overflow in array multiplication */
int	RadixBits;	/* We let r be the power of 2: r = 2^RadixBits; otherwise
				 * some funcs can't work properly, e.g. exp1( ) */
int	P[MaxElem], Q[MaxElem], N[MaxElem], Fai[MaxElem];
int	D[MaxElem], E[MaxElem];	/* Be careful, the lengths of D & E */
int	n;	/* Num of elements of P & Q used; when initialising, use n+1
		 * to avoid overflow in Add( ) & Subtract( ). So, N's length is 2n.
		 * In this program, the max effective lenght of array should be at
		 * least 4n. It's IMPORTANT to initialise each array with SUFFICIENT
		 * length, inclduing temporary arrays */
int	FixLen;	/* The fix length for initialising and copying arrays */
int	NLen;	/* The real length of N, it's <= 2*n. Further, (N, E) pair and
			 * (N, D) pair can encrypt and decrypt an int block with length
			 * of NLen-1 */

int	Mask, BitMove, Pad;	/* Used in encryt & decryt funcs */

#endif