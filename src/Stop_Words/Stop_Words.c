/**
 * @file Stop_Words.c
 *
 * @date 12.08.2022
 * @author: am4
 */

#include "Stop_Words.h"
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../String_Tools.h"
#include "../str2int.h"



const char* GLOBAL_eng_stop_words [] =
{
#include "Stop_Words_English.txt"
        , NULL // The NULL pointer marks the end of the C-String array
};
size_t GLOBAL_eng_stop_words_length = 0;


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
    ASSERT_MSG(c_string != NULL, "C string is NULL !");
    ASSERT_MSG(c_string_length > 0, "C string length is 0 !");
    ASSERT_MSG(language != NO_LANGUAGE, "No language selected !");

    _Bool result = false;
    const char** selected_stop_word_list = NULL;

    switch(language)
    {
    case ENG:
        selected_stop_word_list = GLOBAL_eng_stop_words;
        if (GLOBAL_eng_stop_words_length == 0)
        {
            while (*selected_stop_word_list != NULL)
            {
                ++ selected_stop_word_list;
                ++ GLOBAL_eng_stop_words_length;
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

    // Search the string in the stop word list
    for (size_t i = 0; i < GLOBAL_eng_stop_words_length; ++ i)
    {
        if (Compare_Strings_Case_Insensitive(c_string, c_string_length,
                selected_stop_word_list [i], strlen (selected_stop_word_list [i])) == 0)
        {
            result = true;
            break;
        }
    }
    // A simple number is also a stop word
    // To determine a number: try to run a str to int cast successfully
    long int conversion_result = 0;
    if (str2int(&conversion_result, c_string, 10) == STR2INT_SUCCESS)
    {
        result = true;
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
