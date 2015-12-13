int compute_algebraic3 (int a, int b)
{
  int result = a + (b-b) ;
  result += (b-b) + a ;
  result += a * (a/a) ;
  result += (b/b) * a ;
  result += a - (b-b) ;
  result += a/(b/b) ;

  return result;
}
