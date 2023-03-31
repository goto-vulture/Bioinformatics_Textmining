/**
 * @file Create_Test_Data.h
 *
 * @brief The creation of test data.
 *
 * Two variations:
 * 1. Full pseudo random numbers with an upper bound.
 * 2. Full pseudo random numbers with an upper bound AND with a specified list of data, that will be appear in the
 *    output.
 *
 * @date 22.05.2022
 * @author Gyps
 */

#ifndef CREATE_TEST_DATA_H
#define CREATE_TEST_DATA_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>                 // size_t
#include "../Document_Word_List.h"



/**
 * @brief Create a Document_Word_List with pseudo random data
 *
 * Asserts:
 *      number_of_arrays > 0
 *      max_array_length > 0
 *
 * @param[in] number_of_arrays Number of subsets
 * @param[in] max_array_length Max length of a subset
 * @param[in] rand_upper_bound Upper bound of pseudo random data
 *
 * @return Pointer to the new dynamic Document_Word_List object
 */
extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data
(
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
);

/**
 * @brief Create a Document_Word_List with pseudo random data + a specified set of data, that will be inserted in every
 * subset.
 *
 * The idea is to get the same intersection result. This is for debugging and test purposes useful, because the results
 * will be specified in every usage.
 *
 * Asserts:
 *      specified_data != NULL
 *      specified_data_length > 0
 *      number_of_arrays > 0
 *      max_array_length > 0
 *      specified_data_length <= max_array_length
 *
 * @param[in] specified_data Specified data
 * @param[in] specified_data_length Length of the specified data
 * @param[in] number_of_arrays Number of subsets
 * @param[in] max_array_length Max length of a subset
 * @param[in] rand_upper_bound Upper bound of pseudo random data
 *
 * @return Pointer to the new dynamic Document_Word_List object
 */
extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data_Plus_Specified_Data
(
        const DATA_TYPE* specified_data,
        const size_t specified_data_length,
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CREATE_TEST_DATA */
