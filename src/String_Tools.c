/**
 * @file String_Tools.c
 *
 * @brief Here are several general tools for the handling with C-Strings.
 *
 * @date: 07.03.2022
 * @author: am1
 */

#include "String_Tools.h"
#include <string.h>
#include <ctype.h>
#include "Misc.h"



//---------------------------------------------------------------------------------------------------------------------

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
)
{
    strncpy (to_lower_string, orig_string, to_lower_string_size);

    for (size_t current_char = 0;
            (current_char < strlen (orig_string)) && (current_char < to_lower_string_size);
            ++ current_char)
    {
        // Convert all upper-case char to lower-case char. It is not necessary to check for upper-case char, because
        // tolower() do this check for us.
        // if (isupper (orig_string [current_char]) /* == true */)
            to_lower_string [current_char] = (char) tolower (orig_string [current_char]);
    }

    // Nullterminierung im Zielspeicher garantieren
    to_lower_string [strlen (orig_string)] = '\0';

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Compare two C-Strings case insensitive.
 *
 * There might be a functiom "strncasecmp()" on your system with the same functionality. But this is an GNU extention
 * and no potable C code.
 *
 * @param[in] string_1 First C-String
 * @param[in] string_1_length Length of the first C-String
 * @param[in] string_2 Second C-String
 * @param[in] string_2_length Length of the second C-String
 *
 * @return 0, if the C-String equals, else != 0
 */
extern int
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,
        const size_t string_1_length,
        const char* const restrict string_2,
        const size_t string_2_length
)
{
    int result = 0;

    if (string_1_length == string_2_length)
    {
        for (size_t i = 0;; ++ i)
        {
            // If a string end was found, stop the comparisons
            if (string_1 [i] == '\0')
            // Alternative: if (string_2 [i] == '\0')
            {
                break;
            }
            if (tolower(string_1 [i]) != tolower(string_2 [i]))
            {
                result = -1;
                break;
            }
        }
    }
    else
    {
        result = -1;
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    uint_fast8_t result = 0;

    for (size_t i = 0; i < string_length; ++ i)
    {
        if (c_string [i] == searched_char) { ++ result; }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    const char one_size_string [2] = { character, '\0' };

    for (size_t i = 0; i < times; ++ i)
    {
        strncat (str, one_size_string, COUNT_ARRAY_ELEMENTS(one_size_string));
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    _Bool result = true;

    for (size_t i = 0; i < length; ++ i)
    {
        if (str [i] != '\0')
        {
            result = false;
            break;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Append many C-String to another C-String.
 *
 * If there is not enough memory in the result C-String, the result is detached on the second last char !
 *
 * Asserts:
 *      destination != NULL
 *      destination_size > 1
 *      count > 0
 *      Jeder weitere C-String != NULL
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
)
{
	if (destination == NULL) 	{ return SIZE_MAX; }
	if (destination_size <= 1) 	{ return SIZE_MAX; }
	if (count <= 0)				{ return SIZE_MAX; }
	memset (destination, '\0', destination_size * sizeof (char));

	va_list valist;
	va_start(valist, count);

	size_t memory_left = destination_size - 1;

	for (int i = 0; i < count; ++ i)
	{
		char* next_string = va_arg(valist, char*);
		if (next_string == NULL) { break; }

		strncat (destination, next_string, memory_left);
		memory_left -= strlen (next_string);
	}

	va_end(valist);

	destination [destination_size - 1] = '\0';
	return destination_size - memory_left;
}

//---------------------------------------------------------------------------------------------------------------------
