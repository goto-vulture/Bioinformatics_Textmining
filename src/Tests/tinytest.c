/**
 * @file tinytest.c
 *
 * @brief TinyTest: A really really really tiny and simple no-hassle C unit-testing framework.
 *
 * @date 2010
 * @author Joe Walnes <joe@walnes.com> http://joewalnes.com
 *
 * Adapted from: x86 / Gyps.
 */

#include "tinytest.h"
#include <string.h>





int tt_passes                       = 0;
int tt_fails                        = 0;
int tt_current_test_failed          = 0;
const char* tt_current_msg          = NULL;
const char* tt_current_expression   = NULL;
const char* tt_current_file         = NULL;
int tt_current_line                 = 0;

char tt_failed_function_names [32][128];
int tt_current_failed_function_name = 0;



//---------------------------------------------------------------------------------------------------------------------

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
void tt_execute (const char *name, void (*test_function) (void))
{
    printf ("\n\n===== ===== ===== START RUN (%s) ===== ===== =====\n", name);

    // Variable will be adjusted by the macro ASSERT_EQUALS and ASSERT_STRING_EQUALS !
    tt_current_test_failed = 0;

    // Call test function
    test_function ();

    if (tt_current_test_failed)
    {
        printf ("failure: %s:%d: In test %s():\n    %s (%s)\n", tt_current_file, tt_current_line, name, tt_current_msg,
                tt_current_expression);
        tt_fails ++;

        // While free C-Strings are available, the names of the test functions, that failed, will be recorded
        if ((size_t) tt_current_failed_function_name <
                (sizeof (tt_failed_function_names) / sizeof (tt_failed_function_names [0]) - 1))
        {
            char* current_container_name = tt_failed_function_names [tt_current_failed_function_name];
            const size_t current_container_name_size = sizeof (tt_failed_function_names [tt_current_failed_function_name]);

            memset (current_container_name, '\0', current_container_name_size);
            strncpy (current_container_name, name, current_container_name_size);

            // Gurantee a terminater symbol at the end
            current_container_name [current_container_name_size - 1] = '\0';

            tt_current_failed_function_name ++;
        }
    }
    else
    {
        tt_passes ++;
    }

    printf ("===== ===== ===== END RUN (%s) ===== ===== =====\n\n", name);
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
int tt_assert (const char *file, int line, const char *msg, const char *expression, int pass)
{
    tt_current_msg = msg;
    tt_current_expression = expression;
    tt_current_file = file;
    tt_current_line = line;
    tt_current_test_failed = !pass;
    return pass;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print the test reports.
 *
 * How many tests failed ?
 *
 * @return 0 if all tests passed; -1, if at least one test failed
 */
int tt_report (void)
{
    if (tt_fails)
    {
        for (int i = 0; i < tt_current_failed_function_name; ++ i)
        {
            printf ("Failed function %2d: %s\n", i + 1, tt_failed_function_names [i]);
        }

        printf ("\n%c%sFAILED%c%s [%s] (passed:%d, failed:%d, total:%d)\n",
        TT_COLOR_CODE, TT_COLOR_RED, TT_COLOR_CODE, TT_COLOR_RESET, tt_current_file, tt_passes, tt_fails,
                tt_passes + tt_fails);

        return -1;
    }
    else
    {
        printf ("\n%c%sPASSED%c%s [%s] (total:%d)\n",
        TT_COLOR_CODE, TT_COLOR_GREEN, TT_COLOR_CODE, TT_COLOR_RESET, tt_current_file, tt_passes);
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
