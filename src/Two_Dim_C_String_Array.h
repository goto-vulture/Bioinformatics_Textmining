/**
 * @file Two_Dim_C_String_Array.h
 *
 * @brief A simple struct with a two dimensional C-String arrays.
 *
 * @date 17.08.2022
 * @author am4
 */

#ifndef TWO_DIM_C_STRING_ARRAY_H
#define TWO_DIM_C_STRING_ARRAY_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>
#include <inttypes.h>
#include "Error_Handling/_Generics.h"



/**
 * @brief Allocation steps for the outer dimension; the number of c strings.
 */
#ifndef C_STR_ALLOC_STEP_SIZE
#define C_STR_ALLOC_STEP_SIZE 5
#else
#error "The macro \"C_STR_ALLOC_STEP_SIZE\" is already defined !"
#endif /* C_STR_ALLOC_STEP_SIZE */

/**
 * @brief Allocation steps for the c strings itself; the inner dimension.
 */
#ifndef C_STR_LENGTH_ALLOC_STEP_SIZE
#define C_STR_LENGTH_ALLOC_STEP_SIZE 100
#else
#error "The macro \"C_STR_LENGTH_ALLOC_STEP_SIZE\" is already defined !"
#endif /* C_STR_LENGTH_ALLOC_STEP_SIZE */

/**
 * @brief Check, whether the macro values are valid.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(C_STR_ALLOC_STEP_SIZE > 0, "The marco \"C_STR_ALLOC_STEP_SIZE\" is zero !");
_Static_assert(C_STR_LENGTH_ALLOC_STEP_SIZE > 0, "The marco \"C_STR_LENGTH_ALLOC_STEP_SIZE\" is zero !");

IS_TYPE(C_STR_ALLOC_STEP_SIZE, int)
IS_TYPE(C_STR_LENGTH_ALLOC_STEP_SIZE, int)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//=====================================================================================================================

/**
 * @brief This is a simple container for c strings.
 *
 * For the JSON export such a object is more than useful.
 */
struct Two_Dim_C_String_Array
{
    char** data;                                ///< The c strings
    size_t number_of_c_str;                     ///< Number of allocated c strings

    uint_fast32_t* next_free_char_in_c_str;     ///< Next free char in all c strings
    uint_fast32_t next_free_c_str;              ///< Next free c string in the container

    size_t* allocated_c_str_length;             ///< Allocated c string sizes
};

//=====================================================================================================================

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
TwoDimCStrArray_CreateObject
(
        const size_t number_of_c_str
);

/**
 * @brief Delete a dynamic allocated Two_Dim_C_String_Array object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Two_Dim_C_String_Array object
 */
extern void
TwoDimCStrArray_DeleteObject
(
        struct Two_Dim_C_String_Array* object
);

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
TwoDimCStrArray_AppendDataToSpecificString
(
        struct Two_Dim_C_String_Array* const restrict object,
        const size_t str_index,
        const char* const restrict append_data,
        const size_t append_data_length
);

/**
 * @brief Append data to the newest c string.
 *
 * The newest c string is at index object->next_free_c_str. It is equal to the meaning "the current c string".
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
TwoDimCStrArray_AppendDataToNewestString
(
        struct Two_Dim_C_String_Array* const restrict object,
        const char* const restrict append_data,
        const size_t append_data_length
);

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
TwoDimCStrArray_AppendNewString
(
        struct Two_Dim_C_String_Array* const restrict object,
        const char* const restrict new_str,
        const size_t new_str_length
);

/**
 * @brief Show some attributes about the given Two_Dim_C_String_Array object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object The Two_Dim_C_String_Array
 */
extern void
TwoDimCStrArray_ShowAttributes
(
        const struct Two_Dim_C_String_Array* const object
);

/**
 * @brief Print all strings in the container.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object The Two_Dim_C_String_Array
 */
extern void
TwoDimCStrArray_PrintAllStrings
(
        const struct Two_Dim_C_String_Array* const object
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TWO_DIM_C_STRING_ARRAY_H */
