/**
 * @file Token_Int_Mapping.c
 *
 * @brief The Token_Int_Mapping is an object, which maps a token to a unique unsigned value.
 *
 * This process is for the determination of a intersection not required. But with larger datasets it will be improve
 * the performace, because the intersection process needs only to compare (unsigned) int values rather than full
 * C-Strings char by char.
 *
 * Of course after the intersection it is necessary to invert the mapping. (This could be done with "Int_To_Token()")
 *
 * @date 23.06.2022
 * @author Gyps
 */

#include "Token_Int_Mapping.h"
#include <string.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Print_Tools.h"
#include "File_Reader.h"



/**
 * @brief Memory for this number of tokens per C-String.
 *
 * It is also the allocation step size, if a reallocation is necessary.
 */
#ifndef C_STR_ALLOCATION_STEP_SIZE
#define C_STR_ALLOCATION_STEP_SIZE 350
#else
#error "The macro \"C_STR_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* C_STR_ALLOCATION_STEP_SIZE */

/**
 * @brief A very very very simple "hash function".
 *
 * This function is used to calculate a number for distributing the tokens in different C-Strings.
 *
 * All char of the token will be interpreted as ASCII integer, added and modulo C_STR_ARRAYS.
 *
 * Asserts:
 *      input_str != NULL
 *      input_str_length > 0
 *
 * @param[in] input_str Input C-String
 * @param[in] input_str_length Length of the input string
 *
 * @return The result
 */
static inline uint_fast32_t
Pseudo_Hash_Function
(
        const char* const input_str,
        const size_t input_str_length
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Create new dynamic Token_Int_Mapping object.
 *
 * Asserts:
 *      N/A
 *
 * @return Pointer to the new object
 */
extern struct Token_Int_Mapping*
Create_Token_Int_Mapping
(
        void
)
{
    // Outer object
    struct Token_Int_Mapping* new_object = (struct Token_Int_Mapping*) CALLOC(1, sizeof (struct Token_Int_Mapping));
    ASSERT_ALLOC(new_object, "Cannot create a new token int mapping !", sizeof (struct Token_Int_Mapping));

    // Inner C-Strings and memory for the mapping integer values
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
 * @brief Delete a dynamic allocated Token_Int_Mapping object.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Token_Int_Mapping object
 */
extern void
Delete_Token_Int_Mapping
(
        struct Token_Int_Mapping* object
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");

    // Delete the inner objects
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
 * @brief Add token to the mapping object.
 *
 * The return value is an flag and indicates whether the operation was successful. The process is not successful, if
 * the token already exists in the mapping table. In this case there will be NO duplicate in the mapping table.
 *
 * Asserts:
 *      object != NULL
 *      new_token != NULL
 *      new_token_length > 0
 *
 * @param[in] object Token_Int_Mapping object
 * @param[in] new_token New token
 * @param[in] new_token_length Length of the new token
 *
 * @return Flag, which indicates, whether the operation was successful.
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

    // Is more memory necessary to hold the new token ? Yes: Realloc the memory
    if (object->c_str_array_lengths [chosen_c_string_array] >= object->allocated_c_strings_in_array)
    {
        static size_t token_to_int_realloc_counter = 0;
        ++ token_to_int_realloc_counter;

        const size_t old_size = object->allocated_c_strings_in_array;

        // For all C-Strings the memory will be extended
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

            object->c_str_arrays [i] = tmp_ptr;
            object->int_mapping [i] = tmp_ptr_2;
            object->allocated_c_strings_in_array += C_STR_ALLOCATION_STEP_SIZE;
        }

        PRINTF_FFLUSH("Token to int realloc. From %zu to %zu objects (%zu times)\n", old_size,
                old_size + C_STR_ALLOCATION_STEP_SIZE, token_to_int_realloc_counter);
    }

    char* to_str = object->c_str_arrays [chosen_c_string_array];
    uint_fast32_t* int_mapping_array = object->int_mapping [chosen_c_string_array];

    // Is the new token already in the list ?
    _Bool token_already_in_list = false;
    for (uint_fast32_t i = 0; i < object->c_str_array_lengths [chosen_c_string_array]; ++ i)
    {
        // Before the comparison: Have the strings the same length ?
        if (new_token_length != strlen(&(to_str [i * MAX_TOKEN_LENGTH]))) { continue; }

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

        // Gurantee a zero byte at the end
        to_str [((object->c_str_array_lengths [chosen_c_string_array] + 1) * MAX_TOKEN_LENGTH) - 1] = '\0';
        object->c_str_array_lengths [chosen_c_string_array] ++;

        uint_fast32_t count_c_str_array_lengths = 0;
        for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
        {
            count_c_str_array_lengths += object->c_str_array_lengths [i];
        }
        int_mapping_array [object->c_str_array_lengths [chosen_c_string_array] - 1] = count_c_str_array_lengths;
    }

    return ! token_already_in_list;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print the number of tokens in all C-Strings.
 *
 * This function is especially useful in the debugging.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Token_Int_Mapping object
 */
extern void
Show_C_Str_Array_Usage
(
        const struct Token_Int_Mapping* const object
)
{
    ASSERT_MSG(object != NULL, "Token_Int_Mapping object is NULL !");

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
 * @brief Determine the integer value for the given token. (token -> int)
 *
 * Asserts:
 *      object != NULL
 *      search_token != NULL
 *      search_token_length > 0
 *
 * @param[in] object Token_Int_Mapping object
 * @param[in] search_token Given token
 * @param[in] search_token_length Token length
 *
 * @return Mapping integer or UINT_FAST32_MAX, if the token is not in the mapping list
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

    // Search in the C-String, where the token would be, when it is already in the mapping list
    for (; i < object->c_str_array_lengths [chosen_c_string_array]; ++ i)
    {
        // Before the comparison: Have the strings the same length ?
        if (search_token_length != strlen(&(c_string_array [i * MAX_TOKEN_LENGTH]))) { continue; }

        if (strncmp (search_token, &(c_string_array [i * MAX_TOKEN_LENGTH]), search_token_length) == 0)
        {
            token_found = true;
            break;
        }
    }

    // If the token is in the list, get the mapped integer value
    if (token_found)
    {
        result = object->int_mapping [chosen_c_string_array][i];
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Reverse the mapping. int -> token
 *
 * Asserts:
 *      object != NULL
 *      token_int_value != UINT_FAST32_MAX
 *      result_token_memory != NULL
 *      result_token_memory_size > 0
 *
 * @param[in] object Token_Int_Mapping objckt
 * @param[in] token_int_value Mapped integer
 * @param[out] result_token_memory Memory, where the result token starts
 * @param[in] result_token_memory_size length of the result token
 */
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
    ASSERT_MSG(token_int_value != UINT_FAST32_MAX, "Token integer value is UINT_FAST32_MAX ! This value indicates "
            "errors and therefor cannot be a valid input !");
    ASSERT_MSG(result_token_memory != NULL, "The result token memory is NULL !");
    ASSERT_MSG(result_token_memory_size > 0, "The length of the result token memory is 0 !");

    // In the reverse process we have no further knowlede where the token might be. So it is necessary to search in
    // every C-String
    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        for (size_t i2 = 0; i2 < object->c_str_array_lengths [i]; ++ i2)
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
 * @brief A very very very simple "hash function".
 *
 * This function is used to calculate a number for distributing the tokens in different C-Strings.
 *
 * All char of the token will be interpreted as ASCII integer, added and modulo C_STR_ARRAYS.
 *
 * Asserts:
 *      input_str != NULL
 *      input_str_length > 0
 *
 * @param[in] input_str Input C-String
 * @param[in] input_str_length Length of the input string
 *
 * @return The result
 */
static inline uint_fast32_t
Pseudo_Hash_Function
(
        const char* const input_str,
        const size_t input_str_length
)
{
    ASSERT_MSG(input_str != NULL, "Input string is NULL !");
    ASSERT_MSG(input_str_length > 0, "Input string length is 0 !");

    uint_fast32_t sum_char_in_new_token = 0;
    // Add char per char
    for (size_t i = 0; i < input_str_length; ++ i)
    {
        sum_char_in_new_token += (uint_fast32_t) input_str [i];
    }

    return (sum_char_in_new_token % C_STR_ARRAYS);
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef MAX_TOKEN_LENGTH
#undef MAX_TOKEN_LENGTH
#endif /* MAX_TOKEN_LENGTH */

#ifdef C_STR_ALLOCATION_STEP_SIZE
#undef C_STR_ALLOCATION_STEP_SIZE
#endif /* C_STR_ALLOCATION_STEP_SIZE */
