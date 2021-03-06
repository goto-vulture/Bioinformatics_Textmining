/**
 * @file Document_Word_List.h
 *
 * @brief The Document_Word_List object is a two dimensional dynamic container for integer values (-> The mapped data).
 *
 * This object can be one of the input data of a intersection operation or a result object of them.
 *
 * The object name indicates, that it is a list with data from a whole document.
 *
 * @date 22.05.2022
 * @author Gyps
 */

#ifndef DOCUMENT_WORD_LIST_H
#define DOCUMENT_WORD_LIST_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stddef.h>



//=====================================================================================================================

enum Intersection_Mode
{
    INTERSECTION_MODE_2_NESTED_LOOPS = 1 << 0,
    INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH = 1 << 1,
    INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH = 1 << 2
};

struct Document_Word_List
{
    uint_fast32_t** data;           ///< 2 dimensional Array with the data
    size_t* arrays_lengths;         ///< 1 dimensional Array with the data length information

    uint_fast32_t next_free_array;  ///< Next free array in data
    size_t max_array_length;        ///< Max length of all data arrays
    size_t number_of_arrays;        ///< Number of arrays

    _Bool intersection_data;        ///< Was this object created as intersection result ?
};

//=====================================================================================================================

/**
 * @brief Create new document word list.
 *
 * Asserts:
 *      number_of_arrays > 0
 *      max_array_length > 0
 *
 * @param[in] number_of_arrays Number of arrays (Subsets)
 * @param[in] max_array_length Maximum length of the subsets
 *
 * @return Pointer to the new dynamic allocated Document_Word_List
 */
extern struct Document_Word_List*
Create_Document_Word_List
(
        const size_t number_of_arrays,
        const size_t max_array_length
);

/**
 * @brief Delete a Document_Word_List object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List, which will be deleted
 */
extern void
Delete_Document_Word_List
(
        struct Document_Word_List* object
);

/**
 * @brief Add data to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] data_length Length of the new data
 */
extern void
Append_Data_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const size_t data_length
);

/**
 * @brief Printh data of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
Show_Data_From_Document_Word_List
(
        const struct Document_Word_List* const object
);

/**
 * @brief Printh data and the attribute information of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
Show_Data_And_Attributes_From_Document_Word_List
(
        const struct Document_Word_List* const object
);

/**
 * @brief Determine intersection with a Document_Word_List and a dataset.
 *
 * You can change the intersection mode with the enum parameter Intersection_Mode. Actual following modes are
 * available
 * - 2 nested loops
 * - QSort with binary search
 * - Heapsort with binary search
 *
 * Asserts:
 *      object != NULL
 *      data != NULL
 *      data_length > 0
 *      data_length <= object->max_array_length
 *
 * @param[in] object Document_Word_List (1. Set for the intersection calculation)
 * @param[in] data dataset (2. Set for the intersection calculation)
 * @param[in] data_length Size of the dataset
 * @param[in] mode mode for the intersection calculation (Which approach should be used ?)
 *
 * @return New Document_Word_List with the intersections in the respective subsets
 */
extern struct Document_Word_List*
Intersect_Data_With_Document_Word_List
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length,

    const enum Intersection_Mode mode
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DOCUMENT_WORD_LIST_H */
