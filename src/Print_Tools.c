/**
 * @file Print_Tools.h
 *
 * @brief Einige hilfreiche Ausgabe-Tools. Bereitgestellt als Makros.
 *
 * @date 06.03.2022
 * @author: x86 / Gyps
 */

#include "Print_Tools.h"
#include <string.h>
#include <inttypes.h>
#include "Error_Handling/Assert_Msg.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein 2D-String-Array aud stdout ausgeben.
 *
 * Solch eine Funktionalitaet wird benoetigt, um Fehler bei der Erstellung von textbasierten Zeichnungen schneller zu
 * finden.
 *
 * Asserts:
 *      drawing != NULL
 *      dim_1 > 0
 *      dim_2 > 0
 *
 * @param[in] drawing 2D-String-Array
 * @param[in] dim_1 Groesse der 1. Array Dimension (Anzahl an Zeichenketten)
 * @param[in] dim_2 Groesse der 2. Array Dimension (Maximale Anzahl an Zeichen pro Zeichenkette)
 */
extern void Print_2D_String_Array (const char* const restrict drawing [], const size_t dim_1, const size_t dim_2)
{
    ASSERT_MSG(drawing != NULL, "drawing is NULL !");
    ASSERT_MSG(dim_1 != 0, "First array dimension is 0 !");
    ASSERT_MSG(dim_2 != 0, "Second array dimension is 0 !");

    const uint_fast8_t line_number_print_size = 3;

    size_t longest_string = 0;
    for (size_t i = 0; i < dim_1; ++ i)
    {
        if (strlen (drawing [i]) > longest_string)
        {
            longest_string = strlen (drawing [i]);
        }
    }

    PRINT_X_TIMES_SAME_CHAR(' ', line_number_print_size + 1u)
    printf ("+");
    PRINT_X_TIMES_SAME_CHAR('-', longest_string)
    puts ("+");
    for (size_t i = 0; i < dim_1; ++ i)
    {
        printf ("%*zu |%s%*s|\n", line_number_print_size, i + 1, drawing [i],
                (int) (longest_string - strlen (drawing [i])), "");
    }
    PRINT_X_TIMES_SAME_CHAR(' ', line_number_print_size + 1u)
    printf ("+");
    PRINT_X_TIMES_SAME_CHAR('-', longest_string)
    puts ("+");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Die Anzahl an Ziffern in einer Zahl bestimmen.
 *
 * @param[in] value Zahl, die betrachtet wird.
 */
extern size_t Count_Number_Of_Digits (size_t value)
{
    size_t result = 0;

    do
    {
        ++ result;
    }
    while (value /= 10);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
