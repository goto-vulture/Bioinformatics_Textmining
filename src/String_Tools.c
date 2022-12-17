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
)
{
    if (orig_string == NULL)        { return INT_MAX; }
    if (to_lower_string == NULL)    { return INT_MAX; }
    if (to_lower_string_size == 0)  { return INT_MAX; }

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

    // Guarantee null terminated c string
    to_lower_string [strlen (orig_string)] = '\0';

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------

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
 * @return 0, if the C-String equals, else != 0 (In error cases INT_MAX).
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
    if (string_1 == NULL)       { return INT_MAX; }
    if (string_1_length == 0)   { return INT_MAX; }
    if (string_2 == NULL)       { return INT_MAX; }
    if (string_2_length == 0)   { return INT_MAX; }

    int result = 0;

    if (string_1_length == string_2_length)
    {
       for (size_t i = 0; i < string_1_length; ++ i)
       {
           // If a string end was found, stop the comparisons
           if (string_1 [i] == '\0')
           // Alternative: if (string_2 [i] == '\0')
           {
               break;
           }

           // This simple test is enough, because tolower() returns the input parameter unchanged, if this is not an
           // alpha char
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
)
{
    if (c_string == NULL)   { return INT_MAX; }
    if (string_length == 0) { return INT_MAX; }
    if (searched_char == 0) { return INT_MAX; }

    int result = 0;

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
)
{
    if (str == NULL)        { return INT_MAX; }
    if (character != '\0')  { return INT_MAX; }
    if (times == 0)         { return INT_MAX; }

    const char one_size_string [2] = { character, '\0' };

    for (size_t i = 0; i < times; ++ i)
    {
        strncat (str, one_size_string, COUNT_ARRAY_ELEMENTS(one_size_string));
    }

    return 0;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    if (str == NULL)    { return INT_MAX; }
    if (length == 0)    { return INT_MAX; }

    int result = true;

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
)
{
    if (destination == NULL)    { return INT_MAX; }
    if (destination_size == 0)  { return INT_MAX; }
    if (count <= 0)             { return INT_MAX; }
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
    return (int) (destination_size - memory_left);
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    if (string == NULL)         { return INT_MAX; }
    if (str_length == 0)        { return INT_MAX; }
    if (check_func_ptr == NULL) { return INT_MAX; }

    int return_value = true;

    for (size_t i = 0; i < str_length; ++ i)
    {
        if (! (*check_func_ptr)(string [i]))
        {
            return_value = false;
            break;
        }
    }

    return return_value;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief The function returns true, if all char of a string are printable.
 *
 * A char is printable, if it is not a control char.
 *
 * @param[in] str String, that will be checked
 * @param[in] str_len Length of the string
 *
 * @return True, if all char are printable. If not false
 */
extern _Bool
Is_String_Printable
(
        const char* const str,
        const size_t str_len
)
{
    _Bool result = true;

    for (size_t i = 0; i < str_len; ++ i)
    {
        if (! isprint(str [i]))
        {
            result = false;
            break;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief The function returns true, if all char of a string are alphanumeric char.
 *
 * A char is a alphanumeric, if it a digit or a alphabetic char.
 *
 * @param[in] str String, that will be checked
 * @param[in] str_len Length of the string
 *
 * @return True, if all char are alphanumeric. If not false
 */
extern _Bool
Contain_String_Only_Alnum_Char
(
        const char* const str,
        const size_t str_len
)
{
    _Bool result = true;

    for (size_t i = 0; i < str_len; ++ i)
    {
        if (! isalnum(str [i]))
        {
            result = false;
            break;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
 * @return True, if the given char was found. False otherwise
 */
extern _Bool
Is_Char_In_String
(
        const char* const restrict string,
        const size_t str_length,
        const char search_char,
        size_t* const restrict pos_of_first_occurrence
)
{
    _Bool result = false;

    if (string == NULL)     { return false; }
    if (str_length == 0)    { return false; }

    for (register size_t i = 0; i < str_length; ++ i)
    {
        if (string [i] == search_char)
        {
            result = true;
            if (pos_of_first_occurrence != NULL)
            {
                *pos_of_first_occurrence = i;
            }
            break;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
 * @return True, if the given C-String has a termination symbol, otherwise false.
 */
extern _Bool
Is_String_Null_Terminated
(
        const char* const restrict string,
        const size_t str_length
)
{
    return Is_Char_In_String(string, str_length, '\0', NULL);
}

//---------------------------------------------------------------------------------------------------------------------
