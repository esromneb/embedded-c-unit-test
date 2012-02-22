/***
 * CU - C unit testing framework
 * ---------------------------------
 * Copyright (c)2007,2008,2009 Daniel Fiser <danfis@danfis.cz>
 *
 *
 *  This file is part of CU.
 *
 *  CU is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  CU is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "c_unit_test.h"

const char *cu_current_test;
int cu_success_tests = 0;
int cu_fail_tests = 0;
int cu_success_checks = 0;
int cu_fail_checks = 0;


/* flag to indicate if the test failed, reset with every test */
int test_failed;

/* codes of messages */
#define CHECK_FAILED 0
#define CHECK_SUCCEED 1
#define TEST_FAILED 2
#define TEST_SUCCEED 3


/* predefined messages */
#define MSG_CHECK_FAILED cu_handle_message( CHECK_FAILED )
#define MSG_CHECK_SUCCEED cu_handle_message( CHECK_SUCCEED )
#define MSG_TEST_FAILED cu_handle_message( TEST_FAILED )
#define MSG_TEST_SUCCEED cu_handle_message( TEST_SUCCEED )


/* forward declare */
static void cu_handle_message(int m);
void cu_print_results(void);



void cu_run_test(const char *name, void (*test_pointer)(void), unsigned reset)
{
  static int tests_run = 0;
  
  /* PDQ Board does not reset memory after running test, so we reset tests_run with a flag */
  if( reset ){
    tests_run = 0;
    return;
  }
  
  /* print a newline before the first test for correct formatting on an Embedded board */ 
  if( tests_run == 0 )
    fprintf(stdout, "\n");

  /* reset test_failed flag */
  test_failed = 0;

  /* set up name of test for later messaging */
  cu_current_test = name;

  /* send message what test is currently running */
  printf("%d:    --> Running %s...\n",
                 ++tests_run, cu_current_test);

  /* run test */
  (*(test_pointer))();

  if (test_failed){
      MSG_TEST_FAILED;
  }else{
      MSG_TEST_SUCCEED;
  }

}

static void cu_handle_message(int m)
{
  if (m == CHECK_FAILED){
      cu_fail_checks++;
  }else if (m == CHECK_SUCCEED){
      cu_success_checks++;
  }else if (m == TEST_FAILED){
      cu_fail_tests++;
  }else if (m == TEST_SUCCEED){
      cu_success_tests++;
  }
}

void cu_success_assertation(void)
{
    MSG_CHECK_SUCCEED;
}

void cu_fail_assertation(const char *file, int line, const char *msg)
{
    printf("in %s:%d (%s) :: %s\n",
            file, line, cu_current_test, msg);

    MSG_CHECK_FAILED;

    /* enable test_failed flag */
    test_failed = 1;
}

void cu_reset_counters(void)
{
    cu_success_tests = 0;
    cu_fail_tests = 0;
    cu_success_checks = 0;
    cu_fail_checks = 0;
    
    /* this resets the static int inside cu_run_test() */  
    cu_run_test( 0, 0, 1 );
}

void cu_print_results(void)
{
    fprintf(stdout, "\n");
    fprintf(stdout, "==================================================\n");
    fprintf(stdout, "|               |  failed  |  succeed  |  total  |\n");
    fprintf(stdout, "|------------------------------------------------|\n");
    fprintf(stdout, "| assertions:   |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_checks, cu_success_checks,
                cu_success_checks+cu_fail_checks);
    fprintf(stdout, "| tests:        |  %6d  |  %7d  |  %5d  |\n",
                cu_fail_tests, cu_success_tests,
                cu_success_tests+cu_fail_tests);
    fprintf(stdout, "==================================================\n");

    if( cu_fail_tests )
    {
      fprintf(stdout, "\n\n");
      fprintf(stdout, "==================================================\n");
      fprintf(stdout, "|                                                |\n");
      fprintf(stdout, "|          !!!!!Some Tests Failed!!!!!           |\n");
      fprintf(stdout, "|                                                |\n");
      fprintf(stdout, "==================================================\n");
    }
    
    /* reset for next run */
    cu_reset_counters();
}


