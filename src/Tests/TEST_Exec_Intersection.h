/**
 * @file TEST_Exec_Intersection.h
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

#ifndef TEST_EXEC_INTERSECTION_H
#define TEST_EXEC_INTERSECTION_H ///< Include Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Test, whether the test files have the expected MD5 sum.
 */
extern void TEST_MD5_Of_Test_Files (void);

/**
 * @brief Check, whether the number of tokens, that were found, is the expected value.
 */
extern void TEST_Number_Of_Tokens_Found (void);

/**
 * @brief Check, whether the number of sets, that were found, is the expected value.
 */
extern void TEST_Number_Of_Sets_Found (void);

/**
 * @brief Check, whether the number of tokens is equal with switched input files.
 *
 * For the calculation it does not matter, if file A is used as first or second file. This is also for file B. The
 * reason for that is, that the intersection operation is symmetric.
 */
extern void TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files (void);

/**
 * @brief Check, whether the number of sets is equal with switched input files.
 *
 * For the calculation it does not matter, if file A is used as first or second file. This is also for file B. The
 * reason for that is, that the intersection operation is symmetric.
 */
extern void TEST_Number_Of_Sets_Equal_With_Switched_Input_Files (void);

/**
 * @brief Check, whether the number of tokens is equal with switched input files.
 *
 * One of the input files is a JSON file; the second is a CSV file with whitespace as delimiter.
 *
 * This function is comparable with "TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files()".
 *
 * @see TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files
 */
extern void TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files_JSON_And_CSV (void);

/**
 * @brief Check, whether the number of sets is equal with switched input files.
 *
 * One of the input files is a JSON file; the second is a CSV file with whitespace as delimiter.
 *
 * This function is comparable with "TEST_Number_Of_Sets_Equal_With_Switched_Input_Files()".
 *
 * @see TEST_Number_Of_Sets_Equal_With_Switched_Input_Files
 */
extern void TEST_Number_Of_Sets_Equal_With_Switched_Input_Files_JSON_And_CSV (void);

#if defined(__AVX__) && defined(__AVX2__) && ! defined(NO_AVX2) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with AVX2 extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_AVX2_Extension (void);

#elif defined(__AVX__) && ! defined(NO_AVX) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with AVX extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_AVX_Extension (void);

#elif defined(__SSE__) && defined(__SSE2__) && defined(__SSE3__) && defined(__SSE4_1__) && ! defined(NO_SSE4_1) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with SSE4_1 extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_SSE4_1_Extension (void);

#elif defined(__SSE__) && defined(__SSE2__) && ! defined(NO_SSE2) && ! defined(NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with SSE2 extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_SSE2_Extension (void);

#elif defined(__MMX__) && ! defined(NO_MMX) && ! defined (NO_CPU_EXTENSIONS)
/**
 * @brief Test, whether the results from the calculations with MMX extension and without are the same.
 *
 * It will be compared:
 * - Number of tokens
 * - Number of sets
 */
extern void TEST_MMX_Extension (void);

#else
/**
 * @brief This function is only a placeholder and tests nothing.
 *
 * Such a placeholder is necessary to have the same number of test functions on all supported machines.
 */
extern void TEST_Placeholder_For_No_Extensions (void);
#endif



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_EXEC_INTERSECTION_H */
