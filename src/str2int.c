/**
 * @file str2int.c
 *
 * @brief Konvertierungsfunktion C-String -> int.
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

/*
 * C-String zu long int konvertieren.
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt.
 */
enum str2int_errno                          // Error-enum (STR2INT_SUCCESS: Erfolgreich durchgefuehrt)
str2int
(
        long int* restrict out,             // Ausgabeadresse
        const char* restrict input_string,  // String, der konvertiert werden soll
        const int base                      // Zahlensystem
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

//---------------------------------------------------------------------------------------------------------------------

/*
 * C-String zu long int konvertieren.
 * Ergebnis wird direkt ueber den Rueckgabewert uebermittelt.
 */
long int
str2int_wo_errno
(
        const char* restrict input_string,  // String, der konvertiert werden soll
        const int base                      // Zahlensystem
)
{
    long int result = 0;
    const enum str2int_errno conversion_status = str2int(&result, input_string, base);

    return (conversion_status == STR2INT_SUCCESS) ? result : LONG_MIN;
}

//---------------------------------------------------------------------------------------------------------------------
