/**
 * @file TEST_Two_Dim_C_String_Array.c
 *
 * @date 16.02.2023
 * @author: leer
 */

#include "TEST_Two_Dim_C_String_Array.h"

#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "tinytest.h"



//---------------------------------------------------------------------------------------------------------------------

extern void TEST_AppendNewString (void)
{
    struct Two_Dim_C_String_Array* container = TwoDimCStrArray_CreateObject(10);
    ASSERT_MSG(container != NULL, "Created container is NULL !");

    char* new_string = (char*) "I am the new string ...";

    TwoDimCStrArray_AppendNewString(container, new_string, strlen(new_string));

    _Bool result = true;

    if (strncpy(new_string, container->data [0], strlen(new_string)) != 0) { result = false; }

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

    if (strncpy((char*) "TEST 3 Appended data", container->data [container->next_free_c_str - 1],
            container->allocated_c_str_length [container->next_free_c_str - 1]) != 0)
    {
        result = false;
    }

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

    char* new_string = (char*) "I am the new string ...";
    const size_t str_id = 5;

    TwoDimCStrArray_AppendDataToSpecificString(container, str_id, new_string, strlen(new_string));

    _Bool result = true;

    if (strncpy(new_string, container->data [str_id], strlen(new_string)) != 0) { result = false; }

    TwoDimCStrArray_DeleteObject(container);
    container = NULL;

    ASSERT_EQUALS(true, result);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
