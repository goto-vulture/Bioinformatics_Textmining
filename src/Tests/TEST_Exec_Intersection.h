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



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_EXEC_INTERSECTION_H */
