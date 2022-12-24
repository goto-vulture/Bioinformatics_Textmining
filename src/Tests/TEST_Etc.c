/**
 * @file TEST_etc.c
 *
 * @brief Here are TEST functions, that cannot be placed in a translation unit for specific struct objects.
 *
 * @date 01.11.2022
 * @author: leer
 */

#include "TEST_Etc.h"

#include <math.h>
#include "tinytest.h"
#include "../CLI_Parameter.h"
#include "../Exec_Intersection.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../ANSI_Esc_Seq.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the number of free calls at the end of a calculation.
 *
 * If the dynamic memory management works, than there are exact the same number of free calls compared with the sum of
 * malloc and calloc calls.
 */
extern void TEST_Number_Of_Free_Calls (void)
{
    GLOBAL_CLI_INPUT_FILE = "./src/Tests/Test_Data/test_ebm.json";
    GLOBAL_CLI_INPUT_FILE2 = "./src/Tests/Test_Data/intervention_10MB.txt";
    GLOBAL_CLI_OUTPUT_FILE = "./out.json";

    // Resetting the counter to guarantee, that only the function calls of the next operations will be counted
    GLOBAL_malloc_calls = 0;
    GLOBAL_calloc_calls = 0;
    GLOBAL_free_calls = 0;

    // Call the intersection calculation
    Exec_Intersection(NAN, NULL, NULL);

    const int_fast64_t missing_free_calls = (int_fast64_t) ((GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls);

    // We expect, that no free call is missing
    ASSERT_EQUALS(0, missing_free_calls);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the functionality of the most important ANSI esc sequences.
 *
 * This test cannot fail, because it is not possible for the program to check whether the output on the terminal has
 * the expected view.
 */
extern void TEST_ANSI_Esc_Seq (void)
{
    Test_ANSI_Main_Esc_Seq();

    // We cannot check the view of the terminal output; so such a test cannot fail.
    ASSERT_EQUALS(true, true);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
