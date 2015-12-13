#define N 1024
#define NUM_ITER 1

void mm(int (*__restrict__ A)[N], int (*__restrict__ B)[N], int (*__restrict__ C)[N], int start_indx, int end_indx)
{
  int i,j,k ;
  for(i=start_indx ; i<end_indx ; i++) {
    for(j=0 ; j<N ; j++) {
      for(k=0 ; k<N ; k++) {
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
  int i ;
  for(i=0 ; i<NUM_ITER ; i++)
    mm(A,B,C,0,N) ;

  return 0 ;
}
