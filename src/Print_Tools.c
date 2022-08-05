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
#include <math.h>
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
extern size_t Count_Number_Of_Digits (const size_t value)
{
    size_t result   = 0;
    size_t val_cpy  = value;

    do
    {
        ++ result;
    }
    while (val_cpy /= 10);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine percentage value. (float input parameter)
 *
 * Asserts:
 *      value != NaN
 *      value != -Inf
 *      value != +Inf
 *      one_hundred_percent != NaN
 *      one_hundred_percent != -Inf
 *      one_hundred_percent != +Inf
 *      one_hundred_percent != 0.0f (NOT IMPLEMENTED DUE FLOAT COMPARISONS !)
 *
 * @param[in] value Value
 * @param[in] one_hundred_percent Value that corresponds to 100%
 *
 * @return percentage value
 */
extern float Determine_Percent_f (const float value, const float one_hundred_percent)
{
    ASSERT_MSG(! isnan(value), "Value is NaN !");
    ASSERT_MSG(isinf(value) != -1, "Value is -Inf !");
    ASSERT_MSG(isinf(value) != +1, "Value is +Inf !");
    ASSERT_MSG(! isnan(one_hundred_percent), "One hundred percent is NaN !");
    ASSERT_MSG(isinf(one_hundred_percent) != -1, "One hundred percent is -Inf !");
    ASSERT_MSG(isinf(one_hundred_percent) != +1, "One hundred percent is +Inf !");
    // ASSERT_MSG(one_hundred_percent != 0.0f, "One hundred percent is 0.0 !");

    return (value / one_hundred_percent) * 100.0f;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine percentage value. (size_t input parameter)
 *
 * Asserts:
 *      one_hundred_percent != 0
 *
 * @param[in] value Value
 * @param[in] one_hundred_percent Value that corresponds to 100%
 *
 * @return percentage value
 */
extern float Determine_Percent (const size_t value, const size_t one_hundred_percent)
{
    ASSERT_MSG(one_hundred_percent != 0, "One hundred percent is 0 !");

    return ((float) value / (float) one_hundred_percent) * 100.0f;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine the expected time left for the calculation.
 *
 * Asserts:
 *      second_value >= first_value
 *      end_value >= first_value
 *      end_value >= second_value
 *
 * @param[in] first_value First value
 * @param[in] second_value Second value
 * @param[in] end_value End value
 * @param[in] time_between_values Time, that was used in the interval between the first and second value
 *
 * @return Expected time left for the calculation
 */
extern float Determine_Time_Left (const size_t first_value, const size_t second_value, const size_t end_value,
        const time_t time_between_values)
{
    ASSERT_FMSG(second_value >= first_value, "First value (%zu) is larger than the second value (%zu) !", first_value,
            second_value);
    ASSERT_FMSG(end_value >= first_value, "First value (%zu) is larger than the end value (%zu) !", first_value,
            end_value);
    ASSERT_FMSG(end_value >= second_value, "Second value (%zu) is larger than the end value (%zu) !", second_value,
            end_value);

    const size_t values_left = end_value - second_value;
    const size_t diff_values = second_value - first_value;

    return (((float) values_left / (float) diff_values) * (float) time_between_values) / CLOCKS_PER_SEC;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine the expected average time left for the calculation.
 *
 * A average value is useful to avoid a strongly changing expected time.
 *
 * Asserts:
 *      second_value >= first_value
 *      end_value >= first_value
 *      end_value >= second_value
 *
 * @param[in] first_value First value
 * @param[in] second_value Second value
 * @param[in] end_value End value
 * @param[in] time_between_values Time, that was used in the interval between the first and second value
 *
 * @return Expected average time left for the calculation
 */
extern float Determine_Time_Left_Average (const size_t first_value, const size_t second_value, const size_t end_value,
        const time_t time_between_values)
{
    ASSERT_FMSG(second_value >= first_value, "First value (%zu) is larger than the second value (%zu) !", first_value,
            second_value);
    ASSERT_FMSG(end_value >= first_value, "First value (%zu) is larger than the end value (%zu) !", first_value,
            end_value);
    ASSERT_FMSG(end_value >= second_value, "Second value (%zu) is larger than the end value (%zu) !", second_value,
            end_value);

    static uint_fast32_t value_counter  = 0;
    static float time_left_counter      = 0.0f;
    static float last_return_value      = 0.0f;
    const uint_fast8_t mod_value        = 10;   // The mod value also tries to avoid strongly changing expected times

    ++ value_counter;
    time_left_counter += Determine_Time_Left(first_value, second_value, end_value, time_between_values);

    if ((value_counter % mod_value) == 0)
    {
        last_return_value = time_left_counter / (float) value_counter;
    }

    return last_return_value;
}

//---------------------------------------------------------------------------------------------------------------------
