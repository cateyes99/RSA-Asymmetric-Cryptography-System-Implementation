#ifndef Prototypes_h
#define Prototypes_h

#define	MaxLength	200	// Max num of elements in each array
#define	KeyNFile	"KeyN.txt"
#define	KeyDFile	"KeyD.txt"
#define	KeyEFile	"KeyE.txt"

void	Add( int a[], int b[], int c[], int length);
void	ArrayA2B( int a[], int b[], int length);
int		ArrayEffectiveLength( int a[], int prefix, int length);
void	BlockDecrypt( FILE *FCipher, FILE *FClear);
void	BlockEncrypt( FILE *FClear, FILE *FCipher);
void	DivideBy2( int a[], int b, int length);
int		Divide( int a[], int b[], int c[], int n, int m);
int		ExpMinusOne1( int b[], int length);
int		ExpMinusOne2( int a[], int b[], int length);
void    Exp( int x[], int e[], int res[], int c[], int length);
void	GetE( int Fai[], int D[], int E[], int length);
void	GetFai( int p[], int q[], int fai[], int length);
void	GetPrime( int p[], int length);
void	InitArray( int * array, int value, int n1, int n2);
void	InitMain( void);
int		IntArrayCmp( int a[], int b[], int length);
long long	IntArrayValue( int * array, int radix, int length);
int		J1( int a[], int b[], int length);
void	J1_Mod( int a[], int b[], int length);
int		JudgeSign(int a[], int length);
void	Multiply( int a[], int b[], int c[], int n);
void	NegateIntArray( int a[], int length);
int		OddOrEven( int * array, int length);
long long	Power( long base, long exponent);
void	PrintIntArray( int * array, int length);
void	PureAdd( int a[], int b[], int c[], int length);
void	PureSubtract( int a[], int b[], int c[], int length);
void	RetrieveKey( int Key[], int length, char * KeyFile);
void	StoreKey( int Key[], int length, char * KeyFile);
void	Subtract( int a[], int b[], int c[], int length);

#endif