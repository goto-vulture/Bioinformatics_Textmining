/*
 * Token_Int_Mapping.c
 *
 *  Created on: 23.06.2022
 *      Author: am1
 */

#include "Token_Int_Mapping.h"
#include <string.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Print_Tools.h"



// Maximale Laenge eines Tokens (inkl. Terminator-Symbol)
#ifndef MAX_TOKEN_LENGTH
#define MAX_TOKEN_LENGTH 32
#else
#error "The macro \"MAX_TOKEN_LENGTH\" is already defined !"
#endif /* MAX_TOKEN_LENGTH */

// Allokationsgroesse eines C-String Arrays
#ifndef C_STR_ALLOCATION_STEP_SIZE
#define C_STR_ALLOCATION_STEP_SIZE 350
#else
#error "The macro \"C_STR_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* C_STR_ALLOCATION_STEP_SIZE */

static inline uint_fast32_t
Pseudo_Hash_Function
(
        const char* input_str,
        const size_t input_str_length
);

//---------------------------------------------------------------------------------------------------------------------

extern struct Token_Int_Mapping*
Create_Token_Int_Mapping
(
        void
)
{
    // Aeussere Objekt
    struct Token_Int_Mapping* new_object = (struct Token_Int_Mapping*) CALLOC(1, sizeof (struct Token_Int_Mapping));
    ASSERT_ALLOC(new_object, "Cannot create a new token int mapping !", sizeof (struct Token_Int_Mapping));

    // Inneren C-String Arrays
    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        new_object->c_str_arrays [i] = (char*) CALLOC(C_STR_ALLOCATION_STEP_SIZE, sizeof (char) * MAX_TOKEN_LENGTH);
        ASSERT_ALLOC(new_object->c_str_arrays [i], "Cannot allocate memory for the token int mapping !",
                C_STR_ALLOCATION_STEP_SIZE * sizeof (char) * MAX_TOKEN_LENGTH);
    }

    new_object->allocated_c_strings_in_array = C_STR_ALLOCATION_STEP_SIZE;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Delete_Token_Int_Mapping
(
        struct Token_Int_Mapping* object
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");

    // Inneren C-String Arrays
    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        FREE_AND_SET_TO_NULL(object->c_str_arrays [i]);
    }
    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Add_Token_To_Mapping
(
        struct Token_Int_Mapping* object,
        const char* new_token,
        const size_t new_token_length
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");
    ASSERT_MSG(new_token != NULL, "New token is NULL !");
    ASSERT_MSG(new_token_length > 0, "New token has the length 0 !");


    const uint_fast32_t chosen_c_string_array = Pseudo_Hash_Function (new_token, new_token_length);
    char* to_str = object->c_str_arrays [chosen_c_string_array];

    // Wird mehr Speicher fuer den folgenden Prozess benoetigt ?
    if (object->c_str_array_lengths [chosen_c_string_array] >= object->allocated_c_strings_in_array)
    {
        static size_t token_to_int_realloc_counter = 0;
        ++ token_to_int_realloc_counter;

        const size_t old_size = object->allocated_c_strings_in_array;

        // Fuer alle C-String Arrays den Speicher vergroessern
        for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
        {
            char* tmp_ptr = (char*) REALLOC(object->c_str_arrays [i],
                    (old_size + C_STR_ALLOCATION_STEP_SIZE) * MAX_TOKEN_LENGTH);
            ASSERT_ALLOC(tmp_ptr, "Cannot reallocate memory for token to int mapping data !",
                    (old_size + C_STR_ALLOCATION_STEP_SIZE) * MAX_TOKEN_LENGTH);
            memset(tmp_ptr, '\0', (old_size + C_STR_ALLOCATION_STEP_SIZE) * MAX_TOKEN_LENGTH);

            object->c_str_arrays [i] = tmp_ptr;
            object->allocated_c_strings_in_array += C_STR_ALLOCATION_STEP_SIZE;
        }

        PRINTF_FFLUSH("Token to int realloc. From %zu to %zu objects (%zu times)\n", old_size,
                old_size + C_STR_ALLOCATION_STEP_SIZE, token_to_int_realloc_counter);
    }

    strncpy (&(to_str [object->c_str_array_lengths [chosen_c_string_array] * MAX_TOKEN_LENGTH]),
            new_token, ((new_token_length >= MAX_TOKEN_LENGTH) ? MAX_TOKEN_LENGTH - 1 : new_token_length));

    // Nullterminierung garantieren
    to_str [((object->c_str_array_lengths [chosen_c_string_array] + 1) * MAX_TOKEN_LENGTH) - 1] = '\0';
    object->c_str_array_lengths [chosen_c_string_array] ++;

    return;
}

//=====================================================================================================================

static inline uint_fast32_t
Pseudo_Hash_Function
(
        const char* input_str,
        const size_t input_str_length
)
{
    uint_fast32_t sum_char_in_new_token = 0;
    // Zeichen der Zeichenkette aufaddieren
    for (size_t i = 0; i < input_str_length; ++ i)
    {
        sum_char_in_new_token += (uint_fast32_t) input_str [i];
    }

    return (sum_char_in_new_token % 100);
}

//---------------------------------------------------------------------------------------------------------------------
