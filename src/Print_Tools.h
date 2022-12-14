/**
 * @file Print_Tools.h
 *
 * @brief Some helpful output tools.
 *
 * @date 13.03.2021
 * @author x86
 */

#ifndef PRINT_TOOLS_H
#define PRINT_TOOLS_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>      // printf
#include <inttypes.h>   // uint_fast32_t
#include <time.h>       // clock_t
#include "Error_Handling/_Generics.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print data with printf and flush the stdout cache.
 *
 * The effect is, that the output will be appear on the terminal directly.
 */
#ifndef PRINTF_FFLUSH
    #define PRINTF_FFLUSH(format_string, ...)                                                                           \
    printf ((format_string), __VA_ARGS__); fflush (stdout);                                                             \
    IS_IN_TYPE_LIST_4((format_string), char*, const char*, char* const, const char* const)
#else
    #error "The macro \"PRINTF_FFLUSH\" is already defined !"
#endif /* PRINTF_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_FFLUSH". But here with file streams.
 */
#ifndef FPRINTF_FFLUSH
    #define FPRINTF_FFLUSH(file, format_string, ...)                                                                    \
    if ((file) != stdout && (file) != stderr)                                                                           \
    {                                                                                                                   \
        const int return_value_fprintf = fprintf ((file), (format_string), __VA_ARGS__);                                \
        if (return_value_fprintf < 0)                                                                                   \
        {                                                                                                               \
            fprintf (stderr, "I/O error ! fprintf returned %d !", return_value_fprintf); fflush (stderr);               \
        }                                                                                                               \
        const int return_value_fflush = fflush ((file));                                                                \
        if (return_value_fflush == EOF)                                                                                 \
        {                                                                                                               \
            fprintf (stderr, "I/O error ! fflush returned EOF !"); fflush (stderr);                                     \
        }                                                                                                               \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        PRINTF_FFLUSH ((format_string), __VA_ARGS__);                                                                   \
    }                                                                                                                   \
    IS_INT((file))                                                                                                      \
    IS_IN_TYPE_LIST_4((format_string), char*, const char*, char* const, const char* const)
#else
    #error "The macro \"FPRINTF_FFLUSH\" is already defined !"
#endif /* FPRINTF_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_FFLUSH".
 *
 * It is necessary to create a second macro for the va_args. If the va_args are empty a compiler error would be occur
 * with the macro "PRINTF_FFLUSH".
 */
#ifndef PRINTF_NO_VA_ARGS_FFLUSH
    #define PRINTF_NO_VA_ARGS_FFLUSH(format_string)                                                                     \
    printf ((format_string)); fflush (stdout);                                                                          \
    IS_IN_TYPE_LIST_4((format_string), char*, const char*, char* const, const char* const)
#else
    #error "The macro \"PRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* PRINTF_NO_VA_ARGS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_NO_VA_ARGS_FFLUSH". But here with a file stream.
 */
#ifndef FPRINTF_NO_VA_ARGS_FFLUSH
    #define FPRINTF_FFLUSH_NO_VA_ARGS(file, format_string)                                                              \
    if ((file) != stdout && (file) != stderr)                                                                           \
    {                                                                                                                   \
        const int return_value_fprintf = fprintf ((file), (format_string));                                             \
        if (return_value_fprintf < 0)                                                                                   \
        {                                                                                                               \
            fprintf (stderr, "I/O error ! fprintf returned %d !", return_value_fprintf); fflush (stderr);               \
        }                                                                                                               \
        const int return_value_fflush = fflush ((file));                                                                \
        if (return_value_fflush == EOF)                                                                                 \
        {                                                                                                               \
            fprintf (stderr, "I/O error ! fflush returned EOF !"); fflush (stderr);                                     \
        }                                                                                                               \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        PRINTF_NO_VA_ARGS_FFLUSH ((format_string));                                                                     \
    }                                                                                                                   \
    IS_INT((file))                                                                                                      \
    IS_IN_TYPE_LIST_4((format_string), char*, const char*, char* const, const char* const)
#else
    #error "The macro \"FPRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* FPRINTF_NO_VA_ARGS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_FFLUSH". But here with a automatic newline at the end.
 */
#ifndef PUTS_FFLUSH
    #define PUTS_FFLUSH(string)                                                                                         \
    puts ((string)); fflush (stdout);                                                                                   \
    IS_IN_TYPE_LIST_4((string), char*, const char*, char* const, const char* const)
#else
    #error "The macro \"PUTS_FFLUSH\" is already defined !"
#endif /* PUTS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PUTS_FFLUSH". But here for a user defined stream.
 *
 * Important: fputs() adds NO newline at the end of the string ! puts() do this !
 */
#ifndef FPUTS_FFLUSH
    #define FPUTS_FFLUSH(file, string)                                                                                  \
    if ((file) != stdout && (file) != stderr)                                                                           \
    {                                                                                                                   \
        const int return_value_fputs = fputs ((file), (string));                                                        \
        if (return_value_fprintf == EOF)                                                                                \
        {                                                                                                               \
            fprintf (stderr, "I/O error ! fputs returned %d !", return_value_fputs); fflush (stderr);                   \
        }                                                                                                               \
        const int return_value_fflush = fflush ((file));                                                                \
        if (return_value_fflush == EOF)                                                                                 \
        {                                                                                                               \
            fprintf (stderr, "I/O error ! fflush returned EOF !"); fflush (stderr);                                     \
        }                                                                                                               \
    }                                                                                                                   \
    else                                                                                                                \
    {                                                                                                                   \
        PRINTF_NO_VA_ARGS_FFLUSH ((format_string));                                                                     \
    }                                                                                                                   \
    IS_INT((file))                                                                                                      \
    IS_IN_TYPE_LIST_4((string), char*, const char*, char* const, const char* const)
#else
    #error "The macro \"FPUTS_FFLUSH\" is already defined !"
#endif /* FPUTS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Clean a full terminal line.
 *
 * Approach: the line length of 96 is not exceeded.
 */
#ifndef CLEAN_LINE
    #define CLEAN_LINE()                                                                                                \
    putc ('\r'); PRINT_X_TIMES_SAME_CHAR(' ', 96) fflush (stdout);
#else
    #error "The macro \"CLEAN_LINE\" is already defined !"
#endif /* CLEAN_LINE */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print newlines to stdout with flush.
 */
#ifndef PRINT_NEWLINES
    #define PRINT_NEWLINES(number_of_newlines)                                                                          \
    if ((number_of_newlines) < 0)                                                                                       \
    {                                                                                                                   \
    	fprintf (stderr, "Negative number of newlines !\n"); fflush (stderr);                                           \
    }                                                                                                                   \
    for (unsigned long int print_newlines_counter = 0;                                                                  \
            print_newlines_counter < (unsigned long int) (number_of_newlines);                                          \
            ++ (number_of_newlines))                                                                                    \
    {                                                                                                                   \
        puts("");                                                                                                       \
    }                                                                                                                   \
    if ((number_of_newlines) > 0)                                                                                       \
    {                                                                                                                   \
    	fflush(stdout);                                                                                                 \
    }                                                                                                                   \
    IS_INT((number_of_newlines))
#else
    #error "The macro \"PRINT_NEWLINES\" is already defined !"
#endif /* PRINT_NEWLINES */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a newline to stdout with flush.
 */
#ifndef PRINT_NEWLINE
    #define PRINT_NEWLINE PUTS_FFLUSH("")
#else
    #error "The macro \"PRINT_NEWLINE\" is already defined !"
#endif /* PRINT_NEWLINE */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print x times the same char to stdout. (With flush at the end of the process)
 */
#ifndef PRINT_X_TIMES_SAME_CHAR
#define PRINT_X_TIMES_SAME_CHAR(character, times)                                                                       \
    if ((character) < 0)                                                                                                \
    {                                                                                                                   \
        fprintf (stderr, "Negative number of newlines !\n"); fflush (stderr);                                           \
    }                                                                                                                   \
    for (unsigned long int i = 0; i < (unsigned long int) (times); ++ i)                                                \
    {                                                                                                                   \
        putc(character, stdout);                                                                                        \
    }                                                                                                                   \
    if ((times) > 0)                                                                                                    \
    {                                                                                                                   \
    	fflush (stdout);                                                                                                \
    }                                                                                                                   \
    IS_INT((times))                                                                                                     \
    IS_TYPE((character), int)
#else
    #error "The macro \"PRINT_X_TIMES_SAME_CHAR\" is already defined !"
#endif /* PRINT_X_TIMES_SAME_CHAR */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a 2D-String array to stdout.
 *
 * This is helpful for debugging of text-based drawings.
 *
 * Asserts:
 *      drawing != NULL
 *      dim_1 > 0
 *      dim_2 > 0
 *
 * @param[in] drawing 2D-String array
 * @param[in] dim_1 Size of the first array dimension (Number of C-Strings)
 * @param[in] dim_2 Size of the second array dimension (Max number of char per C-String)
 */
extern void Print_2D_String_Array (const char* const restrict drawing [], const size_t dim_1, const size_t dim_2);

/**
 * @brief Print an array of uint_fast32_t variables to stdout.
 *
 * Asserts:
 *      array != NULL
 *
 * @param[in] array array with the data
 * @param[in] array_length length of the array
 */
extern void Print_uint_fast32_t_Array (const uint_fast32_t* const array, const size_t array_length);

/**
 * @brief Print process information with a user defined function, if new process output is to print.
 *
 * To avoid too many print operations the counter will be decreased, when a output operation was done.
 *
 * Asserts:
 *      print_function != NULL
 *
 * @param[in] print_step_size Minimum size of the counter to output the current process information
 * @param[in] counter_since_last_output Counter since the last process print
 * @param[in] actual_counter Actual process
 * @param[in] hundred_percent Value that represents a process of 100 % (In other words: the value, that will appear
 *      when the operation is done)
 * @param[in] with_carriage_return Print a carriage return at the end of the line
 *
 * @param[in] print_function This is the function, that will be called, when process information are to be printed
 * @param[in] optional_parameter Optional parameter for the optional_second_print_function function ptr
 * @param[in] optional_second_print_function An optional print function for additional work
 *
 * @return The new counter
 */
extern size_t Process_Printer (const size_t print_step_size, const size_t counter_since_last_output,
        const size_t actual_counter, const size_t hundred_percent, const _Bool with_carriage_return,
        void (*print_function)
        (
            const size_t print_step_size,
            const size_t actual_counter,
            const size_t hundred_percent,
            const clock_t interval_begin,
            const clock_t interval_end
        ),
        void* optional_parameter,
        void (*optional_second_print_function)
        (
                void* data
        )
);

/**
 * @brief Print a memory size in Byte, KByte and MByte.
 *
 * Asserts:
 *      byte_size != 0
 *
 * param[in] byte_size Value, that will be converted an printed
 */
extern void Print_Memory_Size_As_B_KB_MB (const size_t byte_size);

/**
 * @brief Print a given integer value with decimal dots. This is helpful to get a direct information in which "size
 * class" the value is.
 *
 * @param[in] value Value, that will be printed with decimal dots
 */
extern void Print_Value_With_Decimal_Points (const long int value);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PRINT_TOOLS_H */
