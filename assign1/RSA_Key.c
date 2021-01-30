/*
 * NOTE: Negative numbers are represented in array as a[ ] = {-23, -51, 0, -7}
 */
#include	<stdio.h>
#include	<stdlib.h>

#include	"Prototypes.h"
#include	"Global.h"

int	main( int argc, char ** argv);

int	main( int argc, char ** argv)
{
	int	ii;

	if (argc != 2)
	{
		printf( "USAGE: %s seed\n  seed - 0, 1, 2, 3 ... "
			"used to generate different P/Q\n\n", argv[0]);
		exit( 1);
	}

	InitMain( );

	srandom( atoi( argv[1]));

	/* Get a prime num for P */
	printf( "\nFinding P ...\n");
	for (ii = 0; ii < n; ii++)
		P[ii] = random( ) % r;
	if (OddOrEven( P, n) == 0)
		P[0]++;	// Max even num is r-2, so no carry caused by P[0]++
	printf( "Initial P[ ]:"); PrintIntArray( P, n+1);
//printf( "P[ ] = %lld\n...\n", IntArrayValue( P, r, n+1));

	GetPrime( P, n);
	printf( "Prime P found is:"); PrintIntArray( P, n+1);
//printf( "P[ ] = %lld\n", IntArrayValue( P, r, n+1));

GetQ:
	/* Get a prime num for Q */
	printf( "\nFinding Q ...\n");
	for (ii = 0; ii < n; ii++)
		Q[ii] = random( ) % r;
	if (OddOrEven( Q, n) == 0)
		Q[0]++;
	printf( "Initial Q[ ]:"); PrintIntArray( Q, n+1);
//printf( "Q[ ] = %lld\n...\n", IntArrayValue( Q, r, n+1));

	GetPrime( Q, n);
	if (IntArrayCmp( Q, P, n) == 0)
		goto GetQ;
	printf( "Prime Q found is:"); PrintIntArray( Q, n+1);
//printf( "Q[ ] = %lld\n", IntArrayValue( Q, r, n+1));

	/* Computing N */
	multiply1( P, Q, N, n);
	printf( "\nN (PxQ) is:"); PrintIntArray( N, 2*n+1);
//printf( "N[ ] = %lld\n", IntArrayValue( N, r, 2*n+1));
	printf( "N's effective length (NLen) is: %d elements\n", \
		NLen = ArrayValueLen( N, 0, 2*n));
	if (NLen <= 1)
	{
		printf( "N's effective length is too short to encipher!\n"
			"Please use another seed to generate new P/Q\n");
		return -1;
	}
	StoreKey( N, 2*n, KeyNFile);

	/* Computing Fai = (P-1)x(Q-1) */
	GetFai( P, Q, Fai, n);
	printf( "\nFai (P-1)x(Q-1) is:"); PrintIntArray( Fai, 2*n+1);
//printf( "Fai[ ] = %lld\n", IntArrayValue( Fai, r, 2*n+1));

	/* Get a prime num for D */
	printf( "\nFinding D ...\n");
	InitArray( D, 0, 0, FixLen-1);
	if (IntArrayCmp( P, Q, n) >= 0)
		Add( P, Two, D, n);
	else
		Add( Q, Two, D, n);

	GetPrime( D, n);
	printf( "Prime D found is:"); PrintIntArray( D, n+1);
//printf( "D[ ] = %lld\n", IntArrayValue( D, r, n+1));
	StoreKey( D, n, KeyDFile);

	printf( "\nFinding E ...\n");
	GetE( Fai, D, E, 2*n);
	printf( "E found is:"); PrintIntArray( E, 2*n+1);
//printf( "E[ ] = %lld\n", IntArrayValue( E, r, 2*n+1));
	StoreKey( E, 2*n, KeyEFile);

	return 0;
}
