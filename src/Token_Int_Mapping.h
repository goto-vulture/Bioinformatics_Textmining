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
     * @brief Allocated number of tokens for every C-String. This means, that every C-String has the same allocated
     * size !
     */
    size_t allocated_c_strings_in_array;

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



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TOKEN_INT_MAPPING_H */
