/**
 * @file Two_Dim_C_String_Array.h
 *
 * @brief A simple struct with a two dimensional C-String arrays.
 *
 * @date 17.08.2022
 * @author am4
 */

#include "Two_Dim_C_String_Array.h"
#include <string.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Misc.h"



/**
 * @brief Are there new c strings necessary ?
 *
 * Asserts:
 *      object != NULL
 *      new_number_of_c_str > 0
 *
 * @param[in] object The Two_Dim_C_String_Array
 * @param[in] new_number_of_c_str The new number of c strings
 */
static void
New_C_String_Necessary
(
        struct Two_Dim_C_String_Array* const object,
        const size_t new_number_of_c_str
);

/**
 * @brief Need a selected c string more memory ?
 *
 * Asserts:
 *      object != NULL
 *      selected_c_str < object->number_of_c_str
 *      new_number_of_c_str > 0
 *
 * @param[in] object The Two_Dim_C_String_Array
 * @param[in] selected_c_str The selected c string
 * @param[in] new_number_of_c_str The new number of c strings
 */
static void
Longer_C_String_Necessary
(
        struct Two_Dim_C_String_Array* const object,
        const size_t selected_c_str,
        const size_t new_c_str_length
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Create a new Two_Dim_C_String_Array object.
 *
 * Asserts:
 *      number_of_c_str > 0
 *
 * @param[in] number_of_c_str Number of c strings, that will be allocated
 *
 * @return Address to the new dynamic Two_Dim_C_String_Array
 */
extern struct Two_Dim_C_String_Array*
Create_Two_Dim_C_String_Array
(
        const size_t number_of_c_str
)
{
    ASSERT_MSG(number_of_c_str > 0, "Number of c strings is 0 !");

    struct Two_Dim_C_String_Array* new_object =
            (struct Two_Dim_C_String_Array*) CALLOC(1, sizeof (struct Two_Dim_C_String_Array));
    ASSERT_ALLOC(new_object, "Cannot create a new Two_Dim_C_String_Array object !",
            sizeof (struct Two_Dim_C_String_Array));

    // Allocate the outer array
    new_object->data = (char**) CALLOC(C_STR_ALLOC_STEP_SIZE, sizeof (char*));
    ASSERT_ALLOC(new_object->data, "Cannot create the outer data array !", C_STR_ALLOC_STEP_SIZE * sizeof (char*));
    new_object->number_of_c_str = C_STR_ALLOC_STEP_SIZE;

    // Allocate an array to save the length of the c strings
    new_object->allocated_c_str_length = (size_t*) CALLOC(C_STR_ALLOC_STEP_SIZE, sizeof (size_t));
    ASSERT_ALLOC(new_object->allocated_c_str_length, "Cannot create an array with allocation information !",
            C_STR_ALLOC_STEP_SIZE * sizeof (size_t));

    // Allocate an array with the data, what is the next free char in each c string
    new_object->next_free_char_in_c_str = (uint_fast32_t*) CALLOC(C_STR_ALLOC_STEP_SIZE, sizeof (uint_fast32_t));
    ASSERT_ALLOC(new_object->next_free_char_in_c_str,
            "Cannot create an array with length information about the c string lengths !",
            C_STR_ALLOC_STEP_SIZE * sizeof (uint_fast32_t));

    // Allocate the inner array
    for (size_t i = 0; i < C_STR_ALLOC_STEP_SIZE; ++ i)
    {
        new_object->data [i] = (char*) CALLOC (C_STR_LENGTH_ALLOC_STEP_SIZE, sizeof (char));
        ASSERT_ALLOC(new_object->data [i], "Cannot create an object of the inner data array !",
                C_STR_LENGTH_ALLOC_STEP_SIZE * sizeof (char));
        new_object->allocated_c_str_length [i] = C_STR_LENGTH_ALLOC_STEP_SIZE;
        new_object->next_free_char_in_c_str [i] = 0;
    }

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Delete a dynamic allocated Two_Dim_C_String_Array object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Two_Dim_C_String_Array object
 */
extern void
Delete_Two_Dim_C_String_Array
(
        struct Two_Dim_C_String_Array* object
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");

    for (size_t i = 0; i < object->number_of_c_str; ++ i)
    {
        FREE_AND_SET_TO_NULL(object->data [i]);
    }
    FREE_AND_SET_TO_NULL(object->data);
    FREE_AND_SET_TO_NULL(object->allocated_c_str_length);
    FREE_AND_SET_TO_NULL(object->next_free_char_in_c_str);

    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Append data to a specific c string. It is allowed, that the selected c string can be empty.
 *
 * Asserts:
 *      object != NULL
 *      str_index < object->number_of_c_str
 *      append_data != NULL
 *      append_data_length > 0
 *
 * @param[in] object The Two_Dim_C_String_Array
 * @param[in] str_index Index of the selected string
 * @param[in] append_data Data, that will be appended
 * @param[in] append_data_length Length of the new data
 */
extern void
Append_Data_In_Two_Dim_C_String_Array
(
        struct Two_Dim_C_String_Array* const restrict object,
        const size_t str_index,
        const char* const restrict append_data,
        const size_t append_data_length
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");
    ASSERT_FMSG(str_index < object->number_of_c_str,
                "Selected c string is out of bounds ! Got: %zu; max valid: %zu !", str_index,
                object->number_of_c_str - 1);
    ASSERT_MSG(append_data != NULL, "Append data is NULL !");
    ASSERT_MSG(append_data_length > 0, "Append data length is 0 !");

    // Need the string more memory for the appending operation ?
    if (object->next_free_char_in_c_str [str_index] + append_data_length >=
            object->allocated_c_str_length [str_index])
    {
        Longer_C_String_Necessary(object, str_index,
                object->next_free_char_in_c_str [str_index] + append_data_length + C_STR_LENGTH_ALLOC_STEP_SIZE);
    }

    // Append the new data
    strncat (object->data [str_index], append_data, append_data_length);
    object->next_free_char_in_c_str [str_index] += append_data_length;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Append data to the current c string.
 *
 * The current c string is at index object->next_free_c_str, or - in the case that object->next_free_c_str is zero - the
 * current c string is at index object->next_free_c_str.
 *
 * Asserts:
 *      object != NULL
 *      append_data != NULL
 *      append_data_length > 0
 *
 * @param[in] object The Two_Dim_C_String_Array object
 * @param[in] append_data New data
 * @param[in] append_data_length New data length
 */
extern void
Append_Data_To_Current_String_In_Two_Dim_C_String_Array
(
        struct Two_Dim_C_String_Array* const restrict object,
        const char* const restrict append_data,
        const size_t append_data_length
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");
    ASSERT_MSG(append_data != NULL, "Append data is NULL !");
    ASSERT_MSG(append_data_length > 0, "Append data length is 0 !");

    // Delegate it to the standard append function
    Append_Data_In_Two_Dim_C_String_Array
    (
            object,
            (object->next_free_c_str == 0) ? 0 : object->next_free_c_str,
            append_data, append_data_length
    );

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Append a new c string. This means, that the next free c string will get the new data.
 *
 * Asserts:
 *      object != NULL
 *      new_str != NULL
 *      new_str_length > 0
 *
 * @param[in] object The Two_Dim_C_String_Array
 * @param[in] new_str C string, that will be appended at the next free c string
 * @param[in] new_str_length Length of the new c string
 */
extern void
Append_New_String_In_Two_Dim_C_String_Array
(
        struct Two_Dim_C_String_Array* const restrict object,
        const char* const restrict new_str,
        const size_t new_str_length
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");
    ASSERT_MSG (new_str != NULL, "New c string is NULL !");
    ASSERT_MSG (new_str_length > 0, "New c string length is NULL !");

    const uint_fast32_t next_free_c_str_index = object->next_free_c_str;

    // New c strings or more data for a c string necessary ?
    if (object->next_free_c_str + 1 > object->number_of_c_str)
    {
        New_C_String_Necessary (object, object->number_of_c_str + C_STR_ALLOC_STEP_SIZE);
    }
    if (new_str_length >= object->allocated_c_str_length [next_free_c_str_index])
    {
        Longer_C_String_Necessary(object, next_free_c_str_index, new_str_length + C_STR_LENGTH_ALLOC_STEP_SIZE);
    }

    // Copy data
    strncpy (object->data [next_free_c_str_index], new_str, new_str_length);
    // Guarantee a terminator byte at the end of the c string
    object->data [next_free_c_str_index][object->allocated_c_str_length [next_free_c_str_index] - 1] = '\0';

    // Update information
    object->next_free_char_in_c_str [next_free_c_str_index] = new_str_length;
    ++ object->next_free_c_str;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Show some attributes about the given Two_Dim_C_String_Array object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object The Two_Dim_C_String_Array
 */
extern void
Show_Attributes_From_Two_Dim_C_String_Array
(
        const struct Two_Dim_C_String_Array* const object
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");

    size_t full_object_size = sizeof (object) + (sizeof (object->number_of_c_str) * sizeof (size_t)) +
            (sizeof (object->number_of_c_str) * sizeof (uint_fast32_t));
    for (size_t i = 0; i < object->number_of_c_str; ++ i)
    {
        full_object_size += object->allocated_c_str_length [i] * sizeof (char);
    }
    size_t longest_allocated_c_str = 0;
    size_t longest_c_str = 0;
    for (size_t i = 0; i < object->number_of_c_str; ++ i)
    {
        if (object->allocated_c_str_length [i] > longest_allocated_c_str)
        {
            longest_allocated_c_str = object->allocated_c_str_length [i];
        }
        if (object->next_free_char_in_c_str [i] > longest_c_str)
        {
            longest_c_str = object->next_free_char_in_c_str [i];
        }
    }

    puts("");
    printf ("Full two dim C string array size: %zu B (%.3f KB | %.3f MB)\n", full_object_size,
            (double) full_object_size / 1024.0, (double) full_object_size / 1024.0 / 1024.0);
    printf ("Number of C strins:            %zu\n", object->number_of_c_str);
    printf ("Next free C string:            %zu\n", object->next_free_c_str);
    printf ("Longest allocated C string:    %zu\n", longest_allocated_c_str);
    printf ("Longest C string:              %zu\n", longest_c_str);
    fflush(stdout);

    return;
}

//=====================================================================================================================

/**
 * @brief Are there new c strings necessary ?
 *
 * Asserts:
 *      object != NULL
 *      new_number_of_c_str > 0
 *
 * @param[in] object The Two_Dim_C_String_Array
 * @param[in] new_number_of_c_str The new number of c strings
 */
static void
New_C_String_Necessary
(
        struct Two_Dim_C_String_Array* const object,
        const size_t new_number_of_c_str
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");
    ASSERT_MSG (new_number_of_c_str > 0, "New number of c strings is 0 !");

    // Are there enough c strings in the container ?
    if (new_number_of_c_str > object->number_of_c_str)
    {
        char** temp_ptr = (char**) REALLOC (object->data,
                (new_number_of_c_str) * sizeof (char*));
        ASSERT_ALLOC(temp_ptr, "Cannot increase the number of c strings in the container !",
                (new_number_of_c_str) * sizeof (char*));
        object->data = temp_ptr;
        SET_POINTER_ARRAY_TO_NULL(object->data + object->number_of_c_str, new_number_of_c_str - object->number_of_c_str);

        size_t* temp_ptr_2 = (size_t*) REALLOC(object->allocated_c_str_length,
                new_number_of_c_str * sizeof (size_t));
        ASSERT_ALLOC(temp_ptr_2, "Cannot increase the number of administration data !",
                new_number_of_c_str * sizeof (size_t));
        object->allocated_c_str_length = temp_ptr_2;
        memset (object->allocated_c_str_length + object->number_of_c_str, '\0',
                (new_number_of_c_str - object->number_of_c_str) * sizeof (size_t));

        uint_fast32_t* temp_ptr_3 = (size_t*) REALLOC(object->next_free_char_in_c_str,
                new_number_of_c_str * sizeof (uint_fast32_t));
        ASSERT_ALLOC(temp_ptr_3, "Cannot increase the number of administration data !",
                new_number_of_c_str * sizeof (uint_fast32_t));
        object->next_free_char_in_c_str = temp_ptr_3;
        memset (object->next_free_char_in_c_str + object->number_of_c_str, '\0',
                (new_number_of_c_str - object->number_of_c_str) * sizeof (uint_fast32_t));

        // Allocate new memory for the new c strings
        for (size_t i = object->number_of_c_str; i < new_number_of_c_str; ++ i)
        {
            object->data [i] = (char*) CALLOC (C_STR_LENGTH_ALLOC_STEP_SIZE, sizeof (char));
            ASSERT_ALLOC(object->data [i], "Cannot create an object of the inner data array !",
                    C_STR_LENGTH_ALLOC_STEP_SIZE * sizeof (char));
            object->allocated_c_str_length [i] = C_STR_LENGTH_ALLOC_STEP_SIZE;
            object->next_free_char_in_c_str [i] = 0;
        }

        object->number_of_c_str = new_number_of_c_str;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Need a selected c string more memory ?
 *
 * Asserts:
 *      object != NULL
 *      selected_c_str < object->number_of_c_str
 *      new_number_of_c_str > 0
 *
 * @param[in] object The Two_Dim_C_String_Array
 * @param[in] selected_c_str The selected c string
 * @param[in] new_number_of_c_str The new number of c strings
 */
static void
Longer_C_String_Necessary
(
        struct Two_Dim_C_String_Array* const object,
        const size_t selected_c_str,
        const size_t new_c_str_length
)
{
    ASSERT_MSG (object != NULL, "Object is NULL !");
    ASSERT_FMSG(selected_c_str < object->number_of_c_str,
            "Selected c string is out of bounds ! Got: %zu; max valid: %zu !", selected_c_str,
            object->number_of_c_str - 1);
    ASSERT_MSG(new_c_str_length > 0, "New c string length is 0 !");

    // New memory necessary ?
    if (new_c_str_length >= object->allocated_c_str_length [selected_c_str])
    {
        char* temp_ptr = (char*) REALLOC(object->data [selected_c_str], (new_c_str_length + 1) * sizeof (char));
        ASSERT_ALLOC(temp_ptr, "Cannot increase the size of a c string !", (new_c_str_length + 1) * sizeof (char));
        object->data [selected_c_str] = temp_ptr;
        memset(object->data [selected_c_str] + object->allocated_c_str_length [selected_c_str],
                '\0', (new_c_str_length + 1) - object->allocated_c_str_length [selected_c_str]);

        object->allocated_c_str_length [selected_c_str] = new_c_str_length + 1;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
