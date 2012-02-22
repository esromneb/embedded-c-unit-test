#include "c_unit_test.h"
#include "global.h"


// calculate 3x+6x^2
int algebra1( int x )
{
  int result;

  result = 3*x + 6*x*x;
  return result;
}

// calculate 3x(1+2x)
int algebra2( int x )
{
  int result;

  result = 3*x*( 1 + 2*x );
  return result;
}


int main()
{
  printf("\n\nThis program demonstrates how to use unit tests to remove bugs from\n");
  printf("your application.  The main.c file contains two functions which calculate\n");
  printf("the same result using different algebraic functions:\n");
  printf("  3x+6x^2\n");
  printf("  3x(1+2x)\n\n");

  printf("The tests.c file contains tests which call these functions and check that the\n");
  printf("results are the same over a range of values.\n\n");

	run_test();
	return 0;
}
