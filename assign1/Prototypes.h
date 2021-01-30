#ifndef Prototypes_h
#define Prototypes_h

#define	MaxElem	200	// Max num of elements in each array
#define	KeyNFile	"KeyN.txt"
#define	KeyDFile	"KeyD.txt"
#define	KeyEFile	"KeyE.txt"

void	Add( int a[], int b[], int c[], int nn);
void	ArrayA2B( int a[], int b[], int nn);
int	ArrayValueLen( int aa[], int prefix, int nn);
void	BlockDecrypt( FILE *FCipher, FILE *FClear);
void	BlockEncrypt( FILE *FClear, FILE *FCipher);
void	DivideBy2( int a[], int bb, int nn);
int	divide1( int a[], int b[], int c[], int n, int m);
int	ExpMinusOne1( int b[], int nn);
int	ExpMinusOne2( int a[], int b[], int nn);
void    exp1( int x[], int e[], int res[], int c[], int nn);
void	GetE( int Fai[], int D[], int E[], int nn);
void	GetFai( int pp[], int qq[], int ff[], int nn);
void	GetPrime( int pp[], int nn);
void	InitArray( int * array, int value, int n1, int n2);
void	InitMain( void);
int	IntArrayCmp( int a[], int b[], int nn);
long long	IntArrayValue( int * array, int r, int nn);
int	J1( int a[], int b[], int nn);
void	J1_Mod( int a[], int b[], int nn);
int	JudgeSign(int a[], int nn);
void	multiply1( int a[], int b[], int c[], int n);
void	NegateIntArray( int a[], int nn);
int	OddOrEven( int * array, int nn);
long long	Power( long x, long y);
void	PrintIntArray( int * array, int nn);
void	PureAdd( int a[], int b[], int c[], int nn);
void	PureSubtract( int a[], int b[], int c[], int nn);
void	RetrieveKey( int Key[], int nn, char * KeyFile);
void	StoreKey( int Key[], int nn, char * KeyFile);
void	Subtract( int a[], int b[], int c[], int nn);

#endif