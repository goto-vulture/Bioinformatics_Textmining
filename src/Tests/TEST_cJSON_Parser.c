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
    const size_t expected_results_file_length = strlen (expected_results_file_content);

    // Allocate memory for the parsing result and execute the parsing process
    // For the sake of simplicity, I assume that the double length of the expected result is enough
    long unsigned int parsing_result_length = (long unsigned int) (expected_results_file_length * 2);
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
            strncat (parsing_result, partial_parsing_result, parsing_result_length);
            parsing_result_length -= strlen(partial_parsing_result);
            // Add at the end of every partial parsing result a newline char
            strncat (parsing_result, "\n", parsing_result_length);
            parsing_result_length -= strlen ("\n");

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
