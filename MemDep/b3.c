#define MAX 100
int A[MAX] ;
int B[MAX] ;
int C[MAX] ;

void loop()
{
    int i ;
    for(i=0 ; i<MAX ; i++) {
        A[i] = B[i] = i ;
    }

    for(i=0 ; i<MAX ; i++) {
        C[i] = A[i] + B[i] ;
    }
}
