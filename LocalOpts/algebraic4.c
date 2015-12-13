int compute_algebraic4 (int a, int b)
{
  int result = a + b + a * (a/a) + (b/b) - b ;

  return result;
}
