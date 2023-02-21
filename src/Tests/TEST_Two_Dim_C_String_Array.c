/**
 * @file TEST_Two_Dim_C_String_Array.c
 *
 * @date 16.02.2023
 * @author: leer
 */

#include "TEST_Two_Dim_C_String_Array.h"

#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Misc.h"
#include "tinytest.h"



//---------------------------------------------------------------------------------------------------------------------

extern void TEST_AppendNewString (void)
{
    struct Two_Dim_C_String_Array* container = TwoDimCStrArray_CreateObject(10);
    ASSERT_MSG(container != NULL, "Created container is NULL !");

    char* new_string = (char*) "I am the new string ...";

    TwoDimCStrArray_AppendNewString(container, new_string, strlen(new_string));

    _Bool result = true;

    if (strncmp(container->data [0], new_string, strlen(new_string)) != 0) { result = false; }
    printf ("\"%s\"\nand\n\"%s\"\nare equal\n", container->data [0], new_string);

    TwoDimCStrArray_DeleteObject(container);
    container = NULL;

    ASSERT_EQUALS(true, result);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TEST_AppendDataToNewestString (void)
{
    struct Two_Dim_C_String_Array* container = TwoDimCStrArray_CreateObject(10);
    ASSERT_MSG(container != NULL, "Created container is NULL !");

    TwoDimCStrArray_AppendNewString(container, "TEST 1", strlen("TEST 1"));
    TwoDimCStrArray_AppendNewString(container, "TEST 2", strlen("TEST 2"));
    TwoDimCStrArray_AppendNewString(container, "TEST 3", strlen("TEST 3"));

    TwoDimCStrArray_AppendDataToNewestString(container, " Appended data", strlen(" Appended data"));

    _Bool result = true;

    if (strncmp(container->data [container->next_free_c_str - 1], (char*) "TEST 3 Appended data",
            container->allocated_c_str_length [container->next_free_c_str - 1]) != 0) { result = false; }
    printf ("\"%s\"\nand\n\"%s\"\nare equal\n", container->data [container->next_free_c_str - 1], "TEST 3 Appended data");


    TwoDimCStrArray_DeleteObject(container);
    container = NULL;

    ASSERT_EQUALS(true, result);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TEST_AppendDataToSpecificString (void)
{
    struct Two_Dim_C_String_Array* container = TwoDimCStrArray_CreateObject(10);
    ASSERT_MSG(container != NULL, "Created container is NULL !");

    char* first_string = (char*) "First string; ";
    char* new_string = (char*) "I am the new appended string ...";
    const size_t str_id = 5;

    char expected_result [100];
    size_t mem_left = COUNT_ARRAY_ELEMENTS(expected_result) - 1;
    ASSERT_FMSG(COUNT_ARRAY_ELEMENTS(expected_result) >= (strlen(first_string) + strlen(new_string) + 1),
            "String is too short. Given length %zu; Min. necessary: %zu", COUNT_ARRAY_ELEMENTS(expected_result),
            strlen(first_string) + strlen(new_string) + 1);

    strncpy (expected_result, first_string, mem_left);
    mem_left -= strlen(first_string);
    strncat (expected_result, new_string, mem_left);

    TwoDimCStrArray_AppendDataToSpecificString(container, str_id, first_string, strlen(first_string));
    TwoDimCStrArray_AppendDataToSpecificString(container, str_id, new_string, strlen(new_string));

    _Bool result = true;

    if (strncmp(expected_result, container->data [str_id], strlen(expected_result)) != 0) { result = false; }
    printf ("\"%s\"\nand\n\"%s\"\nare equal\n", expected_result, container->data [str_id]);

    TwoDimCStrArray_DeleteObject(container);
    container = NULL;

    ASSERT_EQUALS(true, result);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
