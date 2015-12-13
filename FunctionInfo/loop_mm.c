#define N 1024

void mm(int (*__restrict__ A)[N], int (*__restrict__ B)[N], int (*__restrict__ C)[N])
{
	int i,j,k ;
	for(i=0 ; i<N ; i++) {
		for(i=0 ; i<N ; i++) {
			for(i=0 ; i<N ; i++) {
				C[i][j] += A[i][k] * B[k][j] ;
			}
		}
	}
}

int A[N][N] ;
int B[N][N] ;
int C[N][N] ;

int main()
{
	mm(A,B,C) ;
	return 0 ;
}
