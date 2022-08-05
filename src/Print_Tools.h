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



#include <stdio.h>



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print data with printf and flush the stdout cache.
 *
 * The effect is, that the output will be appear on the terminal directly.
 */
#ifndef PRINTF_FFLUSH
    #define PRINTF_FFLUSH(format_string, ...)                   \
    printf (format_string, __VA_ARGS__); fflush (stdout);
#else
    #error "The macro \"PRINTF_FFLUSH\" is already defined !"
#endif /* PRINTF_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_FFLUSH". But here with file streams.
 */
#ifndef FPRINTF_FFLUSH
    #define FPRINTF_FFLUSH(file, format_string, ...)             \
    fprintf (file, format_string, __VA_ARGS__); fflush (file);
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
    #define PRINTF_NO_VA_ARGS_FFLUSH(format_string)             \
    printf (format_string); fflush (stdout);
#else
    #error "The macro \"PRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* PRINTF_NO_VA_ARGS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_NO_VA_ARGS_FFLUSH". But here with a file stream.
 */
#ifndef FPRINTF_NO_VA_ARGS_FFLUSH
    #define FPRINTF_FFLUSH_NO_VA_ARGS(file, format_string)      \
    fprintf (file, format_string); fflush (file);
#else
    #error "The macro \"FPRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* FPRINTF_NO_VA_ARGS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Same as the macro "PRINTF_FFLUSH". But here with a automatíc newline at the end.
 */
#ifndef PUTS_FFLUSH
    #define PUTS_FFLUSH(string)                                 \
    puts (string); fflush (stdout);
#else
    #error "The macro \"PUTS_FFLUSH\" is already defined !"
#endif /* PUTS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Clean a full terminal line.
 *
 * Approach: the line length of 96 is not exceeded.
 */
#ifndef CLEAN_LINE
    #define CLEAN_LINE()                                                                                        \
    printf ("\r"); PRINT_X_TIMES_SAME_CHAR(' ', 96) fflush (stdout);
#else
    #error "The macro \"CLEAN_LINE\" is already defined !"
#endif /* CLEAN_LINE */

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
#define PRINT_X_TIMES_SAME_CHAR(character, times) \
    for (size_t i = 0; i < (times); ++ i) { printf ("%c", (character)); } fflush (stdout);
#else
    #error "The macro \"PRINT_X_TIMES_SAME_CHAR\" is already defined !"
#endif /* PRINT_X_TIMES_SAME_CHAR */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a 2D-String array to stdout. *
 *
 * This is helpful for debugging of text-based drawings.
 *
 * Asserts:
 *      drawing != NULL
 *      dim_1 > 0
 *      dim_2 > 0
 *
 * @param[in] drawing 2D-String array
 * @param[in] dim_1 Groesse der First array dimension (Number of C-Strings)
 * @param[in] dim_2 Groesse der Second array dimension (Max numer of char per C-String)
 */
extern void Print_2D_String_Array (const char* const restrict drawing [], const size_t dim_1, const size_t dim_2);

/**
 * @brief Count number of digits in a value.
 *
 * @param[in] value Value
 *
 * @return The number of digits in the given value
 */
extern size_t Count_Number_Of_Digits (const size_t value);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PRINT_TOOLS_H */
