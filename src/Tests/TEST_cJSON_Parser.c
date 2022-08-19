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
#include <time.h>
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
                    if (! name) { continue; }
                    // Exists a tokens array ?
                    cJSON* tokens_array = cJSON_GetObjectItemCaseSensitive(name, "tokens");
                    if (! tokens_array) { continue; }

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

extern void TEST_cJSON_Parse_Full_JSON_File (void)
{
    const char input_file_name [] = "./src/Tests/Test_Data/test_ebm.json";
    const char test_file_name [] = "./src/Tests/Test_Data/test_ebm_expected_results.txt";

    clock_t start = 0;
    clock_t end = 0;
    float used_seconds = 0.0f;

    // Try to read the full input_file
    FILE* input_file = fopen (input_file_name, "rb");
    ASSERT_FMSG(input_file != NULL, "Cannot open the input file: \"%s\" !", input_file_name);
    PRINTF_FFLUSH("Read file \"%s\" ...", input_file_name);

    // Get file size
    int fseek_return = fseek (input_file, 0, SEEK_END);
    ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");
    const long int input_file_length = ftell (input_file);
    ASSERT_MSG(input_file_length != -1, "ftell() returned -1 !");
    fseek_return = fseek (input_file, 0, SEEK_SET);
    ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");

    char* input_file_data = (char*) MALLOC (((size_t) input_file_length + sizeof ("")) * sizeof (char));
    ASSERT_ALLOC(input_file_data, "Cannot allocate memory for reading the input file !",
            ((size_t) input_file_length + sizeof ("")) * sizeof (char));

    // If there is a read error, less than input_file_length bytes are read
    CLOCK_WITH_RETURN_CHECK(start);
    size_t mem_read = fread (input_file_data, 1, (size_t) input_file_length, input_file); // Read full input file
    ASSERT_FMSG(mem_read == (size_t) input_file_length, "Error while reading the file \"%s\" !", input_file_name);
    CLOCK_WITH_RETURN_CHECK(end);
    used_seconds = DETERMINE_USED_TIME(start, end);

    input_file_data [input_file_length] = '\0';
    PRINTF_FFLUSH(" done ! (%ld byte | Used time: %3.3fs)\n", input_file_length, used_seconds);
    FCLOSE_AND_SET_TO_NULL(input_file);


    // Try to read the full test file
    FILE* test_file = fopen (test_file_name, "rb");
    ASSERT_FMSG(test_file != NULL, "Cannot open the test file: \"%s\" !", test_file_name);
    PRINTF_FFLUSH("Read file \"%s\" ...", test_file_name);

    // Get file size
    fseek_return = fseek (test_file, 0, SEEK_END);
    ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");
    const long int test_file_length = ftell (test_file);
    ASSERT_MSG(input_file_length != -1, "ftell() returned -1 !");
    fseek_return = fseek (test_file, 0, SEEK_SET);
    ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");

    char* test_file_data = (char*) MALLOC (((size_t) test_file_length + sizeof ("")) * sizeof (char));
    ASSERT_ALLOC(test_file_data, "Cannot allocate memory for reading the test file !",
            ((size_t) test_file_length + sizeof ("")) * sizeof (char));

    // If there is a read error, less than input_file_length bytes are read
    CLOCK_WITH_RETURN_CHECK(start);
    mem_read = fread (test_file_data, 1, (size_t) test_file_length, test_file); // Read full test file
    ASSERT_FMSG(mem_read == (size_t) test_file_length, "Error while reading the file \"%s\" !", test_file_name);
    CLOCK_WITH_RETURN_CHECK(end);
    used_seconds = DETERMINE_USED_TIME(start, end);

    test_file_data [test_file_length] = '\0';
    PRINTF_FFLUSH(" done ! (%ld byte | Used time: %3.3fs)\n", test_file_length, used_seconds);
    FCLOSE_AND_SET_TO_NULL(test_file);

    CLOCK_WITH_RETURN_CHECK(start);

    // Allocate memory for the parsing result and execute the parsing process
    // For the sake of simplicity, I assume that the double length of the expected result is enough
    const size_t parsing_result_length = (size_t) (test_file_length * 2);
    register size_t parsing_result_mem_left = parsing_result_length - 1;
    register char* parsing_result = (char*) CALLOC (parsing_result_length, sizeof (char));
    char* parsing_result_orig_ptr = parsing_result;
    ASSERT_ALLOC(parsing_result, "Cannot allocate memory for the parsing result !",
            parsing_result_mem_left * sizeof (char));
    parsing_result [parsing_result_mem_left - 1] = '\0'; // Gurantee a temination

    uint_fast32_t tokens_found = 0;
    const char* current_parsing_position = input_file_data;
    while (*current_parsing_position != '\0')
    {
        // Parse the file JSON fragment per JSON fragment
        cJSON* json = cJSON_ParseWithOpts(current_parsing_position, (const char**) &current_parsing_position, false);
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
            // Is the current name_x_x available ?
            cJSON* name = cJSON_GetObjectItemCaseSensitive(json, curr->string);
            if (! name) { continue; }

            // Exists a tokens array ?
            cJSON* tokens_array = cJSON_GetObjectItemCaseSensitive(name, "tokens");
            if (! tokens_array) { continue; }

            // Get all tokens from tokens array
            //const int tokens_array_size = cJSON_GetArraySize(tokens_array);
            register cJSON* curr_token = tokens_array->child;

            // Add id, if exists
            if (name->string)
            {
                strncat(parsing_result, name->string, parsing_result_mem_left);
                parsing_result += strlen (name->string);
            }

            ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                    "(Allocated size: %lu byte)", parsing_result_length);
            *parsing_result = '\n';
            parsing_result ++;
            parsing_result_mem_left --;
            ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                    "(Allocated size: %lu byte)", parsing_result_length);
            *parsing_result = '[';
            parsing_result ++;
            parsing_result_mem_left --;

            // Go though the full chained list
            while(curr_token != NULL)
            {
                if (! curr_token->valuestring) { continue; }

                // Exists a \' in the string
                // If yes it is necessary to replace the token begin and end with \" instead of \'
                char* double_quote_exists = strchr(curr_token->valuestring, '\'');

                // Faster way to copy one char than "strncat(parsing_result, "\'", parsing_result_mem_left);"
                ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                        "(Allocated size: %lu byte)", parsing_result_length);
                *parsing_result = (double_quote_exists == NULL) ? '\'' : '\"';
                parsing_result ++;
                parsing_result_mem_left --;

                strncat(parsing_result, curr_token->valuestring, parsing_result_mem_left);
                tokens_found ++;
                const size_t curr_token_length = strlen(curr_token->valuestring);
                ASSERT_FMSG(parsing_result_mem_left > curr_token_length, "Not enough memory allocated for the "
                        "parsing result ! (Allocated size: %lu byte)", parsing_result_length);
                parsing_result += curr_token_length;
                parsing_result_mem_left -= curr_token_length;

                // Faster way to copy three char than "strncat(parsing_result, "\", ", parsing_result_mem_left);"
                ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                        "(Allocated size: %lu byte)", parsing_result_length);
                *parsing_result = (double_quote_exists == NULL) ? '\'' : '\"';
                parsing_result ++;
                parsing_result_mem_left --;
                ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                        "(Allocated size: %lu byte)", parsing_result_length);
                *parsing_result = ',';
                parsing_result ++;
                parsing_result_mem_left --;
                ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                        "(Allocated size: %lu byte)", parsing_result_length);
                *parsing_result = ' ';
                parsing_result ++;
                parsing_result_mem_left --;

                curr_token = curr_token->next;
            }
            // Memory left check can be skipped here, because the pointer will be decremented before the write
            // commands
            parsing_result -= 2;
            *parsing_result = ']';
            parsing_result ++;
            *parsing_result = '\n';
            parsing_result ++;
            ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                    "(Allocated size: %lu byte)", parsing_result_length);
            // Faster way to copy one char than "strncat(parsing_result, "\n", parsing_result_mem_left);"
            *parsing_result = '\n';
            parsing_result ++;
            parsing_result_mem_left --;
            curr = curr->next;
        }
        *parsing_result = '\n';
        parsing_result ++;
        ASSERT_FMSG(parsing_result_mem_left > 0, "Not enough memory allocated for the parsing result ! "
                "(Allocated size: %lu byte)", parsing_result_length);
        *parsing_result = '\0';

        cJSON_Delete(json);
        json = NULL;
    }

    CLOCK_WITH_RETURN_CHECK(end);
    used_seconds = DETERMINE_USED_TIME(start, end);
    printf ("=> %3.3fs for parsing the whole file (%" PRIuFAST32 " tokens found)\n", used_seconds, tokens_found);

    const int cmp_result = strncmp (test_file_data, parsing_result_orig_ptr, (size_t) test_file_length);

    ASSERT_EQUALS(0, cmp_result);

    FREE_AND_SET_TO_NULL(input_file_data);
    FREE_AND_SET_TO_NULL(test_file_data);
    FREE_AND_SET_TO_NULL(parsing_result_orig_ptr);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
