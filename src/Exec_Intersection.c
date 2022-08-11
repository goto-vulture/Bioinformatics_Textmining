/**
 * @file Exec_Intersection.c
 *
 * @date 11.08.2022
 * @author am4
 */

#include "Exec_Intersection.h"
#include <stdio.h>
#include <string.h>
#include "CLI_Parameter.h"
#include "File_Reader.h"
#include "Token_Int_Mapping.h"
#include "Document_Word_List.h"
#include "Intersection_Approaches.h"
#include "Misc.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Error_Handling/Assert_Msg.h"
#include "Print_Tools.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute the intersection process.
 *
 * Execution steps:
 * - Read files and extract the tokens                                      (Two Token_List_Container)
 * - Create a token int mapping list                                        (One Token_Int_Mapping)
 * - Use the token int mapping for the creation of a mapped token container (Two Document_Word_List)
 * - Create the intersections and save the information in the output file
 *
 * @return Status value (0: Success; != 0 Error)
 */
extern int
Exec_Intersection
(
        void
)
{
    int result = 0;

    // >>> Read files and extract the tokens <<<
    struct Token_List_Container* token_container_input_1 = Create_Token_Container_From_File (GLOBAL_CLI_INPUT_FILE);
    Print_Token_List_Status_Infos (token_container_input_1);
    struct Token_List_Container* token_container_input_2 = Create_Token_Container_From_File (GLOBAL_CLI_INPUT_FILE2);
    Print_Token_List_Status_Infos (token_container_input_2);



    // >>> Create a token int mapping list <<<
    struct Token_Int_Mapping* token_int_mapping = Create_Token_Int_Mapping ();

    // ... and fill them with all tokens (Content from the first file)
    uint_fast32_t token_added_to_mapping = 0;
    _Bool element_added = false;
    for (uint_fast32_t i = 0; i < token_container_input_1->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < token_container_input_1->token_lists [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container_input_1, i, i2);
            element_added = Add_Token_To_Mapping(token_int_mapping, token, strlen(token));
            if (element_added) { ++ token_added_to_mapping; }
        }
    }

    printf ("\nAfter token container 1: %" PRIuFAST32 " elements added to token int mapping\n", token_added_to_mapping);
    // Content from the second file
    for (uint_fast32_t i = 0; i < token_container_input_2->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < token_container_input_2->token_lists [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container_input_2, i, i2);
            element_added = Add_Token_To_Mapping(token_int_mapping, token, strlen(token));
            if (element_added) { ++ token_added_to_mapping; }
        }
    }
    printf ("After token container 2: %" PRIuFAST32 " elements added to token int mapping\n", token_added_to_mapping);



    // >>> Use the token int mapping for the creation of a mapped token container <<<
    const size_t length_of_longest_token_container = MAX(Get_Lengh_Of_Longest_Token_Container(token_container_input_1),
            Get_Lengh_Of_Longest_Token_Container(token_container_input_2));

    struct Document_Word_List* source_int_values_1 =
            Create_Document_Word_List(token_container_input_1->next_free_element, length_of_longest_token_container);
    struct Document_Word_List* source_int_values_2 =
            Create_Document_Word_List(token_container_input_2->next_free_element, length_of_longest_token_container);

    // Temp memory for appending data in a block
    // This avoids appending operations for each single value
    uint_fast32_t* token_int_values = (uint_fast32_t*) CALLOC(length_of_longest_token_container,
            sizeof (uint_fast32_t));
    ASSERT_ALLOC(token_int_values, "Cannot allocate memory for token int mapping values !",
            length_of_longest_token_container * sizeof (uint_fast32_t));

    size_t next_free_value = 0;

    for (uint_fast32_t i = 0; i < token_container_input_1->next_free_element; ++ i)
    {
        memset(token_int_values, '\0', length_of_longest_token_container * sizeof (uint_fast32_t));
        next_free_value = 0;

        // Map token to int
        for (uint_fast32_t i2 = 0; i2 < token_container_input_1->token_lists [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container_input_1, i, i2);
            token_int_values [next_free_value] = Token_To_Int(token_int_mapping, token, strlen(token));
            // Is the current token in the dictionary ?
            ASSERT_FMSG(token_int_values [next_free_value] != UINT_FAST32_MAX, "Token \"%s\" is not in the dictionary !",
                    token);
            ++ next_free_value;
        }

        // Append data
        if (next_free_value > 0)
        {
            Append_Data_To_Document_Word_List(source_int_values_1, token_int_values, next_free_value);
        }
    }
    for (uint_fast32_t i = 0; i < token_container_input_2->next_free_element; ++ i)
    {
        memset(token_int_values, '\0', length_of_longest_token_container * sizeof (uint_fast32_t));
        next_free_value = 0;

        // Map token to int
        for (uint_fast32_t i2 = 0; i2 < token_container_input_2->token_lists [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container_input_2, i, i2);
            token_int_values [next_free_value] = Token_To_Int(token_int_mapping, token, strlen(token));
            // Is the current token in the dictionary ?
            ASSERT_FMSG(token_int_values [next_free_value] != UINT_FAST32_MAX, "Token \"%s\" is not in the dictionary !",
                    token);
            ++ next_free_value;
        }

        // Append data
        if (next_free_value > 0)
        {
            Append_Data_To_Document_Word_List(source_int_values_2, token_int_values, next_free_value);
        }
    }



    // >>> Create the intersections and save the information in the output file <<<
    FILE* result_file = fopen(GLOBAL_CLI_OUTPUT_FILE, "w");
    ASSERT_FMSG(result_file != NULL, "Cannot open/create the result file: \"%s\" !", GLOBAL_CLI_OUTPUT_FILE);

    // Print file information in the result file
    fprintf (result_file, "> First file:  %s\n", GLOBAL_CLI_INPUT_FILE);
    fprintf (result_file, "> Second file: %s\n\n", GLOBAL_CLI_INPUT_FILE2);

    const uint_fast16_t count_steps                     = 65000;
    const uint_fast32_t number_of_intersection_calls    = source_int_values_2->next_free_array *
            source_int_values_1->next_free_array;
    const uint_fast32_t print_steps                     =
            (((uint_fast32_t) number_of_intersection_calls / count_steps) == 0) ?
                    1 : ((uint_fast32_t) number_of_intersection_calls / count_steps);

    // How many calls were done since the last calculation step was printed ?
    uint_fast32_t print_counter                         = 0;
    // Counter of all calls were done since the execution was started
    size_t call_counter                                 = 0;

    clock_t start       = 0;
    clock_t end         = 0;
    float used_seconds  = 0.0f;

    // Determine the intersections
    start = clock();
    for (uint_fast32_t selected_data_2_array = 0; selected_data_2_array < source_int_values_2->next_free_array;
            ++ selected_data_2_array)
    {
        for (uint_fast32_t selected_data_1_array = 0; selected_data_1_array < source_int_values_1->next_free_array;
                ++ selected_data_1_array)
        {
            // Program exit after a fixed value
            // This is only for debugging purposes to avoid a complete program execution
            //if (Determine_Percent(selected_data_2_array, source_int_values_2->next_free_array) > 10.0f) { exit(1); }

            // Print calculation steps
            if (print_counter == print_steps)
            {
                print_counter = 0;
                PRINTF_FFLUSH("\rCalculate intersections (%.4f %%)", Determine_Percent(call_counter,
                        number_of_intersection_calls));
            }

            // Determine the current intersection
            // The second array (source_int_values_2->data [selected_data_array]) will be used for every data array in
            // source_int_values_1 !
            //struct Document_Word_List* intersection_result = Intersection_Approach_2_Nested_Loops (source_int_values_1,
            //        source_int_values_2->data [selected_data_2_array], source_int_values_2->arrays_lengths [selected_data_2_array]);
            struct Document_Word_List* intersection_result = Intersection_Approach_2_Nested_Loops_2_Raw_Data_Arrays
                    (source_int_values_1->data [selected_data_1_array],
                            source_int_values_1->arrays_lengths [selected_data_1_array],
                            source_int_values_2->data [selected_data_2_array],
                            source_int_values_2->arrays_lengths [selected_data_2_array],
                            token_container_input_1->token_lists [selected_data_1_array].dataset_id,
                            token_container_input_2->token_lists [selected_data_2_array].dataset_id);
            ++ call_counter;
            ++ print_counter;

            // Show only the data block, if there are intersection results
            if (Is_Data_In_Document_Word_List(intersection_result))
            {
                fprintf (result_file, "Token list \"%s\" (from second file): { ", intersection_result->dataset_id_2);
                for (size_t i = 0; i < source_int_values_2->arrays_lengths [selected_data_2_array]; ++ i)
                {
                    // Reverse the mapping to get the original token (int -> token)
                    char int_to_token_mem [MAX_TOKEN_LENGTH];
                    memset (int_to_token_mem, '\0', sizeof (int_to_token_mem));

                    Int_To_Token (token_int_mapping, source_int_values_2->data [selected_data_2_array][i], int_to_token_mem,
                            sizeof (int_to_token_mem) - 1);
                    fputs(int_to_token_mem, result_file);

                    if ((i + 1) < source_int_values_2->arrays_lengths [selected_data_2_array])
                    {
                        fputs(", ", result_file);
                    }
                }
                fputs(" }\n", result_file);

                fputs("Found tokens in:\n", result_file);
                for (size_t i = 0; i < intersection_result->number_of_arrays; ++ i)
                {
                    for (size_t i2 = 0; i2 < intersection_result->arrays_lengths [i]; ++ i2)
                    {
                        if (i2 == 0)
                        {
                            fprintf(result_file, "\t\"%s\": ", intersection_result->dataset_id_1);
                        }

                        // Reverse the mapping to get the original token (int -> token)
                        char int_to_token_mem [MAX_TOKEN_LENGTH];
                        memset (int_to_token_mem, '\0', sizeof (int_to_token_mem));

                        Int_To_Token (token_int_mapping, intersection_result->data [i][i2], int_to_token_mem,
                                sizeof (int_to_token_mem) - 1);
                        fputs(int_to_token_mem, result_file);
                        fputs(", ", result_file);

                        ++ call_counter;
                    }
                }
                fputs("\n\n", result_file);
            }

            Delete_Document_Word_List(intersection_result);
            intersection_result = NULL;
        }
    }
    end = clock();
    used_seconds = DETERMINE_USED_TIME(start, end);
    printf ("\n=> %3.3fs (~ %.3f calc/s) for calculation of all intersections.\n", used_seconds,
            ((float) number_of_intersection_calls) / used_seconds);



    FCLOSE_AND_SET_TO_NULL(result_file);
    FREE_AND_SET_TO_NULL(token_int_values);

    Delete_Token_Container(token_container_input_1);
    token_container_input_1 = NULL;
    Delete_Token_Container(token_container_input_2);
    token_container_input_2 = NULL;
    Delete_Token_Int_Mapping(token_int_mapping);
    token_int_mapping = NULL;
    Delete_Token_Container(token_container_input_1);
    token_container_input_1 = NULL;
    Delete_Token_Container(token_container_input_2);
    token_container_input_2 = NULL;

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
