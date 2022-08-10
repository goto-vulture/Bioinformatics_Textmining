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
#include "Misc.h"



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
 * @brief Check, whether the macro values are valid.
 */
#if __STDC_VERSION__ >= 201112L
_Static_assert(C_STR_ALLOCATION_STEP_SIZE > 0, "The marco \"C_STR_ALLOCATION_STEP_SIZE\" is zero !");
#endif /* __STDC_VERSION__ */

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

        new_object->int_mapping [i] = (uint_fast32_t*) CALLOC(C_STR_ALLOCATION_STEP_SIZE, sizeof (uint_fast32_t) * 1);
        ASSERT_ALLOC(new_object->c_str_arrays [i], "Cannot allocate memory for the token int mapping !",
                C_STR_ALLOCATION_STEP_SIZE * sizeof (uint_fast32_t) * 1); // NO MAX_TOKEN_LENGTH !

        new_object->allocated_c_strings_in_array [i] = C_STR_ALLOCATION_STEP_SIZE;
    }

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
 *
 * How the mapping integers works:
 * The first two digits are equal with the integer mapping array index.
 *
 * The reason:
 * For the reverse mapping process (int -> token) it is necessary, that in the integer values the corresponding
 * integer mapping array index is coded. This will be done with the first two digits. You know with a int
 * value - e.g. a 1942 - that this value can only occur in the integer mapping array with the index 42.
 *
 * Okay. But for adding a token in the mapping we need to encode the knowledge of our chosen C-String array in
 * the mapped integer value.
 *
 * The idea:
 * - Find the maximum value in the selected integer mapping array. In our example: 1942
 * - Remove the encoding:                                           1942 / 100 (C_STR_ARRAYS)   -> 19
 * - Increment the value:                                           19 + 1                      -> 20
 * - Shift the value two digits (in decimal system) to the left:    20 * 100 (C_STR_ARRAYS)     -> 2000
 * - Add the encoding (the chosen C-String array):                  2000 + 42                   -> 2042
 *
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
    if (object->c_str_array_lengths [chosen_c_string_array] >= object->allocated_c_strings_in_array [chosen_c_string_array])
    {
        static size_t token_to_int_realloc_counter = 0;
        ++ token_to_int_realloc_counter;

        const size_t old_size = object->allocated_c_strings_in_array [chosen_c_string_array];
        const size_t new_size = old_size + C_STR_ALLOCATION_STEP_SIZE;
        const size_t new_c_string_array_size    = new_size * MAX_TOKEN_LENGTH * sizeof (char);
        const size_t new_int_mapping_array_size = new_size * 1 * sizeof (uint_fast32_t); // NO MAX_TOKEN_LENGTH !

        // Reallocate the c strings and the int mapping memory
        char* tmp_ptr = (char*) REALLOC(object->c_str_arrays [chosen_c_string_array], new_c_string_array_size);
        ASSERT_ALLOC(tmp_ptr, "Cannot reallocate memory for token to int mapping data !", new_c_string_array_size);
        memset(tmp_ptr + (old_size * MAX_TOKEN_LENGTH), '\0', C_STR_ALLOCATION_STEP_SIZE * MAX_TOKEN_LENGTH *
                sizeof (char));

        uint_fast32_t* tmp_ptr_2 = (uint_fast32_t*) REALLOC(object->int_mapping [chosen_c_string_array], new_int_mapping_array_size);
        ASSERT_ALLOC(tmp_ptr_2, "Cannot reallocate memory for token to int mapping data !", new_int_mapping_array_size);
        memset(tmp_ptr_2 + (old_size), '\0', C_STR_ALLOCATION_STEP_SIZE * 1 * sizeof (uint_fast32_t)); // NO MAX_TOKEN_LENGTH !

        object->c_str_arrays [chosen_c_string_array]    = tmp_ptr;
        object->int_mapping [chosen_c_string_array]     = tmp_ptr_2;

        object->allocated_c_strings_in_array [chosen_c_string_array] = new_size;

        //PRINTF_FFLUSH("Token to int realloc. From %zu to %zu objects (%zu times)\n", old_size, new_size,
        //        token_to_int_realloc_counter);
    }

    char* start_to_str = object->c_str_arrays [chosen_c_string_array];
    char* to_str = start_to_str;
    uint_fast32_t* int_mapping_array = object->int_mapping [chosen_c_string_array];

    // Is the new token already in the list ?
    _Bool token_already_in_list = false;
    for (uint_fast32_t i = 0; i < object->c_str_array_lengths [chosen_c_string_array]; ++ i)
    {
        // Before the comparison: Have the strings the same length ?
        //if (new_token_length != strlen(&(to_str [i * MAX_TOKEN_LENGTH]))) { continue; }

        //if (strncmp (new_token, &(to_str [i * MAX_TOKEN_LENGTH]), new_token_length) == 0)
        // Pre check the first char to avoid strncmp calls
        if (new_token [0] == to_str [0])
        {
            if (strncmp (new_token, to_str, new_token_length) == 0)
            {
                token_already_in_list = true;
                break;
            }
        }
        to_str += MAX_TOKEN_LENGTH;
    }
    to_str = start_to_str;

    if (! token_already_in_list)
    {
        strncpy (&(to_str [object->c_str_array_lengths [chosen_c_string_array] * MAX_TOKEN_LENGTH]),
                new_token, MAX_TOKEN_LENGTH - 1);// ((new_token_length >= MAX_TOKEN_LENGTH) ? MAX_TOKEN_LENGTH - 1 : new_token_length));

        // Gurantee a zero byte at the end
        //to_str [((object->c_str_array_lengths [chosen_c_string_array] + 1) * MAX_TOKEN_LENGTH) - 1] = '\0';

        // Find the max. mapping integer in the chosen array
        uint_fast32_t max_mapping_int_in_chosen_array = 0;
        /*for (uint_fast32_t i = 0; i < object->c_str_array_lengths [chosen_c_string_array]; ++ i)
        {
            if (int_mapping_array [i] > max_mapping_int_in_chosen_array)
            {
                max_mapping_int_in_chosen_array = int_mapping_array [i];
            }
        }*/
        max_mapping_int_in_chosen_array /= C_STR_ARRAYS;            // Remove encoding
        ++ max_mapping_int_in_chosen_array;                         // Increment value
        max_mapping_int_in_chosen_array *= C_STR_ARRAYS;            // Shift two digits (in decimal system) to the left
        max_mapping_int_in_chosen_array += chosen_c_string_array;   // Add the encoding in the fist two digits
        // For a detailed description about the encoding system: See the function comment !

                /*object->c_str_array_lengths [chosen_c_string_array] + (chosen_c_string_array * C_STR_ARRAYS);
        if (count_c_str_array_lengths / C_STR_ARRAYS >= C_STR_ARRAYS)
            printf ("\n%d || %d\n", count_c_str_array_lengths, count_c_str_array_lengths / C_STR_ARRAYS);*/

        int_mapping_array [object->c_str_array_lengths [chosen_c_string_array]] = max_mapping_int_in_chosen_array;
        object->c_str_array_lengths [chosen_c_string_array] ++;
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

    // Find the maximum values (used elements and allocated sizes)
    size_t max_allocated                = 0;
    size_t max_used                     = 0;
    uint_fast32_t sum_allocated_tokens  = 0;
    uint_fast32_t sum_tokens            = 0;
    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        if (object->allocated_c_strings_in_array [i] > max_allocated)
        {
            max_allocated = object->allocated_c_strings_in_array [i];
        }
        if (object->c_str_array_lengths [i] > max_used)
        {
            max_used = object->c_str_array_lengths [i];
        }
        sum_allocated_tokens += object->allocated_c_strings_in_array [i];
        sum_tokens += object->c_str_array_lengths [i];
    }

    for (size_t i = 0; i < C_STR_ARRAYS; ++ i)
    {
        printf ("[%2zu] allocated: %*zu | used: %*" PRIuFAST32 " (%.2f %% used)\n", i,
                (int) Count_Number_Of_Digits(max_allocated), max_allocated, (int) Count_Number_Of_Digits(max_used),
                object->c_str_array_lengths [i], Determine_Percent(object->c_str_array_lengths [i],
                        object->allocated_c_strings_in_array [i]));
    }
    printf ("Sum allocated tokens: %" PRIuFAST32 "\n", sum_allocated_tokens);
    printf ("Sum used tokens:      %" PRIuFAST32 " (%.2f %% used)\n\n", sum_tokens,
            Determine_Percent(sum_tokens, sum_allocated_tokens));

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
        //if (search_token_length != strlen(&(c_string_array [i * MAX_TOKEN_LENGTH]))) { continue; }

        //if (strncmp (search_token, &(c_string_array [i * MAX_TOKEN_LENGTH]), search_token_length) == 0)
        // Pre check the first char to avoid strncmp calls
        if (search_token [0] == c_string_array [0])
        {
            if (strncmp (search_token, c_string_array, search_token_length) == 0)
            {
                token_found = true;
                break;
            }
        }
        c_string_array += MAX_TOKEN_LENGTH;
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
 * In the integer numbers the corresponding index for the integer mapping array is encoded in the first two digits of
 * the integer value.
 *
 * E.g.: The value 2911 can only appear in the integer mapping array with the index 11. It is NOT necessary to search in
 *       all arrays.
 *
 * So the int value indicates in which array is the corresponding C-String
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

    // Use the encoded information (the first two digits) to find the corresponding index for the integer mapping array
    // The idea: With this, it is not necessary to search in the whole mapping data to find the specific information
    //           Especially when the mapped integer is not in the list, it will costs many computation time
    const uint_fast32_t chosen_c_string_array = token_int_value % C_STR_ARRAYS;

    const char* c_string_array = object->c_str_arrays [chosen_c_string_array];

    for (size_t i2 = 0; i2 < object->c_str_array_lengths [chosen_c_string_array]; ++ i2)
    {
        if (object->int_mapping [chosen_c_string_array][i2] == token_int_value)
        {
            strncpy(result_token_memory, &(c_string_array [i2 * MAX_TOKEN_LENGTH]), result_token_memory_size - 1);
            break;
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
