/**
 * @file int2str.c
 *
 * @brief Konvertierungsfunktion C-String -> int.
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

/*
 * long int zu C-String konvertieren.
 */
enum int2str_errno
int2str
(
        char* const output_string,          // Ausgabestring
        const size_t output_string_size,    // Laenge des Ausgabestrings
        const long int input                // Integer, der konvertiert werden soll
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

    // Gab es eine Ausgabe von snprintf ?
    if (output_string [0] == '\0' || isspace (output_string [0]))
    {
        return INT2STR_INCONVERTIBLE;
    }

    // Nullterminierung garantieren
    output_string [output_string_size - 1] = '\0';

    return INT2STR_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------

/*
 * long int zu C-String konvertieren. Ohne Notwendigkeit, dass der Aufrufer vorher Speicher bereitstellen muss.
 */
char*
int2str_wo_errno
(
        const long int input    // Integer, der konvertiert werden soll
)
{
    // 25 Zeichen reichen fuer 32 und 64 Bit Eingaben aus
    static char static_result [25];
    memset (static_result, '\0', sizeof(static_result));

    const enum int2str_errno conversion_status = int2str(static_result, COUNT_ARRAY_ELEMENTS(static_result) - 1, input);

    return (conversion_status == INT2STR_SUCCESS) ? static_result : NULL;
}

//---------------------------------------------------------------------------------------------------------------------
