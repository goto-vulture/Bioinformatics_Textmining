/**
 * @file str2int.h
 *
 * @brief Converting function C-String -> int.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#ifndef STR2INT_H
#define STR2INT_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>



/**
 * @brief Errorcodes for the str2int() usage.
 */
enum str2int_errno
{
    STR2INT_SUCCESS = 0,    ///< Conversion was successful
    STR2INT_OVERFLOW,       ///< Overflow occured
    STR2INT_UNDERFLOW,      ///< Underflow occured
    STR2INT_INCONVERTIBLE   ///< C-String is not convertable
};



/**
 * @brief Convert a C-String to a integer with a width of at least 32 bits.
 *
 * Incorrect inputs are indicated with the return value. In this case the input parameter (out) is not modified.
 *
 * @param[out] out Pointer to the output integer.
 * @param[in] input_string Input C-String
 * @param[in] base Number base
 *
 * @return Error code. On success: STR2INT_SUCCESS; Otherwise != STR2INT_OVERFLOW, STR2INT_UNDERFLOW or
 * STR2INT_INCONVERTIBLE
 */
extern enum str2int_errno str2int (long int* out, const char* input_string, const int base);

/**
 * @brief Convert a C-String to a integer with a width of at least 32 bits.
 *
 * The result will be given with the return values. Errors will be indicated with LONG_MIN. So you shoudn't use an
 * input C-String, that represents LONG_MIN. In this case it is not possible to determine whether an error occured or
 * not.
 *
 * @param[in] input_string Input C-String
 * @param[in] base Number base
 *
 * @return The integer representation of the input C-String or LONG_MIN in case of errors
 */
extern long int str2int_wo_errno (const char* input_string, const int base);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR2INT_H */
