#define SIZE 2048
#define NUM_ITER 100

int dot_product(int* A, int* B, int start_indx, int end_indx)
{
  int i ;
  int sum = 0 ;
  //for (i = start_indx ; i < end_indx ; i++)
  //{
  //  A[i] = B[i] = i ;
  //}

  for (i = start_indx ; i < end_indx ; i++)
  {
    sum += A[i] * B[i] ;
  }

  return sum ;
}

int A[SIZE*SIZE] ;
int B[SIZE*SIZE] ;

int main()
{
  int i ;
  int result ;
  for(i=0 ; i<NUM_ITER ; i++) {
    result = dot_product(A,B,0,SIZE*SIZE) ;
  }
  return 0 ;
}

