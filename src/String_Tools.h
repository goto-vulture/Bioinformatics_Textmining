/**
 * @file String_Tools.h
 *
 * @brief Here are several general tools for the handling with C-Strings.
 *
 * @date: 07.03.2022
 * @author: am1
 */

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>     // size_t
#include <inttypes.h>   // uint_fast8_t
#include <stdbool.h>    // true, false
#include <stdarg.h>     // va_list



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Simple check, if a C-String has the length zero.
 */
#ifndef IS_STRING_LENGTH_ZERO
#define IS_STRING_LENGTH_ZERO(c_string) ((c_string) [0] == '\0')
#else
#error "The macro \"IS_STRING_LENGTH_ZERO\" is already defined !"
#endif /* IS_STRING_LENGTH_ZERO */

/**
 * @brief Simple check, if a C-String has at least one char.
 */
#ifndef IS_STRING_LENGTH_NOT_ZERO
#define IS_STRING_LENGTH_NOT_ZERO(c_string) ((c_string) [0] != '\0')
#else
#error "The macro \"IS_STRING_LENGTH_NOT_ZERO\" is already defined !"
#endif /* IS_STRING_LENGTH_NOT_ZERO */

/**
 * @brief Simple check, if a C-String has the length one.
 *
 * Sometimes it is useful to know whether the string is exact one char long.
 */
#ifndef IS_STRING_LENGTH_ONE
#define IS_STRING_LENGTH_ONE(c_string) ((c_string) [0] != '\0' && (c_string) [1] == '\0')
#else
#error "The macro \"IS_STRING_LENGTH_ONE\" is already defined !"
#endif /* IS_STRING_LENGTH_ONE */

/**
 * @brief Do a string length calculation of string literals at compile time.
 *
 * The idea of this macro is from the cJSON lib !
 */
#ifndef STATIC_STRLEN
#define STATIC_STRLEN(string_literal) (sizeof((string_literal)) - sizeof(""))
#else
#error "The macro \"STATIC_STRLEN\" is already defined !"
#endif /* STATIC_STRLEN */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Copy a C-String and convert all upper-case char to lower-case.
 *
 * Asserts / In this case normal if tests:
 *      orig_string != NULL
 *      to_lower_string != NULL
 *      to_lower_string_size > 0
 *
 * @param[in] orig_string Original C-String
 * @param[out] to_lower_string Memory for the result C-String
 * @param[in] to_lower_string_size Allocated length of the result C-String
 *
 * @return 0 or INT_MAX in error cases
 */
extern int
String_To_Lower
(
        const char* const restrict orig_string,
        char* const restrict to_lower_string,
        const size_t to_lower_string_size
);

/**
 * @brief Compare two C-Strings case insensitive.
 *
 * There might be a function "strncasecmp()" on your system with the same functionality. But this is an GNU extension
 * and no portable C code.
 *
 * Asserts / In this case normal if tests:
 *       string_1 != NULL
 *       string_1_length > 0
 *       string_2 != NULL
 *       string_2_length > 0
 *
 * @param[in] string_1 First C-String
 * @param[in] string_1_length Length of the first C-String
 * @param[in] string_2 Second C-String
 * @param[in] string_2_length Length of the second C-String
 *
 * @return 0, if the C-String equals, else != 0 (In error cases INT_MAX)
 */
extern int
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,
        const size_t string_1_length,
        const char* const restrict string_2,
        const size_t string_2_length
);

/**
 * @brief Determine how many specific char exists in a C-String.
 *
 * Asserts / In this case normal if tests:
 *      c_string != NULL
 *      string_length != 0
 *      searched_char != 0
 *
 * @param[in] c_string Input C-String
 * @param[in] string_length String length
 * @param[in] searched_char Char to be searched
 *
 * @return Counted number of char in the C-String or INT_MAX in error cases
 */
extern int
Count_Char_In_String
(
        const char* const c_string,
        const size_t string_length,
        const char searched_char
);

/**
 * @brief Append X times the same char to a C-String. This function expects, that in the C-String is enough memory for
 * this operation !
 *
 * Asserts / In this case normal if tests:
 *      str != NULL
 *      character != '\0'
 *      times > 0
 *
 * @param[in] str C-String, that will be extended
 * @param[in] character Char
 * @param[in] times How many times the char will be appended
 *
 * @return 0 or INT_MAX in error cases
 */
extern int
Append_X_Times_Char
(
        char* const str,
        const char character,
        const size_t times
);

/**
 * @brief Contain a C-String only null bytes ('\0') ?
 *
 * Asserts / In this case normal if tests:
 *      str != NULL
 *      length > 0
 *
 * @param[in] str Input C-String
 * @param[in] length C-String length
 *
 * @return true, if there only '\0' bytes, otherwise false (in error case: INT_MAX)
 */
extern int
Contain_String_Only_Null_Symbols
(
        const char* const str,
        const size_t length
);

/**
 * @brief Append many C-String to another C-String.
 *
 * If there is not enough memory in the result C-String, the result is detached on the second last char !
 *
 * Asserts / In this case normal if tests:
 *      destination != NULL
 *      destination_size > 1
 *      count > 0
 *      Every additionalC-String != NULL
 *
 * @param[in] destination Destination C-String
 * @param[in] destination_size Length of the destination C-String
 * @param[in] count Number of input C-Strings
 * @param[in] ... va_list C-Strings
 *
 * @return The number of appended char of INT_MAX in case of errors
 */
extern int
Multi_strncat
(
        char* const restrict destination,
        const size_t destination_size,
        const int count,
        ...
);

/**
 * @brief The function returns true, if all char of a string are printable.
 *
 * A char is printable, if it is not a control char.
 *
 * Asserts / Here normal if statements instead of ASSERT:
 *      str != NULL
 *      str_len != 0
 *
 * @param[in] str String, that will be checked
 * @param[in] str_len Length of the string
 *
 * @return True, if all char are printable. If not false (In error cases INT_MAX)
 */
extern int
Is_String_Printable
(
        const char* const str,
        const size_t str_len
);

/**
 * @brief The function returns true, if all char of a string are alphanumeric char.
 *
 * A char is a alphanumeric, if it a digit or a alphabetic char.
 *
 * Asserts / Here normal if statements instead of ASSERT:
 *      str != NULL
 *      str_len != 0
 *
 * @param[in] str String, that will be checked
 * @param[in] str_len Length of the string
 *
 * @return True, if all char are alphanumeric. If not false (In error cases INT_MAX)
 */
extern int
Contain_String_Only_Alnum_Char
(
        const char* const str,
        const size_t str_len
);

/**
 * @brief Check every char with the given function.
 *
 * In normal cases the check functions are for example isalpha() or isalnum().
 *
 * Asserts / In this case normal if tests:
 *      string != NULL
 *      str_length > 0
 *      check_func_ptr != NULL
 *
 * @param[in] string C string, that will be checked
 * @param[in] str_len Length of the C string
 * @param[in] check_func_ptr Check function
 *
 * @return true, if all char pass the check function, otherwise false (In error cases INT_MAX)
 */
extern int
Check_Every_Char_With_Function
(
        const char* const string,
        const size_t str_length,
        int (*check_func_ptr)(int)
);

/**
 * @brief Contain a the string a given char at least one times?
 *
 * The additional parameter pos_of_first_occurrence is optional and can be NULL !
 *
 * Asserts / In this case normal if checks:
 *      string != NULL
 *      str_length > 0
 *
 * @param[in] string Input C-String
 * @param[in] str_length Length of the given C-String
 * @param[in] search_char Search char
 * @param[out] pos_of_first_occurrence Position of the first occurrence (optional ! -> A NULL is not an invalid input!)
 *
 * @return True, if the given char was found. False otherwise (In error cases INT_MAX)
 */
extern int
Is_Char_In_String
(
        const char* const restrict string,
        const size_t str_length,
        const char search_char,
        size_t* const restrict pos_of_first_occurrence
);

/**
 * Is the given C-String null terminated ?
 *
 * The function simply checks, if in the given memory is a '\0' symbol. It does not matter, whether it the terminate
 * symbol is at the end or not.
 *
 * Asserts / In this case normal if checks:
 *      string != NULL
 *      str_length > 0
 *
 * @param[in] string Input C-String
 * @param[in] str_length Length of the given C-String
 *
 * @return True, if the given C-String has a termination symbol, otherwise false. (In error cases INT_MAX)
 */
extern int
Is_String_Null_Terminated
(
        const char* const restrict string,
        const size_t str_length
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STRING_TOOLS_H */
