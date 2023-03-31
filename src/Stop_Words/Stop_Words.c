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
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/_Generics.h"
#include "../String_Tools.h"
#include "../str2int.h"
#include "../Misc.h"



/**
 * @brief How many arrays contains the hash table, that reduces the string comparison calls ?
 */
#ifndef HASH_TABLE_NUMBER_OF_ARRAYS
#define HASH_TABLE_NUMBER_OF_ARRAYS 50
#else
#error "The macro \"HASH_TABLE_NUMBER_OF_ARRAYS\" is already defined !"
#endif /* HASH_TABLE_NUMBER_OF_ARRAYS */

#ifndef HASH_TABLE_ARRAY_LENGTH
#define HASH_TABLE_ARRAY_LENGTH 1000    ///< How long are the arrays in the hash table ?
#else
#error "The macro \"HASH_TABLE_ARRAY_LENGTH\" is already defined !"
#endif /* HASH_TABLE_ARRAY_LENGTH */

#ifndef STOP_WORD_LIST_LENGTH
#define STOP_WORD_LIST_LENGTH 1000      ///< Length of the stop word list buffer
#else
#error "The macro \"STOP_WORD_LIST_LENGTH\" is already defined !"
#endif /* STOP_WORD_LIST_LENGTH */

/**
 * @brief Check, whether the macro values are valid.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(HASH_TABLE_NUMBER_OF_ARRAYS > 1, "The marco \"HASH_TABLE_NUMBER_OF_ARRAYS\" needs to be larger than 1 !");
_Static_assert(HASH_TABLE_ARRAY_LENGTH > 1, "The marco \"HASH_TABLE_ARRAY_LENGTH\" needs to be larger than 1 !");
_Static_assert(STOP_WORD_LIST_LENGTH > 1, "The marco \"STOP_WORD_LIST_LENGTH\" needs to be larger than 1 !");

IS_TYPE(HASH_TABLE_NUMBER_OF_ARRAYS, int)
IS_TYPE(HASH_TABLE_ARRAY_LENGTH, int)
IS_TYPE(STOP_WORD_LIST_LENGTH, int)
#endif /* #defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */



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

/**
 * @brief A very very simple hash function.
 *
 * It is not fair to call this mechanism "Hash", because it is too simple. But for this cases it is enough. It will be
 * used for the hash table. This table reduces the number of Compare_Strings_Case_Insensitive() calls, that determine
 * whether the string is a stop word or not.
 *
 * @param[in] c_string          C-String, that will be hashed
 * @param[in] c_string_length   Length of the C-String
 *
 * @return The hash value
 */
static inline size_t Pseudo_Hash_Function
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
    static size_t eng_stop_word_lengths [STOP_WORD_LIST_LENGTH];
    static size_t hash_table_stop_words [HASH_TABLE_NUMBER_OF_ARRAYS][HASH_TABLE_ARRAY_LENGTH];
    static size_t hash_table_next_free_element [HASH_TABLE_NUMBER_OF_ARRAYS];
    static size_t count_eng_stop_words = 0;

    ASSERT_MSG(c_string != NULL, "C string is NULL !");
    ASSERT_MSG(c_string_length > 0, "C string length is 0 !");
    ASSERT_MSG(language != NO_LANGUAGE, "No language selected !");

    _Bool result = false;
    size_t pseudo_hash = 0;

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
            for (size_t i = 0; i < HASH_TABLE_NUMBER_OF_ARRAYS; ++ i)
            {
                memset (hash_table_stop_words [i], '\0', sizeof (hash_table_stop_words [i]));
            }
            memset (hash_table_next_free_element, '\0', sizeof (hash_table_next_free_element));

            while (*selected_stop_word_list != NULL)
            {
                // Save the lengths of the stop words to avoid massive strlen calls
                if (current_stop_word_index < STOP_WORD_LIST_LENGTH)
                {
                    eng_stop_word_lengths [current_stop_word_index] = strlen (*selected_stop_word_list);

                    pseudo_hash = Pseudo_Hash_Function(*selected_stop_word_list, eng_stop_word_lengths [current_stop_word_index]);

                    // Fill the hash table
                    if (hash_table_next_free_element [pseudo_hash] < HASH_TABLE_ARRAY_LENGTH)
                    {
                        hash_table_stop_words [pseudo_hash][hash_table_next_free_element [pseudo_hash]] = current_stop_word_index;
                        hash_table_next_free_element [pseudo_hash] ++;
                    }

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

    if (eng_stop_words_length > HASH_TABLE_ARRAY_LENGTH)
    {
        // Search the string in the stop word list
        for (register size_t i = 0; i < eng_stop_words_length; ++ i)
        {
            const int strings_case_insensitive_equal = Compare_Strings_Case_Insensitive
            (
                    c_string,
                    c_string_length,
                    selected_stop_word_list [i],
// On some Windows machines the line
// "(i < STOP_WORD_LIST_LENGTH) ? eng_stop_word_lengths [i] : strlen (selected_stop_word_list [i])"
// creates a warning. It's a false positive warning; but to avoid it, an another code fragment will be used
#ifdef _WIN32
                    strlen (selected_stop_word_list [i])
#else
                    (i < STOP_WORD_LIST_LENGTH) ? eng_stop_word_lengths [i] : strlen (selected_stop_word_list [i])
#endif /* _WIN32 */
            );

            if (strings_case_insensitive_equal == 0)
            {
                return true;
            }
        }
    }
    else
    {
        pseudo_hash = Pseudo_Hash_Function(c_string, c_string_length);

        for (register size_t i = 0; i < hash_table_next_free_element [pseudo_hash]; ++ i)
        {
            const size_t to_be_used_index = hash_table_stop_words [pseudo_hash][i];
            const int strings_case_insensitive_equal = Compare_Strings_Case_Insensitive
            (
                    c_string,
                    c_string_length,
                    selected_stop_word_list [to_be_used_index],
                    (to_be_used_index < STOP_WORD_LIST_LENGTH) ?
                            eng_stop_word_lengths [to_be_used_index] : strlen (selected_stop_word_list [to_be_used_index])
            );

            if (strings_case_insensitive_equal == 0)
            {
                return true;
            }
        }
    }

    return result;
}

//=====================================================================================================================

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

/**
 * @brief A very very simple hash function.
 *
 * It is not fair to call this mechanism "Hash", because it is too simple. But for this cases it is enough. It will be
 * used for the hash table. This table reduces the number of Compare_Strings_Case_Insensitive() calls, that determine
 * whether the string is a stop word or not.
 *
 * @param[in] c_string          C-String, that will be hashed
 * @param[in] c_string_length   Length of the C-String
 *
 * @return The hash value
 */
static inline size_t Pseudo_Hash_Function
(
        const char* const c_string,
        const size_t c_string_length
)
{
    size_t result = 0;

    for (size_t i = 0; i < c_string_length; ++ i)
    {
        result += (size_t) tolower (c_string [i]);
    }

    return result % HASH_TABLE_NUMBER_OF_ARRAYS;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef HASH_TABLE_NUMBER_OF_ARRAYS
#undef HASH_TABLE_NUMBER_OF_ARRAYS
#endif /* HASH_TABLE_NUMBER_OF_ARRAYS */

#ifdef HASH_TABLE_ARRAY_LENGTH
#undef HASH_TABLE_ARRAY_LENGTH
#endif /* HASH_TABLE_ARRAY_LENGTH */

#ifdef STOP_WORD_LIST_LENGTH
#undef STOP_WORD_LIST_LENGTH
#endif /* STOP_WORD_LIST_LENGTH */
