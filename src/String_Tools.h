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

#ifndef IS_STRING_LENGTH_ZERO
#define IS_STRING_LENGTH_ZERO(c_string) ((c_string) [0] == '\0') ///< Simple check, if a C-String has the length zero
#else
#error "The macro \"IS_STRING_LENGTH_ZERO\" is already defined !"
#endif /* IS_STRING_LENGTH_ZERO */

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



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STRING_TOOLS_H */
