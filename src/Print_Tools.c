/**
 * @file Print_Tools.c
 *
 * @brief Some helpful output tools.
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
 * @brief Print a 2D-String array to stdout. *
 *
 * This is helpful for debugging of text-based drawings.
 *
 * Asserts:
 *      drawing != NULL
 *      dim_1 > 0
 *      dim_2 > 0
 *
 * @param[in] drawing 2D-String array
 * @param[in] dim_1 Groesse der First array dimension (Number of C-Strings)
 * @param[in] dim_2 Groesse der Second array dimension (Max numer of char per C-String)
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
 * @brief Count number of digits in a value.
 *
 * @param[in] value Value
 *
 * @return The number of digits in the given value
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
