/**
 * @file Stop_Words.c
 *
 * @date 12.08.2022
 * @author: am4
 */

#include "Stop_Words.h"
#include <string.h>
#include <ctype.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../String_Tools.h"
#include "../str2int.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Misc.h"



const char* GLOBAL_eng_stop_words [] =
{
#include "Stop_Words_English.txt"
        , NULL // The NULL pointer marks the end of the C-String array
};



/**
 * @brief Is the given C-String a Latin numeral?
 *
 * This information is useful, because in normal cases a Latin numeral is a stop word.
 *
 * @param[in] c_string          C-String, that will be checked
 * @param[in] c_string_length   Length of the C-String
 *
 * @return true, if the C-String a Latin numeral, false otherwise
 */
static _Bool Is_String_A_Latin_Numeral
(
        const char* const c_string,
        const size_t c_string_length
);
//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine if the given C-String is in the selected stop word list.
 *
 * @param[in] c_string C-String
 * @param[in] c_string_length Length of the given C-String
 * @param[in] language Selected language
 *
 * @return true if the stop word list contains the C-String, otherwise false
 */
extern _Bool Is_Word_In_Stop_Word_List
(
        const char* const c_string,
        const size_t c_string_length,
        const enum Stop_Word_Language language
)
{
    static size_t eng_stop_word_lengths [1000];
    static size_t count_eng_stop_words = 0;

    ASSERT_MSG(c_string != NULL, "C string is NULL !");
    ASSERT_MSG(c_string_length > 0, "C string length is 0 !");
    ASSERT_MSG(language != NO_LANGUAGE, "No language selected !");

    _Bool result = false;

    // Only used for the fist run to initialize the data
    const char** selected_stop_word_list = NULL;
    size_t current_stop_word_index = 0;

    switch(language)
    {
    case ENG:
        selected_stop_word_list = GLOBAL_eng_stop_words;
        if (count_eng_stop_words == 0)
        {
            memset (eng_stop_word_lengths, '\0', sizeof (eng_stop_word_lengths));

            while (*selected_stop_word_list != NULL)
            {
                // Save the lengths of the stop words to avoid massive strlen calls
                if (current_stop_word_index < COUNT_ARRAY_ELEMENTS(eng_stop_word_lengths))
                {
                    eng_stop_word_lengths [current_stop_word_index] = strlen (*selected_stop_word_list);
                    ++ current_stop_word_index;
                }

                ++ selected_stop_word_list;
                ++ count_eng_stop_words;
            }
            selected_stop_word_list = GLOBAL_eng_stop_words;
        }
        break;
        // This case statement is not necessary, because the assert at the begin of the function already did the check
        // Some compilers create a [-Wswitch-enum] warning, if not all enum values are used in a switch case statement
    case NO_LANGUAGE:
        ASSERT_MSG(false, "No language selected !");
        break;

    default:
        ASSERT_MSG(false, "switch case default path executed !");
    }

    // If a token starts with a char, that is no alphabetic char, it cannot be a valid token -> So it will be in every
    // case interpreted as stop word
    // If the token only has the length 1, then it is also interpreted as stop word.
    if (! isalpha(c_string [0]) || c_string [1] == '\0')
    {
        return true;
    }
    // A simple number is also a stop word
    // To determine a number: try to run a str to int cast successfully
    long int conversion_result_int = 0;
    if (str2int(&conversion_result_int, c_string, 10) == STR2INT_SUCCESS)
    {
        return true;
    }
    // Try to run a str to double cast successfully
    double conversion_result_double = 0.0;
    if (str2double(&conversion_result_double, c_string) == STR2DOUBLE_SUCCESS)
    {
        return true;
    }
    // Is the number a Latin numeral ?
    if (Is_String_A_Latin_Numeral(c_string, c_string_length))
    {
        return true;
    }

    // With this construction you have the guarantee, that this value will be at least on the current stack frame.
    // Maybe also in a register ...
    register const size_t eng_stop_words_length = count_eng_stop_words;
    // Search the string in the stop word list
    for (register size_t i = 0; i < eng_stop_words_length; ++ i)
    {
        if (Compare_Strings_Case_Insensitive(c_string, c_string_length,
                selected_stop_word_list [i],
                (i < COUNT_ARRAY_ELEMENTS(eng_stop_word_lengths)) ? eng_stop_word_lengths [i] : strlen (selected_stop_word_list [i]))
                == 0)
        {
            return true;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Is the given C-String a Latin numeral?
 *
 * This information is useful, because in normal cases a Latin numeral is a stop word.
 *
 * @param[in] c_string          C-String, that will be checked
 * @param[in] c_string_length   Length of the C-String
 *
 * @return true, if the C-String a Latin numeral, false otherwise
 */
static _Bool Is_String_A_Latin_Numeral
(
        const char* const c_string,
        const size_t c_string_length
)
{
    _Bool result = true;

    for (size_t i = 0; i < c_string_length; ++ i)
    {
        switch(c_string [i])
        {
        case 'I':
        case 'V':
        case 'X':
        case 'L':
        case 'C':
        case 'D':
        case 'M':
            continue;
        default:
            result = false;
            break;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
