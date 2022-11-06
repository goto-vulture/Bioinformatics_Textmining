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
#define DOCUMENT_WORD_LIST_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>   // uint_fast32_t
#include <stddef.h>     // size_t
#include "Defines.h"    // CHAR_OFFSET_TYPE



//=====================================================================================================================

enum Intersection_Mode
{
    INTERSECTION_MODE_2_NESTED_LOOPS = 1 << 0,
    INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH = 1 << 1,
    INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH = 1 << 2
};

struct Document_Word_List
{
    struct Data_And_Offsets
    {
        uint_fast32_t** data;                       ///< 2 dimensional Array with the data
        CHAR_OFFSET_TYPE** char_offsets;            ///< Char offsets
        SENTENCE_OFFSET_TYPE** sentence_offsets;    ///< Sentence offsets
        WORD_OFFSET_TYPE** word_offsets;            ///< Word offsets
    } data_struct;

    size_t* arrays_lengths;         ///< 1 dimensional Array with the data length information

    uint_fast32_t next_free_array;  ///< Next free array in data
    size_t* allocated_array_size;   ///< Allocated array size
    size_t max_array_length;        ///< Max length of all data arrays
    size_t number_of_arrays;        ///< Number of arrays

    size_t malloc_calloc_calls;     ///< How many malloc / calloc calls were done with this object ?
    size_t realloc_calls;           ///< How many realloc calls were done with this object ?

    _Bool intersection_data;        ///< Was this object created as intersection result ?

    /**
     * @brief First ID of the data set (only valid data, when the object is intersection data).
     *
     * The ID is in our data sets sometimes a integer, sometimes a string. Therefore it is necessary to save the ID as
     * char array.
     */
    char dataset_id_1 [DATASET_ID_LENGTH];
    /**
     * @brief Second ID of the data set (only valid data, when the object is intersection data).
     *
     * The ID is in our data sets sometimes a integer, sometimes a string. Therefore it is necessary to save the ID as
     * char array.
     */
    char dataset_id_2 [DATASET_ID_LENGTH];
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
DocumentWordList_CreateObject
(
        const size_t number_of_arrays,
        const size_t max_array_length
);

/**
 * @brief Create new document word list as result for an intersection process.
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
DocumentWordList_CreateObjectAsIntersectionResult
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
DocumentWordList_DeleteObject
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
DocumentWordList_AppendData
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const size_t data_length
);

/**
 * @brief Add data with up to two offset arrays to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] new_offsets Offset array
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendDataWithOffsets
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const CHAR_OFFSET_TYPE* const new_offsets,
        const size_t data_length
);

/**
 * @brief Add data with up to three offsets (char, sentence and word offset) to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] new_char_offsets Char offset array
 * @param[in] new_sentence_offsets Sentence offset array
 * @param[in] new_word_offsets Word offset array
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendDataWithThreeTypeOffsets
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const CHAR_OFFSET_TYPE* const new_char_offsets,
        const SENTENCE_OFFSET_TYPE* const new_sentence_offsets,
        const WORD_OFFSET_TYPE* const new_word_offsets,
        const size_t data_length
);

/**
 * @brief Add one value to a Document_Word_List.
 *
 * ! Caution: If you use this function the container manages this single value as an data set with the length of one.
 * The effect is, that one array in the container will be used like an full array !
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 * @param[in] new_value New value
 */
extern void
DocumentWordList_AppendOneValueAsNewDataSet
(
        struct Document_Word_List* const object,
        const uint_fast32_t new_value
);

/**
 * @brief Put one value to a Document_Word_List.
 *
 * The new value will be appended to the next free data array. Instead of DocumentWordList_AppendOneValueAsNewDataSet,
 * the new value will NOT be interpreted as new data set !
 * @see DocumentWordList_AppendOneValueAsNewDataSet
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 * @param[in] new_value New value
 */
extern void
Put_One_Value_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t new_value
);

/**
 * @brief Put one value with offsets of the three types to a Document_Word_List.
 *
 * The new value and the given offsets will be appended to the next free data array. Instead of
 * DocumentWordList_AppendOneValueAsNewDataSet, the new value will NOT be interpreted as new data set !
 * @see DocumentWordList_AppendOneValueAsNewDataSet
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 * @param[in] new_value New value
 * @param[in] new_char_offset First new char offset value
 * @param[in] new_sentence_offset First new sentence offset value
 * @param[in] new_word_offset First new word offset value
 */
extern void
Put_One_Value_And_Offset_Types_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t new_value,
        const CHAR_OFFSET_TYPE new_char_offset,
        const SENTENCE_OFFSET_TYPE new_sentence_offset,
        const WORD_OFFSET_TYPE new_word_offset
);

/**
 * @brief Print data of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
DocumentWordList_ShowData
(
        const struct Document_Word_List* const object
);

/**
 * @brief Determine the full memory usage in byte.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Document_Word_List object
 *
 * @return Size of the full object in bytes
 */
extern size_t
DocumentWordList_GetAllocatedMemSize
(
        const struct Document_Word_List* const container
);

/**
 * @brief Print attributes of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
DocumentWordList_ShowAttributes
(
        const struct Document_Word_List* const object
);

/**
 * @brief Print data and the attribute information of a Document_Word_List to stdout.
 *
 * This function is for debugging purposes.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 */
extern void
DocumentWordList_ShowDataAndAttributes
(
        const struct Document_Word_List* const object
);

/**
 * @brief Determine intersection with a Document_Word_List and a data set.
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
 * @param[in] data Data set (2. Set for the intersection calculation)
 * @param[in] data_length Size of the data set
 * @param[in] mode Mode for the intersection calculation (Which approach should be used ?)
 *
 * @return New Document_Word_List with the intersections in the respective subsets
 */
extern struct Document_Word_List*
DocumentWordList_IntersectWithData
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length,

    const enum Intersection_Mode mode
);

/**
 * @brief Is there data in a Document_Word_List?
 *
 * After an intersection variables like "next_free_array" are set to the value of the data, which was used in the
 * intersection. The reason: in every array could be an intersection result. So these values are necessary. But you
 * cannot expect, when "next_free_array" is > 0, that then data is in the object. (At lest when the object was created
 * as result of an intersection process)
 *
 * This function closes the gap. It shows, if there are data in an Document_Word_List object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Document_Word_List
 *
 * @return true, if there is data in the object, otherwise false
 */
extern _Bool
DocumentWordList_IsDataInObject
(
        const struct Document_Word_List* const object
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DOCUMENT_WORD_LIST_H */
