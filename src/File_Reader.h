/**
 * @file File_Reader.h
 *
 * @brief In this translation unit the Token_List_Container object is defined. This is the main object for storing a
 * input dataset.
 *
 * It holds the original tokens from the dataset and make them usable for the mapping process. (See: "Token_Int_Mapping")
 *
 * @date 21.06.2022
 * @author Gyps
 */

#ifndef FILE_READER_H
#define FILE_READER_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>               // uint_fast32_t
#include <stddef.h>                 // size_t
#include "Defines.h"                // CHAR_OFFSET_TYPE
#include "Two_Dim_C_String_Array.h" // struct Two_Dim_C_String_Array



//=====================================================================================================================

struct Token_List_Container
{
    /**
     * @brief This is a container for some tokens. In the current implementation every object represents a dataset for
     * a specific id.
     */
    struct Token_List
    {
        /**
         * @brief C-Strings containing the tokens.
         *
         * It is a flat memory model. This means, that one C-String contains multiple tokens. Every token is saved as an
         * interval in the C-String.
         */
        char* data;

        /**
         * @brief Char offsets of each token.
         */
        CHAR_OFFSET_TYPE* char_offsets;

        /**
         * @brief Sentence offsets of each token.
         *
         * In other words: in which sentence are the tokens, when all sentences will be labeled with ascending numbers
         */
        SENTENCE_OFFSET_TYPE* sentence_offsets;

        /**
         * @brief Word offsets of each token.
         */
        WORD_OFFSET_TYPE* word_offsets;

        /**
         * @brief Max. possible length for one token (inkl. terminator symbol)
         */
        size_t max_token_length;

        /**
         * @brief Next free element in the memory. The specific address will be calculated at runtime.
         */
        uint_fast32_t next_free_element;
        size_t allocated_tokens;                            ///< Allocated number of tokens
        /**
         * @brief ID of the data set
         *
         * The ID is in our data sets sometimes a integer, sometimes a string. Therefore it is necessary to save the ID
         * as char array.
         */
        char dataset_id [DATASET_ID_LENGTH];                ///< ID of the dataset
    }* token_lists;

    size_t longest_token_length;                            ///< Length of the longest token in the whole container

    uint_fast32_t next_free_element;                        ///< Next free element in the Token_List array
    size_t allocated_token_container;                       ///< Allocated number of Token_List objects

    size_t malloc_calloc_calls;                             ///< How many malloc / calloc calls were done with this object ?
    size_t realloc_calls;                                   ///< How many realloc calls were done with this object ?

    struct Two_Dim_C_String_Array* list_of_too_long_token;  ///< List of tokens, that are longer than expected
};

//=====================================================================================================================

/**
 * @brief Create the token list from a JSON file.
 *
 * Asserts:
 *      file_name != NULL
 *      strlen(file_name) > 0
 *
 * @param[in] file_name Input file name
 *
 * @return Address to the new dynamic Token_List_Container
 */
extern struct Token_List_Container*
TokenListContainer_CreateObject
(
        const char* const file_name
);

/**
 * @brief Delete a dynamic allocated Delete_Token_Container object.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] object Delete_Token_Container object
 */
extern void
TokenListContainer_DeleteObject
(
        struct Token_List_Container* object
);

/**
 * @brief Read a specific token from the container.
 *
 * For performance reasons, the memory for each token list is allocated as one memory block. Therefore the address
 * calculation is more complex. This is the reason for this function.
 *
 * Asserts:
 *      container != NULL
 *      container->next_free_element < index_token_list
 *      index_token_in_token_list < container->token_lists [index_token_list].next_free_element
 *
 * @param[in] container Token_List_Container object
 * @param[in] index_token_list Index of the Token_List object
 * @param[in] index_token_in_token_list Index of the token in the Token_List object
 *
 * @return Pointer at the begin of the token. (token is terminated with a null byte !)
 */
extern char*
TokenListContainer_GetToken
(
        const struct Token_List_Container* const container,
        const uint_fast32_t index_token_list,
        const uint_fast32_t index_token_in_token_list
);

/**
 * @brief Determine the full memory usage in byte.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 *
 * @return Size of the full object in bytes
 */
extern size_t
TokenListContainer_GetAllocatedMemSize
(
        const struct Token_List_Container* const container
);

/**
 * @brief Print the content of a Token_List object.
 *
 * This is only for debugging purposes useful.
 *
 * Asserts:
 *      container != NULL
 *      container->next_free_element < index_token_list
 *
 * @param[in] container Token_List_Container object
 * @param[in] index_token_list Index of the Token_List object
 */
extern void
TokenListContainer_ShowSelectedTokenList
(
        const struct Token_List_Container* const container,
        const size_t index_token_list
);

/**
 * @brief Print the content of a Token_List object as array.
 *
 * Example for the array representation:
 *      ID: <Token_List ID>: [ <Token_List data> ]
 *
 * This is only for debugging purposes useful.
 *
 * Asserts:
 *      container != NULL
 *      container->next_free_element < index_token_list
 *
 * @param[in] container Token_List_Container object
 * @param[in] index_token_list Index of the Token_List object
 */
extern void
TokenListContainer_ShowSelectedTokenListAsArray
(
        const struct Token_List_Container* const container,
        const size_t index_token_list
);

/**
 * @brief Print the content of all Token_List containers in a Token_List_Container object.
 *
 * This is only for debugging purposes useful.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 */
extern void
TokenListContainer_ShowAllTokenLists
(
        const struct Token_List_Container* const container
);

/**
 * @brief Print the content of all Token_List containers in a Token_List_Container object with the array representation.
 *
 * This is only for debugging purposes useful.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 */
extern void
TokenListContainer_ShowAllTokenListsAsArrays
(
        const struct Token_List_Container* const container
);

/**
 * @brief Count all tokens in the whole Token_List_Container object.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 *
 * @return Counted number of whole tokens in the Token_List_Container object
 */
extern uint_fast32_t
TokenListContainer_CountAllTokens
(
        const struct Token_List_Container* const container
);

/**
 * @brief Determine the longest token in the whole container and return the value.
 *
 * This function need some time, especially when the container holds many tokens.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 *
 * @return Length of the longest token
 */
extern size_t
TokenListContainer_GetLenghOfLongestToken
(
        const struct Token_List_Container* const container
);

/**
 * @brief Determine the longest Token_List object and return the value.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 *
 * @return Length of the longest Token_List object
 */
extern size_t
TokenListContainer_GetLenghOfLongestTokenList
(
        const struct Token_List_Container* const container
);

/**
 * @brief Determine the array index of the longest Token_List object and return the value.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 *
 * @return Array index of the longest Token_List object
 */
extern size_t
TokenListContainer_GetArrayIndexOfLongestTokenList
(
        const struct Token_List_Container* const container
);

/**
 * @brief Determine the length of the longest data set ID.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 *
 * @return Length of the longest data set ID
 */
extern size_t
TokenListContainer_GetLengthOfLongestDatasetID
(
        const struct Token_List_Container* const container
);

/**
 * @brief Print several container information.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Token_List_Container object
 */
extern void
TokenListContainer_ShowAttributes
(
        const struct Token_List_Container* const container
);

/**
 * @brief Add a tuple with the three offsets to a Token_List.
 *
 * Asserts:
 *      container != NULL
 *      index < container->allocated_tokens
 *
 * @param[in] container Token_List object
 * @param[in] index Data index
 * @param[in] char_offset New char offset
 * @param[in] sentence_offset New sentence offset
 * @param[in] word_offset New word offset
 */
extern void
TokenList_SetOffsets
(
        struct Token_List* const container,
        const size_t index,
        const CHAR_OFFSET_TYPE char_offset,
        const SENTENCE_OFFSET_TYPE sentence_offset,
        const WORD_OFFSET_TYPE word_offset
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FILE_READER_H */
