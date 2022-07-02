/**
 * @file tinytest.h
 *
 * @brief TinyTest: A really really really tiny and simple no-hassle C unit-testing framework.
 *
 * Features:
 *   - No library dependencies. Not even itself. Just a header file.
 *   - Simple ANSI C. Should work with virtually every C or C++ compiler on
 *     virtually any platform.
 *   - Reports assertion failures, including expressions and line numbers.
 *   - Stops test on first failed assertion.
 *   - ANSI color output for maximum visibility.
 *   - Easy to embed in apps for runtime tests (e.g. environment tests).
 *
 * Example Usage:
 *
 *    #include "tinytest.h"
 *    #include "mylib.h"
 *
 *    void test_sheep()
 *    {
 *      ASSERT("Sheep are cool", are_sheep_cool());
 *      ASSERT_EQUALS(4, sheep.legs);
 *    }
 *
 *    void test_cheese()
 *    {
 *      ASSERT("Cheese is tangy", cheese.tanginess > 0);
 *      ASSERT_STRING_EQUALS("Wensleydale", cheese.name);
 *    }
 *
 *    int main()
 *    {
 *      RUN(test_sheep);
 *      RUN(test_cheese);
 *      return TEST_REPORT();
 *    }
 *
 * To run the tests, compile the tests as a binary and run it.
 *
 * Project home page: http://github.com/joewalnes/tinytest
 *
 * @date 2010
 * @author Joe Walnes <joe@walnes.com> http://joewalnes.com
 *
 * Adapted from: x86 / Gyps.
 */

#ifndef _TINYTEST_INCLUDED
#define _TINYTEST_INCLUDED ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>
#include <stdlib.h>
#include "../String_Tools.h"



/**
 * @brief Main test assert macro.
 */
#ifndef ASSERT
#define ASSERT(msg, expression) if (!tt_assert(__FILE__, __LINE__, (msg), (#expression), (expression) ? 1 : 0)) return
#else
#error "The macro \"ASSERT\" is already defined !"
#endif /* ASSERT */

/**
 * @brief Test assert macro for the equalness of two values.
 *
 * A comparison with floating point numbers is possible but not recommended, due the potential inaccuracy of this value
 * types.
 */
#ifndef ASSERT_EQUALS
#define ASSERT_EQUALS(expected, actual) ASSERT((#actual), (expected) == (actual))
#else
#error "The macro \"ASSERT_EQUALS\" is already defined !"
#endif /* ASSERT_EQUALS */

/**
 * @brief Test assert macro for the equalness of two C-Strings.
 */
#ifndef ASSERT_STRING_EQUALS
#define ASSERT_STRING_EQUALS(expected, actual)                                                      \
    if (strcmp((expected),(actual)) != 0)                                                           \
    {                                                                                               \
        PRINTF_FFLUSH("Compare \"%s\" (expected) and \"%s\" (actual).\n", (expected), (actual));    \
        ASSERT((#actual), strcmp((expected),(actual)) == 0);                                        \
    }
#else
#error "The macro \"ASSERT_STRING_EQUALS\" is already defined !"
#endif /* ASSERT_STRING_EQUALS */

/**
 * @brief Test assert macro for the equalness of two C-Strings (case-insensitive).
 */
#ifndef ASSERT_STRING_CASE_INSENSITIVE_EQUALS
#define ASSERT_STRING_CASE_INSENSITIVE_EQUALS(expected, actual)                                                     \
    if (Compare_Strings_Case_Insensitive((expected),(actual)) != 0)                                                 \
    {                                                                                                               \
        PRINTF_FFLUSH("Compare \"%s\" (expected) and \"%s\" (actual) case insensitive.\n", (expected), (actual));   \
        ASSERT((#actual), Compare_Strings_Case_Insensitive((expected),(actual)) == 0);                              \
    }
#else
#error "The macro \"ASSERT_STRING_CASE_INSENSITIVE_EQUALS\" is already defined !"
#endif /* ASSERT_STRING_CASE_INSENSITIVE_EQUALS */

/**
 * @brief Run all test functions.
 */
#ifndef RUN
#define RUN(test_function) tt_execute((#test_function), (test_function))
#else
#error "The macro \"RUN\" is already defined !"
#endif /* RUN */

/**
 * @brief Run all test functions with user-defined name for the function name. The name of the test function will
 * appear in the test results.
 */
#ifndef RUN_2
#define RUN_2(test_function, manual_test_function_name) tt_execute((manual_test_function_name), (test_function))
#else
#error "The macro \"RUN_2\" is already defined !"
#endif /* RUN_2 */

/**
 * @brief Print report of all already executed test asserts.
 */
#ifndef TEST_REPORT
#define TEST_REPORT() tt_report()
#else
#error "The macro \"TEST_REPORT\" is already defined !"
#endif /* TEST_REPORT */

#ifndef TT_COLOR_CODE
#define TT_COLOR_CODE 0x1B ///< Start value for the Terminal. Meaning: The next value will be a colour code
#else
#error "The macro \"TT_COLOR_CODE\" is already defined !"
#endif /* TT_COLOR_CODE */
#ifndef TT_COLOR_RED
#define TT_COLOR_RED "[1;31m" ///< Colour code for red outputs in the terminal (ANSI-Escape-Sequence)
#else
#error "The macro \"TT_COLOR_RED\" is already defined !"
#endif /* TT_COLOR_RED */
#ifndef TT_COLOR_GREEN
#define TT_COLOR_GREEN "[1;32m" ///< Colour code for green outputs in the terminal (ANSI-Escape-Sequence)
#else
#error "The macro \"TT_COLOR_GREEN\" is already defined !"
#endif /* TT_COLOR_GREEN */
#ifndef TT_COLOR_RESET
#define TT_COLOR_RESET "[0m" ///< Reset all colour code, that were set until now (ANSI-Escape-Sequence)
#else
#error "The macro \"TT_COLOR_RESET\" is already defined !"
#endif /* TT_COLOR_RESET */



extern int tt_passes;                           ///< Number of test, that passed
extern int tt_fails;                            ///< Number of tests, that failed
extern int tt_current_test_failed;              ///< Failed the current test ?
extern const char* tt_current_msg;              ///< Name of the current test
extern const char* tt_current_expression;       ///< Current assert statement
extern const char* tt_current_file;             ///< Current file name
extern int tt_current_line;                     ///< Current line

extern char tt_failed_function_names [32][128]; ///< Function name where the tests failed
/**
 * @brief Current index for the array, that holds the function names where the tests failed.
 */
extern int tt_current_failed_function_name;



/**
 * @brief Start the test function.
 *
 * The test function (signature: void NAME (void)) will be handed over and executed. In the test function are macros, if
 * a test failded, adjust global variables. Though this mecanism tt_execute() can determine, that at least one test
 * failed.
 *
 * ! Starting a test is done via the macro "RUN" !
 *   It is not nece
 * ! This function must never be executed directly !
 *
 * @param[in] name Name of the test. Will be used for report information
 * @param[in] test_function Function, that will be tested. Main signature: void NAME (void)
 */
extern void tt_execute (const char *name, void (*test_function) (void));

/**
 * @brief The assert function.
 *
 * In this function the state variables will be adjusted to suitable values.
 *
 * ! Like in the function tt_execute(): This function never has to call directly ! The call will be executed though the
 *   suitable macros (ASSERT, ASSERT_EQUALS, ...)
 *
 * @param[in] file Name of the source file, where the current test was written
 * @param[in] line Line number, where the test failed
 * @param[in] msg Message, that will be printed, if the test fails
 * @param[in] expression The assert statement represented as C-String (is created with preprocessor commands)
 * @param[in] pass Was the test (see: expression variable) successful ?
 *
 * @return Result of the assert statement
 */
extern int tt_assert (const char *file, int line, const char *msg, const char *expression, int pass);

/**
 * @brief Print the test reports.
 *
 * How many tests failed ?
 *
 * @return 0 if all tests passed; -1, if at least one test failed
 */
extern int tt_report (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TINYTEST_INCLUDED */
