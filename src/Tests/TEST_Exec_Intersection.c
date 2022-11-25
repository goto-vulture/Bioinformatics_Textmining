/**
 * @file TEST_Exec_Intersection.c
 *
 * @brief Test functions for the intersection execution.
 *
 * As source files
 * - ./Test_Data/test_ebm.json
 * - ./Test_Data/intervention_10MB.txt
 * will be used.
 *
 * "intervention_10MB.txt" is a 10 MB fragment from the original intervention.txt file. The original file is ~115 MB
 * large.
 *
 * @date 28.10.2022
 * @author: am4
 */

#include "TEST_Exec_Intersection.h"
#include <math.h>
#include "../Error_Handling/_Generics.h"
#include "../Exec_Intersection.h"
#include "tinytest.h"
#include "../CLI_Parameter.h"



#ifndef FILE_1
#define FILE_1 "./src/Tests/Test_Data/test_ebm.json"
#else
#error "The macro \"FILE_1\" is already defined !"
#endif /* FILE_1 */

#ifndef FILE_2
#define FILE_2 "./src/Tests/Test_Data/intervention_10MB.txt"
#else
#error "The macro \"FILE_2\" is already defined !"
#endif /* FILE_2 */

#ifndef OUT_FILE
#define OUT_FILE "./out.json"
#else
#error "The macro \"OUT_FILE\" is already defined !"
#endif /* OUT_FILE */

#ifndef TEST_EBM_FILE_MD5
#define TEST_EBM_FILE_MD5 "d1205477fc08c6e278d905edfdd537fb"
#else
#error "The macro \"TEST_EBM_FILE_MD5\" is already defined !"
#endif /* TEST_EBM_FILE_MD5 */

#ifndef INTERVENTION_10MB_FILE_MD5
#define INTERVENTION_10MB_FILE_MD5 "f5df7b5c5dc4fbd4de14e894c8c393c4"
#else
#error "The macro \"INTERVENTION_10MB_FILE_MD5\" is already defined !"
#endif /* INTERVENTION_10MB_FILE_MD5 */

#ifndef EXPECTED_COUNT_INTERSECTIONS_TOKENS
#define EXPECTED_COUNT_INTERSECTIONS_TOKENS 338306
#else
#error "The macro \"EXPECTED_COUNT_INTERSECTIONS_TOKENS\" is already defined !"
#endif /* EXPECTED_COUNT_INTERSECTIONS_TOKENS */

#ifndef EXPECTED_COUNT_INTERSECTIONS_SETS
#define EXPECTED_COUNT_INTERSECTIONS_SETS 319315
#else
#error "The macro \"EXPECTED_COUNT_INTERSECTIONS_SETS\" is already defined !"
#endif /* EXPECTED_COUNT_INTERSECTIONS_SETS */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(FILE_1) > 0 + 1, "The macro \"FILE_1\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(FILE_2) > 0 + 1, "The macro \"FILE_2\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(OUT_FILE) > 0 + 1, "The macro \"OUT_FILE\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(TEST_EBM_FILE_MD5) > 0 + 1, "The macro \"TEST_EBM_FILE_MD5\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(INTERVENTION_10MB_FILE_MD5) > 0 + 1, "The macro \"INTERVENTION_10MB_FILE_MD5\" needs at least one char (plus '\0') !");
_Static_assert(EXPECTED_COUNT_INTERSECTIONS_TOKENS > 0, "The macro \"EXPECTED_COUNT_INTERSECTIONS_TOKENS\" needs to be at least one !");
_Static_assert(EXPECTED_COUNT_INTERSECTIONS_SETS > 0, "The macro \"EXPECTED_COUNT_INTERSECTIONS_SETS\" needs to be at least one !");

IS_CONST_STR(FILE_1)
IS_CONST_STR(FILE_2)
IS_CONST_STR(OUT_FILE)
IS_CONST_STR(TEST_EBM_FILE_MD5)
IS_CONST_STR(INTERVENTION_10MB_FILE_MD5)
IS_TYPE(EXPECTED_COUNT_INTERSECTIONS_TOKENS, int)
IS_TYPE(EXPECTED_COUNT_INTERSECTIONS_SETS, int)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check, whether the number of tokens, that were found, is the expected value.
 */
extern void TEST_Number_Of_Tokens_Found (void)
{
    uint_fast64_t number_of_intersection_tokens = 0;

    // Adjust the CLI parameter to make the test runnable
    GLOBAL_CLI_INPUT_FILE = FILE_1;
    GLOBAL_CLI_INPUT_FILE2 = FILE_2;
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;
    // Keep results with one token, because the expected value shows all results !
    GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN = true;

    Exec_Intersection(NAN, &number_of_intersection_tokens, NULL);

    GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN = false;

    ASSERT_EQUALS(number_of_intersection_tokens, EXPECTED_COUNT_INTERSECTIONS_TOKENS);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check, whether the number of sets, that were found, is the expected value.
 */
extern void TEST_Number_Of_Sets_Found (void)
{
    Set_CLI_Parameter_To_Default_Values();

    uint_fast64_t number_of_intersection_sets = 0;

    // Adjust the CLI parameter to make the test runnable
    GLOBAL_CLI_INPUT_FILE = FILE_1;
    GLOBAL_CLI_INPUT_FILE2 = FILE_2;
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;
    // Keep results with one token, because the expected value shows all results !
    GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN = true;

    Exec_Intersection(NAN, NULL, &number_of_intersection_sets);

    ASSERT_EQUALS(number_of_intersection_sets, EXPECTED_COUNT_INTERSECTIONS_SETS);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check, whether the number of tokens is equal with switched input files.
 *
 * For the calculation it does not matter, if file A is used as first or second file. This is also for file B. The
 * reason for that is, that the intersection operation is symmetric.
 */
extern void TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files (void)
{
    Set_CLI_Parameter_To_Default_Values();

    uint_fast64_t number_of_intersection_tokens_1 = 0;
    uint_fast64_t number_of_intersection_tokens_2 = 0;

    // Adjust the CLI parameter to make the test runnable
    GLOBAL_CLI_INPUT_FILE = FILE_1;
    GLOBAL_CLI_INPUT_FILE2 = FILE_2;
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;

    Exec_Intersection(NAN, &number_of_intersection_tokens_1, NULL); // First intersection call

    // Swap the input files
    GLOBAL_CLI_INPUT_FILE = FILE_2;
    GLOBAL_CLI_INPUT_FILE2 = FILE_1;

    Exec_Intersection(NAN, &number_of_intersection_tokens_2, NULL); // Second intersection call

    ASSERT_EQUALS(number_of_intersection_tokens_1, number_of_intersection_tokens_2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check, whether the number of sets is equal with switched input files.
 *
 * For the calculation it does not matter, if file A is used as first or second file. This is also for file B. The
 * reason for that is, that the intersection operation is symmetric.
 */
extern void TEST_Number_Of_Sets_Equal_With_Switched_Input_Files (void)
{
    Set_CLI_Parameter_To_Default_Values();

    uint_fast64_t number_of_intersection_sets_1 = 0;
    uint_fast64_t number_of_intersection_sets_2 = 0;

    // Adjust the CLI parameter to make the test runnable
    GLOBAL_CLI_INPUT_FILE = FILE_1;
    GLOBAL_CLI_INPUT_FILE2 = FILE_2;
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;

    Exec_Intersection(NAN, NULL, &number_of_intersection_sets_1); // First intersection call

    // Swap the input files
    GLOBAL_CLI_INPUT_FILE = FILE_2;
    GLOBAL_CLI_INPUT_FILE2 = FILE_1;

    Exec_Intersection(NAN, NULL, &number_of_intersection_sets_2); // Second intersection call

    ASSERT_EQUALS(number_of_intersection_sets_1, number_of_intersection_sets_2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------



#ifdef FILE_1
#undef FILE_1
#endif /* FILE_1 */

#ifdef FILE_2
#undef FILE_2
#endif /* FILE_2 */

#ifdef OUT_FILE
#undef OUT_FILE
#endif /* OUT_FILE */

#ifdef TEST_EBM_FILE_MD5
#undef TEST_EBM_FILE_MD5
#endif /* TEST_EBM_FILE_MD5 */

#ifdef INTERVENTION_10MB_FILE_MD5
#undef INTERVENTION_10MB_FILE_MD5
#endif /* INTERVENTION_10MB_FILE_MD5 */

#ifdef EXPECTED_COUNT_INTERSECTIONS_TOKENS
#undef EXPECTED_COUNT_INTERSECTIONS_TOKENS
#endif /* EXPECTED_COUNT_INTERSECTIONS_TOKENS */

#ifdef EXPECTED_COUNT_INTERSECTIONS_SETS
#undef EXPECTED_COUNT_INTERSECTIONS_SETS
#endif /* EXPECTED_COUNT_INTERSECTIONS_SETS */
