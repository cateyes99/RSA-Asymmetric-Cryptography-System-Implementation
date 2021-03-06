#include	<stdio.h>
#include	<stdlib.h>

#include	"Prototypes.h"
#include	"Global.h"

int	main( int argc, char ** argv);

int	main( int argc, char ** argv)
{
	FILE	*FClear, *FCipher;
	int	ii;

	if (argc != 5)
	{
		printf( "USAGE: %s KeyN KeyD CipherTxt ClearTxt\n\n", argv[0]);
		exit( 1);
	}

/*
	printf( "Continue to decrypt? [y/n]\n");
	if ((ii = fgetc( stdin)) != 'y' && ii != 'Y')
		return 1;
 */

	InitMain( );

	printf( "\nRetrieving Key N from [%s] ...\n", argv[1]);
	RetrieveKey( N, 2*n, argv[1]);
	printf( "N (PxQ) is:"); PrintIntArray( N, 2*n+1);
	printf( "N's effective length (NLen) is: %d elements\n", \
		NLen = ArrayValueLen( N, 0, 2*n));
	if (NLen <= 1)
	{
		printf( "N's effective length is too short to encipher!\n"
			"Please use another seed to generate new P/Q\n");
		return -1;
	}

	printf( "\nRetrieving the Key from [%s] ...\n", argv[2]);
	RetrieveKey( D, 2*n, argv[2]);
	printf( "Prime D retrieved is:"); PrintIntArray( D, n+1);

	// Decrypting
	if ((FCipher = fopen( argv[3], "r")) == NULL)
	{
		printf( "ERROR: [%s] cannot be opened!\n", argv[3]);
		exit( 1);
	}
	if ((FClear = fopen( argv[4], "w")) == NULL)
	{
		printf( "ERROR: [%s] cannot be opened!\n", argv[4]);
		exit( 1);
	}

	BlockDecrypt( FCipher, FClear);

	fclose( FClear); fclose( FCipher);

	return 0;
}
