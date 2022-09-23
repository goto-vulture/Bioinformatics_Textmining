/**
 * @file str2int.c
 *
 * @brief Converting function C-String -> int.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#include "str2int.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>



//---------------------------------------------------------------------------------------------------------------------

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
extern enum str2int_errno
str2int
(
        long int* restrict out,
        const char* restrict input_string,
        const int base
)
{
    char* end = NULL;

    if (input_string == NULL)
    {
        return STR2INT_INCONVERTIBLE;
    }
    if (input_string [0] == '\0' || isspace (input_string [0]))
    {
        return STR2INT_INCONVERTIBLE;
    }
    if (base <= 0)
    {
        return STR2INT_INCONVERTIBLE;
    }
    if (out == NULL)
    {
        return STR2INT_INCONVERTIBLE;
    }
    for (size_t i = 0; i < strlen (input_string); ++ i)
    {
        if (! isdigit (input_string [i]))
        {
            return STR2INT_INCONVERTIBLE;
        }
    }

    errno = 0;
    const long int l = strtol (input_string, &end, base);

    if (l > LONG_MAX || (errno == ERANGE && l == LONG_MAX))
    {
        return STR2INT_OVERFLOW;
    }
    if (l < LONG_MIN || (errno == ERANGE && l == LONG_MIN))
    {
        return STR2INT_UNDERFLOW;
    }
    if (*end != '\0')
    {
        return STR2INT_INCONVERTIBLE;
    }

    *out = l;

    return STR2INT_SUCCESS;
}

extern enum str2uint_errno
str2uint
(
        unsigned long int* out,
        const char* input_string,
        const int base
)
{
    char* end = NULL;

    if (input_string == NULL)
    {
        return STR2UINT_INCONVERTIBLE;
    }
    if (input_string [0] == '\0' || isspace (input_string [0]))
    {
        return STR2UINT_INCONVERTIBLE;
    }
    if (base <= 0)
    {
        return STR2UINT_INCONVERTIBLE;
    }
    if (out == NULL)
    {
        return STR2UINT_INCONVERTIBLE;
    }
    for (size_t i = 0; i < strlen (input_string); ++ i)
    {
        if (! isdigit (input_string [i]))
        {
            return STR2UINT_INCONVERTIBLE;
        }
    }

    errno = 0;
    const unsigned long int lu = strtoul (input_string, &end, base);

    if (lu > ULONG_MAX || (errno == ERANGE && lu == ULONG_MAX))
    {
        return STR2UINT_OVERFLOW;
    }
    if (/*lu < 0 ||*/ (errno == ERANGE && lu == 0))
    {
        return STR2UINT_UNDERFLOW;
    }
    if (*end != '\0')
    {
        return STR2UINT_INCONVERTIBLE;
    }

    *out = lu;

    return STR2UINT_SUCCESS;
}

extern enum str2double_errno str2double
(
        double* out,
        const char* input_string
)
{
    char* end = NULL;

    if (input_string == NULL)
    {
        return STR2DOUBLE_INCONVERTIBLE;
    }
    if (input_string [0] == '\0' || isspace (input_string [0]))
    {
        return STR2DOUBLE_INCONVERTIBLE;
    }

    errno = 0;
    const double d = strtod (input_string, &end);

    if (errno == ERANGE)
    {
        return STR2DOUBLE_INCONVERTIBLE;
    }
    if (*end != '\0')
    {
        return STR2DOUBLE_INCONVERTIBLE;
    }

    *out = d;

    return STR2DOUBLE_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Convert a C-String to a integer with a width of at least 32 bits.
 *
 * The result will be given with the return values. Errors will be indicated with LONG_MIN. So you shoudn't use an
 * input C-String, that represents LONG_MIN. In this case it is not possible to determine whether an error occurred or
 * not.
 *
 * @param[in] input_string Input C-String
 * @param[in] base Number base
 *
 * @return The integer representation of the input C-String or LONG_MIN in case of errors
 */
extern long int
str2int_wo_errno
(
        const char* input_string,
        const int base
)
{
    long int result = 0;
    const enum str2int_errno conversion_status = str2int(&result, input_string, base);

    return (conversion_status == STR2INT_SUCCESS) ? result : LONG_MIN;
}

extern unsigned long int
str2uint_wo_errno
(
        const char* input_string,
        const int base
)
{
    unsigned long int result = 0;
    const enum str2uint_errno conversion_status = str2uint(&result, input_string, base);

    return (conversion_status == STR2UINT_SUCCESS) ? result : 0;
}

extern double
str2d_wo_errno
(
        const char* input_string
)
{
    double result = 0.0;
    const enum str2double_errno conversion_status = strtod(input_string, NULL);

    return (conversion_status == STR2DOUBLE_SUCCESS) ? result : 0.0;
}

//---------------------------------------------------------------------------------------------------------------------
