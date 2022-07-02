/**
 * @file Misc.h
 *
 * @brief Here are some macros defined, that cannot be classified for a specific translation unit. E.g.: The size
 * determination of a static defined array.
 *
 * @date 14.03.2021
 * @author x86 / Gyps
 */

#ifndef MISC_H
#define MISC_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This macro determines the number of elements in an array.
 *
 * ! Attention !
 * This only works, if the number ob objects in the array is already known during the compiling process ! In other cases
 * (e.g.: memory allocated with malloc()) wrong result will be determined !
 */
#ifndef COUNT_ARRAY_ELEMENTS
#define COUNT_ARRAY_ELEMENTS(array) ((sizeof (array)) / (sizeof (array [0])))
#else
#error "The macro \"COUNT_ARRAY_ELEMENTS\" is already defined !"
#endif /* COUNT_ARRAY_ELEMENTS */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This macro helps to create a C-String representation of a object (struct).
 *
 * => For the usage of a format string: See the macro "TO_STRING_HELPER_VA_ARGS"
 */
#ifndef TO_STRING_HELPER
#define TO_STRING_HELPER(new_string)                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, new_string);   \
        next_free_byte += used_char;                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER\" is already defined !"
#endif /* TO_STRING_HELPER */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This macro helps to create a C-String representation of a object (struct). Here with a format string.
 * Diesmal mit einem Formatstring.
 */
#ifndef TO_STRING_HELPER_VA_ARGS
#define TO_STRING_HELPER_VA_ARGS(format_string, ...)                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, format_string, __VA_ARGS__);   \
        next_free_byte += used_char;                                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER_VA_ARGS\" is already defined !"
#endif /* TO_STRING_HELPER_VA_ARGS */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparison macro to find the lower value.
 *
 * ! This is a macro, not a function !
 * It is important, that the "input parameter" have no side effects. (For example: MIN (x ++, ++ y) will determine
 * wrong and unexpected results.
 *
 * Therefore it is recommended to use this macro only with "raw" valus and not with statements like "x + 2" to fully
 * avoid mistakes with side effects.
 */
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#else
#error "The macro \"MIN\" is already defined !"
#endif /* MIN */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparison macro to find the greater value.
 *
 * ! This is a macro, not a function !
 * It is important, that the "input parameter" have no side effects. (For example: MAX (x ++, ++ y) will determine
 * wrong and unexpected results.
 *
 * Therefore it is recommended to use this macro only with "raw" valus and not with statements like "x + 2" to fully
 * avoid mistakes with side effects.
 */
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#else
#error "The macro \"MAX\" is already defined !"
#endif /* MAX */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Initialize an array (or memory range) of pointers with NULL pointers.
 */
#ifndef SET_POINTER_ARRAY_TO_NULL
#define SET_POINTER_ARRAY_TO_NULL(array, number_of_elements)                                                            \
    for (size_t array_index = 0; array_index < (number_of_elements); array_index ++)                                    \
    {                                                                                                                   \
        (array) [array_index] = NULL;                                                                                   \
    }
#else
#error "The macro \"SET_POINTER_ARRAY_TO_NULL\" is already defined !"
#endif /* SET_POINTER_ARRAY_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MISC_H */
