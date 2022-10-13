/**
 * @file Document_Word_List.c
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

#include "Document_Word_List.h"

#include <string.h>
#include <limits.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Misc.h"
#include "Print_Tools.h"
#include "Intersection_Approaches.h"



/**
 * @brief Allocation step size for the inner data arrays. (For the real data; not for management objects)
 */
#ifndef INT_ALLOCATION_STEP_SIZE
#define INT_ALLOCATION_STEP_SIZE 5
#else
#error "The macro \"INT_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* INT_ALLOCATION_STEP_SIZE */

/**
 * @brief Check, whether the macro value are valid.
 */
#if __STDC_VERSION__ >= 201112L
_Static_assert(INT_ALLOCATION_STEP_SIZE > 0, "The marco \"INT_ALLOCATION_STEP_SIZE\" is zero !");
#endif /* __STDC_VERSION__ */

/**
 * @brief Create a new Document_Word_List and initialize the main structure.
 *
 * Additional operations (e.g. data for a intersection result) will be done in the specific creation function.
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
static struct Document_Word_List*
Create_Main_Object_Structure
(
        const size_t number_of_arrays,
        const size_t max_array_length
);

/**
 * @brief Increase the size of a data array.
 *
 * increase_number_of_objects is NOT the sum of the array objects ! It is the number of new objects ! This sounds
 * trivial with the name "increase_number_of_objects". But it should be 100% clear for everyone.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *      increase_number_of_objects > 0
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 * @param[in] increase_number_of_objects Number of new objects
 */
static void Increase_Data_Array_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index,
        const size_t increase_number_of_objects
);

/**
 * @brief Increase the size of a data array with the INT_ALLOCATION_STEP_SIZE macro.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 */
static inline void Increase_Data_Array_Size_Allocation_Step_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index
);

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    // Avoid a unused warning
    (void) Increase_Data_Array_Size_Allocation_Step_Size;
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = Create_Main_Object_Structure (number_of_arrays, max_array_length);

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    // Avoid a unused warning
    (void) Increase_Data_Array_Size_Allocation_Step_Size;
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = Create_Main_Object_Structure (number_of_arrays, max_array_length);

    // Outer dimension for both char_offsets
    new_object->data_struct.char_offsets_1 = (CHAR_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (CHAR_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.char_offsets_1, "Cannot create new Document_Word_List !",
            sizeof (CHAR_OFFSET_TYPE*) * number_of_arrays);
    new_object->malloc_calloc_calls ++;
    new_object->data_struct.char_offsets_2 = (CHAR_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (CHAR_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.char_offsets_2, "Cannot create new Document_Word_List !",
            sizeof (CHAR_OFFSET_TYPE*) * number_of_arrays);
    new_object->malloc_calloc_calls ++;

    // Outer dimension for both sentence offsets
    new_object->data_struct.sentence_offsets_1 =
            (SENTENCE_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (SENTENCE_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.sentence_offsets_1, "Cannot create new Document_Word_List !",
            sizeof (SENTENCE_OFFSET_TYPE*) * number_of_arrays);
    new_object->malloc_calloc_calls ++;
    new_object->data_struct.sentence_offsets_2 =
            (SENTENCE_OFFSET_TYPE**) CALLOC(number_of_arrays, sizeof (SENTENCE_OFFSET_TYPE*));
    ASSERT_ALLOC(new_object->data_struct.sentence_offsets_2, "Cannot create new Document_Word_List !",
            sizeof (SENTENCE_OFFSET_TYPE*) * number_of_arrays);
    new_object->malloc_calloc_calls ++;

    // Inner dimension for char_offsets and sentence offsets (1 and 2)
    for (uint_fast32_t i = 0; i < number_of_arrays; ++ i)
    {
        new_object->data_struct.char_offsets_1 [i] = (CHAR_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.char_offsets_1 [i], "Cannot create new Document_Word_List !",
                sizeof (CHAR_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);
        new_object->malloc_calloc_calls ++;
        new_object->data_struct.char_offsets_2 [i] = (CHAR_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.char_offsets_2 [i], "Cannot create new Document_Word_List !",
                sizeof (CHAR_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);
        new_object->malloc_calloc_calls ++;

        new_object->data_struct.sentence_offsets_1 [i] = (SENTENCE_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.sentence_offsets_1 [i], "Cannot create new Document_Word_List !",
                sizeof (SENTENCE_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);
        new_object->malloc_calloc_calls ++;
        new_object->data_struct.sentence_offsets_2 [i] = (SENTENCE_OFFSET_TYPE*) CALLOC(INT_ALLOCATION_STEP_SIZE,
                sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(new_object->data_struct.sentence_offsets_2 [i], "Cannot create new Document_Word_List !",
                sizeof (SENTENCE_OFFSET_TYPE) * INT_ALLOCATION_STEP_SIZE);
        new_object->malloc_calloc_calls ++;
    }

    // Show, that this object was created for intersection results
    new_object->intersection_data = true;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    // Inner dimension
    for (uint_fast32_t i = 0; i < object->number_of_arrays; ++ i)
    {
        FREE_AND_SET_TO_NULL(object->data_struct.data [i]);
    }
    if (object->intersection_data)
    {
        for (uint_fast32_t i = 0; i < object->number_of_arrays; ++ i)
        {
            FREE_AND_SET_TO_NULL(object->data_struct.char_offsets_1 [i]);
            FREE_AND_SET_TO_NULL(object->data_struct.char_offsets_2 [i]);
            FREE_AND_SET_TO_NULL(object->data_struct.sentence_offsets_1 [i]);
            FREE_AND_SET_TO_NULL(object->data_struct.sentence_offsets_2 [i]);
        }
    }

    // Outer dimension
    FREE_AND_SET_TO_NULL(object->data_struct.data);
    if (object->intersection_data)
    {
        FREE_AND_SET_TO_NULL(object->data_struct.char_offsets_1);
        FREE_AND_SET_TO_NULL(object->data_struct.char_offsets_2);
        FREE_AND_SET_TO_NULL(object->data_struct.sentence_offsets_1);
        FREE_AND_SET_TO_NULL(object->data_struct.sentence_offsets_2);
    }

    FREE_AND_SET_TO_NULL(object->allocated_array_size)
    FREE_AND_SET_TO_NULL(object->arrays_lengths);
    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_MSG(new_data != NULL, "New data is NULL !");
    ASSERT_MSG(data_length != 0, "New data length is 0 !");

    //ASSERT_FMSG(data_length <= object->max_array_length, "New data is too large ! Value %zu; max. valid: %zu",
    //        data_length, object->max_array_length);
    ASSERT_FMSG(object->number_of_arrays > (size_t) object->next_free_array, "All arrays are in use ! (%zu arrays)",
            object->number_of_arrays);

    // Increase data, if necessary
    if (data_length > object->allocated_array_size [object->next_free_array])
    {
        Increase_Data_Array_Size(object, object->next_free_array,
                data_length - object->allocated_array_size [object->next_free_array]);
    }

    // Copy the new data
    memcpy (object->data_struct.data [object->next_free_array], new_data, sizeof (uint_fast32_t) * data_length);
    object->arrays_lengths [object->next_free_array] = data_length;
    object->next_free_array ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
 * @param[in] new_offsets_1 First offset array
 * @param[in] new_offsets_2 Second offset array
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendDataWithOffsets
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const CHAR_OFFSET_TYPE* const new_offsets_1,
        const CHAR_OFFSET_TYPE* const new_offsets_2,
        const size_t data_length
)
{
    DocumentWordList_AppendData(object, new_data, data_length);

    // Because the last function alters the array length value, it is necessary to undo this
    const uint_fast32_t next_free_array = object->next_free_array - 1;
    if (new_offsets_1 != NULL)
    {
        memcpy (object->data_struct.char_offsets_1 [next_free_array], new_offsets_1, sizeof (CHAR_OFFSET_TYPE) * data_length);
    }
    if (new_offsets_2 != NULL)
    {
        memcpy (object->data_struct.char_offsets_2 [next_free_array], new_offsets_2, sizeof (CHAR_OFFSET_TYPE) * data_length);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Add data with up to two char and sentence offset arrays to a Document_Word_List.
 *
 * Asserts:
 *      object != NULL
 *      new_data != NULL
 *      data_length > 0
 *
 * @param[in] object Document_Word_List
 * @param[in] new_data New data
 * @param[in] new_char_offsets_1 First char offset array
 * @param[in] new_char_offsets_2 Second char offset array
 * @param[in] new_sentence_offsets_1 First sentence offset array
 * @param[in] new_sentence_offsets_2 Second sentence offset array
 * @param[in] data_length Length of the new data
 */
extern void
DocumentWordList_AppendDataWithTwoTypeOffsets
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const CHAR_OFFSET_TYPE* const new_char_offsets_1,
        const CHAR_OFFSET_TYPE* const new_char_offsets_2,
        const SENTENCE_OFFSET_TYPE* const new_sentence_offsets_1,
        const SENTENCE_OFFSET_TYPE* const new_sentence_offsets_2,
        const size_t data_length
)
{
    DocumentWordList_AppendDataWithOffsets(object, new_data, new_char_offsets_1, new_char_offsets_2, data_length);

    if (new_sentence_offsets_1 != NULL)
    {
        memcpy (object->data_struct.sentence_offsets_1 [object->next_free_array - 1], new_sentence_offsets_1,
                sizeof (SENTENCE_OFFSET_TYPE) * data_length);
    }
    if (new_sentence_offsets_2 != NULL)
    {
        memcpy (object->data_struct.sentence_offsets_2 [object->next_free_array - 1], new_sentence_offsets_2,
                sizeof (SENTENCE_OFFSET_TYPE) * data_length);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    // The function redirects the appending. =>  The new value is a dataset with one element
    DocumentWordList_AppendData(object, &new_value, 1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Put one value to a Document_Word_List.
 *
 * The new value will be appended to the next free data array. Instead of Append_One_Value_To_Document_Word_List, the
 * new value will NOT be interpreted as new data set !
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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    const uint_fast32_t next_free_array = object->next_free_array;

    // Is enough memory available ?
    if (object->allocated_array_size [next_free_array] <= object->arrays_lengths [next_free_array])
    {
        Increase_Data_Array_Size_Allocation_Step_Size(object, next_free_array);
    }
    object->data_struct.data [next_free_array][object->arrays_lengths [next_free_array]] = new_value;
    object->arrays_lengths [next_free_array] ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Put one value with offsets to a Document_Word_List.
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
 * @param[in] new_offset_1 First new offset value
 * @param[in] new_offset_2 Second new offset value
 */
extern void
Put_One_Value_And_Offets_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t new_value,
        const CHAR_OFFSET_TYPE new_offset_1,
        const CHAR_OFFSET_TYPE new_offset_2
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    Put_One_Value_To_Document_Word_List(object, new_value);
    const uint_fast32_t next_free_array = object->next_free_array;

    // Because the last function alters the array length value, it is necessary to undo this
    object->arrays_lengths [next_free_array] --;
    object->data_struct.char_offsets_1 [next_free_array][object->arrays_lengths [next_free_array]] = new_offset_1;
    object->data_struct.char_offsets_2 [next_free_array][object->arrays_lengths [next_free_array]] = new_offset_2;
    object->arrays_lengths [next_free_array] ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    puts ("> Data <");
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        printf ("%2zu: { ", (i + 1));
        for (size_t i2 = 0; i2 < object->arrays_lengths [i]; ++ i2)
        {
            printf ("%2" PRIuFAST32, object->data_struct.data [i][i2]);

            if ((i2 + 1) < object->arrays_lengths [i])
            {
                printf (", ");
            }
        }
        puts(" }");
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine the full memory usage in byte.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] container Document_Word_List object
 *
 * @return Size of the full object in bytes
 */
extern size_t
DocumentWordList_GetAllocatedMemSize
(
        const struct Document_Word_List* const object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    size_t result = sizeof (size_t) * object->number_of_arrays;
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        result += object->allocated_array_size [i] * sizeof (uint_fast32_t);
    }
    result += sizeof (struct Document_Word_List);
    result += object->number_of_arrays * sizeof (uint_fast32_t*);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    const int formatter_int = (int) MAX(Count_Number_Of_Digits(object->number_of_arrays),
                Count_Number_Of_Digits(object->max_array_length));

    puts("");
    printf("Full document word list container size: ");
    Print_Memory_Size_As_B_KB_MB(DocumentWordList_GetAllocatedMemSize(object));

    puts ("> Attributes <");
    printf ("Intersection data:     %s\n", (object->intersection_data /* == true */) ? "YES" : "NO");
    printf ("Number of arrays:      %*zu\n", formatter_int, object->number_of_arrays);
    printf ("Max. array length:     %*zu\n", formatter_int, object->max_array_length);
    printf ("Malloc / calloc calls: %*zu\n", formatter_int, object->malloc_calloc_calls);
    printf ("Realloc calls:         %*zu\n", formatter_int, object->realloc_calls);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    DocumentWordList_ShowData(object);
    DocumentWordList_ShowAttributes(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersection with a Document_Word_List and a data set.
 *
 * You can change the intersection mode with the enum parameter Intersection_Mode. Actual following modes are
 * available
 * - 2 nested loops
 * - QSort with binary search(data and char_offsets (1 and 2))
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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_MSG(data != NULL, "data is NULL !");
    ASSERT_MSG(data_length != 0, "data length is 0 !");
    // ASSERT_MSG(mode == INTERSECTION_MODE_2_NESTED_LOOPS, "Invalid intersection mode !");
    ASSERT_FMSG(data_length <= object->max_array_length, "data is too large ! Value %zu; max. valid: %zu",
            data_length, object->max_array_length)

    // Execute the proper function for the selected mode
    struct Document_Word_List* intersection_result = NULL;
    switch (mode)
    {
    case INTERSECTION_MODE_2_NESTED_LOOPS:
        intersection_result = IntersectionApproach_TwoNestedLoops(object, data, data_length);
        break;
    case INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH:
        intersection_result = IntersectionApproach_QSortAndBinarySearch(object, data, data_length);
        break;
    case INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH:
        intersection_result = IntersectionApproach_HeapSortAndBinarySearch(object, data, data_length);
        break;
    default:
        ASSERT_MSG(false, "Default path executed !");
    }
    ASSERT_MSG(intersection_result != NULL, "Intersection result is NULL !");

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    _Bool result = false;

    // For the test is one exception: When the object only contains one array or when the object uses the first array
    if (object->next_free_array == 0 && object->number_of_arrays > 0)
    {
        if (object->arrays_lengths [0] > 0)
        {
            result = true;
        }
    }
    else
    {
        for (uint_fast32_t i = 0; i < object->next_free_array; ++ i)
        {
            if (object->arrays_lengths [i] > 0)
            {
                result = true;
                break;
            }
        }
    }

    return result;
}

//=====================================================================================================================

/**
 * @brief Create a new Document_Word_List and initialize the main structure.
 *
 * Additional operations (e.g. data for a intersection result) will be done in the specific creation function.
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
static struct Document_Word_List*
Create_Main_Object_Structure
(
        const size_t number_of_arrays,
        const size_t max_array_length
)
{
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = (struct Document_Word_List*) CALLOC(1, sizeof (struct Document_Word_List));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List));
    new_object->malloc_calloc_calls ++;

    // Outer dimension
    new_object->data_struct.data = (uint_fast32_t**) CALLOC(number_of_arrays, sizeof (uint_fast32_t*));
    ASSERT_ALLOC(new_object->data_struct.data, "Cannot create new Document_Word_List !",
            sizeof (uint_fast32_t*) * number_of_arrays);
    new_object->malloc_calloc_calls ++;

    // New management data
    new_object->allocated_array_size = (size_t*) CALLOC(number_of_arrays, sizeof (size_t));
    ASSERT_ALLOC(new_object->data_struct.data, "Cannot create new Document_Word_List !",
                sizeof (size_t) * number_of_arrays);
    new_object->malloc_calloc_calls ++;

    // Inner dimension
    for (uint_fast32_t i = 0; i < number_of_arrays; ++ i)
    {
        new_object->data_struct.data [i] = (uint_fast32_t*) CALLOC(INT_ALLOCATION_STEP_SIZE, sizeof (uint_fast32_t));
        ASSERT_ALLOC(new_object->data_struct.data [i], "Cannot create new Document_Word_List !",
                sizeof (uint_fast32_t) * INT_ALLOCATION_STEP_SIZE);
        new_object->malloc_calloc_calls ++;

        new_object->allocated_array_size [i] = INT_ALLOCATION_STEP_SIZE;
    }

    // Length list
    new_object->arrays_lengths = (size_t*) CALLOC(number_of_arrays, sizeof (size_t));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (size_t) * number_of_arrays);
    new_object->malloc_calloc_calls ++;

    new_object->max_array_length = INT_ALLOCATION_STEP_SIZE;
    new_object->number_of_arrays = number_of_arrays;
    new_object->next_free_array = 0;

    new_object->data_struct.char_offsets_1 = NULL;
    new_object->data_struct.char_offsets_2 = NULL;
    new_object->data_struct.sentence_offsets_1 = NULL;
    new_object->data_struct.sentence_offsets_2 = NULL;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Increase the size of a data array.
 *
 * increase_number_of_objects is NOT the sum of the array objects ! It is the number of new objects ! This sounds
 * trivial with the name "increase_number_of_objects". But it should be 100% clear for everyone.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *      increase_number_of_objects > 0
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 * @param[in] increase_number_of_objects Number of new objects
 */
static void Increase_Data_Array_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index,
        const size_t increase_number_of_objects
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_FMSG(data_array_index < object->number_of_arrays, "Data array index is invald ! Got: %zu; max valid: %zu !",
            data_array_index, object->number_of_arrays - 1);
    ASSERT_MSG(increase_number_of_objects > 0, "Number of increase objects ins 0 !");

    ++ object->realloc_calls;

    size_t* tmp_ptr = (size_t*) REALLOC(object->data_struct.data [data_array_index],
            (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (uint_fast32_t));
    ASSERT_ALLOC(tmp_ptr, "Cannot increase the data array size !",
            (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (uint_fast32_t))
    object->data_struct.data [data_array_index] = tmp_ptr;
    memset (&(object->data_struct.data [data_array_index][object->allocated_array_size [data_array_index]]), '\0',
            increase_number_of_objects * sizeof (uint_fast32_t));

    // If the object is a intersection data, then the offset arrays also need a resize operation
    if (object->intersection_data)
    {
        CHAR_OFFSET_TYPE* tmp_ptr_offset_1 = (CHAR_OFFSET_TYPE*) REALLOC(object->data_struct.char_offsets_1 [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_offset_1, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (CHAR_OFFSET_TYPE))
        CHAR_OFFSET_TYPE* tmp_ptr_offset_2 = (CHAR_OFFSET_TYPE*) REALLOC(object->data_struct.char_offsets_2 [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_offset_2, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (CHAR_OFFSET_TYPE))
        object->data_struct.char_offsets_1 [data_array_index] = tmp_ptr_offset_1;
        object->data_struct.char_offsets_2 [data_array_index] = tmp_ptr_offset_2;

        SENTENCE_OFFSET_TYPE* tmp_ptr_sentence_offset_1 = (SENTENCE_OFFSET_TYPE*) REALLOC(object->data_struct.sentence_offsets_1 [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_sentence_offset_1, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (SENTENCE_OFFSET_TYPE))
        SENTENCE_OFFSET_TYPE* tmp_ptr_sentence_offset_2 = (SENTENCE_OFFSET_TYPE*) REALLOC(object->data_struct.sentence_offsets_2 [data_array_index],
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(tmp_ptr_sentence_offset_2, "Cannot increase the data array size !",
                (object->allocated_array_size [data_array_index] + increase_number_of_objects) * sizeof (SENTENCE_OFFSET_TYPE))
        object->data_struct.sentence_offsets_1 [data_array_index] = tmp_ptr_sentence_offset_1;
        object->data_struct.sentence_offsets_2 [data_array_index] = tmp_ptr_sentence_offset_2;

        // Init the new memory (No zero, because a zero is a valid offset !)
        for (size_t i = object->allocated_array_size [data_array_index];
                i < (object->allocated_array_size [data_array_index] + increase_number_of_objects); ++ i)
        {
            object->data_struct.char_offsets_1 [data_array_index][i] = USHRT_MAX;
            object->data_struct.char_offsets_2 [data_array_index][i] = USHRT_MAX;
            object->data_struct.sentence_offsets_1 [data_array_index][i] = UCHAR_MAX;
            object->data_struct.sentence_offsets_2 [data_array_index][i] = UCHAR_MAX;
        }

        object->realloc_calls += 4;
    }

    object->allocated_array_size [data_array_index] += increase_number_of_objects;

    // If the new allocated array size bigger than the saved max allocated size
    if (object->allocated_array_size [data_array_index] > object->max_array_length)
    {
        object->max_array_length = object->allocated_array_size [data_array_index];
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Increase the size of a data array with the INT_ALLOCATION_STEP_SIZE macro.
 *
 * Asserts:
 *      object != NULL
 *      data_array_index < object->number_of_arrays
 *
 * @param[in] object The Document_Word_List
 * @param[in] data_array_index Index of the data array
 */
static inline void Increase_Data_Array_Size_Allocation_Step_Size
(
        struct Document_Word_List* const object,
        const size_t data_array_index
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_FMSG(data_array_index < object->number_of_arrays, "Data array index is invald ! Got: %zu; max valid: %zu !",
            data_array_index, object->number_of_arrays - 1);

    Increase_Data_Array_Size(object, data_array_index, INT_ALLOCATION_STEP_SIZE);

    return;
}

//---------------------------------------------------------------------------------------------------------------------



#ifdef INT_ALLOCATION_STEP_SIZE
#undef INT_ALLOCATION_STEP_SIZE
#endif /* INT_ALLOCATION_STEP_SIZE */
