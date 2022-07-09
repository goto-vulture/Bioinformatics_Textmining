/**
 * @file File_Reader.h
 *
 * @brief In this translation unit the Token_List_Container object is defined. This is the main object for storeing a
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



#include <inttypes.h>
#include <stddef.h>



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
         * @brief Max. possible length for one token (inkl. terminator symbol)
         */
        size_t max_token_length;

        /**
         * @brief Next free element in the memory. The specific address will be calculated at runtime.
         */
        uint_fast32_t next_free_element;
        size_t allocated_tokens;            ///< Allocated number of tokens
        char id [16];                       ///< ID of the dataset
    }* token_lists;

    uint_fast32_t next_free_element;        ///< Next free element in the Token_List array
    size_t allocated_token_container;       ///< Allocated number of Token_List objects
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
Create_Token_Container_From_File
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
Delete_Token_Container
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
 * @return Poiner at the begin of the token. (token is terminated with a null byte !)
 */
extern char*
Get_Token_From_Token_Container
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
Get_Token_Container_Size
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
Show_Selected_Token_Container
(
        const struct Token_List_Container* const container,
        const size_t index_token_list
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
Count_All_Tokens_In_Token_Container
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
Get_Lengh_Of_Longest_Token
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
Get_Lengh_Of_Longest_Token_Container
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
Print_Token_List_Status_Infos
(
        const struct Token_List_Container* const container
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FILE_READER_H */
