#define MAX 1000000
int add(int a, int b, int count)
{
  int i ;
  for(i=0 ; i<count ; i++)
  {
    a += b ;
  }
  return a ;
}

int x ;

int main()
{
  x = add(0,3,MAX) ;
  return 0 ;
}
