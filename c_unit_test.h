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

#ifndef _CU_H_
#define _CU_H_

/***** PUBLIC API *****/
/**
 * Define test
 */
#define CU_TEST(name) \
    void name(void)

#define CU_RUN_TEST(name) \
    cu_run_test( #name, name, 0 ) \

#define CU_TESTS_FINISHED( ) \
    cu_print_results();


/**
 * Assertations
 * Assertations with suffix 'M' (e.g. assertTrueM) is variation of macro
 * where is possible to specify error message.
 */
#define assertTrueM(a, message) \
    if (a){ \
        cu_success_assertation(); \
    }else{ \
        cu_fail_assertation(__FILE__, __LINE__, message); \
    }
#define assertTrue(a) \
    assertTrueM((a), #a " is not true")

#define assertFalseM(a, message) \
    assertTrueM(!(a), message)
#define assertFalse(a) \
    assertFalseM((a), #a " is not false")

#define assertEqualsM(a,b,message) \
    assertTrueM((a) == (b), message)
#define assertEquals(a,b) \
    assertEqualsM((a), (b), #a " not equals " #b)

#define assertNotEqualsM(a,b,message) \
    assertTrueM((a) != (b), message)
#define assertNotEquals(a,b) \
    assertNotEqualsM((a), (b), #a " equals " #b)


/***** PUBLIC API END *****/



extern const char *cu_current_test;


extern int cu_success_tests;
extern int cu_fail_tests;
extern int cu_success_checks;
extern int cu_fail_checks;


void cu_print_results(void);
void cu_success_assertation(void);
void cu_fail_assertation(const char *file, int line, const char *msg);
void cu_run_test(const char *name, void (*test_pointer)(void), unsigned reset);



// This block of statements creates aliases with long unique names pointing
// to standard C functions.  The purpose of these staements is to instruct the
// linker that this library will be linking against / calling these functions.
// Without these aliases, the user's application must call every function covered here.
//
// For code clarity, don't use these aliases in your application program!

int   _c_unit_test_linker_reference_fputc     (int, FILE *) __attribute__ ((weak, alias("fputc")));
int   _c_unit_test_linker_reference_fputs     (const char *, FILE *) __attribute__ ((weak, alias("fputs")));
int   _c_unit_test_linker_reference_fprintf   (FILE *, const char *, ...) __attribute__ ((weak, alias("fprintf")));
int   _c_unit_test_linker_reference_printf    (const char *, ...) __attribute__ ((weak, alias("printf")));



#endif
