/**
 * @file Exec_Intersection.c
 *
 * @date 11.08.2022
 * @author am4
 */

#include "Exec_Intersection.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "CLI_Parameter.h"
#include "File_Reader.h"
#include "Token_Int_Mapping.h"
#include "Document_Word_List.h"
#include "Intersection_Approaches.h"
#include "Misc.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Error_Handling/Assert_Msg.h"
#include "Print_Tools.h"
#include "Stop_Words/Stop_Words.h"
#include "JSON_Parser/cJSON.h"



#ifndef cJSON_NOT_NULL
#define cJSON_NOT_NULL(cJSON_object)                                                                                    \
    ASSERT_MSG(cJSON_object != NULL, "Creation of a cJSON object for exporting data as JSON file failed !");
#else
#error "The macro \"cJSON_NOT_NULL\" is already defined !"
#endif /* cJSON_NOT_NULL */

#ifndef cJSON_NEW_OBJ_CHECK
#define cJSON_NEW_OBJ_CHECK(cJSON_object)                                                                               \
    cJSON_object = cJSON_CreateObject();                                                                                \
    cJSON_NOT_NULL(cJSON_object);
#else
#error "The macro \"cJSON_NEW_OBJ_CHECK\" is already defined !"
#endif /* cJSON_NEW_OBJ_CHECK */

#ifndef cJSON_NEW_ARR_CHECK
#define cJSON_NEW_ARR_CHECK(cJSON_object)                                                                               \
    cJSON_object = cJSON_CreateArray();                                                                                 \
    cJSON_NOT_NULL(cJSON_object);
#else
#error "The macro \"cJSON_NEW_ARR_CHECK\" is already defined !"
#endif /* cJSON_NEW_ARR_CHECK */

#ifndef cJSON_NEW_STR_CHECK
#define cJSON_NEW_STR_CHECK(cJSON_object, str)                                                                          \
    cJSON_object = cJSON_CreateString(str);                                                                             \
    cJSON_NOT_NULL(cJSON_object);
#else
#error "The macro \"cJSON_NEW_STR_CHECK\" is already defined !"
#endif /* cJSON_NEW_STR_CHECK */

/**
 * @brief Add general information to the export cJSON object.
 *
 * General information are:
 *      - Input file 1
 *      - Input file 2
 *      - Creation time (ctime format)
 *
 * Asserts:
 *      export_results != NULL
 *
 * @param export_results The main cJSON pointer for the export JSON file
 */
static void
Add_General_Information_To_Export_File
(
        cJSON* const export_results
);

/**
 * @brief Append the data from a Token_List_Container object (-> data from a input file) to the Token_Int_Mapping.
 *
 * Asserts:
 *      token_list_container != NULL
 *      token_int_mapping != NULL
 *
 * @param token_list_container Token_List_Container object (information from a input file)
 * @param token_int_mapping Token_Int_Mapping
 *
 * @return Number of token that were added to the Token_Int_Mapping
 */
static uint_fast32_t
Append_Token_List_Container_Data_To_Token_Int_Mapping
(
        const struct Token_List_Container* const token_list_container,
        struct Token_Int_Mapping* const restrict token_int_mapping
);

/**
 * @brief This function fills the Document_Word_List with data. But this data are the mapped integer values. The mapping
 * information will be uses from the given Token_Int_Mapping.
 *
 * This will prepare the Document_Word_List for the intersection process.
 *
 * Asserts:
 *      token_int_mapping != NULL
 *      token_list_container != NULL
 *      document_word_list != NULL
 *
 * @param token_int_mapping Token_Int_Mapping object (contains the used mapping information)
 * @param token_list_container Token_List_Container object (contains tokens from a input file)
 * @param document_word_list Document_Word_List object (result object for the mapped data)
 */
static void
Append_Token_Int_Mapping_Data_To_Document_Word_List
(
        const struct Token_Int_Mapping* const restrict token_int_mapping,
        const struct Token_List_Container* const restrict token_list_container,
        struct Document_Word_List* const restrict document_word_list
);

static void
Exec_Add_Token_To_Mapping_Process_Print_Function
(
        const size_t print_step_size,
        const size_t actual,
        const size_t hundred_percent,
        const clock_t interval_begin,
        const clock_t interval_end
);

static void
Exec_Intersection_Process_Print_Function
(
        const size_t print_step_size,
        const size_t actual,
        const size_t hundred_percent,
        const clock_t interval_begin,
        const clock_t interval_end
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute the intersection process.
 *
 * Execution steps:
 * - Read files and extract the tokens                                      (Two Token_List_Container)
 *      -- Token_List_Container is a container for token lists
 *      -- Every token list represents the tokens of one tokens array in the source JSON file
 *
 * - Create a token int mapping list                                        (One Token_Int_Mapping)
 *      -- The token int mapping give every token a unique integer value (This idea was chosen to speed up the
 *         intersection process, because integer comparisons are significant faster than string comparisons, especially
 *         with long strings)
 *      -- Also every token will be unique in the mapping object
 *      -- To avoid searching in the full data, a pseudo hash function was implemented. There are 100 data buckets
 *      -- The "hash sum" of a string is the sum of all char mod 100
 *      -- The integer values have the chosen bucket encoded in it itself
 *      -- the first two lowest digits (in decimal system) encodes the bucket. E.g.: xxx10 means, that this integer can
 *         found in the 11. bucket, when it exists in the mapping data
 *
 * - Use the token int mapping for the creation of a mapped token container (Two Document_Word_List)
 *      -- A Document_Word_List contains a 2 dimensional integer array
 *      -- In this array is the data for the intersection or for the intersection result (Yes Document_Word_List will
 *         be used for the intersection input data and for the intersection result !)
 *      -- Every array in the 2 dimensional array represents the data of one tokens array
 *      -- This means, that the results are focused on the tokens array from the input data
 *      -- This is necessary, because one of the goals of this project is to find the intersections between the tokens
 *         arrays in the input data
 *
 * - Create the intersections and save the information in the output file
 *      -- At the end the intersection between two Document_Word_List objects will be calculated
 *      -- The results will be written in the result file
 *
 *
 *
 * In this function will be NO input value tests, because NaN, +Inf, ... are good possibilities to say the function,
 * that the calculation should not stopped before the it is completed.
 *
 * Every invalid value will be interpreted in this way!
 *
 * Asserts:
 *      N/A
 *
 * @param[in] abort_progress_percent After this progress percent value the process will be stopped
 *
 * @return Status value (0: Success; != 0 Error)
 */
extern int
Exec_Intersection
(
        const float abort_progress_percent
)
{
    int result = 0;

    // >>> Read files and extract the tokens <<<
    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (GLOBAL_CLI_INPUT_FILE);
    TokenListContainer_ShowAttributes (token_container_input_1);
    struct Token_List_Container* token_container_input_2 = TokenListContainer_CreateObject (GLOBAL_CLI_INPUT_FILE2);
    TokenListContainer_ShowAttributes (token_container_input_2);



    // >>> Create a token int mapping list <<<
    struct Token_Int_Mapping* token_int_mapping = TokenIntMapping_CreateObject ();

    // ... and fill them with all tokens (Content from the first file)
    uint_fast32_t token_added_to_mapping =
            Append_Token_List_Container_Data_To_Token_Int_Mapping (token_container_input_1, token_int_mapping);
    printf ("\nAfter token container 1: %" PRIuFAST32 " elements added to token int mapping\n", token_added_to_mapping);
    // Content from the second file
    token_added_to_mapping +=
            Append_Token_List_Container_Data_To_Token_Int_Mapping (token_container_input_2, token_int_mapping);
    printf ("\nAfter token container 2: %" PRIuFAST32 " elements added to token int mapping\n", token_added_to_mapping);



    // >>> Use the token int mapping for the creation of a mapped token container <<<
    const size_t length_of_longest_token_container = MAX(TokenListContainer_GetLenghOfLongestTokenList(token_container_input_1),
            TokenListContainer_GetLenghOfLongestTokenList(token_container_input_2));

    // token_container_input_1->next_free_element and token_container_input_2->next_free_element
    // are the number of arrays in the two Token_List_Container. The meaning is, that every array in a
    // Token_List_Container represents a token array in the input JSON files
    // And length_of_longest_token_container is the length of the longest array. The idea is, that a Document_Word_List
    // will be created with the worst case in the memory usage. This is massive inefficient, but for the development it
    // is okay.
    // => ! It will be changed for real use cases ! <=
    struct Document_Word_List* source_int_values_1 =
            DocumentWordList_CreateObject(token_container_input_1->next_free_element, length_of_longest_token_container);
    struct Document_Word_List* source_int_values_2 =
            DocumentWordList_CreateObject(token_container_input_2->next_free_element, length_of_longest_token_container);

    Append_Token_Int_Mapping_Data_To_Document_Word_List(token_int_mapping, token_container_input_1,
            source_int_values_1);
    Append_Token_Int_Mapping_Data_To_Document_Word_List(token_int_mapping, token_container_input_2,
            source_int_values_2);

    DocumentWordList_ShowAttributes(source_int_values_1);
    DocumentWordList_ShowAttributes(source_int_values_2);
    puts("");



    // >>> Create the intersections and save the information in the output file <<<
    FILE* result_file = fopen(GLOBAL_CLI_OUTPUT_FILE, "w");
    ASSERT_FMSG(result_file != NULL, "Cannot open/create the result file: \"%s\" !", GLOBAL_CLI_OUTPUT_FILE);

    const uint_fast16_t count_steps                     = 10000;
    const uint_fast32_t number_of_intersection_calls    = source_int_values_2->next_free_array *
            source_int_values_1->next_free_array;
    const uint_fast32_t print_steps                     =
            (((uint_fast32_t) number_of_intersection_calls / count_steps) == 0) ?
                    1 : ((uint_fast32_t) number_of_intersection_calls / count_steps);

    // How many calls were done since the last calculation step was printed ?
    uint_fast32_t intersection_calls_before_last_output = 0;
    // Counter of all calls were done since the execution was started
    size_t intersection_call_counter                    = 0;

    char dataset_id_1 [16];
    memset(dataset_id_1, '\0', sizeof (dataset_id_1));
    char dataset_id_2 [16];
    memset(dataset_id_2, '\0', sizeof (dataset_id_2));

    // Objects for exporting the intersection results as JSON file
    cJSON* src_token                        = NULL; // Token from a source file
    cJSON* src_token_no_stop_word           = NULL; // Token from a source file (no stop word)
    cJSON* src_tokens_array                 = NULL; // Array of tokens, which are from a source file
    cJSON* src_tokens_array_wo_stop_words   = NULL; // Array of tokens, which are from a source file (no stop words)
    cJSON* token                            = NULL; // Token
    cJSON* tokens_array                     = NULL; // Array of tokens
    cJSON* intersections_partial_match      = NULL; // Intersections with a partial match (An intersection does not
                                                    // contain a full source set)
    cJSON* intersections_full_match         = NULL; // Intersections with a full match (An intersection contains a full
                                                    // source set)
    cJSON* outer_object                     = NULL; // Outer object, that contains the intersections and token arrays

    cJSON* export_results = cJSON_CreateObject();
    cJSON_NOT_NULL(export_results);
    Add_General_Information_To_Export_File(export_results);

    clock_t start           = 0;
    clock_t end             = 0;

    // Determine the intersections
    CLOCK_WITH_RETURN_CHECK(start);

    uint_fast32_t last_used_selected_data_2_array = UINT_FAST32_MAX;

    for (uint_fast32_t selected_data_2_array = 0; selected_data_2_array < source_int_values_2->next_free_array;
            ++ selected_data_2_array)
    {
        cJSON_NEW_OBJ_CHECK(intersections_partial_match);
        cJSON_NEW_OBJ_CHECK(intersections_full_match);
        cJSON_NEW_OBJ_CHECK(outer_object);
        _Bool data_found = false;

        for (uint_fast32_t selected_data_1_array = 0; selected_data_1_array < source_int_values_1->next_free_array;
                ++ selected_data_1_array)
        {
            // Program exit after a given progress
            // This is only for debugging purposes to avoid a complete program execution
            if (Determine_Percent(intersection_call_counter, number_of_intersection_calls) > abort_progress_percent)
            {
                PRINTF_FFLUSH("\nCalculation stopped intended after %.4f %% !\n", abort_progress_percent);
                goto abort_label;
            }

            // Print calculation steps
            intersection_calls_before_last_output = Process_Printer(print_steps, intersection_calls_before_last_output,
                    intersection_call_counter, number_of_intersection_calls,
                    Exec_Intersection_Process_Print_Function);

            // Determine the current intersection
            // The second array (source_int_values_2->data [selected_data_array]) will be used for every data array in
            // source_int_values_1 !
            //struct Document_Word_List* intersection_result = Intersection_Approach_2_Nested_Loops (source_int_values_1,
            //        source_int_values_2->data [selected_data_2_array], source_int_values_2->arrays_lengths [selected_data_2_array]);
            struct Document_Word_List* intersection_result = IntersectionApproach_TwoNestedLoopsWithTwoRawDataArrays
                    (source_int_values_1->data [selected_data_1_array],
                            source_int_values_1->arrays_lengths [selected_data_1_array],
                            source_int_values_2->data [selected_data_2_array],
                            source_int_values_2->arrays_lengths [selected_data_2_array],
                            token_container_input_1->token_lists [selected_data_1_array].dataset_id,
                            token_container_input_2->token_lists [selected_data_2_array].dataset_id);
            ++ intersection_call_counter;
            ++ intersection_calls_before_last_output;

            // Copy the two dataset IDs and use them for the export JSON file
            // memset(dataset_id_1, '\0', sizeof (dataset_id_1));
            // strncpy do the memset for the c string
            strncpy (dataset_id_1, intersection_result->dataset_id_1, COUNT_ARRAY_ELEMENTS(dataset_id_1) - 1);
            dataset_id_1 [COUNT_ARRAY_ELEMENTS(dataset_id_1) - 1] = '\0';
            // memset(dataset_id_2, '\0', sizeof (dataset_id_2));
            // strncpy do the memset for the c string
            strncpy (dataset_id_2, intersection_result->dataset_id_2, COUNT_ARRAY_ELEMENTS(dataset_id_2) - 1);
            dataset_id_2 [COUNT_ARRAY_ELEMENTS(dataset_id_2) - 1] = '\0';

            // Remove stop words from the result
            for (size_t i = 0; i < intersection_result->arrays_lengths [0]; ++ i)
            {
                // Reverse the mapping to get the original token (int -> token)
                const char* int_to_token_mem = TokenIntMapping_IntToTokenStaticMem(token_int_mapping,
                        intersection_result->data [0][i]);

                // Is the token in the list with the stop words ?
                if (Is_Word_In_Stop_Word_List(int_to_token_mem, strlen (int_to_token_mem), ENG))
                {
                    // Override the mapped int value
                    intersection_result->data [0][i] = UINT_FAST32_MAX;
                }
            }

            // How many objects left after removing stop words
            size_t tokens_left = 0;
            for (size_t i = 0; i < intersection_result->arrays_lengths [0]; ++ i)
            {
                if (intersection_result->data [0][i] != UINT_FAST32_MAX)
                {
                    ++ tokens_left;
                }
            }

            // Show only the data block, if there are intersection results
            if (DocumentWordList_IsDataInObject(intersection_result) && tokens_left > 0)
            {
                data_found = true;
                // "selected_data_2_array" is the counter for the outer loop
                // This test has the effect, that the tokens array only appear once for each outer element
                if (selected_data_2_array != last_used_selected_data_2_array)
                {
                    last_used_selected_data_2_array = selected_data_2_array;

                    cJSON_NEW_ARR_CHECK(src_tokens_array);
                    cJSON_NEW_ARR_CHECK(src_tokens_array_wo_stop_words);

                    for (size_t i = 0; i < source_int_values_2->arrays_lengths [selected_data_2_array]; ++ i)
                    {
                        // Reverse the mapping to get the original token (int -> token)
                        const char* int_to_token_mem = TokenIntMapping_IntToTokenStaticMem(token_int_mapping,
                                source_int_values_2->data [selected_data_2_array][i]);

                        // Is the token a stop word ?
                        if (! Is_Word_In_Stop_Word_List(int_to_token_mem, strlen (int_to_token_mem), ENG))
                        {
                            cJSON_NEW_STR_CHECK(src_token_no_stop_word, int_to_token_mem);
                            cJSON_AddItemToArray(src_tokens_array_wo_stop_words, src_token_no_stop_word);
                        }
                        cJSON_NEW_STR_CHECK(src_token, int_to_token_mem);
                        cJSON_AddItemToArray(src_tokens_array, src_token);
                    }
                    cJSON_AddItemToObject(outer_object, "tokens", src_tokens_array);
                    cJSON_AddItemToObject(outer_object, "tokens w/o stop words", src_tokens_array_wo_stop_words);
                }

                cJSON_NEW_ARR_CHECK(tokens_array);

                //fputs("Found tokens_array in:\n", result_file);
                // In the intersection result is always only one array ! Therefore a second loop is not necessary
                size_t tokens_wrote = 0;
                for (size_t i = 0; i < intersection_result->arrays_lengths [0]; ++ i)
                {
                    if (intersection_result->data [0][i] == UINT_FAST32_MAX)
                    {
                        continue;
                    }

                    // Reverse the mapping to get the original token (int -> token)
                    const char* int_to_token_mem = TokenIntMapping_IntToTokenStaticMem(token_int_mapping,
                            intersection_result->data [0][i]);
                    cJSON_NEW_STR_CHECK(token, int_to_token_mem);
                    cJSON_AddItemToArray(tokens_array, token);

                    ++ intersection_call_counter;
                    ++ tokens_wrote;
                }
                // Add data to the specific cJSON object
                if (cJSON_GetArraySize(tokens_array) == cJSON_GetArraySize(src_tokens_array))
                {
                    cJSON_AddItemToObject(intersections_full_match, dataset_id_1, tokens_array);
                }
                else
                {
                    cJSON_AddItemToObject(intersections_partial_match, dataset_id_1, tokens_array);
                }
            }

            DocumentWordList_DeleteObject(intersection_result);
            intersection_result = NULL;
        } // Inner loop end

        // Only append the objects from the current outer loop run, when data was found in the inner loop
        if (data_found)
        {
            cJSON_AddItemToObject(outer_object, "Intersections (partial)", intersections_partial_match);
            cJSON_AddItemToObject(outer_object, "Intersections (full)", intersections_full_match);
            cJSON_AddItemToObject(export_results, dataset_id_2, outer_object);
        }
        // Delete the new cJSON object from the current outer loop call to avoid memory leaks
        // cJSON_Delete (that will be used for freeing the resources) do this only for cJSOn objects, that are added to
        // the main object !
        else
        {
            cJSON_free(intersections_partial_match);
            intersections_partial_match = NULL;
            cJSON_free(intersections_full_match);
            intersections_full_match = NULL;
            cJSON_free(outer_object);
            outer_object = NULL;
        }
    }
    // Label for a debugging end of the calculations
abort_label:
    CLOCK_WITH_RETURN_CHECK(end);

    const float used_seconds = DETERMINE_USED_TIME(start, end);
    const float intersection_calls_div_abort = ((float) number_of_intersection_calls) * (GLOBAL_ABORT_PROCESS_PERCENT / 100.0f);
    printf ("\n=> %3.3fs (~ %zu calc/s) for calculation of all intersections.\n",
            Replace_NaN_And_Inf_With_Zero(used_seconds),
            (size_t) Replace_NaN_And_Inf_With_Zero (intersection_calls_div_abort));

    char* json_export_str = cJSON_Print(export_results); //< Create the JSON string for the export file
    const size_t result_str_length = strlen (json_export_str);
    fputs(json_export_str, result_file);
    printf ("=> Result file size: %zu B (%.3f KB | %.3f MB)\n", result_str_length,
            (double) result_str_length / 1024.0, (double) result_str_length / 1024.0 / 1024.0);
    FCLOSE_AND_SET_TO_NULL(result_file);

    // NO FREE_AND_SET_TO_NULL, because this object was not created with MALLOC or CALLOC. The counter for the dynamic
    // memory calls would be manipulated with the macro usage
    free (json_export_str);
    json_export_str = NULL;
    cJSON_Delete(export_results);
    export_results = NULL;

    if (intersections_partial_match != NULL)  { cJSON_free(intersections_partial_match); intersections_partial_match = NULL;  }
    if (intersections_full_match != NULL)  { cJSON_free(intersections_full_match); intersections_full_match = NULL;  }
    if (outer_object != NULL)   { cJSON_free(outer_object); outer_object = NULL;    }

    DocumentWordList_DeleteObject(source_int_values_1);
    source_int_values_1 = NULL;
    DocumentWordList_DeleteObject(source_int_values_2);
    source_int_values_2 = NULL;
    TokenListContainer_DeleteObject(token_container_input_1);
    token_container_input_1 = NULL;
    TokenListContainer_DeleteObject(token_container_input_2);
    token_container_input_2 = NULL;
    TokenIntMapping_DeleteObject(token_int_mapping);
    token_int_mapping = NULL;

    return result;
}

//=====================================================================================================================

/**
 * @brief Add general information to the export cJSON object.
 *
 * General information are:
 *      - Input file 1
 *      - Input file 2
 *      - Creation time (ctime format)
 *
 * Asserts:
 *      export_results != NULL
 *
 * @param export_results The main cJSON pointer for the export JSON file
 */
static void
Add_General_Information_To_Export_File
(
        cJSON* const export_results
)
{
    ASSERT_MSG(export_results != NULL, "Main cJSON result pointer is NULL !");

    // Insert some general infos to the export file
    cJSON* general_infos    = cJSON_CreateObject();
    cJSON_NOT_NULL(general_infos);
    cJSON* first_file       = cJSON_CreateString(GLOBAL_CLI_INPUT_FILE);
    cJSON_NOT_NULL(first_file);
    cJSON* second_file      = cJSON_CreateString(GLOBAL_CLI_INPUT_FILE2);
    cJSON_NOT_NULL(second_file);

    // Creation time
    const time_t raw_time = time(NULL);
    ASSERT_MSG(raw_time != (time_t) -1, "Current calendar time is not available !");
    const struct tm* const converted_time = localtime(&raw_time);
    ASSERT_MSG(converted_time != NULL, "time_t cannot be represented as a broken-down time !");

    char time_string [100];
    time_string [0] = '\1'; // Necessary to detect problems in the strftime call
    const size_t ret = strftime(time_string, COUNT_ARRAY_ELEMENTS(time_string), "%c", converted_time);
    // This way of checking the strftime call is described in the strftime documentation !
    ASSERT_MSG(!(ret == 0 && time_string [0] != '\0'), "Something went wrong in the strftime call.")
    time_string [ret] = '\0';

    // Creation mode (Part match ? Full match ? Part and full match ?)
    cJSON* creation_mode = cJSON_CreateObject();
    cJSON_NOT_NULL(creation_mode);
    cJSON* part_match = cJSON_CreateBool(true);
    cJSON_NOT_NULL(part_match);
    cJSON* full_match = cJSON_CreateBool(true);
    cJSON_NOT_NULL(full_match);
    cJSON* stop_word_list = cJSON_CreateBool(true);
    cJSON_NOT_NULL(stop_word_list);
    cJSON_AddItemToObject(creation_mode, "Part match", part_match);
    cJSON_AddItemToObject(creation_mode, "Full match", full_match);
    cJSON_AddItemToObject(creation_mode, "Stop word list used", stop_word_list);
    cJSON_AddItemToObject(general_infos, "Creation mode", creation_mode);

    cJSON* creation_time    = cJSON_CreateString(time_string);
    cJSON_NOT_NULL(creation_time);

    cJSON_AddItemToObject(general_infos, "First file", first_file);
    cJSON_AddItemToObject(general_infos, "Second file", second_file);
    cJSON_AddItemToObject(general_infos, "Creation time", creation_time);
    cJSON_AddItemToObject(export_results, "General infos", general_infos);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Append the data from a Token_List_Container object (-> data from a input file) to the Token_Int_Mapping.
 *
 * Asserts:
 *      token_list_container != NULL
 *      token_int_mapping != NULL
 *
 * @param token_list_container Token_List_Container object (information from a input file)
 * @param token_int_mapping Token_Int_Mapping
 *
 * @return Number of token that were added to the Token_Int_Mapping
 */
static uint_fast32_t
Append_Token_List_Container_Data_To_Token_Int_Mapping
(
        const struct Token_List_Container* const restrict token_list_container,
        struct Token_Int_Mapping* const restrict token_int_mapping
)
{
    ASSERT_MSG(token_list_container != NULL, "Token_List_Container is NULL !");
    ASSERT_MSG(token_int_mapping != NULL, "Token_Int_Mapping is NULL !");

    const uint_fast8_t count_steps = 100;
    uint_fast32_t inner_loop_runs = 0;
    for (uint_fast32_t i = 0; i < token_list_container->next_free_element; ++ i)
    {
        inner_loop_runs += token_list_container->token_lists [i].next_free_element;
    }
    const uint_fast32_t print_steps = (((uint_fast32_t) inner_loop_runs / count_steps) == 0) ? 1 :
            ((uint_fast32_t) inner_loop_runs / count_steps);
    uint_fast32_t inner_loop_runs_before_last_print = 0;
    uint_fast32_t inner_loop_counter = 0;

    uint_fast32_t token_added_to_mapping = 0;

    _Bool element_added = false;
    for (uint_fast32_t i = 0; i < token_list_container->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < token_list_container->token_lists [i].next_free_element; ++ i2)
        {
            // Print calculation steps
            inner_loop_runs_before_last_print = Process_Printer(print_steps, inner_loop_runs_before_last_print,
                    inner_loop_counter, inner_loop_runs,
                    Exec_Add_Token_To_Mapping_Process_Print_Function);

            char* token = TokenListContainer_GetToken (token_list_container, i, i2);
            element_added = TokenIntMapping_AddToken(token_int_mapping, token, strlen(token));
            if (element_added) { ++ token_added_to_mapping; }

            ++ inner_loop_counter;
            ++ inner_loop_runs_before_last_print;
        }
    }

    return token_added_to_mapping;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This function fills the Document_Word_List with data. But this data are the mapped integer values. The mapping
 * information will be uses from the given Token_Int_Mapping.
 *
 * This will prepare the Document_Word_List for the intersection process.
 *
 * Asserts:
 *      token_int_mapping != NULL
 *      token_list_container != NULL
 *      document_word_list != NULL
 *
 * @param token_int_mapping Token_Int_Mapping object (contains the used mapping information)
 * @param token_list_container Token_List_Container object (contains tokens from a input file)
 * @param document_word_list Document_Word_List object (result object for the mapped data)
 */
static void
Append_Token_Int_Mapping_Data_To_Document_Word_List
(
        const struct Token_Int_Mapping* const restrict token_int_mapping,
        const struct Token_List_Container* const restrict token_list_container,
        struct Document_Word_List* const restrict document_word_list
)
{
    ASSERT_MSG(token_int_mapping != NULL, "Token_Int_Mapping is NULL !");
    ASSERT_MSG(token_list_container != NULL, "Token_List_Container is NULL !");
    ASSERT_MSG(document_word_list != NULL, "Document_Word_List is NULL !");

    const size_t length_of_longest_token_container = TokenListContainer_GetLenghOfLongestTokenList(token_list_container);

    // Temp memory for appending data in a block
    // This avoids appending operations for each single value
    uint_fast32_t* token_int_values = (uint_fast32_t*) CALLOC(length_of_longest_token_container,
            sizeof (uint_fast32_t));
    ASSERT_ALLOC(token_int_values, "Cannot allocate memory for token int mapping values !",
            length_of_longest_token_container * sizeof (uint_fast32_t));

    uint_fast32_t next_free_value = 0;

    for (uint_fast32_t i = 0; i < token_list_container->next_free_element; ++ i)
    {
        memset(token_int_values, '\0', length_of_longest_token_container * sizeof (uint_fast32_t));
        next_free_value = 0;

        // Map token to int
        for (uint_fast32_t i2 = 0; i2 < token_list_container->token_lists [i].next_free_element; ++ i2)
        {
            char* token = TokenListContainer_GetToken (token_list_container, i, i2);
            token_int_values [next_free_value] = TokenIntMapping_TokenToInt(token_int_mapping, token, strlen(token));
            // Is the current token in the dictionary ?
            ASSERT_FMSG(token_int_values [next_free_value] != UINT_FAST32_MAX, "Token \"%s\" is not in the dictionary !",
                    token);
            ++ next_free_value;
        }

        // Append data
        if (next_free_value > 0)
        {
            DocumentWordList_AppendData(document_word_list, token_int_values, next_free_value);
        }
    }
    FREE_AND_SET_TO_NULL(token_int_values);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Exec_Add_Token_To_Mapping_Process_Print_Function
(
        const size_t print_step_size,
        const size_t actual,
        const size_t hundred_percent,
        const clock_t interval_begin,
        const clock_t interval_end
)
{
    const size_t call_counter_interval_begin    = (actual > hundred_percent) ? hundred_percent : actual;
    const size_t all_calls                      = hundred_percent;
    const size_t call_counter_interval_end      = (call_counter_interval_begin + print_step_size > all_calls) ?
            all_calls : (call_counter_interval_begin + print_step_size);

    const float percent     = Determine_Percent(call_counter_interval_begin, all_calls);
    const float time_left   = Determine_Time_Left(call_counter_interval_begin, call_counter_interval_end,
            all_calls, interval_end - interval_begin);

    PRINTF_FFLUSH("Add data to token int mapping (%3.2f %% | %.2f sec.)   \r",
            ceil(Replace_NaN_And_Inf_With_Zero(percent)), Replace_NaN_And_Inf_With_Zero(time_left));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Exec_Intersection_Process_Print_Function
(
        const size_t print_step_size,
        const size_t actual,
        const size_t hundred_percent,
        const clock_t interval_begin,
        const clock_t interval_end
)
{
    const size_t call_counter_interval_begin    = (actual > hundred_percent) ? hundred_percent : actual;
    const size_t all_calls                      = hundred_percent;
    const size_t call_counter_interval_end      = (call_counter_interval_begin + print_step_size > all_calls) ?
            all_calls : (call_counter_interval_begin + print_step_size);

    const float percent     = Determine_Percent(call_counter_interval_begin, all_calls);
    const float time_left   = Determine_Time_Left(call_counter_interval_begin, call_counter_interval_end,
            all_calls, interval_end - interval_begin);

    PRINTF_FFLUSH("Calculate intersections (%3.2f %% | %.2f sec.)   \r", Replace_NaN_And_Inf_With_Zero(percent),
            Replace_NaN_And_Inf_With_Zero(time_left));

    return;
}

//---------------------------------------------------------------------------------------------------------------------



#ifdef cJSON_NOT_NULL
#undef cJSON_NOT_NULL
#endif /* cJSON_NOT_NULL */

#ifdef cJSON_NEW_OBJ_CHECK
#undef cJSON_NEW_OBJ_CHECK
#endif /* cJSON_NEW_OBJ_CHECK */

#ifdef cJSON_NEW_ARR_CHECK
#undef cJSON_NEW_ARR_CHECK
#endif /* cJSON_NEW_ARR_CHECK */

#ifdef cJSON_NEW_STR_CHECK
#undef cJSON_NEW_STR_CHECK
#endif /* cJSON_NEW_STR_CHECK */
