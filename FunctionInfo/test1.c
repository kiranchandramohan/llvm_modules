#include<stdio.h>

int gbl ;

int fib(int n)
{
	if(n==1)
		return 1 ;
	else {
		int nm1 = fib(n-1) ;
		int nm2 = fib(n-2) ;
		return nm1 + nm2 ;
	}
}

int mult(int a, int b)
{
	return a * b ;
}

int g_incr(int *b, int* c)
{
	int temp = 0 ;
	for (int i = 0; i < gbl ; i++) {
		temp = temp << *b ;
	}

	printf("Function : g_incr\n") ;

	temp += fib(*c) ;

	return mult(*c,temp) ;
}

int loop (int a, int b, int c)
{
	int i;
	int ret = 0 ;
	int g = 0 ;
	c = 20 ;
	for (i = a; i < b; i++) {
		c += mult(g,c) ;
		g = g_incr(&b,&c);
	}
	return ret + g;
}
