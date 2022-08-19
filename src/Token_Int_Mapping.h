/**
 * @file Token_Int_Mapping.h
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

#ifndef TOKEN_INT_MAPPING_H
#define TOKEN_INT_MAPPING_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>



/**
 * @brief Number of C-Strings per object.
 *
 * This value can be altered to make the memory usage more efficient or to reduce the number of reallocs in the
 * runtime.
 */
#ifndef C_STR_ARRAYS
#define C_STR_ARRAYS 100
#else
#error "The macro \"C_STR_ARRAYS\" is already defined !"
#endif /* C_STR_ARRAYS */

/**
 * @brief Maximum length of a token (inkl. the terminator byte)
 *
 * The value is a approximate order of magnitude and could be change in the future.
 */
#ifndef MAX_TOKEN_LENGTH
#define MAX_TOKEN_LENGTH 64
#else
#error "The macro \"MAX_TOKEN_LENGTH\" is already defined !"
#endif /* MAX_TOKEN_LENGTH */

/**
 * @brief Check, whether the macro values are valid.
 */
#if __STDC_VERSION__ >= 201112L
_Static_assert(C_STR_ARRAYS > 0, "The marco \"C_STR_ARRAYS\" is zero !");
_Static_assert(MAX_TOKEN_LENGTH > 0, "The marco \"MAX_TOKEN_LENGTH\" is zero !");
#endif /* __STDC_VERSION__ */

//=====================================================================================================================

struct Token_Int_Mapping
{
    /**
     * @brief C-String arrays containing the tokens.
     *
     * It is a flat memory model. This means, that one C-String contains multiple tokens. Every token is saved as an
     * interval in the C-String.
     */
    char* c_str_arrays [C_STR_ARRAYS];

    uint_fast32_t* int_mapping [C_STR_ARRAYS];          ///< Mapping integers

    /**
     * @brief Allocated number of tokens for the C-String. This means, that every C-String has individual allocation
     * sizes !
     */
    size_t allocated_c_strings_in_array [C_STR_ARRAYS];

    /**
     * @brief Used number of tokens per C-String.
     */
    uint_fast32_t c_str_array_lengths [C_STR_ARRAYS];
};

//=====================================================================================================================

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
);

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
);

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
);

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
);

/**
 * @brief Print information about the allocated memory size and the used memory size.
 *
 * This function is especially useful in the debugging.
 *
 * Asserts:
 *      object != NULL
 *
 * @param[in] object Token_Int_Mapping object
 */
extern void
Show_Memory_Usage
(
        const struct Token_Int_Mapping* const object
);

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
);

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
);

/**
 * @brief This function is comparable with the Int_To_Token() function. But it uses a static array as result memory.
 *
 * With the address and the length of the static memory this function delegates the data to Int_To_Token().
 *
 * Asserts:
 *      N/A (Int_To_Token() do the checks)
 *
 * @param[in] object Token_Int_Mapping objckt
 * @param[in] token_int_value Mapped integer
 *
 * @return Pointer to the modified static array
 */
extern const char*
Int_To_Token_Static_Mem
(
        const struct Token_Int_Mapping* const object,
        const uint_fast32_t token_int_value
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TOKEN_INT_MAPPING_H */
