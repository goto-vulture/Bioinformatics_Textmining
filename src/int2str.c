/**
 * @file int2str.c
 *
 * @brief Converting function: Int -> C-String.
 *
 * @date 09.12.2021
 * @author x86 / Gyps
 */

#include "int2str.h"
#include "Misc.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>



//---------------------------------------------------------------------------------------------------------------------

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
enum int2str_errno
int2str
(
        char* const output_string,
        const size_t output_string_size,
        const long int input
)
{
    if (output_string == NULL)
    {
        return INT2STR_INCONVERTIBLE;
    }
    if (output_string_size < 2)
    {
        return INT2STR_INCONVERTIBLE;
    }

    memset (output_string, '\0', output_string_size);
    snprintf (output_string, output_string_size - 1, "%ld", input);

    // Created snprintf an output ?
    if (output_string [0] == '\0' || isspace (output_string [0]))
    {
        return INT2STR_INCONVERTIBLE;
    }

    // Gurantee a terminator symbol at the end of the C string
    output_string [output_string_size - 1] = '\0';

    return INT2STR_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------

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
char*
int2str_wo_errno
(
        const long int input
)
{
    // 25 Char are enough for 32 and 64 bit integer values
    // Max. possible (unsigned) 64 bit integer: 18.446.744.073.709.551.615 (20 + 1 char)
    // The last 4 bytes are for padding purposes !
    static char static_result [25];
    memset (static_result, '\0', sizeof(static_result));

    const enum int2str_errno conversion_status = int2str(static_result, COUNT_ARRAY_ELEMENTS(static_result) - 1, input);

    return (conversion_status == INT2STR_SUCCESS) ? static_result : NULL;
}

//---------------------------------------------------------------------------------------------------------------------
