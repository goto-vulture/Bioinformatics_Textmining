/**
 * @file File_Reader.c
 *
 * @brief In this translation unit the Token_List_Container object is defined. This is the main object for storeing a
 * input dataset.
 *
 * It holds the original tokens from the dataset and make them usable for the mapping process. (See: "Token_Int_Mapping")
 *
 * @date 21.06.2022
 * @author Gyps
 */

#include "File_Reader.h"
#include <string.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Print_Tools.h"



/**
 * @brief Maximum length of a token (inkl. the terminator byte)
 *
 * The value is a approximate order of magnitude and could be change in the future.
 */
#ifndef MAX_TOKEN_LENGTH
#define MAX_TOKEN_LENGTH 32
#else
#error "The macro \"MAX_TOKEN_LENGTH\" is already defined !"
#endif /* MAX_TOKEN_LENGTH */

/**
 * @brief Number of tokens in a Token_List
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef TOKENS_ALLOCATION_STEP_SIZE
#define TOKENS_ALLOCATION_STEP_SIZE 350
#else
#error "The macro \"TOKENS_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKENS_ALLOCATION_STEP_SIZE */

/**
 * @brief Number of Token_Lists in a Token_List_Container
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#define TOKEN_CONTAINER_ALLOCATION_STEP_SIZE 100
#else
#error "The macro \"TOKEN_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKEN_CONTAINER_ALLOCATION_STEP_SIZE */

#ifndef READ_FILE_BUFFER_SIZE
#define READ_FILE_BUFFER_SIZE 100000 ///< Buffer size for reading files
#else
#error "The macro \"READ_FILE_BUFFER_SIZE\" is already defined !"
#endif /* READ_FILE_BUFFER_SIZE */

/**
 * @brief Read a line from a already opened file.
 *
 * The return value is a pointer to the buffer. In this way errors can be shown (e.g. with a NULL pointer).
 *
 * Asserts:
 *      token_list_container != NULL
 *      file != NULL
 *      file_buffer != NULL
 *      buffer_length > 0
 *      next_char_in_buffer < buffer_length
 *
 * @param[in] token_list_container Token_List_Container
 * @param[in] file FILE object of a already opened file
 * @param[in] file_buffer User defined buffer for file reading
 * @param[in] buffer_length Size of the buffer
 * @param[in] next_char_in_buffer Index to the next free char in the buffer
 *
 * @return Pointer to the buffer
 */
static char*
Read_Line
(
        struct Token_List_Container* const restrict token_list_container,
        FILE* const file,
        char* const restrict file_buffer,
        const size_t buffer_length,
        size_t next_char_in_buffer
);

/**
 * @brief Extract tokens from a buffer and add them to a Token_List_Container.
 *
 * Note: The parsing process to determine the tokens is simple and rudimentary and NOT usable for real datasets. Current
 * this very simple implementation is used to focus the development to the important parts.
 *
 * The parsing process can be outsourced in e.g. python scripts.
 *
 * Asserts:
 *      token_list_container != NULL
 *      file_buffer != NULL
 *      buffer_length > 0
 *      used_char_in_buffer < buffer_length
 *
 * @param[in] token_list_container Token_List_Container
 * @param[in] file_buffer User defined buffer with tokens
 * @param[in] buffer_length Size of the buffer
 */
static void
Extract_Tokens_From_Line
(
        struct Token_List_Container* const restrict token_list_container,
        const char* const restrict file_buffer,
        const size_t buffer_length,
        const size_t used_char_in_buffer
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Create the token list from a file content.
 *
 * Actual the "parsing mechanism" is very simple. Therefore the program execpts, that the JSON file is already
 * preprocessed.
 *
 * A possible python script for a suitable preprocessed JSON file:
 *
    import json

    f = open('test_ebm.json')

    data = json.load(f)

    for i in data:
        print(i)
        print(data[i]["tokens"])
        print("")

    f.close()

 *
 * The goal of the program is to work directly with JSON files. But this is for the core functionality not necessary.
 * Therefore the priority is not on this feature.
 *
 * Asserts:
 *      file_name != NULL
 *      strlen(file_name) > 0
 *
 * @param[in] file_name Preprocessed file
 *
 * @return Address to the new dynamic Token_List_Container
 */
extern struct Token_List_Container*
Create_Token_Container_From_File
(
        const char* const file_name
)
{
    ASSERT_MSG(file_name != NULL, "File name is NULL !");
    ASSERT_MSG(strlen(file_name) > 0, "File name is empty !");

    // Create the (outer) container
    struct Token_List_Container* new_container =
            (struct Token_List_Container*) CALLOC(1, sizeof (struct Token_List_Container));
    ASSERT_ALLOC(new_container, "Cannot create new Token_Container !", 1 * sizeof (struct Token_List_Container));

    // Create the inner container
    new_container->allocated_token_container = TOKEN_CONTAINER_ALLOCATION_STEP_SIZE;
    new_container->token_lists = (struct Token_List*) CALLOC(new_container->allocated_token_container, sizeof (struct Token_List));
    ASSERT_ALLOC(new_container->token_lists, "Cannot create new Token objects !", new_container->allocated_token_container *
            sizeof (struct Token_List));

    // Allocate memory for the inner container
    for (size_t i = 0; i < new_container->allocated_token_container; ++ i)
    {
        new_container->token_lists [i].data = (char*) CALLOC(MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE, sizeof (char));
        ASSERT_ALLOC(new_container->token_lists [i].data, "Cannot create data for a Token object !",
                MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
        // memset(new_container->tokens [i].data, '\0', MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);

        new_container->token_lists [i].max_token_length = MAX_TOKEN_LENGTH;
        new_container->token_lists [i].allocated_tokens = TOKENS_ALLOCATION_STEP_SIZE;
    }

    FILE* file = fopen(file_name, "r");
    ASSERT_FMSG(file != NULL, "Cannot open the file: \"%s\" !", file_name);

    char* file_buffer = (char*) MALLOC(READ_FILE_BUFFER_SIZE * sizeof (char));
    ASSERT_ALLOC(file_buffer, "Cannot create the read file buffer !", READ_FILE_BUFFER_SIZE * sizeof (char));

    uint_fast32_t line_counter = 0;
    while (Read_Line (new_container, file, file_buffer, READ_FILE_BUFFER_SIZE, 0) != NULL)
    {
        if (file_buffer [0] != '[')
        {
            continue;
        }

        ++ line_counter;

        // Is it necessary to realloc/increase the number of Token_Container ?
        if ((line_counter % TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) == 0)
        {
            static size_t token_container_realloc_counter = 0;
            ++ token_container_realloc_counter;
            const size_t old_allocated_token_container = new_container->allocated_token_container;

            // Adjust the number of Token_List object
            struct Token_List* temp_ptr = (struct Token_List*) REALLOC(new_container->token_lists,
                    (old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) * sizeof (struct Token_List));
            ASSERT_ALLOC(temp_ptr, "Cannot reallocate memory for Token_Container objects !",
                    (old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) * sizeof (struct Token_List));
            memset(temp_ptr + old_allocated_token_container, '\0', sizeof (struct Token_List) * TOKEN_CONTAINER_ALLOCATION_STEP_SIZE);

            new_container->token_lists = temp_ptr;
            new_container->allocated_token_container = old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE;

            // Create memory for the new Token_List objects
            for (size_t i = old_allocated_token_container; i < new_container->allocated_token_container; ++ i)
            {
                new_container->token_lists [i].data = (char*) MALLOC(MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
                ASSERT_ALLOC(new_container->token_lists [i].data, "Cannot create data for a Token object !",
                        MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
                memset(new_container->token_lists [i].data, '\0', MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);

                new_container->token_lists [i].max_token_length = MAX_TOKEN_LENGTH;
                new_container->token_lists [i].allocated_tokens = TOKENS_ALLOCATION_STEP_SIZE;
            }

            PRINTF_FFLUSH("Token_Container realloc. From %zu to %zu objects (%zu times)\n",
                    old_allocated_token_container, new_container->allocated_token_container,
                    token_container_realloc_counter);
        }

        Extract_Tokens_From_Line
        (
                new_container,
                file_buffer,
                READ_FILE_BUFFER_SIZE * sizeof (char),
                strlen(file_buffer)
        );
    }


    FCLOSE_AND_SET_TO_NULL(file);
    FREE_AND_SET_TO_NULL(file_buffer);

    return new_container;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(object != NULL, "Token_List_Container is NULL !");

    // Delete from inner to the outer objects
    for (size_t i = 0; i < object->allocated_token_container; ++ i)
    {
        if (&(object->token_lists [i]) != NULL)
        {
            FREE_AND_SET_TO_NULL(object->token_lists [i].data);
        }
    }

    FREE_AND_SET_TO_NULL(object->token_lists);
    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Read a specific token from the container.
 * @brief Ein bestimmtes Token aus dem Container auslesen.
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
)
{
    ASSERT_MSG(container != NULL, "Token_Container is NULL !");
    ASSERT_FMSG(index_token_list < container->next_free_element, "Tokens object id is invalid ! Max. valid: %"
            PRIuFAST32 "; Got: %" PRIuFAST32 " !", container->next_free_element - 1, index_token_list);
    ASSERT_FMSG(index_token_in_token_list < container->token_lists [index_token_list].next_free_element,
            "Token in Tokens object %" PRIuFAST32 " is invalid ! Max. valid: %" PRIuFAST32 "; Got: %" PRIuFAST32 " !",
            index_token_list, container->token_lists [index_token_list].next_free_element - 1, index_token_in_token_list);

    const size_t max_token_size = container->token_lists [index_token_list].max_token_length;

    return container->token_lists [index_token_list].data + (max_token_size * index_token_in_token_list);
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    size_t result = 0;

    const size_t max_token_size = container [0].token_lists->max_token_length;
    result += sizeof (struct Token_List) * container->allocated_token_container;
    result += sizeof (struct Token_List_Container);

    for (size_t i = 0; i < container->allocated_token_container; ++ i)
    {
        result += (container->token_lists [i].allocated_tokens * max_token_size);
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");
    ASSERT_FMSG(container->next_free_element < index_token_list, "Index for the Token_List object is invalid ! Max. "
            "valid: %" PRIuFAST32 "; Got %zu !", container->next_free_element - 1, index_token_list);

    const size_t token_size = container->token_lists [index_token_list].max_token_length;

    printf ("Container: %zu\n", index_token_list);
    for (size_t i = 0; i < container->token_lists [index_token_list].next_free_element; ++ i)
    {
        printf ("%4zu: %s\n", i, &(container->token_lists [index_token_list].data [i * token_size]));
    }
    PUTS_FFLUSH("");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    uint_fast32_t result = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        result += container->token_lists [i].next_free_element;
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    size_t result = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        if (container->token_lists [i].next_free_element > result)
        {
            result = container->token_lists [i].next_free_element;
        }
    }

    return result;
}

//=====================================================================================================================

/**
 * @brief Read a line from a already opened file.
 *
 * The return value is a pointer to the buffer. In this way errors can be shown (e.g. with a NULL pointer).
 *
 * Asserts:
 *      token_list_container != NULL
 *      file != NULL
 *      file_buffer != NULL
 *      buffer_length > 0
 *      next_char_in_buffer < buffer_length
 *
 * @param[in] token_list_container Token_List_Container
 * @param[in] file FILE object of a already opened file
 * @param[in] file_buffer User defined buffer for file reading
 * @param[in] buffer_length Size of the buffer
 * @param[in] next_char_in_buffer Index to the next free char in the buffer
 *
 * @return Pointer to the buffer
 */
static char*
Read_Line
(
        struct Token_List_Container* const restrict token_list_container,
        FILE* const file,
        char* const restrict file_buffer,
        const size_t buffer_length,
        size_t next_char_in_buffer
)
{
    ASSERT_MSG(token_list_container != NULL, "Token_Container is NULL !");
    ASSERT_MSG(file != NULL, "FILE is NULL !");
    ASSERT_MSG(file_buffer != NULL, "file_buffer is NULL !");
    ASSERT_MSG(buffer_length > 0, "buffer_length is 0 !");
    ASSERT_FMSG(next_char_in_buffer < buffer_length, "Next char buffer index (%zu) is equal/larger than the buffer "
            "length (%zu) !", next_char_in_buffer, buffer_length);

    static uint_fast32_t current_file_line = 0;
    static size_t sum_char_read = 0;
    ++ current_file_line;

    char* return_value = fgets (file_buffer + next_char_in_buffer, (int) (buffer_length - next_char_in_buffer), file);

    //ASSERT_FMSG(return_value != NULL, "Cannot read the file line %" PRIuFAST32 " !", current_file_line);
    if (return_value != NULL)
    {
        sum_char_read += strlen(file_buffer);
        //PRINTF_FFLUSH("Read line %5" PRIuFAST32 ". %8zu char. Sum char read %zu\n", current_file_line,
        //        strlen(file_buffer), sum_char_read);
    }

    // Is the size of the buffer large enough ? Or rather was it possible to read the full line ?
    //ASSERT_FMSG(file_buffer [buffer_length - 1] != '\0', "File buffer length %zu is not large enough to read the full "
    //        "line %" PRIuFAST32 " !", buffer_length, current_file_line);

    return return_value;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Extract tokens from a buffer and add them to a Token_List_Container.
 *
 * Note: The parsing process to determine the tokens is simple and rudimentary and NOT usable for real datasets. Current
 * this very simple implementation is used to focus the development to the important parts.
 *
 * The parsing process can be outsourced in e.g. python scripts.
 *
 * Asserts:
 *      token_list_container != NULL
 *      file_buffer != NULL
 *      buffer_length > 0
 *      used_char_in_buffer < buffer_length
 *
 * @param[in] token_list_container Token_List_Container
 * @param[in] file_buffer User defined buffer with tokens
 * @param[in] buffer_length Size of the buffer
 * @param[in] next_char_in_buffer Index to the next free char in the buffer
 */
static void
Extract_Tokens_From_Line
(
        struct Token_List_Container* const restrict token_list_container,
        const char* const restrict file_buffer,
        const size_t buffer_length,
        const size_t used_char_in_buffer
)
{
    ASSERT_MSG(token_list_container != NULL, "Token_Container is NULL !");
    ASSERT_MSG(file_buffer != NULL, "file_buffer is NULL !");
    ASSERT_MSG(buffer_length > 0, "buffer length is 0 !");
    ASSERT_FMSG(used_char_in_buffer < buffer_length, "Used char buffer index (%zu) is equal/larger than the buffer "
            "length (%zu) !", used_char_in_buffer, buffer_length);

    if (used_char_in_buffer <= 2) { return; }

    size_t file_buffer_cursor = 0;

    while (file_buffer_cursor < used_char_in_buffer)
    {
        const uint_fast32_t next_free_element_in_tokens =
                token_list_container->token_lists [token_list_container->next_free_element].next_free_element;
        struct Token_List* current_tokens_obj = &(token_list_container->token_lists [token_list_container->next_free_element]);

        // Here the same question: Is more memory for the new tokens necessary ?
        if (next_free_element_in_tokens >= current_tokens_obj->allocated_tokens)
        {
            static size_t tokens_realloc_counter = 0;
            ++ tokens_realloc_counter;
            const size_t old_tokens_size = current_tokens_obj->allocated_tokens;
            const size_t token_size = current_tokens_obj->max_token_length;

            char* tmp_ptr = (char*) REALLOC(current_tokens_obj->data,
                    (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
            ASSERT_ALLOC(tmp_ptr, "Cannot reallocate memory for Tokens data !",
                    (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
            memset(tmp_ptr + (old_tokens_size * token_size), '\0',
                    (TOKENS_ALLOCATION_STEP_SIZE) * token_size);

            current_tokens_obj->data = tmp_ptr;
            current_tokens_obj->allocated_tokens += TOKENS_ALLOCATION_STEP_SIZE;

            PRINTF_FFLUSH("Tokens realloc. From %zu to %zu objects (%zu times)\n", old_tokens_size,
                    current_tokens_obj->allocated_tokens, tokens_realloc_counter);
        }


        // Search the begin of the next token
        while (file_buffer [file_buffer_cursor] != '\'' && file_buffer [file_buffer_cursor] != '\"')
        {
            ++ file_buffer_cursor;
            if (file_buffer [file_buffer_cursor] == ']' || (file_buffer_cursor >= used_char_in_buffer))
            {
                goto end;
            }
        }
        ++ file_buffer_cursor;

        // Search the end of the current token
        size_t end_token = file_buffer_cursor;
        while (file_buffer [end_token] != '\'' && file_buffer [end_token] != '\"')
        {
            ++ end_token;
            if (file_buffer [end_token] == ']' || (end_token >= used_char_in_buffer))
            {
                goto end;
            }
        }
        const size_t token_length = end_token - file_buffer_cursor;

        // All tokens smaller than 3 characters will be skipped. The size is adjustable and will be skipped tokens like
        // 'a', 'or', etc. -> Tokens without usable information
        if (token_length < 2)
        {
            file_buffer_cursor = end_token + 1;
            continue;
        }

        const size_t token_size = token_list_container->token_lists [token_list_container->next_free_element].max_token_length;
        strncpy (&(token_list_container->token_lists [token_list_container->next_free_element].data [next_free_element_in_tokens * token_size]),
                &(file_buffer [file_buffer_cursor]),
                ((token_length >= MAX_TOKEN_LENGTH) ? MAX_TOKEN_LENGTH - 1 : token_length));

        token_list_container->token_lists [token_list_container->next_free_element].data [((next_free_element_in_tokens + 1) * token_size) - 1] = '\0';
        token_list_container->token_lists [token_list_container->next_free_element].next_free_element ++;
        file_buffer_cursor = end_token + 1;
    }
    end:
    token_list_container->next_free_element ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
