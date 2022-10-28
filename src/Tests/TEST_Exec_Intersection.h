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
 * @brief Check, whether the number of tokens, that were found, is the expected value.
 */
extern void TEST_Number_Of_Tokens_Found (void);

/**
 * @brief Check, whether the number of sets, that were found, is the expected value.
 */
extern void TEST_Number_Of_Sets_Found (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_EXEC_INTERSECTION_H */
