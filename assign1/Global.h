#ifndef Global_h
#define Global_h

int	Zero[MaxLength], One[MaxLength], Two[MaxLength], MinusOne[MaxLength];

int	Radix;	/* Radix of each int array. For sizeof( int) = 4, max is 2^15,
			 * to avoid overflow in array multiplication */
int	RadixBits;	/* We let Radix be the power of 2: Radix = 2^RadixBits;
				 * otherwise some funcs can't work properly, e.g. exp1( ) */
int	P[MaxLength], Q[MaxLength], N[MaxLength], Fai[MaxLength];
int	D[MaxLength], E[MaxLength];	/* Be careful, the lengths of D & E */
int	n;	/* The effective num of elements of P & Q used; when initialising,
		 * use n+1 to avoid overflow in Add( ) & Subtract( ). So, N's length is 2n.
		 * In this program, the max effective length of array should be at
		 * least 4n. It's IMPORTANT to initialise each array with SUFFICIENT
		 * length, including temporary arrays */
int	FixedLength;	/* The fixed length for initialising and copying arrays */
int	NLength;	/* The real length of N, it's <= 2*n. Further, (N, E) pair and
				 * (N, D) pair can encrypt and decrypt an int block with length
				 * of NLength-1 */

int	Mask, BitMove, Pad;	/* Used in encrypt & decrypt funcs */

#endif