/**
 * @file File_Reader.c
 *
 * @brief In this translation unit the Token_List_Container object is defined. This is the main object for storing a
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
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Error_Handling/_Generics.h"
#include "Print_Tools.h"
#include "Misc.h"
#include "JSON_Parser/cJSON.h"
#include "str2int.h"
#include "int2str.h"
#include "Token_Int_Mapping.h"
#include "String_Tools.h"
#include "UTF8/utf8.h"
#include "ANSI_Esc_Seq.h"



/**
 * @brief Number of tokens in a Token_List
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef TOKENS_ALLOCATION_STEP_SIZE
#define TOKENS_ALLOCATION_STEP_SIZE 15
#else
#error "The macro \"TOKENS_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKENS_ALLOCATION_STEP_SIZE */

/**
 * @brief Number of Token_Lists in a Token_List_Container
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#define TOKEN_CONTAINER_ALLOCATION_STEP_SIZE 4
#else
#error "The macro \"TOKEN_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKEN_CONTAINER_ALLOCATION_STEP_SIZE */

/**
 * @brief Name of the JSON array with the tokens.
 */
#ifndef JSON_TOKENS_ARRAY_NAME
#define JSON_TOKENS_ARRAY_NAME "tokens"
#else
#error "The macro \"JSON_TOKENS_ARRAY_NAME\" is already defined !"
#endif /* JSON_TOKENS_ARRAY_NAME */

/**
 * @brief Name of the JSON array with the char offsets.
 */
#ifndef JSON_CHAR_OFFSET_ARRAY_NAME
#define JSON_CHAR_OFFSET_ARRAY_NAME "abs_char_offsets"
#else
#error "The macro \"JSON_CHAR_OFFSET_ARRAY_NAME\" is already defined !"
#endif /* JSON_CHAR_OFFSET_ARRAY_NAME */

/**
 * @brief Check, whether the macro values are valid.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(TOKENS_ALLOCATION_STEP_SIZE > 0, "The marco \"TOKENS_ALLOCATION_STEP_SIZE\" is zero !");
_Static_assert(TOKEN_CONTAINER_ALLOCATION_STEP_SIZE > 0, "The marco \"TOKEN_CONTAINER_ALLOCATION_STEP_SIZE\" is zero !");

IS_TYPE(TOKENS_ALLOCATION_STEP_SIZE, int)
IS_TYPE(TOKEN_CONTAINER_ALLOCATION_STEP_SIZE, int)

_Static_assert(sizeof(JSON_TOKENS_ARRAY_NAME) > 0 + 1, "The macro \"JSON_TOKENS_ARRAY_NAME\" needs at least one char (plus '\0') !");
_Static_assert(sizeof(JSON_CHAR_OFFSET_ARRAY_NAME) > 0 + 1, "The macro \"JSON_CHAR_OFFSET_ARRAY_NAME\" needs at least one char (plus '\0') !");
IS_CONST_STR(JSON_TOKENS_ARRAY_NAME)
IS_CONST_STR(JSON_CHAR_OFFSET_ARRAY_NAME)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ */

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

/**
 * @brief Calculate the begin of the token specified with an index in a Token_List.
 *
 * Asserts:
 *      token_list != NULL
 *      token_index < allocated_tokens
 *
 * @param[in] token_list Token_List object
 * @param[in] token_index Index of the token
 *
 * @return Pointer to the begin of the token with the given index
 */
static char*
Get_Address_Of_Token
(
        const struct Token_List* const token_list,
        const uint_fast32_t token_index
);

/**
 * @brief Calculate the the average of all tokens in the object.
 *
 * This information is for debugging and statistical purposes helpful.
 *
 * Asserts:
 *      token_list_container != NULL
 *
 * @param[in] token_list_container Token_List_Container object
 *
 * @return Average token length
 */
static size_t
Get_Average_Token_Length
(
        const struct Token_List_Container* const token_list_container
);

/**
 * @brief Increase the number of Token_List objects in a Token_List_Container.
 *
 * The default allocation step size (TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) will be used.
 *
 * Asserts:
 *      token_list_container != NULL
 *
 * @param[in] token_list_container Token_List_Container object
 */
static void
Increase_Number_Of_Token_Lists
(
        struct Token_List_Container* const token_list_container
);

/**
 * @brief Increase the number of tokens in a Token_List object.
 *
 * The default allocation step size (TOKENS_ALLOCATION_STEP_SIZE) will be used.
 *
 * Asserts:
 *      token_list != NULL
 *
 * @param[in] token_list Token_List object
 */
static void
Increase_Number_Of_Tokens
(
        struct Token_List* const token_list
);

/**
 * @brief Read the next line from the file and return the number of char, that were read.
 *
 * Asserts:
 *      input_file != NULL
 *      input_file_data != NULL
 *      input_file_data_length > 0
 *
 * @param[in] input_file Input file pointer
 * @param[in] input_file_data Buffer for the new input file data
 * @param[in] input_file_data_length Length of the input file buffer
 *
 * @return Number of char, that were read
 */
static size_t
Read_Next_Line
(
        FILE* restrict input_file,
        char* const restrict input_file_data,
        const long int input_file_data_length
);

/**
 * @brief The process print function for the file processing operation.
 *
 * Asserts:
 *      N/A
 *
 * @param print_step_size Number of inner loop calls, that needs to be done, for a new process print
 * @param actual Counter since last process print
 * @param hundred_percent The number of all inner loop calls (see the intersection code)
 * @param interval_begin clock_t value at the begin of the last process print
 * @param interval_end clock_t value since the last process print
 */
static void
Read_File_Process_Print_Function
(
        const size_t print_step_size,
        const size_t actual,
        const size_t hundred_percent,
        const clock_t interval_begin,
        const clock_t interval_end
);

/**
 * @brief Use the current cJSON object and identify the tokens and the offsets of them in this object.
 *
 * The function expects, that all given pointer are valid !
 *
 * Asserts:
 *      N/A
 *
 * @param[in] main_json Main cJSON object (It holds the object, that works direct with the source file)
 * @param[in] curr The current cJSON object
 * @param[in] new_container The new container, that will save the new information
 *
 * @return Number of tokens, that were found (This value can be 0)
 */
static uint_fast32_t
Use_Current_JSON_Fragment
(
        const cJSON* const main_json,
        const cJSON* const curr,
        struct Token_List_Container* const new_container
);

/**
 * @brief Use the current text line and determine the tokens, char, sentence and the word offset.
 *
 * The idea of the curr_line_num is, that the data sets get an created ID, because the data fragments from text files
 * have - unlike the JSON fragments - no ID. Technically an ID is not necessary for the calculations, but it is a good
 * information to get the position of the data in the source file.
 *
 * Asserts:
 *      N/A
 *
 * @param[in] curr_text Content of the current text line
 * @param[in] curr_text_len Length of the current text line
 * @param[in] curr_line_num Current line number or UINT_FAST32_MAX, if this variable is unused
 * @param[in] tokenize_data Already calculated tokenize data
 * @param[in] new_container The new container, that will save the new information
 *
 * @return Number of tokens, that were found (This value can be 0 !)
 */
static uint_fast32_t
Use_Current_Text_Fragment
(
        char* const curr_text,
        const size_t curr_text_len,
        const uint_fast32_t curr_line_num,
        const struct Tokenized_String* const tokenize_data,
        struct Token_List_Container* const new_container
);



enum File_Type
{
    NOT_SPECIFIED_FILE_TYPE = 0,    ///< File type not specified (This can be used as initial value for new variables)
    JSON_FILE_TYPE,                 ///< JSON file type
    TXT_FILE_TYPE,                  ///< Text file type

    UNKNOWN_FILE_TYPE               ///< Type is not determinable (This should not be used as initial value for new variables !)
};

/**
 * @brief Try to determine the file type.
 *
 * The function will check for a JSON or a text file.
 *
 * @param input_file FILE pointer to the already opened file.
 *
 * @return Type of the file; if a determination was not possible UNKNOWN_FILE_TYPE will be returned
 */
static enum File_Type
Determine_File_Type
(
        FILE* input_file
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
TokenListContainer_CreateObject
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
    new_container->malloc_calloc_calls ++;

    // Create the inner container
    new_container->allocated_token_container = TOKEN_CONTAINER_ALLOCATION_STEP_SIZE;
    new_container->token_lists = (struct Token_List*) CALLOC(new_container->allocated_token_container, sizeof (struct Token_List));
    ASSERT_ALLOC(new_container->token_lists, "Cannot create new Token objects !", new_container->allocated_token_container *
            sizeof (struct Token_List));
    new_container->malloc_calloc_calls ++;

    // Allocate memory for the inner container
    for (size_t i = 0; i < new_container->allocated_token_container; ++ i)
    {
        new_container->token_lists [i].data = (char*) CALLOC(MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE, sizeof (char));
        ASSERT_ALLOC(new_container->token_lists [i].data, "Cannot create data for a Token object !",
                MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
        new_container->malloc_calloc_calls ++;

        new_container->token_lists [i].char_offsets = (CHAR_OFFSET_TYPE*) MALLOC (TOKENS_ALLOCATION_STEP_SIZE * sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(new_container->token_lists [i].char_offsets, "Cannot create data for a Token object !",
                TOKENS_ALLOCATION_STEP_SIZE * sizeof (CHAR_OFFSET_TYPE));
        new_container->malloc_calloc_calls ++;

        new_container->token_lists [i].sentence_offsets = (SENTENCE_OFFSET_TYPE*) MALLOC (TOKENS_ALLOCATION_STEP_SIZE * sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(new_container->token_lists [i].sentence_offsets, "Cannot create data for a Token object !",
                TOKENS_ALLOCATION_STEP_SIZE * sizeof (SENTENCE_OFFSET_TYPE));
        new_container->malloc_calloc_calls ++;

        new_container->token_lists [i].word_offsets = (WORD_OFFSET_TYPE*) MALLOC (TOKENS_ALLOCATION_STEP_SIZE * sizeof (WORD_OFFSET_TYPE));
        ASSERT_ALLOC(new_container->token_lists [i].word_offsets, "Cannot create data for a Token object !",
                TOKENS_ALLOCATION_STEP_SIZE * sizeof (WORD_OFFSET_TYPE));
        new_container->malloc_calloc_calls ++;

        // Init new values
        for (size_t i2 = 0; i2 < TOKENS_ALLOCATION_STEP_SIZE; ++ i2)
        {
            new_container->token_lists [i].char_offsets [i2] = CHAR_OFFSET_TYPE_MAX;
            new_container->token_lists [i].sentence_offsets [i2] = SENTENCE_OFFSET_TYPE_MAX;
            new_container->token_lists [i].word_offsets [i2] = WORD_OFFSET_TYPE_MAX;
        }

        new_container->token_lists [i].max_token_length = MAX_TOKEN_LENGTH;
        new_container->token_lists [i].allocated_tokens = TOKENS_ALLOCATION_STEP_SIZE;
    }

    // Create the container for too long token
    new_container->list_of_too_long_token = TwoDimCStrArray_CreateObject (10);

    clock_t start       = 0;
    clock_t end         = 0;
    float used_seconds  = 0.0f;

    // Try to open the file
    FILE* input_file = fopen (file_name, "rb");
    ASSERT_FMSG(input_file != NULL, "Cannot open the input file: \"%s\" !", file_name);

    // Get file size
    int fseek_return = fseek (input_file, 0, SEEK_END);
    ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");
    const long int input_file_length = ftell (input_file);
    ASSERT_MSG(input_file_length != -1, "ftell() returned -1 !");
    fseek_return = fseek (input_file, 0, SEEK_SET);
    ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");

    ASSERT_FMSG(input_file_length > 0, "Input file (%s) has the length 0 !", file_name);

    char* input_file_data = (char*) CALLOC (((size_t) input_file_length + sizeof ("")), sizeof (char));
    ASSERT_ALLOC(input_file_data, "Cannot allocate memory for reading the input file !",
            ((size_t) input_file_length + sizeof ("")) * sizeof (char));
    new_container->malloc_calloc_calls ++;

    // Determine the file type
    const enum File_Type file_type = Determine_File_Type (input_file);
    switch (file_type)
    {
    case NOT_SPECIFIED_FILE_TYPE:
        printf("Not specified file type for \"%s\"\n", file_name);
        break;
    case JSON_FILE_TYPE:
        printf("Assume, that \"%s\" is a " ANSI_TEXT_BOLD "JSON file" ANSI_RESET_ALL "\n", file_name);
        break;
    case TXT_FILE_TYPE:
        printf("Assume, that \"%s\" is a " ANSI_TEXT_BOLD "text file" ANSI_RESET_ALL "\n", file_name);
        break;
    case UNKNOWN_FILE_TYPE:
        printf("Cannot determine the file type for \"%s\" !\n", file_name);
        break;
    default:
        ASSERT_MSG(false, "Switch case default path executed !");
    }
    puts("Start file loading ...");

    uint_fast32_t line_counter              = 0;
    uint_fast32_t sum_tokens_found          = 0;
    const uint_fast8_t count_steps          = 200;
    const size_t unsigned_input_file_length = (size_t) input_file_length;
    const uint_fast32_t print_steps         = ((unsigned_input_file_length / count_steps) == 0) ?
            1 : (unsigned_input_file_length / count_steps);

    // Read the first line from the file
    size_t char_read                    = Read_Next_Line (input_file, input_file_data, input_file_length);
    size_t sum_char_read                = char_read;
    size_t char_read_before_last_output = 0;

    CLOCK_WITH_RETURN_CHECK(start);
    // ===== ===== ===== ===== ===== BEGIN Read file line by line ===== ===== ===== ===== =====
    while(char_read > 0)
    {
        ++ line_counter;
        const char* current_parsing_position = input_file_data;
        // ===== ===== ===== ===== BEGIN Parse current line ===== ===== ===== =====
        if (file_type == JSON_FILE_TYPE)
        {
        while (*current_parsing_position != '\0')
        {
            // Parse the file JSON fragment per JSON fragment
            cJSON* json = cJSON_ParseWithOpts(current_parsing_position, (const char**) &current_parsing_position, false);

            // Print process information
            char_read_before_last_output = Process_Printer(print_steps, char_read_before_last_output,
                    sum_char_read, unsigned_input_file_length, true,
                    Read_File_Process_Print_Function,
                    NULL,
                    NULL);

            if (! json)
            {
                // Sometimes the json pointer is NULL. But an error only occurs, when an error message is available
                //if (strlen (cJSON_GetErrorPtr()) > 0)
                if (! IS_STRING_LENGTH_ZERO(cJSON_GetErrorPtr()))
                {
                    printf("Error before: [%s] %" PRIuFAST32 ": %ld\n", cJSON_GetErrorPtr(), line_counter,
                            (long int) (current_parsing_position - input_file_data));
                }
                break;
            }
            cJSON* curr = json->child;

            // ===== ===== ===== BEGIN Use current cJSON object ===== ===== =====
            while (curr != NULL)
            {
                // Extract the information from the current cJSON object
                sum_tokens_found += Use_Current_JSON_Fragment(json, curr, new_container);
                curr = curr->next;
            }
            // ===== ===== ===== BEGIN Use current cJSON object ===== ===== =====

            cJSON_Delete(json);
            json = NULL;
        }
        }
        else if (file_type == TXT_FILE_TYPE)
        {
            // Add explicit a delimiter at the end of the input data to avoid problems with the last token
            // The tokenize function go one char behind the last token and - when there is no extra char - the function
            // determines a '\0' and stop working. The result: the last token will be skipped
            // But this modification is only possible, when the input array has at least one char left
            ASSERT_FMSG((char_read + 1) < unsigned_input_file_length,
                    "Cannot prepare data for the tokenization ! The array needs to be at least %zu large; current length: %zu",
                    (char_read + 1) + 1, unsigned_input_file_length)
            input_file_data [char_read] = ' ';
            input_file_data [char_read + 1] = '\0';
            const struct Tokenized_String tokenized_string = Tokenize_String(input_file_data, " \t\n\r");

            // Print process information
            char_read_before_last_output = Process_Printer(print_steps, char_read_before_last_output,
                    sum_char_read, unsigned_input_file_length, true,
                    Read_File_Process_Print_Function,
                    NULL,
                    NULL);

            if (input_file_length > 0 && tokenized_string.next_free_pos_len == 0)
            {
                printf ("Error in the line %" PRIuFAST32 "\n", line_counter);
                continue; // while (*current_parsing_position != '\0')
            }

            if (tokenized_string.next_free_pos_len > 0)
            {
                sum_tokens_found +=
                        Use_Current_Text_Fragment(input_file_data, unsigned_input_file_length, line_counter,
                                &tokenized_string, new_container);
            }
        }
        else
        {
            ASSERT_MSG(false,
                    "Else path in the line parsing executed ! (No code for parsing the current file format available)");
        }
        // ===== ===== ===== ===== BEGIN Parse current line ===== ===== ===== =====

        // Read next line
        char_read                       = Read_Next_Line (input_file, input_file_data, input_file_length);
        sum_char_read                   += char_read;
        char_read_before_last_output    += char_read;
        //fgets_res = fgets(input_file_data, (int) input_file_length, input_file);
        //input_file_data [input_file_length] = '\0';
    }
    // ===== ===== ===== ===== ===== END Read file line by line ===== ===== ===== ===== =====

    // Print tokens, that was longer than the expected length
    if (new_container->list_of_too_long_token->next_free_c_str > 0)
    {
        printf("\n\nTokens, that are longer than expected (max. expected length: %d):\n", MAX_TOKEN_LENGTH - 1);
        if (new_container->list_of_too_long_token->next_free_c_str <= 50)
        {
            TwoDimCStrArray_PrintAllStrings(new_container->list_of_too_long_token);
        }
        else
        {
            const uint_fast32_t next_free_c_str = new_container->list_of_too_long_token->next_free_c_str;
            const int num_of_digits             = (int) Count_Number_Of_Digits(next_free_c_str);
            const uint_fast32_t print_range     = 15;

            // Print the first and the last 15 tokens
            for (uint_fast32_t i = 0; i < print_range; ++ i)
            {
                printf("%*" PRIuFAST32 ": %s\n", num_of_digits, i + 1, new_container->list_of_too_long_token->data [i]);
            }
            PRINT_X_TIMES_SAME_CHAR(' ', num_of_digits + 2);
            puts("...");
            for (uint_fast32_t i = (next_free_c_str - print_range); i < next_free_c_str; ++ i)
            {
                printf("%*" PRIuFAST32 ": %s\n", num_of_digits, i + 1, new_container->list_of_too_long_token->data [i]);
            }
        }
    }

    CLOCK_WITH_RETURN_CHECK(end);
    used_seconds = DETERMINE_USED_TIME(start, end);

    const float file_size_in_MB = ((float) input_file_length / 1024.0f / 1024.0f);
    printf ("\n=> %.3f MB in %3.3fs (~ %.3f MB/s) for parsing the whole file (" ANSI_TEXT_BOLD ANSI_TEXT_ITALIC
            "%" PRIuFAST32 " tokens found" ANSI_RESET_ALL ")\n",
            file_size_in_MB, used_seconds, file_size_in_MB / used_seconds, sum_tokens_found);

    FCLOSE_AND_SET_TO_NULL(input_file);
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
TokenListContainer_DeleteObject
(
        struct Token_List_Container* object
)
{
    ASSERT_MSG(object != NULL, "Token_List_Container is NULL !");

    TwoDimCStrArray_DeleteObject (object->list_of_too_long_token);
    object->list_of_too_long_token = NULL;

    if (object->token_lists != NULL)
    {
        // Delete from inner to the outer objects
        for (size_t i = 0; i < object->allocated_token_container; ++ i)
        {
            FREE_AND_SET_TO_NULL(object->token_lists [i].data);
            FREE_AND_SET_TO_NULL(object->token_lists [i].char_offsets);
            FREE_AND_SET_TO_NULL(object->token_lists [i].sentence_offsets);
            FREE_AND_SET_TO_NULL(object->token_lists [i].word_offsets);
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
 * @return Pointer at the begin of the token. (token is terminated with a null byte !)
 */
extern char*
TokenListContainer_GetToken
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
TokenListContainer_GetAllocatedMemSize
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
        result += (container->token_lists [i].allocated_tokens * sizeof (CHAR_OFFSET_TYPE));
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
TokenListContainer_ShowSelectedTokenList
(
        const struct Token_List_Container* const container,
        const size_t index_token_list
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");
    ASSERT_FMSG(container->next_free_element > index_token_list, "Index for the Token_List object is invalid ! Max. "
            "valid: %" PRIuFAST32 "; Got %zu !", container->next_free_element - 1, index_token_list);

    const size_t token_size = container->token_lists [index_token_list].max_token_length;

    printf ("Container: %zu (Dataset ID: %s)\n", index_token_list, container->token_lists [index_token_list].dataset_id);
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
TokenListContainer_ShowSelectedTokenListAsArray
(
        const struct Token_List_Container* const container,
        const size_t index_token_list
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");
    ASSERT_FMSG(container->next_free_element > index_token_list, "Index for the Token_List object is invalid ! Max. "
            "valid: %" PRIuFAST32 "; Got %zu !", container->next_free_element - 1, index_token_list);

    const size_t token_size = container->token_lists [index_token_list].max_token_length;

    printf ("Dataset ID: %s [ ", container->token_lists [index_token_list].dataset_id);
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
TokenListContainer_ShowAllTokenLists
(
        const struct Token_List_Container* const container
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        TokenListContainer_ShowSelectedTokenList (container, i);
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
TokenListContainer_ShowAllTokenListsAsArrays
(
        const struct Token_List_Container* const container
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        TokenListContainer_ShowSelectedTokenListAsArray (container, i);
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
TokenListContainer_CountAllTokens
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
TokenListContainer_GetLenghOfLongestToken
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
            result = MAX_WITH_TYPE_CHECK(result, strlen (&(container->token_lists [i].data [i2 * token_size])));
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
TokenListContainer_GetLenghOfLongestTokenList
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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    size_t max_length   = 0;
    size_t result       = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        if (container->token_lists [i].next_free_element > max_length)
        {
            max_length = container->token_lists [i].next_free_element;
            result = (size_t) i;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    size_t longest_dataset_id = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        const size_t cur_id_length = strlen (container->token_lists [i].dataset_id);
        if (cur_id_length > longest_dataset_id)
        {
            longest_dataset_id = cur_id_length;
        }
    }

    return longest_dataset_id;
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
TokenListContainer_ShowAttributes
(
        const struct Token_List_Container* const container
)
{
    ASSERT_MSG(container != NULL, "Token_List_Container is NULL !");

    puts("");
    printf ("Full token list container size: ");
    Print_Memory_Size_As_B_KB_MB(TokenListContainer_GetAllocatedMemSize(container));
    printf ("Sum all tokens:                 %" PRIuFAST32 "\n", TokenListContainer_CountAllTokens(container));
    printf ("Number of token lists:          %" PRIuFAST32 "\n", container->next_free_element);
    // Actual every token list has the same max token length !
    printf ("Max. possible token length:     %zu\n", container->token_lists [0].max_token_length);
    printf ("Average token length:           %zu\n", Get_Average_Token_Length(container));
    printf ("Longest token list:             %zu\n", TokenListContainer_GetLenghOfLongestTokenList(container));
    printf ("Array index longest token list: %zu\n", TokenListContainer_GetArrayIndexOfLongestTokenList(container));
    printf ("Longest saved token:            %zu\n", TokenListContainer_GetLenghOfLongestToken(container));
    printf ("Longest token in the container: %zu\n", container->longest_token_length);
    printf ("Longest dataset id:             %zu\n", TokenListContainer_GetLengthOfLongestDatasetID(container));
    printf ("Malloc / calloc calls:          %zu\n", container->malloc_calloc_calls);
    printf ("Realloc calls:                  %zu\n", container->realloc_calls);
    puts("");
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(container != NULL, "Token_List object is NULL !");
    ASSERT_FMSG(index < container->allocated_tokens, "Index (%zu) is out of bounds ! Max. valid %zu !", index,
            container->allocated_tokens);

    container->char_offsets [index]     = char_offset;
    container->sentence_offsets [index] = sentence_offset;
    container->word_offsets [index]     = word_offset;

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

/**
 * @brief Calculate the begin of the token specified with an index in a Token_List.
 *
 * Asserts:
 *      token_list != NULL
 *      token_index < allocated_tokens
 *
 * @param[in] token_list Token_List object
 * @param[in] token_index Index of the token
 *
 * @return Pointer to the begin of the token with the given index
 */
static char*
Get_Address_Of_Token
(
        const struct Token_List* const token_list,
        const uint_fast32_t token_index
)
{
    ASSERT_MSG(token_list != NULL, "Token_List is NULL !");
    ASSERT_FMSG(token_index < token_list->allocated_tokens,
            "Specified token index is too large ! Given: %" PRIuFAST32 "; Max. valid: %" PRIuFAST32 " !",
            token_index, token_list->allocated_tokens - 1);

    return token_list->data + (token_list->max_token_length * token_index);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Calculate the the average of all tokens in the object.
 *
 * This information is for debugging and statistical purposes helpful.
 *
 * Asserts:
 *      token_list_container != NULL
 *
 * @param[in] token_list_container Token_List_Container object
 *
 * @return Average token length
 */
static size_t
Get_Average_Token_Length
(
        const struct Token_List_Container* const token_list_container
)
{
    ASSERT_MSG(token_list_container != NULL, "Token_List_Container is NULL !");

    size_t sum_token_length = 0;

    for (uint_fast32_t i = 0; i < token_list_container->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < token_list_container->token_lists [i].next_free_element; ++ i2)
        {
            sum_token_length += strlen (TokenListContainer_GetToken(token_list_container, i, i2));
        }
    }

    const float result = (float) sum_token_length / (float) TokenListContainer_CountAllTokens(token_list_container);
    return (size_t) ceil (result);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Increase the number of Token_List objects in a Token_List_Container.
 *
 * The default allocation step size (TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) will be used.
 *
 * Asserts:
 *      token_list_container != NULL
 *
 * @param[in] token_list_container Token_List_Container object
 */
static void
Increase_Number_Of_Token_Lists
(
        struct Token_List_Container* const token_list_container
)
{
    ASSERT_MSG(token_list_container != NULL, "Token_List_Container is NULL !");

    static size_t token_container_realloc_counter = 0;
    ++ token_container_realloc_counter;
    const size_t old_allocated_token_container = token_list_container->allocated_token_container;

    // Adjust the number of Token_List object
    struct Token_List* temp_ptr = (struct Token_List*) REALLOC(token_list_container->token_lists,
            (old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) * sizeof (struct Token_List));
    ASSERT_ALLOC(temp_ptr, "Cannot reallocate memory for Token_Container objects !",
            (old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) * sizeof (struct Token_List));
    memset(temp_ptr + old_allocated_token_container, '\0', sizeof (struct Token_List) * TOKEN_CONTAINER_ALLOCATION_STEP_SIZE);
    token_list_container->realloc_calls ++;

    token_list_container->token_lists = temp_ptr;
    token_list_container->allocated_token_container = old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE;

    // Create memory for the new Token_List objects
    for (size_t i = old_allocated_token_container; i < token_list_container->allocated_token_container; ++ i)
    {
        token_list_container->token_lists [i].data = (char*) CALLOC(MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE, sizeof (char));
        ASSERT_ALLOC(token_list_container->token_lists [i].data, "Cannot create data for a Token object !",
                MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE * sizeof (char));
        token_list_container->malloc_calloc_calls ++;

        token_list_container->token_lists [i].char_offsets =
                (CHAR_OFFSET_TYPE*) MALLOC (TOKENS_ALLOCATION_STEP_SIZE * sizeof (CHAR_OFFSET_TYPE));
        ASSERT_ALLOC(token_list_container->token_lists [i].char_offsets, "Cannot create data for a Token object !",
                TOKENS_ALLOCATION_STEP_SIZE * sizeof (CHAR_OFFSET_TYPE));
        token_list_container->malloc_calloc_calls ++;

        token_list_container->token_lists [i].sentence_offsets =
                (SENTENCE_OFFSET_TYPE*) MALLOC (TOKENS_ALLOCATION_STEP_SIZE * sizeof (SENTENCE_OFFSET_TYPE));
        ASSERT_ALLOC(token_list_container->token_lists [i].sentence_offsets, "Cannot create data for a Token object !",
                TOKENS_ALLOCATION_STEP_SIZE * sizeof (SENTENCE_OFFSET_TYPE));

        token_list_container->token_lists [i].word_offsets =
                        (WORD_OFFSET_TYPE*) MALLOC (TOKENS_ALLOCATION_STEP_SIZE * sizeof (WORD_OFFSET_TYPE));
        ASSERT_ALLOC(token_list_container->token_lists [i].word_offsets, "Cannot create data for a Token object !",
                TOKENS_ALLOCATION_STEP_SIZE * sizeof (WORD_OFFSET_TYPE));

        token_list_container->malloc_calloc_calls ++;
        // Init new values
        for (size_t i2 = 0; i2 < TOKENS_ALLOCATION_STEP_SIZE; ++ i2)
        {
            token_list_container->token_lists [i].char_offsets [i2] = CHAR_OFFSET_TYPE_MAX;
            token_list_container->token_lists [i].sentence_offsets [i2] = SENTENCE_OFFSET_TYPE_MAX;
            token_list_container->token_lists [i].word_offsets [i2] = WORD_OFFSET_TYPE_MAX;
        }

        token_list_container->token_lists [i].max_token_length = MAX_TOKEN_LENGTH;
        token_list_container->token_lists [i].allocated_tokens = TOKENS_ALLOCATION_STEP_SIZE;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Increase the number of tokens in a Token_List object.
 *
 * The default allocation step size (TOKENS_ALLOCATION_STEP_SIZE) will be used.
 *
 * Asserts:
 *      token_list != NULL
 *
 * @param[in] token_list Token_List object
 */
static void
Increase_Number_Of_Tokens
(
        struct Token_List* const token_list
)
{
    ASSERT_MSG(token_list != NULL, "Token_List is NULL !");

    static size_t tokens_realloc_counter = 0;
    ++ tokens_realloc_counter;
    const size_t old_tokens_size    = token_list->allocated_tokens;
    const size_t token_size         = token_list->max_token_length;

    char* tmp_ptr = (char*) REALLOC(token_list->data,
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
    ASSERT_ALLOC(tmp_ptr, "Cannot reallocate memory for Tokens data !",
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
    memset(tmp_ptr + (old_tokens_size * token_size), '\0',
            (TOKENS_ALLOCATION_STEP_SIZE) * token_size);

    CHAR_OFFSET_TYPE* tmp_ptr2 = (CHAR_OFFSET_TYPE*) REALLOC (token_list->char_offsets,
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * sizeof (CHAR_OFFSET_TYPE));
    ASSERT_ALLOC(tmp_ptr2, "Cannot create data for a Token object !",
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * sizeof (CHAR_OFFSET_TYPE));

    SENTENCE_OFFSET_TYPE* tmp_ptr3 = (SENTENCE_OFFSET_TYPE*) REALLOC (token_list->sentence_offsets,
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * sizeof (SENTENCE_OFFSET_TYPE));
    ASSERT_ALLOC(tmp_ptr2, "Cannot create data for a Token object !",
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * sizeof (SENTENCE_OFFSET_TYPE));

    WORD_OFFSET_TYPE* tmp_ptr4 = (WORD_OFFSET_TYPE*) REALLOC (token_list->word_offsets,
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * sizeof (WORD_OFFSET_TYPE));
    ASSERT_ALLOC(tmp_ptr4, "Cannot create data for a Token object !",
            (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * sizeof (WORD_OFFSET_TYPE))

    // Init new values
    for (size_t i2 = old_tokens_size; i2 < (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE); ++ i2)
    {
        tmp_ptr2 [i2] = CHAR_OFFSET_TYPE_MAX;
        tmp_ptr3 [i2] = SENTENCE_OFFSET_TYPE_MAX;
        tmp_ptr4 [i2] = WORD_OFFSET_TYPE_MAX;
    }

    token_list->data                = tmp_ptr;
    token_list->char_offsets        = tmp_ptr2;
    token_list->sentence_offsets    = tmp_ptr3;
    token_list->word_offsets        = tmp_ptr4;
    token_list->allocated_tokens    += TOKENS_ALLOCATION_STEP_SIZE;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Read the next line from the file and return the number of char, that were read.
 *
 * Asserts:
 *      input_file != NULL
 *      input_file_data != NULL
 *      input_file_data_length > 0
 *
 * @param[in] input_file Input file pointer
 * @param[in] input_file_data Buffer for the new input file data
 * @param[in] input_file_data_length Length of the input file buffer
 *
 * @return Number of char, that were read
 */
static size_t
Read_Next_Line
(
        FILE* restrict input_file,
        char* const restrict input_file_data,
        const long int input_file_data_length
)
{
    ASSERT_MSG(input_file != NULL, "FILE is NULL !");
    ASSERT_MSG(input_file_data != NULL, "Input file data is NULL !");
    ASSERT_MSG(input_file_data_length > 0, "Input file data length is zero !");

    size_t char_read = 0;

    for (int c = getc(input_file); c != EOF; c = getc(input_file), ++ char_read)
    {
        if (c == '\n')
        {
            input_file_data [char_read] = '\0';
            input_file_data [input_file_data_length] = '\0';
            break;
        }
        input_file_data [char_read] = (char) c;
    }

    return char_read;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief The process print function for the file processing operation.
 *
 * Asserts:
 *      N/A
 *
 * @param print_step_size Number of inner loop calls, that needs to be done, for a new process print
 * @param actual Counter since last process print
 * @param hundred_percent The number of all inner loop calls (see the intersection code)
 * @param interval_begin clock_t value at the begin of the last process print
 * @param interval_end clock_t value since the last process print
 */
static void
Read_File_Process_Print_Function
(
        const size_t print_step_size,
        const size_t actual,
        const size_t hundred_percent,
        const clock_t interval_begin,
        const clock_t interval_end
)
{
    const size_t char_read_interval_begin   = (actual > hundred_percent) ? hundred_percent : actual;
    const size_t input_file_length          = hundred_percent;
    const size_t char_read_interval_end     = (char_read_interval_begin + print_step_size > input_file_length) ?
            input_file_length : (char_read_interval_begin + print_step_size);

    const int digits        = (int) Count_Number_Of_Digits(input_file_length);
    const float percent     = Determine_Percent(char_read_interval_begin, input_file_length);
    const float time_left   = Determine_Time_Left(char_read_interval_begin, char_read_interval_end, hundred_percent,
            interval_end - interval_begin);

    PRINTF_FFLUSH("Read file: %*" PRIuFAST32 " KByte (%3.2f %% | %.2f sec.)   ",
            (digits > 3) ? digits - 3 : 3, char_read_interval_begin / 1024,
                    Replace_NaN_And_Inf_With_Zero((percent > 100.0f) ? 100.0f : percent),
                    Replace_NaN_And_Inf_With_Zero(time_left));
    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Use the current cJSON object and identify the tokens and the offsets of them in this object.
 *
 * The function expects, that all given pointer are valid !
 *
 * Asserts:
 *      N/A
 *
 * @param[in] main_json Main cJSON object (It holds the object, that works direct with the source file)
 * @param[in] curr The current cJSON object
 * @param[in] new_container The new container, that will save the new information
 *
 * @return Number of tokens, that were found (This value can be 0)
 */
static uint_fast32_t
Use_Current_JSON_Fragment
(
        const cJSON* const main_json,
        const cJSON* const curr,
        struct Token_List_Container* const new_container
)
{
    uint_fast32_t tokens_found = 0;

    const cJSON* const name = cJSON_GetObjectItemCaseSensitive(main_json, curr->string);
    if (! name)                         { return 0; }
    if (! name->string)                 { return 0; }

    // Exists a tokens array ?
    const cJSON* const tokens_array = cJSON_GetObjectItemCaseSensitive(name, JSON_TOKENS_ARRAY_NAME);
    if (! tokens_array)                 { return 0; }
    if (! cJSON_IsArray(tokens_array))  { return 0; }

    // If a array with offsets is available ? Use them
    const cJSON* char_offsets_array = cJSON_GetObjectItemCaseSensitive(name, JSON_CHAR_OFFSET_ARRAY_NAME);
    if (! char_offsets_array) { if (! cJSON_IsArray(char_offsets_array)) { char_offsets_array = NULL; } }

    // Get all tokens from tokens array
    //const int tokens_array_size = cJSON_GetArraySize(tokens_array);
    register const cJSON* curr_token = tokens_array->child;
    if (! curr_token)                   { return 0; }
    register const cJSON* curr_char_offset = NULL;
    if (char_offsets_array != NULL) { curr_char_offset = char_offsets_array->child; }


    // Realloc necessary ?
    // Is it necessary to realloc/increase the number of Token_List objects in the container ?
    if (new_container->next_free_element >= new_container->allocated_token_container)
    {
        Increase_Number_Of_Token_Lists (new_container);
    }
    const size_t dataset_id_length =
            COUNT_ARRAY_ELEMENTS(new_container->token_lists [new_container->next_free_element].dataset_id);
    strncpy (new_container->token_lists [new_container->next_free_element].dataset_id, name->string,
            dataset_id_length - 1);
    new_container->token_lists [new_container->next_free_element].dataset_id [dataset_id_length - 1] = '\0';


    struct Token_List* const current_token_list_obj = &(new_container->token_lists [new_container->next_free_element]);

    // ===== ===== ===== BEGIN Go though the full chained list (the tokens array in the JSON file) ===== ===== =====
    while (curr_token != NULL)
    {
        if (! curr_token->valuestring) { curr_token = curr_token->next; continue; }

        // Is more memory for the new token in the Token_List necessary ?
        if (current_token_list_obj->next_free_element >= current_token_list_obj->allocated_tokens)
        {
            Increase_Number_Of_Tokens (current_token_list_obj);

            // Adjust the number of reallocs in the upper container
            new_container->realloc_calls += 3;
        }

        char* res_mem_for_curr_token = Get_Address_Of_Next_Free_Token (current_token_list_obj);

        const size_t current_token_len = strlen (curr_token->valuestring);

        // Copy token to the current Token_List
        strncpy(res_mem_for_curr_token, curr_token->valuestring, current_token_list_obj->max_token_length - 1);

        // Save the full token, if it is too long
        if (current_token_len > (current_token_list_obj->max_token_length - 1))
        {
            TwoDimCStrArray_AppendNewString
            (
                    new_container->list_of_too_long_token,
                    curr_token->valuestring,
                    current_token_len
            );
        }

        // Adjust the next offset value
        // Zero for the fist element
        if (current_token_list_obj->next_free_element == 0)
        {
            TokenList_SetOffsets(current_token_list_obj, 0, 0, 0, 0);
        }
        else
        {
            const char* last_token =
                    Get_Address_Of_Token (current_token_list_obj, current_token_list_obj->next_free_element - 1);
            // VVV This is the old way without notifying UTF8 char VVV
            // const size_t last_token_length = strlen(last_token);
            const size_t last_token_length = (size_t) u8_strlen((char*) last_token);

            size_t new_char_offset = 0;
            if (curr_char_offset != NULL)
            {
                new_char_offset = (size_t) curr_char_offset->valueint;
            }
            else
            {
                new_char_offset = current_token_list_obj->char_offsets [current_token_list_obj->next_free_element - 1] +
                        last_token_length;

                // Don't forget, that the char offsets in original data includes the blanks between the tokens !
                // Example from test_ebm_formatted.json:
                /* "tokens":            [ "[", "The", "chemotherapy", "of", ... ] */
                /* abs_char_offsets":   [ 0, 2, 6, 19, ... ] */
                /* => */ new_char_offset ++;
            }

            const size_t new_sentence_offset =
                    current_token_list_obj->sentence_offsets [current_token_list_obj->next_free_element - 1] +
                    (last_token [0] == '.' && (IS_STRING_LENGTH_ONE(last_token))) ? 1 : 0;
            const size_t new_word_offset = (size_t)
                    current_token_list_obj->word_offsets [current_token_list_obj->next_free_element - 1] + 1;

            CAST_CHECK(new_char_offset, size_t, CHAR_OFFSET_TYPE);

            TokenList_SetOffsets(current_token_list_obj, current_token_list_obj->next_free_element,
                    (CHAR_OFFSET_TYPE) new_char_offset, (SENTENCE_OFFSET_TYPE) new_sentence_offset, (WORD_OFFSET_TYPE) new_word_offset);
        }

        current_token_list_obj->next_free_element ++;
        tokens_found ++;

        // Is the current token longer than the previous tokens ?
        new_container->longest_token_length = MAX(new_container->longest_token_length, current_token_len);

        curr_token = curr_token->next;
        if (curr_char_offset != NULL)
        {
            curr_char_offset = curr_char_offset->next;
        }
    }
    // ===== ===== ===== END Go though the full chained list (the tokens array in the JSON file) ===== ===== =====

    // Use next element in the container
    new_container->next_free_element ++;

    return tokens_found;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Use the current text line and determine the tokens, char, sentence and the word offset.
 *
 * The idea of the curr_line_num is, that the data sets get an created ID, because the data fragments from text files
 * have - unlike the JSON fragments - no ID. Technically an ID is not necessary for the calculations, but it is a good
 * information to get the position of the data in the source file.
 *
 * Asserts:
 *      N/A
 *
 * @param[in] curr_text Content of the current text line
 * @param[in] curr_text_len Length of the current text line
 * @param[in] curr_line_num Current line number or UINT_FAST32_MAX, if this variable is unused
 * @param[in] tokenize_data Already calculated tokenize data
 * @param[in] new_container The new container, that will save the new information
 *
 * @return Number of tokens, that were found (This value can be 0 !)
 */
static uint_fast32_t
Use_Current_Text_Fragment
(
        char* const curr_text,
        const size_t curr_text_len,
        const uint_fast32_t curr_line_num,
        const struct Tokenized_String* const tokenize_data,
        struct Token_List_Container* const new_container
)
{
    uint_fast32_t tokens_found = 0;

    // Realloc necessary ?
    // Is it necessary to realloc/increase the number of Token_List objects in the container ?
    if (new_container->next_free_element >= new_container->allocated_token_container)
    {
        Increase_Number_Of_Token_Lists (new_container);
    }

    if (curr_line_num != UINT_FAST32_MAX)
    {
        char int_to_str_mem [10] = { '\0', '\0', '\0', '\0', '\0',  '\0', '\0', '\0', '\0', '\0' };

        const enum int2str_errno convert_status = int2str(int_to_str_mem,
                COUNT_ARRAY_ELEMENTS(int_to_str_mem), (long int) curr_line_num);
        ASSERT_FMSG(convert_status == INT2STR_SUCCESS, "Cannot convert the int value %" PRIuFAST32 " to a c string ! "
                "Error code: %d !", curr_line_num, (int) convert_status);

        Multi_strncat(new_container->token_lists [new_container->next_free_element].dataset_id,
                DATASET_ID_LENGTH - 1, 3, "Line ", int_to_str_mem, "\0");
    }

    struct Token_List* const current_token_list_obj = &(new_container->token_lists [new_container->next_free_element]);

    // ===== ===== ===== BEGIN Use all tokens in the current text line ===== ===== =====
    for (uint_fast32_t i = 0; i < tokenize_data->next_free_pos_len; ++ i)
    {
        // Skip empty tokens
        if (tokenize_data->token_data [i].len == 0) { continue; }

        ASSERT_FMSG((size_t) (tokenize_data->token_data[i].pos + tokenize_data->token_data[i].len) <= curr_text_len,
                "Invalid tokenize data found ! Length needs to be at least %zu; but a text with %zu is given !",
                (size_t) (tokenize_data->token_data[i].pos + tokenize_data->token_data[i].len), curr_text_len);

        // Is more memory for the new token in the Token_List necessary ?
        if (current_token_list_obj->next_free_element >= current_token_list_obj->allocated_tokens)
        {
            Increase_Number_Of_Tokens (current_token_list_obj);

            // Adjust the number of reallocs in the upper container
            new_container->realloc_calls += 3;
        }

        char* const res_mem_for_curr_token = Get_Address_Of_Next_Free_Token (current_token_list_obj);

        const size_t curr_token_len = (size_t) tokenize_data->token_data [i].len;
        char* const token_begin = &(curr_text [tokenize_data->token_data [i].pos]);

        // Temporary override end char of the current token to make the char range null terminated
        const char saved_char = *(token_begin + curr_token_len);
        *(token_begin + curr_token_len) = '\0';

        // Copy token to the current Token_List
        strncpy(res_mem_for_curr_token, token_begin, current_token_list_obj->max_token_length - 1);

        // Save the full token, if it is too long
        if (curr_token_len > (current_token_list_obj->max_token_length - 1))
        {
            TwoDimCStrArray_AppendNewString
            (
                    new_container->list_of_too_long_token,
                    token_begin,
                    curr_token_len
            );
        }

        if (current_token_list_obj->next_free_element == 0)
        {
            TokenList_SetOffsets(current_token_list_obj, 0, 0, 0, 0);
        }
        else
        {
            const char* last_token =
                    Get_Address_Of_Token (current_token_list_obj, current_token_list_obj->next_free_element - 1);
            // VVV This is the old way without notifying UTF8 char VVV
            // const size_t last_token_length = strlen(last_token);
            const size_t last_token_length = (size_t) u8_strlen((char*) last_token);

            size_t new_char_offset = current_token_list_obj->char_offsets [current_token_list_obj->next_free_element - 1] +
                    last_token_length;

            // Don't forget, that the char offsets in original data includes the blanks between the tokens !
            // Example from test_ebm_formatted.json:
            /* "tokens":            [ "[", "The", "chemotherapy", "of", ... ] */
            /* abs_char_offsets":   [ 0, 2, 6, 19, ... ] */
            /* => */ new_char_offset ++;

            const size_t new_sentence_offset =
                    current_token_list_obj->sentence_offsets [current_token_list_obj->next_free_element - 1] +
                    (last_token [0] == '.' && (IS_STRING_LENGTH_ONE(last_token))) ? 1 : 0;
            const size_t new_word_offset = (size_t)
                    current_token_list_obj->word_offsets [current_token_list_obj->next_free_element - 1] + 1;

            CAST_CHECK(new_char_offset, size_t, CHAR_OFFSET_TYPE);

            TokenList_SetOffsets(current_token_list_obj, current_token_list_obj->next_free_element,
                    (CHAR_OFFSET_TYPE) new_char_offset, (SENTENCE_OFFSET_TYPE) new_sentence_offset, (WORD_OFFSET_TYPE) new_word_offset);
        }

        current_token_list_obj->next_free_element ++;
        tokens_found ++;

        // Is the current token longer than the previous tokens ?
        new_container->longest_token_length = MAX(new_container->longest_token_length, curr_token_len);

        // Recover the origin text content
        *(token_begin + tokenize_data->token_data [i].len) = saved_char;
    }
    // ===== ===== ===== END Use all tokens in the current text line ===== ===== =====

    // Use next element in the container
    new_container->next_free_element ++;

    return tokens_found;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Try to determine the file type.
 *
 * The function will check for a JSON or a text file.
 *
 * Asserts:
 *      input_file != NULL
 *
 * @param input_file FILE pointer to the already opened file.
 *
 * @return Type of the file; if a determination was not possible UNKNOWN_FILE_TYPE will be returned
 */
static enum File_Type
Determine_File_Type
(
        FILE* input_file
)
{
    ASSERT_MSG(input_file != NULL, "FILE is NULL !");

    _Bool JSON_start_char_found = false;
    _Bool JSON_end_char_found   = false;

    // For a simple check mechanism we expect, that an JSON file starts with a '{' and end with a '}'
    // There will be no check, if the input file is a full valid JSON file. This will done in the reading process, when
    // this function determines a JSON file

    // Set the cursor to the start of the file and save the current cursor position
    const long int curr_cursor_pos = ftell (input_file);
    ASSERT_FMSG(curr_cursor_pos != -1L, "ftell() returned -1: %s", strerror(errno));
    int fseek_return = fseek (input_file, 0, SEEK_SET);
    ASSERT_FMSG(fseek_return == 0, "fseek() returned a nonzero value: %s", strerror(errno));

    // Reading until an '{' or a not space char was found
    int curr_c = 0;
    while ((curr_c = fgetc(input_file)) != EOF)
    {
        if (curr_c == '{')
        {
            JSON_start_char_found = true;
            break;
        }
        else if (! isspace(curr_c))
        {
            JSON_start_char_found = false;
            break;
        }
    }

    // Reading until an '}' or a not space char was found
    curr_c = 0;
    for (long int i = 0; curr_c != EOF; i ++)
    {
        fseek_return = fseek (input_file, -(i + 1), SEEK_END);
        ASSERT_FMSG(fseek_return == 0, "fseek() returned a nonzero value: %s", strerror(errno));

        curr_c = fgetc (input_file);
        if (curr_c == '}')
        {
            JSON_end_char_found = true;
            break;
        }
        else if (! isspace(curr_c))
        {
            JSON_end_char_found = true;
            break;
        }
    }

    // Set the file cursor the origin position
    fseek_return = fseek (input_file, curr_cursor_pos, SEEK_SET);
    ASSERT_FMSG(fseek_return == 0, "fseek() returned a nonzero value: %s", strerror(errno));

    return (JSON_start_char_found && JSON_end_char_found) ? JSON_FILE_TYPE : TXT_FILE_TYPE;
}

//---------------------------------------------------------------------------------------------------------------------



#ifdef TOKENS_ALLOCATION_STEP_SIZE
#undef TOKENS_ALLOCATION_STEP_SIZE
#endif /* TOKENS_ALLOCATION_STEP_SIZE */

#ifdef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#undef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#endif /* TOKEN_CONTAINER_ALLOCATION_STEP_SIZE */

#ifdef JSON_TOKENS_ARRAY_NAME
#undef JSON_TOKENS_ARRAY_NAME
#endif /* JSON_TOKENS_ARRAY_NAME */

#ifdef JSON_CHAR_OFFSET_ARRAY_NAME
#undef JSON_CHAR_OFFSET_ARRAY_NAME
#endif /* JSON_CHAR_OFFSET_ARRAY_NAME */
