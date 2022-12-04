/**
 * @file Misc.h
 *
 * @brief Here are some macros defined, that cannot be classified for a specific translation unit. E.g.: The size
 * determination of a static defined array.
 *
 * And here are also helpful functions, that fits in no specific translation unit.
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



#include <time.h>   // time_t
#include <stdio.h>      // FILE*
#include <inttypes.h>   // int_fast64_t
#include "Error_Handling/_Generics.h"   // For type checking macros



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This macro determines the number of elements in an array.
 *
 * ! Attention !
 * This only works, if the number are objects in the array is already known during the compiling process ! In other cases
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
#define TO_STRING_HELPER(new_string)                                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, new_string);                   \
        next_free_byte += used_char;                                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER\" is already defined !"
#endif /* TO_STRING_HELPER */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This macro helps to create a C-String representation of a object (struct). Here with a format string.
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
 * wrong and unexpected results.)
 *
 * Therefore it is recommended to use this macro only with "raw" values and not with statements like "x + 2" to fully
 * avoid mistakes with side effects.
 */
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#else
#error "The macro \"MIN\" is already defined !"
#endif /* MIN */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Similar like MAX(x, y) but with a no pointer and a no floating point type check.
 */
#ifndef MAX_WITH_TYPE_CHECK
#define MAX_WITH_TYPE_CHECK(x, y) MAX(x, y);                                                                            \
    IS_NO_PTR(x)                                                                                                        \
    IS_NO_PTR(y)                                                                                                        \
    IS_NO_FLOAT(x)                                                                                                      \
    IS_NO_FLOAT(y)                                                                                                      \
    TYPE_EQUAL(x, y)
#else
#error "The macro \"MAX_WITH_TYPE_CHECK\" is already defined !"
#endif /* MAX_WITH_TYPE_CHECK */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Similar like MIN(x, y) but with a no pointer and a no floating point type check.
 */
#ifndef MIN_WITH_TYPE_CHECK
#define MIN_WITH_TYPE_CHECK(x, y) MIN(x, y);                                                                            \
    IS_NO_PTR(x)                                                                                                        \
    IS_NO_PTR(y)                                                                                                        \
    IS_NO_FLOAT(x)                                                                                                      \
    IS_NO_FLOAT(y)                                                                                                      \
    TYPE_EQUAL(x, y)
#else
#error "The macro \"MIN_WITH_TYPE_CHECK\" is already defined !"
#endif /* MIN_WITH_TYPE_CHECK */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Comparison macro to find the greater value.
 *
 * ! This is a macro, not a function !
 * It is important, that the "input parameter" have no side effects. (For example: MAX (x ++, ++ y) will determine
 * wrong and unexpected results.)
 *
 * Therefore it is recommended to use this macro only with "raw" values and not with statements like "x + 2" to fully
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
    }                                                                                                                   \
    IS_PTR(array)
#else
#error "The macro \"SET_POINTER_ARRAY_TO_NULL\" is already defined !"
#endif /* SET_POINTER_ARRAY_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Calculate the execution time with start and end information generated by clock() function.
 */
#ifndef DETERMINE_USED_TIME
#define DETERMINE_USED_TIME(start_time, end_time)                                                                       \
    (float)(end_time - start_time) / CLOCKS_PER_SEC;                                                                    \
    IS_TYPE(start_time, clock_t)                                                                                        \
    IS_TYPE(end_time, clock_t)
#else
#error "The macro \"DETERMINE_USED_TIME\" is already defined !"
#endif /* DETERMINE_USED_TIME */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief clock() call with a check of the return value.
 */
#ifndef CLOCK_WITH_RETURN_CHECK
#define CLOCK_WITH_RETURN_CHECK(time_value)                                                                             \
    time_value = clock();                                                                                               \
    if (time_value == (clock_t) -1)                                                                                     \
    {                                                                                                                   \
        ASSERT_MSG(time_value != (clock_t)(-1),                                                                         \
                "Time values are not available on this system ! Return value: (clock_t)(-1) !");                        \
    }                                                                                                                   \
	IS_TYPE(time_value, clock_t)
#else
#error "The macro \"CLOCK_WITH_RETURN_CHECK\" is already defined !"
#endif /* CLOCK_WITH_RETURN_CHECK */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief On windows systems a system command "PAUSE" is necessary, because the terminal window will close immediately
 * after the exit call. So it is not possible to read the error message.
 */
#ifndef EXIT
#ifdef _WIN32
#define EXIT(exit_value)                                                                                                \
    system("PAUSE");                                                                                                    \
    exit(exit_value);                                                                                                   \
    IS_INT(exit_value)
#else
#define EXIT(exit_value)                                                                                                \
    exit(exit_value);                                                                                                   \
    IS_INT(exit_value)
#endif /* _WIN32 */
#else
#error "The macro \"EXIT\" is already defined !"
#endif /* EXIT */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Count number of digits in a value.
 *
 * @param[in] value Value
 *
 * @return The number of digits in the given value
 */
extern size_t Count_Number_Of_Digits (const size_t value);

/**
 * @brief Determine percentage value. (float input parameter)
 *
 * Asserts:
 *      value != NaN
 *      value != -Inf
 *      value != +Inf
 *      one_hundred_percent != NaN
 *      one_hundred_percent != -Inf
 *      one_hundred_percent != +Inf
 *      one_hundred_percent != 0.0f (NOT IMPLEMENTED DUE FLOAT COMPARISONS !)
 *
 * @param[in] value Value
 * @param[in] one_hundred_percent Value that corresponds to 100%
 *
 * @return percentage value
 */
extern float Determine_Percent_f (const float value, const float one_hundred_percent);

/**
 * @brief Determine percentage value. (size_t input parameter)
 *
 * Asserts:
 *      one_hundred_percent != 0
 *
 * @param[in] value Value
 * @param[in] one_hundred_percent Value that corresponds to 100%
 *
 * @return percentage value
 */
extern float Determine_Percent (const size_t value, const size_t one_hundred_percent);

/**
 * @brief Determine the expected time left for the calculation.
 *
 * Asserts:
 *      second_value >= first_value
 *      end_value >= first_value
 *      end_value >= second_value
 *
 * @param[in] first_value First value
 * @param[in] second_value Second value
 * @param[in] end_value End value
 * @param[in] time_between_values Time, that was used in the interval between the first and second value
 *
 * @return Expected time left for the calculation
 */
extern float Determine_Time_Left (const size_t first_value, const size_t second_value, const size_t end_value,
        const time_t time_between_values);

/**
 * @brief Determine the expected average time left for the calculation.
 *
 * A average value is useful to avoid a strongly changing expected time.
 *
 * Asserts:
 *      second_value >= first_value
 *      end_value >= first_value
 *      end_value >= second_value
 *
 * @param[in] first_value First value
 * @param[in] second_value Second value
 * @param[in] end_value End value
 * @param[in] time_between_values Time, that was used in the interval between the first and second value
 *
 * @return Expected average time left for the calculation
 */
extern float Determine_Time_Left_Average (const size_t first_value, const size_t second_value, const size_t end_value,
        const time_t time_between_values);

/**
 * @brief Simple thing: When the input is NaN oder +/-Inf the function returns 0.0f.
 *
 * @param input test float value
 *
 * @return 0.0 if input is NaN or +/-Inf, otherwise the value itself
 */
extern float Replace_NaN_And_Inf_With_Zero (const float input);

/**
 * @brief Determine the size of a FILE* object.
 *
 * In normal cases this is the file size.
 *
 * Asserts:
 *      file != NULL
 *
 * @param[in] file FILE pointer to an opened file stream.
 *
 * @return The file size in bytes or -1, if an error occurs.
 */
extern int_fast64_t Determine_FILE_Size (FILE* file);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MISC_H */
