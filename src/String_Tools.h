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



#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>



/**
 * @brief Copy a C-String and convert all upper-case char to lower-case.
 *
 * @param[in] orig_string Original C-String
 * @param[out] to_lower_string Memory for the result C-String
 * @param[in] to_lower_string_size Allocated length of the result C-String
 */
extern void
String_To_Lower
(
        const char* const restrict orig_string,
        char* const restrict to_lower_string,
        const size_t to_lower_string_size
);

/**
 * @brief Compare two C-Strings case insensitive.
 *
 * There might be a functiom "strncasecmp()" on your system with the same functionality. But this is an GNU extention
 * and no potable C code.
 *
 * Asserts:
 *      strlen (string_1) == strlen (string_2)
 *
 * @param[in] string_1 First C-String
 * @param[in] string_2 Second C-String
 *
 * @return 0, if the C-String equals, else != 0
 */
extern int
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,
        const char* const restrict string_2
);

/**
 * @brief Determine how many specific char exists in a C-String.
 *
 * The function cannot indicate any types of errors. For example: an input NULL pointer will be cause a segfault !
 *
 * @param[in] c_string Input C-String
 * @param[in] string_length String length
 * @param[in] searched_char Char to be searched
 *
 * @return Counted number of char in the C-String
 */
extern uint_fast8_t
Count_Char_In_String
(
        const char* const restrict c_string,
        const size_t string_length,
        const char searched_char
);

/**
 * @brief Append X times the same char to a C-String. This function expects, that in the C-String is enough memory for
 * this operation !
 *
 * The function cannot indicate any types of errors. For example: an input NULL pointer will be cause a segfault !
 *
 * @param[in] str C-String, that will be extended
 * @param[in] character Char
 * @param[in] times How many times the char will be appended
 */
extern void
Append_X_Times_Char
(
        char* const str,
        const char character,
        const size_t times
);

/**
 * @brief Contain a C-String only null bytes ('\0') ?
 *
 * The function cannot indicate any types of errors. For example: an input NULL pointer will be cause a segfault !
 *
 * @param[in] str Input C-String
 * @param[in] length C-String length
 *
 * @return true, if there only '\0' bytes, otherwise false
 */
extern _Bool
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
 * Asserts:
 * 		destination != NULL
 * 		destination_size > 1
 * 		count > 0
 * 		Jeder weitere C-String != NULL
 *
 * @param[in] destination Destination C-String
 * @param[in] destination_size Length of the destination C-String
 * @param[in] count Number of input C-Strings
 * @param[in] ... va_list C-Strings
 *
 * @return The number of appended char of SIZE_MAX in case of errors
 */
extern size_t
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
