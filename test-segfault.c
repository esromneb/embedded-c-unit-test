#include <stdio.h>
#include "cu.h"

/**
 * Definition of test function
 */
TEST(testFunction)
{
    int *a = 0;
    *a = 10;
    assertTrue(0);
}

TEST_SUITE(testSuiteSegfault)
{
    TEST_ADD(testFunction),
    TEST_SUITE_CLOSURE
};


TEST_SUITES{
    TEST_SUITE_ADD(testSuiteSegfault),
    TEST_SUITES_CLOSURE
};

int main(int argc, char *argv[])
{
    CU_SET_OUT_PREFIX("regressions/"); /* define prefix for files written
                                           by testsuites */
    CU_RUN(argc, argv); /* Run testsuites defined by TEST_SUITES macro
                           in its own process and stdout is redirected to
                           regressions/tmp.testSuiteName.out
                           and stderr to regressions/tmp.testSuiteName.err
                           according to CU_SET_OUT_PREFIX called before */
    return 0;
}