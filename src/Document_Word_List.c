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
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Misc.h"
#include "Print_Tools.h"
#include "Intersection_Approaches.h"



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
Create_Document_Word_List
(
        const size_t number_of_arrays,
        const size_t max_array_length
)
{
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* new_object = (struct Document_Word_List*) CALLOC(1, sizeof (struct Document_Word_List));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List));

    // Outer dimension
    new_object->data = (uint_fast32_t**) CALLOC(number_of_arrays, sizeof (uint_fast32_t*));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (uint_fast32_t*) * number_of_arrays);

    // Inner dimension
    for (uint_fast32_t i = 0; i < number_of_arrays; ++ i)
    {
        new_object->data [i] = (uint_fast32_t*) CALLOC(max_array_length, sizeof (uint_fast32_t));
        ASSERT_ALLOC(new_object->data [i], "Cannot create new Document_Word_List !",
                sizeof (uint_fast32_t) * max_array_length);
    }

    // Length list
    new_object->arrays_lengths = (size_t*) CALLOC(number_of_arrays, sizeof (size_t));
    ASSERT_ALLOC(new_object, "Cannot create new Document_Word_List !", sizeof (size_t) * number_of_arrays);

    new_object->max_array_length = max_array_length;
    new_object->number_of_arrays = number_of_arrays;
    new_object->next_free_array = 0;

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
Delete_Document_Word_List
(
        struct Document_Word_List* object
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    // Inner dimension
    for (uint_fast32_t i = 0; i < object->number_of_arrays; ++ i) { FREE_AND_SET_TO_NULL(object->data [i]); }

    // Outer dimension
    FREE_AND_SET_TO_NULL(object->data);

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
Append_Data_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const size_t data_length
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");
    ASSERT_MSG(new_data != NULL, "New data is NULL !");
    ASSERT_MSG(data_length != 0, "New data length is 0 !");

    ASSERT_FMSG(data_length <= object->max_array_length, "New data is too large ! Value %zu; max. valid: %zu",
            data_length, object->max_array_length);
    ASSERT_FMSG(object->number_of_arrays > (size_t) object->next_free_array, "All arrays are in use ! (%zu arrays)",
            object->number_of_arrays);

    // Copy the new data
    memcpy (object->data [object->next_free_array], new_data, sizeof (uint_fast32_t) * data_length);
    object->arrays_lengths [object->next_free_array] = data_length;
    object->next_free_array ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    puts ("> Data <");
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        printf ("%2zu: { ", (i + 1));
        for (size_t i2 = 0; i2 < object->arrays_lengths [i]; ++ i2)
        {
            printf ("%2" PRIuFAST32, object->data [i][i2]);

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
)
{
    ASSERT_MSG(object != NULL, "Object is NULL !");

    Show_Data_From_Document_Word_List(object);

    const int formatter_int = (int) MAX(Count_Number_Of_Digits(object->number_of_arrays),
            Count_Number_Of_Digits(object->max_array_length));

    puts ("> Attributes <");
    printf ("Intersection data: %s\n", (object->intersection_data /* == true */) ? "YES" : "NO");
    printf ("Number of arrays:  %*zu\n", formatter_int, object->number_of_arrays);
    printf ("Max. array length: %*zu\n", formatter_int, object->max_array_length);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
        intersection_result = Intersection_Approach_2_Nested_Loops(object, data, data_length);
        break;
    case INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH:
        intersection_result = Intersection_Approach_QSort_And_Binary_Search(object, data, data_length);
        break;
    case INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH:
        intersection_result = Intersection_Approach_HeapSort_And_Binary_Search(object, data, data_length);
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
Is_Data_In_Document_Word_List
(
        const struct Document_Word_List* const object
)
{
    _Bool result = false;

    for (uint_fast32_t i = 0; i < object->next_free_array; ++ i)
    {
        if (object->arrays_lengths [i] > 0)
        {
            result = true;
            break;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
