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

/**
 * @brief Eine sehr sehr sehr einfache "Hash-Funktion".
 *
 * Diese wird verwendet, um die Tokens auf die Listen zu verteilen. Als Verfahren wird die ASCII Darstellung jedes
 * Zeichen der Zeichenkette aufaddiert. Der ganzzahlige Rest mit 100 (Modulo 100) ist der "Hash-Wert".
 *
 * @param[in] input_str Eingabezeichenkette, wozu der "Hash-Wert" berechnet wird
 * @param[in] input_str_length Laenge der Eingabezeichenkette
 */
static inline uint_fast32_t
Pseudo_Hash_Function
(
        const char* const input_str,
        const size_t input_str_length
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Neues Token_Int_Mapping-Objekt dynamisch erzeugen.
 */
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
        new_object->int_mapping [i] = (uint_fast32_t*) CALLOC(C_STR_ALLOCATION_STEP_SIZE, sizeof (uint_fast32_t) * MAX_TOKEN_LENGTH);
        ASSERT_ALLOC(new_object->c_str_arrays [i], "Cannot allocate memory for the token int mapping !",
                C_STR_ALLOCATION_STEP_SIZE * sizeof (uint_fast32_t) * MAX_TOKEN_LENGTH);
    }

    new_object->allocated_c_strings_in_array = C_STR_ALLOCATION_STEP_SIZE;

    return new_object;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Dynamisch erzeugtes Token_Int_Mapping-Objekt loeschen.
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 */
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
        FREE_AND_SET_TO_NULL(object->int_mapping [i]);
    }
    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Token zur Mapping Tabelle hinzufuegen.
 *
 * Der Rueckgabewert ist ein Flag, welches anzeigt, ob die Operation erfolgreich war. Die Operation ist nicht
 * erfolgreich, wenn das neue Token bereits in der Mapping Tabelle ist. In diesem Fall wird es NICHT nochmal eingebaut !
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 * @param[in] new_token Neues Token
 * @param[in] new_token_length Laenge des neuen Tokens
 *
 * @return Flag, welches anzeigt, ob die Operation erfolgreich war
 */
extern _Bool
Add_Token_To_Mapping
(
        struct Token_Int_Mapping* const restrict object,
        const char* const restrict new_token,
        const size_t new_token_length
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");
    ASSERT_MSG(new_token != NULL, "New token is NULL !");
    ASSERT_MSG(new_token_length > 0, "New token has the length 0 !");


    const uint_fast32_t chosen_c_string_array = Pseudo_Hash_Function (new_token, new_token_length);
    char* to_str = object->c_str_arrays [chosen_c_string_array];
    uint_fast32_t* int_mapping_array = object->int_mapping [chosen_c_string_array];

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

            uint_fast32_t* tmp_ptr_2 = (uint_fast32_t*) REALLOC(object->int_mapping [i],
                    (old_size + C_STR_ALLOCATION_STEP_SIZE) * MAX_TOKEN_LENGTH);
            ASSERT_ALLOC(tmp_ptr_2, "Cannot reallocate memory for token to int mapping data !",
                    (old_size + C_STR_ALLOCATION_STEP_SIZE) * MAX_TOKEN_LENGTH);
            //memset(tmp_ptr_2, '\0', (old_size + C_STR_ALLOCATION_STEP_SIZE) * MAX_TOKEN_LENGTH);

            object->c_str_arrays [i] = tmp_ptr;
            object->int_mapping [i] = tmp_ptr_2;
            object->allocated_c_strings_in_array += C_STR_ALLOCATION_STEP_SIZE;
        }

        PRINTF_FFLUSH("Token to int realloc. From %zu to %zu objects (%zu times)\n", old_size,
                old_size + C_STR_ALLOCATION_STEP_SIZE, token_to_int_realloc_counter);
    }

    // Ist das Token bereits in der Liste ?
    _Bool token_already_in_list = false;
    for (uint_fast32_t i = 0; i < object->c_str_array_lengths [chosen_c_string_array]; ++ i)
    {
        if (strncmp (new_token, &(to_str [i * MAX_TOKEN_LENGTH]), new_token_length) == 0)
        {
            token_already_in_list = true;
            break;
        }
    }

    if (! token_already_in_list)
    {
        strncpy (&(to_str [object->c_str_array_lengths [chosen_c_string_array] * MAX_TOKEN_LENGTH]),
                new_token, ((new_token_length >= MAX_TOKEN_LENGTH) ? MAX_TOKEN_LENGTH - 1 : new_token_length));

        // Nullterminierung garantieren
        to_str [((object->c_str_array_lengths [chosen_c_string_array] + 1) * MAX_TOKEN_LENGTH) - 1] = '\0';
        object->c_str_array_lengths [chosen_c_string_array] ++;

        uint_fast32_t count_c_str_array_lengths = 0;
        for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
        {
            count_c_str_array_lengths += object->c_str_array_lengths [i];
        }
        int_mapping_array [object->c_str_array_lengths [chosen_c_string_array] - 1] = count_c_str_array_lengths;
    }
    else
    {
        //printf ("\"%s\" already in list !\n", new_token);
    }

    return ! token_already_in_list;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Anzahl an Tokens in allen Untercontainern ausgeben.
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 */
extern void
Show_C_Str_Array_Usage
(
        const struct Token_Int_Mapping* const object
)
{
    uint_fast32_t sum_token = 0;
    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        printf ("Array %3zu: %4" PRIuFAST32 "\n", i, object->c_str_array_lengths [i]);
        sum_token += object->c_str_array_lengths [i];
    }
    printf ("Sum tokens: %" PRIuFAST32 "\n\n", sum_token);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Den gemappten Wert fuer das uebergebene Token ermitteln und zurueckgeben.
 *
 * @param[in] objekt Token_Int_Mapping-Objekt
 * @param[in] search_token Token wofuer der gemappte Wert gesucht wird
 * @param[in] search_token_length Laenge vom uebergebenen Token
 *
 * @return Gemappter Wert oder UINT_FAST32_MAX, falls das Token nicht in der Mapping-Liste vorhanden ist.
 */
extern uint_fast32_t
Token_To_Int
(
        const struct Token_Int_Mapping* const restrict object,
        const char* const restrict search_token,
        const size_t search_token_length
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");
    ASSERT_MSG(search_token != NULL, "New token is NULL !");
    ASSERT_MSG(search_token_length > 0, "New token has the length 0 !");

    uint_fast32_t result = UINT_FAST32_MAX;
    const uint_fast32_t chosen_c_string_array = Pseudo_Hash_Function (search_token, search_token_length);
    _Bool token_found = false;
    uint_fast32_t i = 0;
    char* c_string_array = object->c_str_arrays [chosen_c_string_array];

    // Im berechneten C-String-Array nach dem Token suchen
    for (; i < object->c_str_array_lengths [chosen_c_string_array]; ++ i)
    {
        if (strncmp (search_token, &(c_string_array [i * MAX_TOKEN_LENGTH]), search_token_length) == 0)
        {
            token_found = true;
            break;
        }
    }

    // Nun bestimmen welche Position das Token bezogen auf alle C-String Arrays besitzt. Dies ist dann der gemappte
    // Wert
    if (token_found)
    {
        result = object->int_mapping [chosen_c_string_array][i];
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

extern void
Int_To_Token
(
        const struct Token_Int_Mapping* const restrict object,
        const uint_fast32_t token_int_value,
        char* const restrict result_token_memory,
        const size_t result_token_memory_size
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");

    // Da wir keine weiteren Informationen haben, muessen wir alle Int Arrays durchsuchen
    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        for (size_t i2 = 0; i2 < object->allocated_c_strings_in_array; ++ i)
        {
            char* c_string_array = object->c_str_arrays [i];

            if (object->int_mapping [i][i2] == token_int_value)
            {
                strncpy(result_token_memory, &(c_string_array [i2 * MAX_TOKEN_LENGTH]), result_token_memory_size - 1);
                break;
            }
        }
    }

    return;
}

//=====================================================================================================================

/**
 * @brief Eine sehr sehr sehr einfache "Hash-Funktion".
 *
 * Diese wird verwendet, um die Tokens auf die Listen zu verteilen. Als Verfahren wird die ASCII Darstellung jedes
 * Zeichen der Zeichenkette aufaddiert. Der ganzzahlige Rest mit 100 (Modulo 100) ist der "Hash-Wert".
 *
 * @param[in] input_str Eingabezeichenkette, wozu der "Hash-Wert" berechnet wird
 * @param[in] input_str_length Laenge der Eingabezeichenkette
 */
static inline uint_fast32_t
Pseudo_Hash_Function
(
        const char* const input_str,
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
