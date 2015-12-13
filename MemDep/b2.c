#define MAX 100

void loop(int* A, int* B, int* C, int start, int end)
{
    int i ;
    for(i=start ; i<end ; i++) {
        A[i] = B[i] + C[i] ;
    }
}

int A[MAX] ;
int B[MAX] ;
int C[MAX] ;

int main()
{
    loop(A,B,C,0,MAX) ;
    return 0 ;
}
