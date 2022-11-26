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
#include "Misc.h"
#include "int2str.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a 2D-String array to stdout.
 *
 * This is helpful for debugging of text-based drawings.
 *
 * Asserts:
 *      drawing != NULL
 *      dim_1 > 0
 *      dim_2 > 0
 *
 * @param[in] drawing 2D-String array
 * @param[in] dim_1 Size of the first array dimension (Number of C-Strings)
 * @param[in] dim_2 Size of the second array dimension (Max number of char per C-String)
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
 * @param[in] counter_since_last_output Counter since the last process print
 * @param[in] actual_counter Actual process
 * @param[in] hundred_percent Value that represents a process of 100 % (In other words: the value, that will appear
 *      when the operation is done)
 * @param[in] with_carriage_return Print a carriage return at the end of the line
 *
 * @param[in] print_function This is the function, that will be called, when process information are to be printed
 * @param[in] optional_parameter Optional parameter for the optional_second_print_function function ptr
 * @param[in] optional_second_print_function An optional print function for additional work
 *
 * @return The new counter
 */
extern size_t Process_Printer (const size_t print_step_size, const size_t counter_since_last_output,
        const size_t actual_counter, const size_t hundred_percent, const _Bool with_carriage_return,
        void (*print_function)
        (
            const size_t print_step_size,
            const size_t actual_counter,
            const size_t hundred_percent,
            const clock_t interval_begin,
            const clock_t interval_end
        ),
        void* optional_parameter,
        void (*optional_second_print_function)
        (
                void* data
        )
)
{
    ASSERT_MSG(print_function != NULL, "print_function is NULL !");

    static clock_t interval_begin = 0;
    static clock_t interval_end = 0;
    size_t new_counter = counter_since_last_output;

    if (counter_since_last_output >= print_step_size)
    {
        CLOCK_WITH_RETURN_CHECK(interval_end);
        print_function(print_step_size, actual_counter, hundred_percent, interval_begin, interval_end);
        CLOCK_WITH_RETURN_CHECK(interval_begin);

        // Use the optional function pointer, if it was given
        if (optional_second_print_function != NULL)
        {
            optional_second_print_function(optional_parameter);
        }

        if (with_carriage_return)
        {
           printf ("\r");
           fflush(stdout);
        }

        // Update counter (The if statement before is also a underflow check)
        new_counter -= print_step_size;
    }

    return new_counter;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a memory size in Byte, KByte and MByte.
 *
 * Asserts:
 *      byte_size != 0
 *
 * param[in] byte_size Value, that will be converted an printed
 */
extern void Print_Memory_Size_As_B_KB_MB (const size_t byte_size)
{
    ASSERT_MSG(byte_size != 0, "Byte size is 0 !")

    printf ("%zu B (%.3f KB | %.3f MB)\n", byte_size, (double) byte_size / 1024.0,
            (double) byte_size / 1024.0 / 1024.0);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a given integer value with decimal dots. This is helpful to get a direct information in which "size
 * class" the value is.
 *
 * @param[in] value Value, that will be printed with decimal dots
 */
extern void Print_Value_With_Decimal_Points (const long int value)
{
    char value_to_str [30];
    memset(value_to_str, '\0', sizeof(value_to_str));

    const enum int2str_errno converting_result = int2str(value_to_str, COUNT_ARRAY_ELEMENTS(value_to_str) - 1, value);
    ASSERT_FMSG(converting_result == INT2STR_SUCCESS, "Cannot convert %lu to str !", value);

    const size_t value_to_str_length = strlen (value_to_str);
    const size_t mod_3 = value_to_str_length % 3;

    // Print the converted long int value
    for (size_t i = 0; i < mod_3; ++ i)
    {
        printf ("%c", value_to_str [i]);
    }
    if (mod_3 != 0) { putchar ('.'); }
    for (size_t i = mod_3; i < value_to_str_length; ++ i)
    {
        if (((i - mod_3) % 3) == 0 && i != mod_3)
        {
            putchar ('.');
        }
        printf ("%c", value_to_str [i]);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
