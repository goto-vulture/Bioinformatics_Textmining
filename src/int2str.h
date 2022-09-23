/**
 * @file int2str.h
 *
 * @brief Converting function: Int -> C-String.
 *
 * @date 09.12.2021
 * @author x86 / Gyps
 */

#ifndef INT2STR_H
#define INT2STR_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>     // size_t



/**
 * @brief Error codes for the function int2str().
 */
enum int2str_errno
{
    INT2STR_SUCCESS = 0,    ///< Converting successful war
    INT2STR_INCONVERTIBLE   ///< Number is not convertible
};



/**
 * @brief Convert long int (mind. 32 Bit) to C-String.
 *
 * Incorrect input are indicated with the return value. (INT2STR_INCONVERTIBLE)
 *
 * @param[out] output_string Output C-String
 * @param[in] output_string_size Max length of the output string
 * @param[in] input Input integer
 *
 * @return Error code (INT2STR_INCONVERTIBLE) on failure. On success: INT2STR_SUCCESS
 */
extern enum int2str_errno int2str (char* const output_string, const size_t output_string_size, const long int input);

/**
 * @brief Convert long int (mind. 32 Bit) to C-String.
 *
 * The differences to "int2str()":
 * - This function uses a static C-String for the result. Therefore the caller does not have to allocate memory
 *   beforehand.
 * - There is no error enum as return value. Errors will be indicate with a NULL pointer.
 *
 * @param[in] input Input integer
 *
 * @return Address to the static result C-String; or, in case of errors, a NULL pointer
 */
extern char* int2str_wo_errno (const long int input);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INT2STR_H */
