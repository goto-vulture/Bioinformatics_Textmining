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
#include "../Error_Handling/Assert_Msg.h"
#include "../Exec_Intersection.h"
#include "tinytest.h"
#include "../CLI_Parameter.h"
#include "../Defines.h"
#include "md5.h"



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

#ifndef FILE_CSV
#define FILE_CSV "./src/Tests/Test_Data/Gene_or_Genome.csv" ///< The delimiter in this file is a whitespace
#else
#error "The macro \"FILE_CSV\" is already defined !"
#endif /* FILE_CSV */

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

#ifndef GENE_OR_GENOME_FILE_MD5
#define GENE_OR_GENOME_FILE_MD5 "a6bca16326273b218758042dcd09ea1a"
#else
#error "The macro \"GENE_OR_GENOME_FILE_MD5\" is already defined !"
#endif /* GENE_OR_GENOME_FILE_MD5 */

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
_Static_assert(sizeof(FILE_CSV) > 0 + 1, "The macro \"FILE_CSV\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(OUT_FILE) > 0 + 1, "The macro \"OUT_FILE\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(TEST_EBM_FILE_MD5) > 0 + 1, "The macro \"TEST_EBM_FILE_MD5\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(INTERVENTION_10MB_FILE_MD5) > 0 + 1, "The macro \"INTERVENTION_10MB_FILE_MD5\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(GENE_OR_GENOME_FILE_MD5) > 0 + 1, "The macro \"GENE_OR_GENOME_FILE_MD5\" needs at least one char (plus '\0') !");
_Static_assert(EXPECTED_COUNT_INTERSECTIONS_TOKENS > 0, "The macro \"EXPECTED_COUNT_INTERSECTIONS_TOKENS\" needs to be at least one !");
_Static_assert(EXPECTED_COUNT_INTERSECTIONS_SETS > 0, "The macro \"EXPECTED_COUNT_INTERSECTIONS_SETS\" needs to be at least one !");

IS_CONST_STR(FILE_1)
IS_CONST_STR(FILE_2)
IS_CONST_STR(FILE_CSV)
IS_CONST_STR(OUT_FILE)
IS_CONST_STR(TEST_EBM_FILE_MD5)
IS_CONST_STR(INTERVENTION_10MB_FILE_MD5)
IS_CONST_STR(GENE_OR_GENOME_FILE_MD5)
IS_TYPE(EXPECTED_COUNT_INTERSECTIONS_TOKENS, int)
IS_TYPE(EXPECTED_COUNT_INTERSECTIONS_SETS, int)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

/**
 * @brief This is the code, that will be used in the test of CPU extension.
 *
 * Why not a new function instead of a big macro ?
 * The test framework gives a error message with the function name, where the error occurred. With a single function the
 * message will be the same - independent of the tested extension. This could lead to bad understandable error messages.
 */
#ifndef EXTENSION_TEST
#define EXTENSION_TEST                                                                                                  \
    Set_CLI_Parameter_To_Default_Values();                                                                              \
                                                                                                                        \
    uint_fast64_t number_of_intersection_sets_1 = 0;                                                                    \
    uint_fast64_t number_of_intersection_tokens_1 = 0;                                                                  \
                                                                                                                        \
    uint_fast64_t number_of_intersection_sets_2 = 0;                                                                    \
    uint_fast64_t number_of_intersection_tokens_2 = 0;                                                                  \
                                                                                                                        \
    /* Test with default input and output files */                                                                      \
    GLOBAL_CLI_INPUT_FILE = FILE_1;                                                                                     \
    GLOBAL_CLI_INPUT_FILE2 = FILE_CSV;                                                                                  \
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;                                                                                  \
                                                                                                                        \
    GLOBAL_CLI_NO_CPU_EXTENSIONS = true;                                                                                \
    Exec_Intersection(NAN, &number_of_intersection_tokens_1, &number_of_intersection_sets_1);                           \
                                                                                                                        \
    GLOBAL_CLI_NO_CPU_EXTENSIONS = false;                                                                               \
    Exec_Intersection(NAN, &number_of_intersection_tokens_2, &number_of_intersection_sets_2);                           \
                                                                                                                        \
    ASSERT_EQUALS(number_of_intersection_sets_1, number_of_intersection_sets_2);                                        \
    ASSERT_EQUALS(number_of_intersection_tokens_1, number_of_intersection_tokens_2);                                    \
                                                                                                                        \
    /* Test with another second input file */                                                                           \
    GLOBAL_CLI_INPUT_FILE2 = FILE_2;                                                                                    \
                                                                                                                        \
    GLOBAL_CLI_NO_CPU_EXTENSIONS = true;                                                                                \
    Exec_Intersection(NAN, &number_of_intersection_tokens_1, &number_of_intersection_sets_1);                           \
                                                                                                                        \
    GLOBAL_CLI_NO_CPU_EXTENSIONS = false;                                                                               \
    Exec_Intersection(NAN, &number_of_intersection_tokens_2, &number_of_intersection_sets_2);                           \
                                                                                                                        \
    ASSERT_EQUALS(number_of_intersection_sets_1, number_of_intersection_sets_2);                                        \
    ASSERT_EQUALS(number_of_intersection_tokens_1, number_of_intersection_tokens_2);
#else
#error "The macro \"EXTENSION_TEST\" is already defined !"
#endif /* EXTENSION_TEST */



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Test, whether the test files have the expected MD5 sum.
 */
extern void TEST_MD5_Of_Test_Files (void)
{
    const char* files [] =
    {
            FILE_1,
            FILE_2,
            FILE_CSV
    };
    const char* md5_sums [] =
    {
            TEST_EBM_FILE_MD5,
            INTERVENTION_10MB_FILE_MD5,
            GENE_OR_GENOME_FILE_MD5
    };

    for (size_t i = 0; i < sizeof (files) / sizeof (files [0]); ++ i)
    {
        _Bool err_occurred = true;
        const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(files [i], md5_sums [i],
                &err_occurred);
        ASSERT_MSG(err_occurred == false, "Error occurred while checking a MD5 sum of a file !");
        ASSERT_FMSG(md5_sum_check_result == true, "MD5 sum of the file (%s) is not equal with the expected sum (%s) !",
                files [i], md5_sums [i]);

        ASSERT_EQUALS(true, md5_sum_check_result);
    }

    return;
}

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

/**
 * @brief Check, whether the number of tokens is equal with switched input files.
 *
 * One of the input files is a JSON file; the second is a CSV file with whitespace as delimiter.
 *
 * This function is comparable with "TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files()".
 *
 * @see TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files
 */
extern void TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files_JSON_And_CSV (void)
{
    Set_CLI_Parameter_To_Default_Values();

    uint_fast64_t number_of_intersection_tokens_1 = 0;
    uint_fast64_t number_of_intersection_tokens_2 = 0;

    // Adjust the CLI parameter to make the test runnable
    GLOBAL_CLI_INPUT_FILE = FILE_1;
    GLOBAL_CLI_INPUT_FILE2 = FILE_CSV;
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;

    Exec_Intersection(NAN, &number_of_intersection_tokens_1, NULL); // First intersection call

    // Swap the input files
    GLOBAL_CLI_INPUT_FILE = FILE_CSV;
    GLOBAL_CLI_INPUT_FILE2 = FILE_1;

    Exec_Intersection(NAN, &number_of_intersection_tokens_2, NULL); // Second intersection call

    ASSERT_EQUALS(number_of_intersection_tokens_1, number_of_intersection_tokens_2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check, whether the number of sets is equal with switched input files.
 *
 * One of the input files is a JSON file; the second is a CSV file with whitespace as delimiter.
 *
 * This function is comparable with "TEST_Number_Of_Sets_Equal_With_Switched_Input_Files()".
 *
 * @see TEST_Number_Of_Sets_Equal_With_Switched_Input_Files
 */
extern void TEST_Number_Of_Sets_Equal_With_Switched_Input_Files_JSON_And_CSV (void)
{
    Set_CLI_Parameter_To_Default_Values();

    uint_fast64_t number_of_intersection_sets_1 = 0;
    uint_fast64_t number_of_intersection_sets_2 = 0;

    // Adjust the CLI parameter to make the test runnable
    GLOBAL_CLI_INPUT_FILE = FILE_1;
    GLOBAL_CLI_INPUT_FILE2 = FILE_CSV;
    GLOBAL_CLI_OUTPUT_FILE = OUT_FILE;

    Exec_Intersection(NAN, NULL, &number_of_intersection_sets_1); // First intersection call

    // Swap the input files
    GLOBAL_CLI_INPUT_FILE = FILE_CSV;
    GLOBAL_CLI_INPUT_FILE2 = FILE_1;

    Exec_Intersection(NAN, NULL, &number_of_intersection_sets_2); // Second intersection call

    ASSERT_EQUALS(number_of_intersection_sets_1, number_of_intersection_sets_2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#if defined(__AVX__) && defined(__AVX2__) && ! defined(NO_AVX2) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with AVX2 extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_AVX2_Extension (void)
{
    EXTENSION_TEST

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#elif defined(__AVX__) && ! defined(NO_AVX) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with AVX extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_AVX_Extension (void)
{
    EXTENSION_TEST

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#elif defined(__SSE__) && defined(__SSE2__) && defined(__SSE3__) && defined(__SSE4_1__) && ! defined(NO_SSE4_1) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with SSE4_1 extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_SSE4_1_Extension (void)
{
    EXTENSION_TEST

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#elif defined(__SSE__) && defined(__SSE2__) && ! defined(NO_SSE2) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with SSE2 extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_SSE2_Extension (void)
{
    EXTENSION_TEST

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#elif defined(__MMX__) && ! defined(NO_MMX) && ! defined (NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with MMX extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_MMX_Extension (void)
{
    EXTENSION_TEST

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#else
/**
 * @brief This function is only a placeholder and tests nothing.
 *
 * Such a placeholder is necessary to have the same number of test functions on all supported machines.
 */
extern void TEST_Placeholder_For_No_Extensions (void)
{
    // Nothing to do here ...

    ASSERT_EQUALS(true, true);

    return;
}
#endif

//---------------------------------------------------------------------------------------------------------------------



#ifdef FILE_1
#undef FILE_1
#endif /* FILE_1 */

#ifdef FILE_2
#undef FILE_2
#endif /* FILE_2 */

#ifdef FILE_CSV
#undef FILE_CSV
#endif /* FILE_CSV */

#ifdef OUT_FILE
#undef OUT_FILE
#endif /* OUT_FILE */

#ifdef TEST_EBM_FILE_MD5
#undef TEST_EBM_FILE_MD5
#endif /* TEST_EBM_FILE_MD5 */

#ifdef INTERVENTION_10MB_FILE_MD5
#undef INTERVENTION_10MB_FILE_MD5
#endif /* INTERVENTION_10MB_FILE_MD5 */

#ifdef GENE_OR_GENOME_FILE_MD5
#undef GENE_OR_GENOME_FILE_MD5
#endif /* GENE_OR_GENOME_FILE_MD5 */

#ifdef EXPECTED_COUNT_INTERSECTIONS_TOKENS
#undef EXPECTED_COUNT_INTERSECTIONS_TOKENS
#endif /* EXPECTED_COUNT_INTERSECTIONS_TOKENS */

#ifdef EXPECTED_COUNT_INTERSECTIONS_SETS
#undef EXPECTED_COUNT_INTERSECTIONS_SETS
#endif /* EXPECTED_COUNT_INTERSECTIONS_SETS */

#ifdef EXTENSION_TEST
#undef EXTENSION_TEST
#endif /* EXTENSION_TEST */
