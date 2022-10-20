/**
 * @file Misc.c
 *
 * @brief Helpful functions, that fits in no specific translation unit.
 *
 * @date 07.08.2022
 * @author: t430
 */

#include "Misc.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include "Error_Handling/Assert_Msg.h"



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
    ASSERT_MSG(isinf(value), "Value is +/-Inf !");
    ASSERT_MSG(! isnan(one_hundred_percent), "One hundred percent is NaN !");
    ASSERT_MSG(isinf(one_hundred_percent), "One hundred percent is +/-Inf !");
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
    const size_t diff_values = ((second_value - first_value) > 0) ? (second_value - first_value) : 1;
    const float time_between_values_sec = (float) time_between_values / (float) CLOCKS_PER_SEC;

    return ((float) values_left / (float) diff_values) * time_between_values_sec;
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

/**
 * @brief Simple thing: When the input is NaN oder +/-Inf the function returns 0.0f.
 *
 * @param input test float value
 *
 * @return 0.0 if input is NaN or +/-Inf, otherwise the value itself
 */
extern inline float Replace_NaN_And_Inf_With_Zero (const float input)
{
    float result = input;

    if (isnan(input) || isinf(input) != 0) { result = 0.0; }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine the size of a FILE* object. In normal cases this is the file size.
 *
 * The problem is the magical 2 GB bound. The standard way with fseek() und ftell() can only guarantee a handling with
 * files up to 2GB size. For larger files there is no portable way to bypass this limitations. On 64 bit systems the
 * type "long int" (this is the return type of ftell()) is 8 byte long. So the problem not occurs in this situations,
 * but you cannot guarantee this. Especially on 32 bit systems.
 *
 * This problem has a own Wikipedia entry:
 * EN: https://en.wikipedia.org/wiki/Large_file_support
 * DE: https://de.wikipedia.org/wiki/Large_File_Support
 *
 * So the workaround:
 * Use the preprocessor to encapsulate system specific code. The portable way with the C standard lib will be the
 * fallback (with its limitations).
 *
 * Asserts:
 *      file != NULL
 *
 * @param[in] file FILE pointer to an opened file stream.
 *
 * @return The file size in bytes or -1, if an error occurs.
 */
extern int_fast64_t Determine_FILE_Size (FILE* file)
{
    ASSERT_MSG(file != NULL, "The FILE pointer is NULL !");

    int_fast64_t result = 0;

    // OS check
    // A good list for system specific #defines: https://iq.opengenus.org/detect-operating-system-in-c/
#ifdef _WIN32
    #ifdef _WIN64
        // Windows 64 bit
        // TODO: Implement the Windows specific code
    #else
        // Windows 32 bit
        // TODO: Implement the Windows specific code
    #endif /* _WIN64 */
#else
    #ifdef __unix__
        // Unix system
        #include <sys/types.h>

        int fseek_return = fseeko (file, 0, SEEK_END);
        ASSERT_MSG(fseek_return == 0, "fseeko() returned a nonzero value !");

        const off_t file_length = ftello(file);
        ASSERT_MSG(file_length != -1, "ftello() returned -1 !");

        fseek_return = fseeko (file, 0, SEEK_SET);
        ASSERT_MSG(fseek_return == 0, "fseeko() returned a nonzero value !");

        result = (int_fast64_t) file_length;
    #else
        // Other system: Using the fallback
        int fseek_return = fseek (file, 0, SEEK_END);
        ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");

        const long int file_length = ftell (file); // Possible limit to 2 GB !
        ASSERT_MSG(file_length != -1, "ftell() returned -1 !");

        fseek_return = fseek (file, 0, SEEK_SET);
        ASSERT_MSG(fseek_return == 0, "fseek() returned a nonzero value !");

        result = (int_fast64_t) file_length;
    #endif /* __unix__ */
#endif

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
