/**
 * @file TEST_Document_Word_List.h
 *
 * @brief Some test functions for the Document_Word_List object.
 *
 * @date 22.05.2022
 * @author Gyps
 */

#ifndef DOCUMENT_WORD_LIST_TEST_H
#define DOCUMENT_WORD_LIST_TEST_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



//#include <stdbool.h>    // _Bool



/**
 * @brief First intersection test with test data from the first meeting (11.05.2022).
 *
 * Now (26.12.2022) this function is a test function to check the intersection results with trivial input.
 */
extern void TEST_Intersection (void);

/**
 * @brief Intersection tests with full pseudo random numbers.
 */
extern _Bool TEST_Intersection_With_Random_Data (void);

/**
 * @brief Determine intersections with pseudo random data + a specified set of data, that will be inserted in every
 * subset.
 *
 * With this approach the excepted result is clear and the functionality of the intersection calculation can be checked.
 */
extern _Bool TEST_Intersection_With_Random_Data_And_Specified_Result (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DOCUMENT_WORD_LIST_TEST_H */
