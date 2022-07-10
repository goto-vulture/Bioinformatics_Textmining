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
#include <time.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Print_Tools.h"
#include "Misc.h"
#include "JSON_Parser/cJSON.h"
#include "str2int.h"



/**
 * @brief Maximum length of a token (inkl. the terminator byte)
 *
 * The value is a approximate order of magnitude and could be change in the future.
 */
#ifndef MAX_TOKEN_LENGTH
#define MAX_TOKEN_LENGTH 64
#else
#error "The macro \"MAX_TOKEN_LENGTH\" is already defined !"
#endif /* MAX_TOKEN_LENGTH */

/**
 * @brief Number of tokens in a Token_List
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef TOKENS_ALLOCATION_STEP_SIZE
#define TOKENS_ALLOCATION_STEP_SIZE 5
#else
#error "The macro \"TOKENS_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKENS_ALLOCATION_STEP_SIZE */

/**
 * @brief Number of Token_Lists in a Token_List_Container
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#define TOKEN_CONTAINER_ALLOCATION_STEP_SIZE 5
#else
#error "The macro \"TOKEN_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKEN_CONTAINER_ALLOCATION_STEP_SIZE */

/**
 * @brief Calculate the begin of the next free token in a Token_List.
 *
 * Asserts:
 *      token_list != NULL
 *
 * @param[in] token_list Token_List object
 *
 * @return Pointer to the begin of the next free token
 */
static char*
Get_Address_Of_Next_Free_Token
(
        const struct Token_List* const token_list
);

//---------------------------------------------------------------------------------------------------------------------

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

    clock_t start = 0;
    clock_t end = 0;
    float used_seconds = 0.0f;

    // Try to read the full input_file
    FILE* input_file = fopen (file_name, "rb");
    ASSERT_FMSG(input_file != NULL, "Cannot open the input file: \"%s\" !", file_name);
    PRINTF_FFLUSH("Read file \"%s\" ...", file_name);
    // Get file size
    fseek (input_file, 0, SEEK_END);
    const long int input_file_length = ftell (input_file);
    fseek (input_file, 0, SEEK_SET);
    char* input_file_data = (char*) MALLOC ((long unsigned int) (input_file_length + 1) * sizeof (char));
    ASSERT_ALLOC(input_file_data, "Cannot allocate memory for reading the input file !",
            (input_file_length + 1) * sizeof (char));

    // If there is a read error, less than input_file_length bytes are read
    start = clock();
    size_t mem_read = fread (input_file_data, 1, (size_t) input_file_length, input_file); // Read full input file
    ASSERT_FMSG(mem_read == (size_t) input_file_length, "Error while reading the file \"%s\" !", file_name);
    end = clock();
    used_seconds = DETERMINE_USED_TIME(start, end);

    input_file_data [input_file_length] = '\0';
    PRINTF_FFLUSH(" done ! (%ld byte | Used time: %3.3fs)\n", input_file_length, used_seconds);
    FCLOSE_AND_SET_TO_NULL(input_file);

    printf ("\nParsing file \"%s\"\n", file_name);
    uint_fast32_t tokens_found      = 0;
    uint_fast16_t last_percent_done = 0;
    time_t start_percent_done       = 0;
    const float max_steps           = 10000.0f;
    const char* current_parsing_position = input_file_data;
    while (*current_parsing_position != '\0')
    {
        // Parse the file JSON fragment per JSON fragment
        cJSON* json = cJSON_ParseWithOpts(current_parsing_position, (const char**) &current_parsing_position, false);
        const size_t used_input_file_char = (size_t) (current_parsing_position - input_file_data);

        const uint_fast16_t percent_done =
                ((uint_fast16_t) used_input_file_char / ((uint_fast16_t) input_file_length / (uint_fast16_t) max_steps));
        // Show next percent step on termial ?
        if (percent_done > last_percent_done)
        {
            const float time_used       = DETERMINE_USED_TIME(start_percent_done, clock ());
            const float current_percent = (float) percent_done / (max_steps / 100.0f);
            const float eta             = (max_steps - (float) percent_done) * time_used;
            PRINTF_FFLUSH ("\r%6.2f %% (ETA: ~ %10.2fs)",
                    (current_percent > 100.0f) ? 100.0f : current_percent, (eta < 0.0f) ? 0.0f : eta);
            last_percent_done = percent_done;
            start_percent_done = clock ();
        }

        if (! json)
        {
            // Sometimes the json pointer is NULL. But an error only occurrs, when an error message is available
            if (strlen (cJSON_GetErrorPtr()) > 0)
            {
                printf("Error before: [%s]\n", cJSON_GetErrorPtr());
            }
            break;
        }
        cJSON* curr = json->child;

        while (curr != NULL)
        {
            cJSON* name = cJSON_GetObjectItemCaseSensitive(json, curr->string);
            if (! name) { continue; }

            // Exists a tokens array ?
            cJSON* tokens_array = cJSON_GetObjectItemCaseSensitive(name, "tokens");
            if (! tokens_array) { continue; }

            // Get all tokens from tokens array
            //const int tokens_array_size = cJSON_GetArraySize(tokens_array);
            register cJSON* curr_token = tokens_array->child;

            // ===== ===== ===== BEGIN Is it an new dataset ? ===== ===== =====
            if (name->string)
            {
                // Is it not the first token list?
                if (new_container->token_lists [new_container->next_free_element].id != 0)
                {
                    // Use the next element in the container
                    new_container->next_free_element ++;

                    // Is it necessary to realloc/increase the number of Token_Container ?
                    if (new_container->next_free_element >= new_container->allocated_token_container)
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

                        //PRINTF_FFLUSH("Token_Container realloc. From %zu to %zu objects (%zu times)\n",
                        //        old_allocated_token_container, new_container->allocated_token_container,
                        //        token_container_realloc_counter);
                    }
                }

                //long int new_id = 0;
                //const enum str2int_errno convert_result = str2int(&new_id, name->string, 10);
                //ASSERT_FMSG(convert_result == STR2INT_SUCCESS, "Cannot convert string \"%s\" to int !",
                //        name->string);
                //new_container->token_lists [new_container->next_free_element].id = new_id;
                if (strlen (name->string) >= COUNT_ARRAY_ELEMENTS(new_container->token_lists [new_container->next_free_element].id))
                {
                    ASSERT_MSG(false, "<->");
                }
                else
                {
                    strncpy (new_container->token_lists [new_container->next_free_element].id, name->string, 15);
                }
            }
            // ===== ===== ===== END Is it an new dataset ? ===== ===== =====


            struct Token_List* const current_token_list_obj = &(new_container->token_lists [new_container->next_free_element]);

            // ===== BEGIN Go though the full chained list (the tokens array in the JSON file) =====
            while (curr_token != NULL)
            {
                if (! curr_token->valuestring) { continue; }

                // Is more memory for the new token in the Token_List necessary ?
                if (current_token_list_obj->next_free_element >= current_token_list_obj->allocated_tokens)
                {
                    static size_t tokens_realloc_counter = 0;
                    ++ tokens_realloc_counter;
                    const size_t old_tokens_size = current_token_list_obj->allocated_tokens;
                    const size_t token_size = current_token_list_obj->max_token_length;

                    char* tmp_ptr = (char*) REALLOC(current_token_list_obj->data,
                            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
                    ASSERT_ALLOC(tmp_ptr, "Cannot reallocate memory for Tokens data !",
                            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
                    memset(tmp_ptr + (old_tokens_size * token_size), '\0',
                            (TOKENS_ALLOCATION_STEP_SIZE) * token_size);

                    current_token_list_obj->data = tmp_ptr;
                    current_token_list_obj->allocated_tokens += TOKENS_ALLOCATION_STEP_SIZE;

                    //PRINTF_FFLUSH("Tokens realloc. From %zu to %zu objects (%zu times)\n", old_tokens_size,
                    //        current_token_list_obj->allocated_tokens, tokens_realloc_counter);
                }

                char* res_mem_for_curr_token = Get_Address_Of_Next_Free_Token (current_token_list_obj);

                // Copy token to the current Token_List
                strncpy(res_mem_for_curr_token, curr_token->valuestring, current_token_list_obj->max_token_length - 1);
                current_token_list_obj->next_free_element ++;
                tokens_found ++;

                curr_token = curr_token->next;
            }
            // ===== BEGIN Go though the full chained list (the tokens array in the JSON file) =====

            curr = curr->next;
        }

        cJSON_Delete(json);
        json = NULL;
    }

    // Don't forget to indicate, that the counter still point to the last used Token_List object
    new_container->next_free_element ++;

    end = clock ();
    used_seconds = DETERMINE_USED_TIME(start, end);
    printf ("\n=> %3.3fs (~ %.3f MB/s) for parsing the whole file (%" PRIuFAST32 " tokens found)\n", used_seconds,
            ((float) input_file_length / 1024.0f / 1024.0f) / used_seconds, tokens_found);

    FREE_AND_SET_TO_NULL(input_file_data);

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
    ASSERT_FMSG(container->next_free_element > index_token_list, "Index for the Token_List object is invalid ! Max. "
            "valid: %" PRIuFAST32 "; Got %zu !", container->next_free_element - 1, index_token_list);

    const size_t token_size = container->token_lists [index_token_list].max_token_length;

    printf ("Container: %zu (ID: %s)\n", index_token_list, container->token_lists [index_token_list].id);
    for (size_t i = 0; i < container->token_lists [index_token_list].next_free_element; ++ i)
    {
        printf ("%4zu: %s\n", i, &(container->token_lists [index_token_list].data [i * token_size]));
    }
    PUTS_FFLUSH("");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
Show_Selected_Token_Container_As_Array
(
        const struct Token_List_Container* const container,
        const size_t index_token_list
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");
    ASSERT_FMSG(container->next_free_element > index_token_list, "Index for the Token_List object is invalid ! Max. "
            "valid: %" PRIuFAST32 "; Got %zu !", container->next_free_element - 1, index_token_list);

    const size_t token_size = container->token_lists [index_token_list].max_token_length;

    printf ("ID: %s [ ", container->token_lists [index_token_list].id);
    for (size_t i = 0; i < container->token_lists [index_token_list].next_free_element; ++ i)
    {
        printf ("\"%s\"", &(container->token_lists [index_token_list].data [i * token_size]));

        if ((i + 1) < container->token_lists [index_token_list].next_free_element)
        {
            printf (", ");
        }
    }
    PUTS_FFLUSH(" ]");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
Show_All_Token_Container
(
        const struct Token_List_Container* const container
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        Show_Selected_Token_Container (container, i);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
Show_All_Token_Container_With_Array_Representation
(
        const struct Token_List_Container* const container
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        Show_Selected_Token_Container_As_Array (container, i);
    }

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    const size_t token_size = container->token_lists [0].max_token_length;
    size_t result = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < container->token_lists [i].next_free_element; ++ i2)
        {
            result = MAX(result, strlen (&(container->token_lists [i].data [i2 * token_size])));
        }
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

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    printf ("Full token list container size: %zu B (%.3f KB | %.3f MB)\n", Get_Token_Container_Size(container),
            (double) Get_Token_Container_Size(container) / 1024.0,
            (double) Get_Token_Container_Size(container) / 1024.0 / 1024.0);
    printf ("Sum all tokens:                 %" PRIuFAST32 "\n", Count_All_Tokens_In_Token_Container(container));
    printf ("Longest token list:             %zu\n", Get_Lengh_Of_Longest_Token_Container(container));
    printf ("Longest token:                  %zu\n", Get_Lengh_Of_Longest_Token(container));
    fflush (stdout);

    return;
}

//=====================================================================================================================

/**
 * @brief Calculate the begin of the next free token in a Token_List.
 *
 * Asserts:
 *      token_list != NULL
 *
 * @param[in] token_list Token_List object
 *
 * @return Pointer to the begin of the next free token
 */
static char*
Get_Address_Of_Next_Free_Token
(
        const struct Token_List* const token_list
)
{
    ASSERT_MSG(token_list != NULL, "Token_List is NULL !");

    return token_list->data + (token_list->max_token_length * token_list->next_free_element);
}

//---------------------------------------------------------------------------------------------------------------------
