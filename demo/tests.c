// This file has the ability to disable tests
// It also contains forward declarations for the functions in main.c
#include "global.h"



// This file contains example unit tests for this application
//
// Each test must be declared with the CU_TEST(name) macro.  The name parameter
// should describe what the test covers.
//
// There are four macros which can be used to test conditions in your application:
//   assertTrue(a)
//   assertFalse(a)
//   assertEquals(a,b)
//   assertNotEquals(a,b)
//
// The assertTrue(a) and assertFalse(a) macros test the object inside the brackets
// using a simple if() statement.  This means that non-zero values are considered true
// and zero values are considered false.
//
// The assertEquals(a,b) and assertNotEquals(a,b) macros use the == and != operators
// to test for equality.
//
// The last function in this file, "void run_test( void )", is used to run each
// test case.  This file must be manually updated so that every test defined with CU_TEST()
// has a a matching CU_RUN_TEST().  If you add a new unit test, do not forget to update
// this file.
//
// You can disable tests for your application by editing the global.h file.
//
// For more information about how assert statements work, see the c_unit_test.h file




// Everything below this line is conditinally enabled from within the globals.h file
#ifdef ENABLE_UNIT_TESTS


// Include files needed to run the tests
#include "c_unit_test.h"




CU_TEST(testTrueAndEquals)
{
  // This passes with any value except for 0
  assertTrue(1);

  // This passes when the first and second parameter are equal
  // when compared using the == operator
  assertEquals(1, 1);
}


CU_TEST(testFalseAndNotEquals)
{
  // This passes with a 0 value
  assertFalse(0);

  // This passes when the first and second parameter are not equal
  // when compared using the != operator
  assertNotEquals(1, 2);
}


// Test the returned values of the algebra1() and algebra2() functions.

CU_TEST(testAlgebra)
{
  int x;
  int left, right;
  for( x = 0; x < 10; x++ )
  {
    left = algebra1(x);
    right = algebra2(x);

    // check that 3x+6x^2 = 3x(1+2x)
    assertEquals(left, right);
  }
}


void run_test( void )
{
  // These lines must match the CU_TEST() functions above
  CU_RUN_TEST(testTrueAndEquals);
  CU_RUN_TEST(testFalseAndNotEquals);
  CU_RUN_TEST(testAlgebra);

  // This prints out a count of failed and succeed tests
  CU_TESTS_FINISHED();
}

#endif
