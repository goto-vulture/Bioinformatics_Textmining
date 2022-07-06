/**
 * @file TEST_cJSON_Parser.c
 *
 * @brief In this translation unit are a few tests for the cJSON parser usage.
 *
 * @date 06.07.2022
 * @author am1
 */

#include "TEST_Document_Word_List.h"
#include <stdio.h>
#include <string.h>
#include "../JSON_Parser/cJSON.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"
#include "tinytest.h"
#include "../Print_Tools.h"
#include "../int2str.h"
#include "../Misc.h"



//---------------------------------------------------------------------------------------------------------------------

extern void TEST_cJSON_Parse_JSON_Fragment (void)
{
    const char test_file_content [] =
    {
            #include "./Test_Data/JSON_Fragment_1.json"
    };
    const char expected_results_file_content [] =
    {
            #include "./Test_Data/Expected_Result_1.txt"
    };
    const size_t test_file_length = strlen (test_file_content);
    const size_t expected_results_file_length = strlen (expected_results_file_content);
    PRINTF_FFLUSH("Test file size:            %5zu byte\n"
                  "Expected result file size: %5zu byte\n",test_file_length, expected_results_file_length);

    // Allocate memory for the parsing result and execute the parsing process
    // For the sake of simplicity, I assume that the double length of the expected result is enough
    const long unsigned int parsing_result_length = (long unsigned int) (expected_results_file_length * 2);
    long unsigned int parsing_result_mem_left = parsing_result_length - 1;
    char* parsing_result = (char*) MALLOC (parsing_result_length * sizeof (char));
    ASSERT_ALLOC(parsing_result, "Cannot allocate memory for the parsing result !",
            (expected_results_file_length * 2) * sizeof (char));
    parsing_result [expected_results_file_length * 2 - 1] = '\0'; // Gurantee a temination

    const char* current_parsing_position = test_file_content;
    while (*current_parsing_position != '\0')
    {
        // Parse the file JSON fragment per JSON fragment
        cJSON* json = cJSON_ParseWithOpts(current_parsing_position, (const char**) &current_parsing_position, false);
        if (! json)
        {
            printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        }
        else
        {
            // Display the cJSON object as C string
            char* partial_parsing_result = cJSON_Print(json);

            // Append the partial parsing results
            strncat (parsing_result, partial_parsing_result, parsing_result_mem_left);
            parsing_result_mem_left -= strlen(partial_parsing_result);
            ASSERT_FMSG(parsing_result_mem_left < parsing_result_length, "Memory for parsing result is too small ! "
                    "%zu byte were allocated.", parsing_result_length);

            // Add at the end of every partial parsing result a newline char
            strncat (parsing_result, "\n", parsing_result_mem_left);
            parsing_result_mem_left -= strlen ("\n");
            ASSERT_FMSG(parsing_result_mem_left < parsing_result_length, "Memory for parsing result is too small ! "
                    "%zu byte were allocated.", parsing_result_length);

            free (partial_parsing_result);
            partial_parsing_result = NULL;
        }
        cJSON_Delete(json);
        json = NULL;
    }

    // Compare the C strings
    const int cmp_result =
            strncmp (expected_results_file_content, parsing_result, strlen (expected_results_file_content));

    ASSERT_EQUALS(0, cmp_result);

    FREE_AND_SET_TO_NULL(parsing_result);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TEST_cJSON_Get_Token_Array_From_JSON_Fragment (void)
{
    const char test_file_content [] =
    {
            #include "./Test_Data/JSON_Fragment_2.json"
    };
    const char expected_results_file_content [] =
    {
            #include "./Test_Data/Expected_Result_2.txt"
    };
    const size_t test_file_length = strlen (test_file_content);
    const size_t expected_results_file_length = strlen (expected_results_file_content);
    PRINTF_FFLUSH("Test file size:            %5zu byte\n"
                  "Expected result file size: %5zu byte\n",test_file_length, expected_results_file_length);

    // Allocate memory for the parsing result and execute the parsing process
    // For the sake of simplicity, I assume that the double length of the expected result is enough
    const long unsigned int parsing_result_length = (long unsigned int) (expected_results_file_length * 2);
    long unsigned int parsing_result_mem_left = parsing_result_length - 1;
    char* parsing_result = (char*) CALLOC (parsing_result_length, sizeof (char));
    ASSERT_ALLOC(parsing_result, "Cannot allocate memory for the parsing result !",
            (expected_results_file_length * 2) * sizeof (char));
    parsing_result [expected_results_file_length * 2 - 1] = '\0'; // Gurantee a temination

    const char* current_parsing_position = test_file_content;
    while (*current_parsing_position != '\0')
    {
        // Parse the file JSON fragment per JSON fragment
        cJSON* json = cJSON_ParseWithOpts(current_parsing_position, (const char**) &current_parsing_position, false);
        if (! json)
        {
            printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        }
        else
        {
            // Now search for the "tokens" array in the JSON file
            // The major object is the "name_x_x" object ("x" is a placeholder for a digit (inkl. 0)). It is not
            // possible to determine the corrent "name_x_x" object. So it is necessary to try any possibility
            char name_x_x [20];
            memset (name_x_x, '\0', sizeof (name_x_x));

            for (int first_x = 0; first_x < 10; ++ first_x)
            {
                for (int second_x = 0; second_x < 10; ++ second_x)
                {
                    char first_x_as_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
                    char second_x_as_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };

                    const enum int2str_errno err_1 =
                            int2str(first_x_as_str, COUNT_ARRAY_ELEMENTS(first_x_as_str) - 1, first_x);
                    const enum int2str_errno err_2 =
                            int2str(second_x_as_str, COUNT_ARRAY_ELEMENTS(second_x_as_str) - 1, second_x);
                    ASSERT_FMSG(err_1 == INT2STR_SUCCESS, "Cannot convert %d to string !", first_x);
                    ASSERT_FMSG(err_2 == INT2STR_SUCCESS, "Cannot convert %d to string !", second_x);

                    // Build name_x_x
                    memset (name_x_x, '\0', sizeof (name_x_x));
                    size_t mem_left = COUNT_ARRAY_ELEMENTS(name_x_x) - 1;
                    strncat(name_x_x, "name_", mem_left);
                    mem_left -= strlen ("name_");
                    strncat(name_x_x, first_x_as_str, mem_left);
                    mem_left -= strlen (first_x_as_str);
                    strncat(name_x_x, "_", mem_left);
                    mem_left -= strlen ("_");
                    strncat(name_x_x, second_x_as_str, mem_left);
                    mem_left -= strlen (second_x_as_str);

                    // Is the current name_x_x available ?
                    cJSON* name = cJSON_GetObjectItemCaseSensitive(json, name_x_x);
                    if (! name)
                    {
                        continue;
                    }
                    // Exists a tokens array ?
                    cJSON* tokens_array = cJSON_GetObjectItemCaseSensitive(name, "tokens");
                    if (! tokens_array)
                    {
                        continue;
                    }

                    // Get all tokens from tokens array
                    const int tokens_array_size = cJSON_GetArraySize(tokens_array);
                    for (int i = 0; i < tokens_array_size; ++ i)
                    {
                        cJSON* current_token = cJSON_GetArrayItem(tokens_array, i);
                        if (cJSON_IsString(current_token))
                        {
                            //printf ("%s\n", current_token->valuestring);
                            // Insert token to result object
                            strncat(parsing_result, "\"", parsing_result_mem_left);
                            parsing_result_mem_left -= strlen ("\"");
                            ASSERT_FMSG(parsing_result_mem_left < parsing_result_length, "Memory for parsing result "
                                    "is too small ! %zu byte were allocated.", parsing_result_length);
                            strncat(parsing_result, current_token->valuestring, parsing_result_mem_left);
                            parsing_result_mem_left -= strlen(current_token->valuestring);
                            ASSERT_FMSG(parsing_result_mem_left < parsing_result_length, "Memory for parsing result "
                                    "is too small ! %zu byte were allocated.", parsing_result_length);
                            strncat(parsing_result, "\", ", parsing_result_mem_left);
                            parsing_result_mem_left -= strlen ("\", ");
                            ASSERT_FMSG(parsing_result_mem_left < parsing_result_length, "Memory for parsing result "
                                    "is too small ! %zu byte were allocated.", parsing_result_length);
                        }
                    }
                }
            }
        }
        cJSON_Delete(json);
        json = NULL;
    }

    // Remove the last two char from the parsing result
    parsing_result [strlen (parsing_result) - 1] = '\0';
    parsing_result [strlen (parsing_result) - 1] = '\0';

    // printf("=> %s\n", parsing_result);

    // Compare the C strings
    const int cmp_result =
            strncmp (expected_results_file_content, parsing_result, strlen (expected_results_file_content));

    ASSERT_EQUALS(0, cmp_result);

    FREE_AND_SET_TO_NULL(parsing_result);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
