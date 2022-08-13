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
 * @brief Print an array of uint_fast32_t variables to stdout.
 *
 * Asserts:
 *      array != NULL
 *
 * @param[in] array array with the data
 * @param[in] array_length length of the array
 */
extern void Print_uint_fast32_t_Array (const uint_fast32_t* const array, const size_t array_length)
{
    ASSERT_MSG(array != NULL, "array is NULL !");

    printf ("[ ");
    for (size_t i = 0; i < array_length; ++ i)
    {
        printf ("%" PRIuFAST32, array [i]);
        if ((i + 1) < array_length)
        {
            printf (", ");
        }
    }
    puts (" ]");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print process information with a user defined function, if new process output is to print.
 *
 * To avoid too many print operations the counter will be decreased, when a output operation was done.
 *
 * Asserts:
 *      print_function != NULL
 *
 * @param[in] print_step_size Minimum size of the counter to output the current process information
 * @param[in] counter Counter Counter since the last process print
 * @param[in] actual Actual process
 * @param[in] hundred_percent Value that represents a process of 100 % (In other words: the value, that will appear
 *      when the operation is done)
 * @param[in] print_function This is the function, that will be called, when process information are to be printed
 *
 * @return The new counter
 */
extern size_t Process_Printer (const size_t print_step_size, const size_t counter, const size_t actual,
        const size_t hundred_percent,
        void (*print_function) (const size_t actual, const size_t hundred_percent))
{
    ASSERT_MSG(print_function != NULL, "print_function is NULL !");

    size_t new_counter = counter;

    if (counter >= print_step_size)
    {
        print_function(actual, hundred_percent);

        // Update counter (The if statement before is also a underflow check)
        new_counter -= print_step_size;
    }

    return new_counter;
}

//---------------------------------------------------------------------------------------------------------------------
