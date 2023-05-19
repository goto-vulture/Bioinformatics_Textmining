/**
 * @file TEST_etc.c
 *
 * @brief Here are TEST functions, that cannot be placed in a translation unit for specific struct objects.
 *
 * @date 01.11.2022
 * @author: leer
 */

#include "TEST_Etc.h"

#include <math.h>
#include <string.h>
#include "tinytest.h"
#include "../Print_Tools.h"
#include "../CLI_Parameter.h"
#include "../Exec_Intersection.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../ANSI_Esc_Seq.h"
#include "../String_Tools.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the number of free calls at the end of a calculation.
 *
 * If the dynamic memory management works, than there are exact the same number of free calls compared with the sum of
 * malloc and calloc calls.
 */
extern void TEST_Number_Of_Free_Calls (void)
{
    GLOBAL_CLI_INPUT_FILE = "./src/Tests/Test_Data/test_ebm.json";
    GLOBAL_CLI_INPUT_FILE2 = "./src/Tests/Test_Data/intervention_10MB.txt";
    GLOBAL_CLI_OUTPUT_FILE = "./out.json";

    // Resetting the counter to guarantee, that only the function calls of the next operations will be counted
    GLOBAL_malloc_calls = 0;
    GLOBAL_calloc_calls = 0;
    GLOBAL_free_calls = 0;

    // Call the intersection calculation
    Exec_Intersection(NAN, NULL, NULL);

    const int_fast64_t missing_free_calls = (int_fast64_t) ((GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls);

    // We expect, that no free call is missing
    ASSERT_EQUALS(0, missing_free_calls);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the functionality of the most important ANSI esc sequences.
 *
 * This test cannot fail, because it is not possible for the program to check whether the output on the terminal has
 * the expected view.
 */
extern void TEST_ANSI_Esc_Seq (void)
{
    Test_ANSI_Main_Esc_Seq();

    // We cannot check the view of the terminal output; so such a test cannot fail.
    ASSERT_EQUALS(true, true);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Test, whether the Tokenize_String function do the tokenisation calculation properly.
 */
extern void TEST_Tokenize_String (void)
{
    const char* test_str    = "hello world, friend of mine!";
    const char* delimiter   = " ,!";
    const char* const expected_results [] =
    {
            "hello",
            "world",
            "friend",
            "of",
            "mine"
    };

    _Bool test_results = true;

    struct Tokenized_String tokenized_string = Tokenize_String (test_str, delimiter);
    ASSERT_EQUALS(sizeof (expected_results) / sizeof (expected_results [0]), tokenized_string.next_free_pos_len);

    for (size_t i = 0; i < (sizeof (expected_results) / sizeof (expected_results [0])); ++ i)
    {
        const char* curr_token = &(test_str [tokenized_string.token_data[i].pos]);
        printf ("Expected: \"%s\"; Got: \"", expected_results [i]);
        // Use the current pointer to print only the token char by char
        for (size_t i2 = 0; i2 < (size_t) tokenized_string.token_data[i].len; ++ i2)
        {
            putc(curr_token [i2], stdout);
        }
        puts("\"");

        const int cmp_res = strncmp(curr_token, expected_results [i], (size_t) tokenized_string.token_data[i].len);
        if (cmp_res != 0)
        {
            test_results = false;
            break;
        }
    }
    ASSERT_EQUALS(true, test_results);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Test the Any_Print function. If a C11 compiler is given, the additional ANY_PRINT macro will be also tested.
 */
extern void TEST_Any_Print (void)
{
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    unsigned char u_char_value                      = GET_MAX((unsigned char) 42);
    // Here it is not possible to use the max value, because the max value doesn't represent a printable char
    signed char s_char_value                        = 42; //GET_MAX((signed char) 42);
    unsigned short u_short_value                    = GET_MAX((unsigned short) 42);
    signed short s_short_value                      = GET_MAX((signed short) 42);
    unsigned int u_int_value                        = GET_MAX((unsigned int) 42);
    signed int s_int_value                          = GET_MAX((signed int) 42);
    unsigned long int u_long_int_value              = GET_MAX((unsigned long int) 42);
    signed long int s_long_int_value                = GET_MAX((signed long int) 42);
    unsigned long long int u_long_long_int_value    = GET_MAX((unsigned long long int) 42);
    signed long long int s_long_long_int_value      = GET_MAX((signed long long int) 42);

    size_t size_t_value                             = GET_MAX((size_t) 42);

    // On machines with sizeof(int_fast8_t) == sizeof(char) the max value is not printable, because it will be used as
    // char
    int_fast8_t int_fast8_t_value                   = 42;
    int_fast16_t int_fast16_t_value                 = GET_MAX((int_fast16_t) 42);
    int_fast32_t int_fast32_t_value                 = GET_MAX((int_fast32_t) 42);
    int_fast64_t int_fast64_t_value                 = GET_MAX((int_fast64_t) 42);
    uint_fast8_t uint_fast8_t_value                 = GET_MAX((uint_fast8_t) 42);
    uint_fast16_t uint_fast16_t_value               = GET_MAX((uint_fast16_t) 42);
    uint_fast32_t uint_fast32_t_value               = GET_MAX((uint_fast32_t) 42);
    uint_fast64_t uint_fast64_t_value               = GET_MAX((uint_fast64_t) 42);

    // On machines with sizeof(int_least8_t) == sizeof(char) the max value is not printable, because it will be used as
    // char
    int_least8_t int_least8_t_value                 = 42;
    int_least16_t int_least16_t_value               = GET_MAX((int_least16_t) 42);
    int_least32_t int_least32_t_value               = GET_MAX((int_least32_t) 42);
    int_least64_t int_least64_t_value               = GET_MAX((int_least64_t) 42);
    uint_least8_t uint_least8_t_value               = GET_MAX((uint_least8_t) 42);
    uint_least16_t uint_least16_t_value             = GET_MAX((uint_least16_t) 42);
    uint_least32_t uint_least32_t_value             = GET_MAX((uint_least32_t) 42);
    uint_least64_t uint_least64_t_value             = GET_MAX((uint_least64_t) 42);
#else
    unsigned char u_char_value                      = UCHAR_MAX;
    // Here it is not possible to use the max value, because the max value doesn't represent a printable char
    signed char s_char_value                        = 42; //GET_MAX((signed char) 42);
    unsigned short u_short_value                    = USHRT_MAX;
    signed short s_short_value                      = SHRT_MAX;
    unsigned int u_int_value                        = UINT_MAX;
    signed int s_int_value                          = INT_MAX;
    unsigned long int u_long_int_value              = ULONG_MAX;
    signed long int s_long_int_value                = LONG_MAX;
    unsigned long long int u_long_long_int_value    = ULLONG_MAX;
    signed long long int s_long_long_int_value      = LLONG_MAX;

    // The following variables are not usable in C99
//    size_t size_t_value                             = SIZE_MAX;
//
//    // On machines with sizeof(int_fast8_t) == sizeof(char) the max value is not printable, because it will be used as
//    // char
//    int_fast8_t int_fast8_t_value                   = 42;
//    int_fast16_t int_fast16_t_value                 = INT_FAST16_MAX;
//    int_fast32_t int_fast32_t_value                 = INT_FAST32_MAX;
//    int_fast64_t int_fast64_t_value                 = INT_FAST64_MAX;
//    uint_fast8_t uint_fast8_t_value                 = UINT_FAST8_MAX;
//    uint_fast16_t uint_fast16_t_value               = UINT_FAST16_MAX;
//    uint_fast32_t uint_fast32_t_value               = UINT_FAST32_MAX;
//    uint_fast64_t uint_fast64_t_value               = UINT_FAST64_MAX;
//
//    // On machines with sizeof(int_least8_t) == sizeof(char) the max value is not printable, because it will be used as
//    // char
//    int_least8_t int_least8_t_value                 = 42;
//    int_least16_t int_least16_t_value               = INT_LEAST16_MAX;
//    int_least32_t int_least32_t_value               = INT_LEAST32_MAX;
//    int_least64_t int_least64_t_value               = INT_LEAST64_MAX;
//    uint_least8_t uint_least8_t_value               = UINT_LEAST8_MAX;
//    uint_least16_t uint_least16_t_value             = UINT_LEAST16_MAX;
//    uint_least32_t uint_least32_t_value             = UINT_LEAST32_MAX;
//    uint_least64_t uint_least64_t_value             = UINT_LEAST64_MAX;
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

    float float_value                               = 42.42f;
    double double_value                             = 42.42;
    long double long_double_value                   = 42.42L;
    _Bool _bool_value                               = false;
    void* pointer                                   = &u_char_value;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    printf ("unsigned char:          ");
    ANY_PRINT(u_char_value);
    puts("");
    printf ("char:                   ");
    ANY_PRINT(s_char_value);
    puts("");
    printf ("unsigned short:         ");
    ANY_PRINT(u_short_value);
    puts("");
    printf ("short:                  ");
    ANY_PRINT(s_short_value);
    puts("");
    printf ("unsigned int:           ");
    ANY_PRINT(u_int_value);
    puts("");
    printf ("int:                    ");
    ANY_PRINT(s_int_value);
    puts("");
    printf ("unsigned long int:      ");
    ANY_PRINT(u_long_int_value);
    puts("");
    printf ("long int:               ");
    ANY_PRINT(s_long_int_value);
    puts("");
    printf ("unsigned long long int: ");
    ANY_PRINT(u_long_long_int_value);
    puts("");
    printf ("long long int:          ");
    ANY_PRINT(s_long_long_int_value);
    puts("");
    printf ("size_t:                 ");
    ANY_PRINT(size_t_value);
    puts("");

    printf ("int_fast8_t:            ");
    ANY_PRINT(int_fast8_t_value);
    puts("");
    printf ("int_fast16_t:           ");
    ANY_PRINT(int_fast16_t_value);
    puts("");
    printf ("int_fast32_t:           ");
    ANY_PRINT(int_fast32_t_value);
    puts("");
    printf ("int_fast64_t:           ");
    ANY_PRINT(int_fast64_t_value);
    puts("");
    printf ("uint_fast8_t:           ");
    ANY_PRINT(uint_fast8_t_value);
    puts("");
    printf ("uint_fast16_t:          ");
    ANY_PRINT(uint_fast16_t_value);
    puts("");
    printf ("uint_fast32_t:          ");
    ANY_PRINT(uint_fast32_t_value);
    puts("");
    printf ("uint_fast64_t:          ");
    ANY_PRINT(uint_fast64_t_value);
    puts("");

    printf ("int_least8_t:           ");
    ANY_PRINT(int_least8_t_value);
    puts("");
    printf ("int_least16_t:          ");
    ANY_PRINT(int_least16_t_value);
    puts("");
    printf ("int_least32_t:          ");
    ANY_PRINT(int_least32_t_value);
    puts("");
    printf ("int_least64_t:          ");
    ANY_PRINT(int_least64_t_value);
    puts("");
    printf ("uint_least8_t:          ");
    ANY_PRINT(uint_least8_t_value);
    puts("");
    printf ("uint_least16_t:         ");
    ANY_PRINT(uint_least16_t_value);
    puts("");
    printf ("uint_least32_t:         ");
    ANY_PRINT(uint_least32_t_value);
    puts("");
    printf ("uint_least64_t:         ");
    ANY_PRINT(uint_least64_t_value);
    puts("");

    printf ("float:                  ");
    ANY_PRINT(float_value);
    puts("");
    printf ("double:                 ");
    ANY_PRINT(double_value);
    puts("");
    printf ("long double:            ");
    ANY_PRINT(long_double_value);
    puts("");

    printf ("_Bool:                  ");
    ANY_PRINT(_bool_value);
    puts("");

    printf ("pointer:                ");
    ANY_PRINT(pointer);
    puts("");
#else
    printf ("unsigned char:          ");
    Any_Print(&u_char_value, UNSIGNED_CHAR);
    puts("");
    printf ("char:                   ");
    Any_Print(&s_char_value, SIGNED_CHAR);
    puts("");
    printf ("unsigned short:         ");
    Any_Print(&u_short_value, UNSIGNED_SHORT);
    puts("");
    printf ("short:                  ");
    Any_Print(&s_short_value, SIGNED_SHORT);
    puts("");
    printf ("unsigned int:           ");
    Any_Print(&u_int_value, UNSIGNED_INT);
    puts("");
    printf ("int:                    ");
    Any_Print(&s_int_value, SIGNED_INT);
    puts("");
    printf ("unsigned long int:      ");
    Any_Print(&u_long_int_value, UNSIGNED_LONG_INT);
    puts("");
    printf ("long int:               ");
    Any_Print(&s_long_int_value, SIGNED_LONG_INT);
    puts("");
    printf ("unsigned long long int: ");
    Any_Print(&u_long_long_int_value, UNSIGNED_LONG_LONG_INT);
    puts("");
    printf ("long long int:          ");
    Any_Print(&s_long_long_int_value, SIGNED_LONG_LONG_INT);
    puts("");

    // Printing values from type size_t, uint_fast8_t, ... is without the _Generic keyword not possible !
    // The real type of "size_t", "int_fast32_t_value" and Co. can differ between different systems; so a hard coded
    // type cannot be used !

    printf ("float:                  ");
    Any_Print(&float_value, FLOAT);
    puts("");
    printf ("double:                 ");
    Any_Print(&double_value, DOUBLE);
    puts("");
    printf ("long double:            ");
    Any_Print(&long_double_value, LONG_DOUBLE);
    puts("");

    printf ("_Bool:                  ");
    Any_Print(&_bool_value, BOOL);
    puts("");

    printf ("pointer:                ");
    Any_Print(&pointer, POINTER);
    puts("");
#endif /* #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

    ASSERT_EQUALS(true, true);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
