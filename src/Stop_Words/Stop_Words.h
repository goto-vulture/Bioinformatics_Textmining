/**
 * @file Stop_Words.h
 *
 * @date 12.08.2022
 * @author: am4
 */


#ifndef STOP_WORDS_H
#define STOP_WORDS_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdbool.h>    // _Bool
#include <stddef.h>     // size_t



extern const char* GLOBAL_eng_stop_words [];    ///< Array with common English stop words



/**
 * @brief Language selector for the function, that calculates, if a given C-String is in the stop word list.
 */
enum Stop_Word_Language
{
    NO_LANGUAGE = 0,    ///< NO_LANGUAGE

    ENG                 ///< ENG
};



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
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STOP_WORDS_H */
