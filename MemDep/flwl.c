#define N 512
#define NUM_ITER 10

inline int mod(int n)
{
  if(n<0)
    return -n ;
  else
    return n ;
}

void floyd_warshall(int path[N][N], int start_indx, int end_indx)
{
  int i, j, k;

  for(i=start_indx ; i<end_indx ; i++)
    for(j=0 ; j<N ; j++)
      path[i][j] = mod(i-j) ;

  for (k = 0; k < N; k++)
  {
    for(i = start_indx; i < end_indx; i++)
      for (j = 0; j < N; j++)
        path[i][j] = path[i][j] < path[i][k] + path[k][j] ?
          path[i][j] : path[i][k] + path[k][j] ;
  }
}

int path[N][N] ;

int main()
{
  int i ;
  for(i=0 ; i<NUM_ITER ; i++) {
    floyd_warshall(path,0,N) ;
  }
  return 0 ;
}

