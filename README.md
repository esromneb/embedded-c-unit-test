This project was copied and modified from http://cu.danfis.cz/  
  
# Instructions
Each test must be declared with the CU_TEST(name) macro.  The name parameter  
should describe what the test covers.  
  
There are four macros which can be used to test conditions in your application:  
  assertTrue(a)  
  assertFalse(a)  
  assertEquals(a,b)  
  assertNotEquals(a,b)  
  
The assertTrue(a) and assertFalse(a) macros test the object inside the brackets  
using a simple if() statement.  This means that non-zero values are considered true  
and zero values are considered false.  
  
The assertEquals(a,b) and assertNotEquals(a,b) macros use the == and != operators  
to test for equality.  
  
The last function in this file, "void run_test( void )", is used to run each  
test case.  This file must be manually updated so that every test defined with CU_TEST()  
has a a matching CU_RUN_TEST().  If you add a new unit test, do not forget to update  
this file.  
  
You can disable tests for the demo by editing the global.h file.  
  
For more information about how assert statements work, see the c_unit_test.h file  