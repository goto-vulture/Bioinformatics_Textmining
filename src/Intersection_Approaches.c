/**
 * @file Intersection_Approaches.c
 *
 * @brief For the intersection operation three approaches are available:
 *
 * 1. The simplest way: Two nested loops
 * 2. Data sorting (with QSort from the C lib) and a binary search
 * 3. Data sorting (with heap sort) and a binary search
 *
 * Plots with runtime comparisons are in the "Plots" folder.
 *
 * Summary of the tests: In normal cases (with datasets of a few hundred integers) the simplest way is faster, because
 * the sorting of the data needs most of the time. Only with a larger amount of data the approaches with a data sorting
 * are faster.
 *
 * @date 23.05.2022
 * @author Gyps
 */
#include "Intersection_Approaches.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Misc.h"
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Error_Handling/_Generics.h"
#include "Print_Tools.h"



#ifndef MULTIPLE_GUARD_ALLOC_STEP
#define MULTIPLE_GUARD_ALLOC_STEP 100
#else
#error "The macro \"MULTIPLE_GUARD_ALLOC_STEP\" is already defined !"
#endif /* MULTIPLE_GUARD_ALLOC_STEP */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(MULTIPLE_GUARD_ALLOC_STEP > 0, "The marco \"MULTIPLE_GUARD_ALLOC_STEP\" needs to be at least 1 !");

IS_TYPE(MULTIPLE_GUARD_ALLOC_STEP, int)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

/**
 * @brief Create a copy of the submitted Document_Word_List and initialize them.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Original Document_Word_List
 *
 * @return Pointer to the new initialized Document_Word_List
 */
static struct Document_Word_List*
Init_Intersection
(
        const struct Document_Word_List* const object
);

/**
 * @brief Start a intersection process.
 *
 * It is necessary to prealloc the intersection result. Also there will be no realloc process. So the size of the data
 * needs also be suitable.
 *
 * Asserts:
 *      intersection_result != NULL
 *      object != NULL
 *      data != NULL
 *      data_length > 0
 *
 * @param[in] intersection_result Return memory; here the results of the intersection can be found
 * @param[in] object Original Document_Word_List
 * @param[in] data Data, that will be used for the intersection with the Document_Word_List object
 * @param[in] data_length Number of the elements in the data
 */
static void
Find_Intersection_Data
(
        const struct Document_Word_List* const restrict intersection_result,
        const struct Document_Word_List* const restrict object,
        const uint_fast32_t* const restrict data,
        const size_t data_length
);

/**
 * @brief Compare function for qsort()
 *
 * @param[in] a First value
 * @param[in] b Second value
 *
 * @return -1, if a < b; +1 if a > b; else 0
 */
static int
Compare_Function
(
        const void* const a,
        const void* const b
);

/**
 * @brief Execute binary search.
 *
 * ! This function excepts, that the data is already ascending sorted !
 *
 * Asserts:
 *      data != NULL
 *
 * @param[in] data Data in which is searched
 * @param[in] data_size Size of the data
 * @param[in] search_value Value to be searched
 *
 * @return true, if the value is in the dataset, else false
 */
static _Bool
Binary_Search
(
        const uint_fast32_t* const data,
        const size_t data_size,
        const uint_fast32_t search_value
);

/**
 * @brief Implementation of the sorting algorithm "HeapSort".
 *
 * This implementation was derived from the example implementation on Wikipedia.
 * https://de.wikipedia.org/wiki/Heapsort#Implementierung
 *
 * Asserts:
 *      data != NULL
 *
 * @param[in] data Data which will be sorted (ascending)
 * @param[in] data_size Size of the data
 */
static void
Heapsort
(
        uint_fast32_t* const data,
        const size_t data_size
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersections with a naive approach (Compare everyone with everyone -> to nested loops).
 *
 * Asserts:
 *      object != NULL
 *      data != NULL
 *      data_length = 0
 *
 * @param[in] object Document_Word_List
 * @param[in] data Data, that will be used for the intersection with the Document_Word_List object
 * @param[in] data_length Number of the elements in the data
 *
 * @return New dynamic object containing the intersection result to each subset of the word list
 */
extern struct Document_Word_List*
IntersectionApproach_TwoNestedLoops
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
)
{
    ASSERT_MSG(object != NULL, "Document_Word_List is NULL !");
    ASSERT_MSG(data != NULL, "Data is NULL !");
    ASSERT_MSG(data_length > 0, "Length of the data is 0 !");

    struct Document_Word_List* intersection_result = DocumentWordList_CreateObject (object->number_of_arrays,
            object->max_array_length);
    ASSERT_ALLOC(intersection_result, "Cannot create new Document Word List for intersection !",
            sizeof (struct Document_Word_List) + object->number_of_arrays * object->max_array_length *
            sizeof (uint_fast32_t));
    intersection_result->next_free_array = object->next_free_array;
    intersection_result->intersection_data = true;

    // Array, which display, if a value is already in the intersection
    size_t size_multiple_guard = (data_length > MULTIPLE_GUARD_ALLOC_STEP) ? data_length : MULTIPLE_GUARD_ALLOC_STEP;

    _Bool* multiple_guard = (_Bool*) CALLOC(size_multiple_guard, sizeof (_Bool));
    ASSERT_ALLOC(multiple_guard, "Cannot create the multiple guard !", size_multiple_guard * sizeof (_Bool));


    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        memset(multiple_guard, '\0', size_multiple_guard * sizeof (_Bool));

        for (size_t i2 = 0; i2 < object->arrays_lengths [i]; ++ i2)
        {
            // Compare everyone with everyone
            for (size_t i3 = 0; i3 < data_length; ++ i3)
            {
                if (object->data_struct.data [i][i2] == data [i3])
                {
                    // Has the array, which display, if a value is already in the intersection, enough memory ?
                    if (data [i3] >= size_multiple_guard)
                    {
                        //size_multiple_guard += MULTIPLE_GUARD_ALLOC_STEP;
                        const size_t old_size_multiple_guard = size_multiple_guard;
                        size_multiple_guard = data [i3] + 1;
                        multiple_guard = (_Bool*) REALLOC(multiple_guard, size_multiple_guard * sizeof (_Bool));
                        ASSERT_ALLOC(multiple_guard, "Cannot increase the memory for the multiple guard !",
                                size_multiple_guard * sizeof (_Bool));
                        memset(multiple_guard + old_size_multiple_guard, '\0', size_multiple_guard - old_size_multiple_guard);
                    }

                    //if (multiple_guard [data [i3]] == false)
                    {
                        intersection_result->data_struct.data [i][intersection_result->arrays_lengths [i]] = data [i3];
                        intersection_result->arrays_lengths [i] ++;
                        multiple_guard [data [i3]] = true;
                    }
                }
            }
        }
    }

    // printf ("Used size for the multiple guard: %zu\n", size_multiple_guard * sizeof (_Bool));
    FREE_AND_SET_TO_NULL(multiple_guard);

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersections with a previous ascending sort of the elements in the Document_Word_List. Afterwards
 * for the search process the binary search will be used.
 *
 * Algorithm for the sorting: QSort (https://en.wikipedia.org/wiki/Qsort)
 *
 * Asserts:
 *      object != NULL
 *      data != NULL
 *      data_length = 0
 *
 * @param[in] object Document_Word_List
 * @param[in] data Data, that will be used for the intersection with the Document_Word_List object
 * @param[in] data_length Number of the elements in the data
 *
 * @return New dynamic object containing the intersection result to each subset of the word list
 */
extern struct Document_Word_List*
IntersectionApproach_QSortAndBinarySearch
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
)
{
    ASSERT_MSG(object != NULL, "Document_Word_List is NULL !");
    ASSERT_MSG(data != NULL, "Data is NULL !");
    ASSERT_MSG(data_length > 0, "Length of the data is 0 !");

    struct Document_Word_List* intersection_result = Init_Intersection (object);

    // Sort all data ascending with the QSort algorithm
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        qsort (object->data_struct.data [i], object->arrays_lengths [i], sizeof (uint_fast32_t), Compare_Function);
    }

    Find_Intersection_Data (intersection_result, object, data, data_length);

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersections with a previous ascending sort of the elements in the Document_Word_List. Afterwards
 * for the search process the binary search will be used.
 *
 * Algorithm for the sorting: Heapsort (https://en.wikipedia.org/wiki/Heapsort)
 *
 * Asserts:
 *      object != NULL
 *      data != NULL
 *      data_length = 0
 *
 * @param[in] object Document_Word_List
 * @param[in] data Data, that will be used for the intersection with the Document_Word_List object
 * @param[in] data_length Number of the elements in the data
 *
 * @return New dynamic object containing the intersection result to each subset of the word list
 */
extern struct Document_Word_List*
IntersectionApproach_HeapSortAndBinarySearch
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
)
{
    ASSERT_MSG(object != NULL, "Document_Word_List is NULL !");
    ASSERT_MSG(data != NULL, "Data is NULL !");
    ASSERT_MSG(data_length > 0, "Length of the data is 0 !");

    struct Document_Word_List* intersection_result = Init_Intersection (object);

    // Sort all data ascending with the Heapsort algorithm
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        Heapsort(object->data_struct.data [i], object->arrays_lengths [i]);
    }

    Find_Intersection_Data (intersection_result, object, data, data_length);

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersections with a naive approach (Compare everyone with everyone -> to nested loops).
 *
 * ATTENTION: Here are two raw data arrays used. NO Document_Word_List as one of the input parameter.
 *
 * Asserts:
 *      data_1 != NULL
 *      data_1_length = 0
 *      data_2 != NULL
 *      data_2_length = 0
 *      char_offsets != NULL
 *      sentence_offsets != NULL
 *      word_offsets != NULL
 *
 * @param[in] data_1 Data, that will be used for the intersection with the second data array
 * @param[in] char_offsets Char offsets of the data focused on the source file, that was also the source for data_1
 * @param[in] sentence_offsets Sentence offsets of the data focused on the source file, that was also the source for data_1
 * @param[in] word_offset Offset of the words focused on the source file
 * @param[in] data_1_length Number of the elements in the first data array
 * @param[in] data_2 Data, that will be used for the intersection with the first data array
 * @param[in] data_2_length Number of the elements in the second data array
 *
 * @param[in] id_1 ID of the first data array
 * @param[in] id_2 ID of the second data array
 *
 * @return New dynamic object containing the intersection result
 */
extern struct Document_Word_List*
IntersectionApproach_TwoNestedLoopsWithTwoRawDataArrays
(
    const uint_fast32_t* const restrict data_1,
    const CHAR_OFFSET_TYPE* const restrict char_offsets,
    const SENTENCE_OFFSET_TYPE* const restrict sentence_offsets,
    const WORD_OFFSET_TYPE* const restrict word_offsets,
    const size_t data_1_length,
    const uint_fast32_t* const restrict data_2,
    const size_t data_2_length,

    const char* const restrict id_1,
    const char* const restrict id_2
)
{
    ASSERT_MSG(data_1 != NULL, "Data 1 is NULL !");
    ASSERT_MSG(data_1_length > 0, "Length of the data 1 is 0 !");
    ASSERT_MSG(data_2 != NULL, "Data 2 is NULL !");
    ASSERT_MSG(data_2_length > 0, "Length of the data 2 is 0 !");
    ASSERT_MSG(char_offsets != NULL, "Char offset is NULL !");
    ASSERT_MSG(sentence_offsets != NULL, "Sentence offset is NULL !");
    ASSERT_MSG(word_offsets != NULL, "Word offset is NULL !");

#ifndef __STDC_NO_VLA__
    #ifndef UNSAFE_VLA_USAGE
        ASSERT_FMSG(data_1_length <= MAX_VLA_LENGTH, "Length of the data 1 is too large for VLA ! Max. valid: %zu; Got %zu !",
                MAX_VLA_LENGTH, data_1_length);
        ASSERT_FMSG(data_2_length <= MAX_VLA_LENGTH, "Length of the data 2 is too large for VLA ! Max. valid: %zu; Got %zu !",
                MAX_VLA_LENGTH, data_2_length);
    #endif /* UNSAFE_VLA_USAGE */
#else
    // Nothing to do
#endif /* #ifndef __STDC_NO_VLA__ */

    // This result contains only one array, because two raw data arrays will be used for the intersection
    struct Document_Word_List* intersection_result = DocumentWordList_CreateObjectAsIntersectionResult
            (1, MAX(data_1_length, data_2_length));

    // Arrays, which display, if a value is already in the intersection
    _Bool* multiple_guard_data_1 = NULL;
    _Bool* multiple_guard_data_2 = NULL;
#ifndef __STDC_NO_VLA__
    #ifndef UNSAFE_VLA_USAGE
        if (data_1_length > MAX_VLA_LENGTH)
        {
            multiple_guard_data_1 = (_Bool*) CALLOC(data_1_length, sizeof (_Bool));
            ASSERT_ALLOC(multiple_guard_data_1, "Cannot create the multiple guard for data 1 !", data_1_length * sizeof (_Bool));
        }
        else
        {
    #endif /* UNSAFE_VLA_USAGE */
        _Bool tmp_multiple_guard_data_1 [data_1_length];
        multiple_guard_data_1 = tmp_multiple_guard_data_1;
        memset(multiple_guard_data_1, '\0', data_1_length);
    #ifndef UNSAFE_VLA_USAGE
        }
        if (data_2_length > MAX_VLA_LENGTH)
        {
            multiple_guard_data_2 = (_Bool*) CALLOC(data_2_length, sizeof (_Bool));
            ASSERT_ALLOC(multiple_guard_data_2, "Cannot create the multiple guard for data 2 !", data_2_length * sizeof (_Bool));
        }
        else
        {
    #endif /* UNSAFE_VLA_USAGE */
        _Bool tmp_multiple_guard_data_2 [data_2_length];
        multiple_guard_data_2 = tmp_multiple_guard_data_2;
        memset(multiple_guard_data_2, '\0', data_2_length);
    #ifndef UNSAFE_VLA_USAGE
        }
    #endif /* UNSAFE_VLA_USAGE */
#else
    multiple_guard_data_1 = (_Bool*) CALLOC(data_1_length, sizeof (_Bool));
    ASSERT_ALLOC(multiple_guard_data_1, "Cannot create the multiple guard for data 1 !", data_1_length * sizeof (_Bool));
    multiple_guard_data_2 = (_Bool*) CALLOC(data_2_length, sizeof (_Bool));
    ASSERT_ALLOC(multiple_guard_data_2, "Cannot create the multiple guard for data 2 !", data_2_length * sizeof (_Bool));
#endif /* __STDC_NO_VLA__ */

    // In an intersection the size of the result set can only be the max. size of the smaller source set
    const size_t min_data_length = MIN(data_1_length, data_2_length);
    size_t data_found_counter = 0;

    // Calculate intersection
    for (register size_t d1 = 0; d1 < data_1_length; ++ d1)
    {
        for (register size_t d2 = 0; d2 < data_2_length; ++ d2)
        {
            if (data_1 [d1] == data_2 [d2])
            {
                // Try to leave the loops as early as possible
                if (data_found_counter >= min_data_length)
                {
                    goto end;
                }

                // Was the current value already inserted in the intersection result ?
                if (! multiple_guard_data_1 [d1] && ! multiple_guard_data_2 [d2])
                {
                    //Put_One_Value_To_Document_Word_List(intersection_result, data_1 [d1]);
                    Put_One_Value_And_Offset_Types_To_Document_Word_List(intersection_result, data_1 [d1],
                            char_offsets [d1], sentence_offsets [d1], word_offsets [d1]);
                    // intersection_result->data [0][intersection_result->arrays_lengths [0]] = data_1 [d1];
                    //++ intersection_result->arrays_lengths [0];
                    multiple_guard_data_1 [d1] = true;
                    multiple_guard_data_2 [d2] = true;

                    ++ data_found_counter;
                }
            }
        }
    }
    end:

    intersection_result->intersection_data = true;
    if (id_1 != NULL)
    {
        strncpy (intersection_result->dataset_id_1, id_1, COUNT_ARRAY_ELEMENTS(intersection_result->dataset_id_1) - 1);
        intersection_result->dataset_id_1 [COUNT_ARRAY_ELEMENTS(intersection_result->dataset_id_1) - 1] = '\0';
    }
    if (id_2 != NULL)
    {
        strncpy (intersection_result->dataset_id_2, id_2, COUNT_ARRAY_ELEMENTS(intersection_result->dataset_id_2) - 1);
        intersection_result->dataset_id_2 [COUNT_ARRAY_ELEMENTS(intersection_result->dataset_id_2) - 1] = '\0';
    }

#ifndef __STDC_NO_VLA__
    #ifndef UNSAFE_VLA_USAGE
        if (data_1_length > MAX_VLA_LENGTH)
        {
            FREE_AND_SET_TO_NULL(multiple_guard_data_1);
        }
        if (data_2_length > MAX_VLA_LENGTH)
        {
            FREE_AND_SET_TO_NULL(multiple_guard_data_2);
        }
    #endif /* UNSAFE_VLA_USAGE */
#else
    FREE_AND_SET_TO_NULL(multiple_guard_data_1);
    FREE_AND_SET_TO_NULL(multiple_guard_data_2);
#endif /* __STDC_NO_VLA__ */

    return intersection_result;
}

//=====================================================================================================================

/**
 * @brief Create a copy of the submitted Document_Word_List and initialize them.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Original Document_Word_List
 *
 * @return Pointer to the new initialized Document_Word_List
 */
static struct Document_Word_List*
Init_Intersection
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Document_Word_List is NULL !");

    struct Document_Word_List* intersection_result = DocumentWordList_CreateObject (object->number_of_arrays,
            object->max_array_length);
    ASSERT_ALLOC(intersection_result, "Cannot create new Document Word List for intersection !",
            sizeof (struct Document_Word_List) + object->number_of_arrays * object->max_array_length *
            sizeof (uint_fast32_t));
    intersection_result->next_free_array = object->next_free_array;
    intersection_result->intersection_data = true;

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Start a intersection process.
 *
 * It is necessary to prealloc the intersection result. Also there will be no realloc process. So the size of the data
 * needs also be suitable.
 *
 * Asserts:
 *      intersection_result != NULL
 *      object != NULL
 *      data != NULL
 *      data_length > 0
 *
 * @param[in] intersection_result Return memory; here the results of the intersection can be found
 * @param[in] object Original Document_Word_List
 * @param[in] data Data, that will be used for the intersection with the Document_Word_List object
 * @param[in] data_length Number of the elements in the data
 */
static void
Find_Intersection_Data
(
        const struct Document_Word_List* const restrict intersection_result,
        const struct Document_Word_List* const restrict object,
        const uint_fast32_t* const restrict data,
        const size_t data_length
)
{
    ASSERT_MSG(intersection_result != NULL, "Memory for the intersection result is NULL !");
    ASSERT_MSG(object != NULL, "Original Document_Word_List is NULL !");
    ASSERT_MSG(data != NULL, "Data is NULL !");
    ASSERT_MSG(data_length > 0, "Data length is 0 !");

    // Array, which display, if a value is already in the intersection
    size_t size_multiple_guard = (data_length > MULTIPLE_GUARD_ALLOC_STEP) ? data_length : MULTIPLE_GUARD_ALLOC_STEP;

    _Bool* multiple_guard = (_Bool*) CALLOC(size_multiple_guard, sizeof (_Bool));
    ASSERT_ALLOC(multiple_guard, "Cannot create the multiple guard !", size_multiple_guard * sizeof (_Bool));

    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        memset(multiple_guard, '\0', size_multiple_guard * sizeof (_Bool));

        // Execute the binary search
        for (size_t i2 = 0; i2 < data_length; ++ i2)
        {
            if (Binary_Search(object->data_struct.data[i], object->arrays_lengths [i], data [i2]) == true)
            {
                // Has the array, which display, if a value is already in the intersection, enough memory ?
                if (data [i2] >= size_multiple_guard)
                {
                    //size_multiple_guard += MULTIPLE_GUARD_ALLOC_STEP;
                    const size_t old_size_multiple_guard = size_multiple_guard;
                    size_multiple_guard = data [i2] + 1;
                    multiple_guard = (_Bool*) REALLOC(multiple_guard, size_multiple_guard * sizeof (_Bool));
                    ASSERT_ALLOC(multiple_guard, "Cannot increase the memory for the multiple guard !",
                            size_multiple_guard * sizeof (_Bool));
                    memset(multiple_guard + old_size_multiple_guard, '\0', size_multiple_guard - old_size_multiple_guard);
                }

                if (multiple_guard [data [i2]] == false)
                {
                    intersection_result->data_struct.data [i][intersection_result->arrays_lengths [i]] = data [i2];
                    intersection_result->arrays_lengths [i] ++;
                    multiple_guard [data [i2]] = true;
                }
            }
        }
    }

    FREE_AND_SET_TO_NULL(multiple_guard);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Compare function for qsort()
 *
 * @param[in] a First value
 * @param[in] b Second value
 *
 * @return -1, if a < b; +1 if a > b; else 0
 */
static int
Compare_Function
(
        const void* const a,
        const void* const b
)
{
    const uint_fast32_t* const val_a = (uint_fast32_t*) a;
    const uint_fast32_t* const val_b = (uint_fast32_t*) b;

    if (*val_a < *val_b) { return -1; }
    if (*val_a > *val_b) { return +1; }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute binary search.
 *
 * ! This function excepts, that the data is already ascending sorted !
 *
 * Asserts:
 *      data != NULL
 *
 * @param[in] data Data in which is searched
 * @param[in] data_size Size of the data
 * @param[in] search_value Value to be searched
 *
 * @return true, if the value is in the dataset, else false
 */
static _Bool
Binary_Search
(
        const uint_fast32_t* const data,
        const size_t data_size,
        const uint_fast32_t search_value
)
{
    ASSERT_MSG(data != NULL, "data is NULL !");
    // ASSERT_MSG(data_size != 0, "data size is 0 !");
    if (data_size == 0) { return false; }

    _Bool result = false;

    // This values must be signed values. In some cases right can get the value -1 !
    int_fast32_t left = 0;
    int_fast32_t right = (int_fast32_t) (data_size - 1);

    while (left <= right)
    {
        // Here the possible negative value for the variable "right" is evaluated to -0 by integer division
        // Statement for this special case: 0 + ((-1 + 0) / 2) => 0 + (-0) => 0
        const int_fast32_t middle = left + ((right - left) / 2);
        if (data [middle] == search_value)
        {
            result = true;
            break;
        }
        else
        {
            if (data [middle] > search_value) { right = middle - 1; } else { left = middle + 1; }
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Implementation of the sorting algorithm "HeapSort".
 *
 * This implementation was derived from the example implementation on Wikipedia.
 * https://de.wikipedia.org/wiki/Heapsort#Implementierung
 *
 * Asserts:
 *      data != NULL
 *
 * @param[in] data Data which will be sorted (ascending)
 * @param[in] data_size Size of the data
 */
static void
Heapsort
(
        uint_fast32_t* const data,
        const size_t data_size
)
{
    ASSERT_MSG(data != NULL, "data is NULL !");
    // ASSERT_MSG(data_size != 0, "data size is 0 !");
    if (data_size == 0) { return; }

    uint_fast32_t value     = 0;
    uint_fast32_t parent    = 0;
    uint_fast32_t child     = 0;
    uint_fast32_t root      = data_size / 2;
    uint_fast32_t n         = data_size;

    while (1)
    {
        if (root > 0)
        {
            -- root;
            parent = root;
            value = data [root];
        }
        else
        {
            -- n;
            if (n > 0)
            {
                value = data [n];
                data [n] = data [0];
                parent = 0;
            }
            else { break; }
        }

        while ((child = (parent + 1) << 1) < n)
        {
            if (data [child - 1] > data [child])
            {
                -- child;
            }
            data [parent] = data [child];
            parent = child;
        }

        if (child == n)
        {
            -- child;
            if (data [child] >= value)
            {
                data [parent] = data [child];
                data [child] = value;
                continue;
            }
            child = parent;
        }
        else
        {
            if (data [parent] >= value)
            {
                data [parent] = value;
                continue;
            }
            child = (parent - 1) >> 1;
        }

        while (child != root)
        {
            parent = (child - 1) >> 1;
            if (data [parent] >= value) { break; }

            data [child] = data [parent];
            child = parent;
        }

        data [child]= value;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef MULTIPLE_GUARD_ALLOC_STEP
#undef MULTIPLE_GUARD_ALLOC_STEP
#endif /* MULTIPLE_GUARD_ALLOC_STEP */
