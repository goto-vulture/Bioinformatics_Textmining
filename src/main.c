/**
 * @file main.c
 *
 * @brief The calculation of so called "Dominating Word Sets" from data sets in the field of bioinformatics.
 *
 * @author x86 / Gyps
 */

/*         .-..-.__
 *       /`  |\_( "\__
 *       |   / __/`'--)
 *       |  /  | |
 *       /_/ _/_/
 * jgs  `|/))))
 *
 * For me, a vulture is more desirable than money.
 */

/**
 * @brief This is a #define for Linux systems. With this value the type "off_t" will get in every case the size of 64
 * bit.
 *
 * In most cases "off_t" already has the size of 64 bit.
 *
 * The main page of fseeko() and ftello():
       The fseeko() and ftello() functions are identical to fseek(3) and
       ftell(3) (see fseek(3)), respectively, except that the offset
       argument of fseeko() and the return value of ftello() is of type
       off_t instead of long.

       On some architectures, both off_t and long are 32-bit types, but
       defining _FILE_OFFSET_BITS with the value 64 (before including
       any header files) will turn off_t into a 64-bit type.
 */
#ifdef __unix__
    #ifndef _FILE_OFFSET_BITS
    #define _FILE_OFFSET_BITS 64
    #else
    #error "The macro \"_FILE_OFFSET_BITS\" is already defined and should be redefined with 64 !"
    #endif /* _FILE_OFFSET_BITS */
#endif /* __unix__ */

/*
 * Check the used C version, to replace keywords in the language if necessary. (E.g.: The C version is an older version
 * and contains some keywords which were included in newer releases of the language.
 *
 * The possibility is high, that another components in the code exists, which are not compatible with older versions (I
 * used only the C11 standard while the development)
 * At least the conflict with the keywords can be solved with the following preprocessor statements.
 *
 * The macro __STDC_VERSION__ is for C90 not defined
 * ->   Replace the keywords inline and restrict with empty macros to make a translation with the C90 standard possible.
 */
#ifndef __STDC_VERSION__
    /**
     * @brief Define the keyword "inline" as macro to replace them with empty code. (In C90 this keyword don't exists)
     */
    #ifndef inline
        #define inline
    #else
        #error "The macro \"inline\" is already defined !"
    #endif /* inline */

    /**
     * @brief Here is the same situation like with the keyword "inline". Define the keyword "restrict" as macro to
     * replace them with empty code. (In C90 this keyword also don't exists)
     */
    #ifndef restrict
        #define restrict
    #else
        #error "The macro \"restrict\" is already defined !"
    #endif /* restrict */

    /**
     * @brief Solve the problem with boolean values before C99. DIY "_Bool" with a macro. A "_Bool" will be replaced
     * with "int".
     */
    #ifndef _Bool
        #define _Bool int
    #else
        #error "The macro \"_Bool\" is already defined !"
    #endif /* _Bool */

    /**
     * @brief In rare situations "true" is not defined (e.g. before C99) -> DIY
     */
    #ifndef true
        #define true 1
    #else
        #error "The macro \"true\" is already defined !"
    #endif /* true */

    /**
     * @brief In rare situations "false" is not defined (e.g. before C99) -> DIY
     */
    #ifndef false
        #define false 0
    #else
        #error "The macro \"false\" is already defined !"
    #endif /* false */

/*
 * Is C11 available: Use _Statc_asserts to check the system before compiling the program.
 */
#elif __STDC_VERSION__ >= 201112L
    #include <limits.h>
    #include <stddef.h>
    #include <time.h>

    _Static_assert(CHAR_BIT == 8, "A char must be exact 8 bit long !");
    _Static_assert(sizeof(size_t) >= 4, "The type \"size_t\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(time_t) >= 4, "The type \"time_t\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(clock_t) >= 4, "The type \"clock_t\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(int) >= 4, "The type \"int\" needs to be at least 4 byte wide !");

    // It would be nice, when all systems fits this requirement. But on 32-Bit systems this will fail. Especially for
    // determining the file size it is important to support files, that are larger than 2 GB (signed variable !).
    // Anyway you need to accept this restriction ... :(
    //_Static_assert(sizeof(long int) >= 8, "The type \"long int\" needs to be at least 8 byte wide !");

    _Static_assert(sizeof(void*) >= 4, "The type \"void*\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(float) >= 4, "The type \"float\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(double) >= 4, "The type \"double\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(long double) >= 4, "The type \"long double\" needs to be at least 4 byte wide !");
#endif /* __STDC_VERSION__ */

/**
 * @brief A simple string, that shows, if an information was not given.
 */
#ifndef N_A
    #define N_A "N/A"
#else
    #error "The macro \"N_A\" is already defined !"
#endif /* N_A */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#include "Error_Handling/_Generics.h"

_Static_assert(sizeof(N_A) > 0 + 1, "The macro \"N_A\" needs at least one char (plus '\0') !");
IS_CONST_STR(N_A)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */



#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include "argparse.h"
#include "CLI_Parameter.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Tests/TEST_Document_Word_List.h"
#include "File_Reader.h"
#include "Token_Int_Mapping.h"
#include "Print_Tools.h"
#include "Document_Word_List.h"
#include "Error_Handling/Assert_Msg.h"
#include "Intersection_Approaches.h"
#include "Misc.h"
#include "Exec_Intersection.h"
#include "ANSI_Esc_Seq.h"
#include "Defines.h"
#include "CPUID.h"

#include "Tests/tinytest.h"
#include "Tests/TEST_cJSON_Parser.h"
#include "Tests/TEST_File_Reader.h"
#include "Tests/TEST_Exec_Intersection.h"
#include "Tests/TEST_Etc.h"
#include "Tests/TEST_Two_Dim_C_String_Array.h"



/**
 * @brief Run all test functions.
 */
static void
Run_All_Test_Functions (void);

/**
 * @brief Show the dynamic memory status. (How many malloc / calloc calls were done and is the number equal with the
 * amount of free calls). -> Exists memory leaks ?
 */
static void
At_Exit_Function (void);

//=====================================================================================================================

/**
 * @brief main function of the program.
 *
 * @param[in] argc argc parameter
 * @param[in] argv Number of argc parameter
 *
 * @return 0, if the program execution was successful; Else != 0
 */
int main (const int argc, const char* argv [])
{
    // Init pseudo random number generator
    const time_t curr_time = time (NULL);
    ASSERT_MSG(curr_time != (time_t) (-1), "time () return value is (time_t) (-1) !");
    srand ((unsigned int) curr_time);

    // Runtime check of the system
    // This check is necessary, because a _Static_assert test is only available in C11
#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 201112L
    #include <limits.h> // Necessary for "CHAR_BIT"

    ASSERT_MSG(CHAR_BIT == 8, "A char must be exact 8 bit long !");
    ASSERT_MSG(sizeof(size_t) >= 4, "The type \"size_t\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(time_t) >= 4, "The type \"time_t\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(clock_t) >= 4, "The type \"clock_t\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(int) >= 4, "The type \"int\" needs to be at least 4 byte wide !");

    // It would be nice, when all systems fits this requirement. But on 32-Bit systems this will fail. Especially for
    // determining the file size it is important to support files, that are larger than 2 GB (signed variable !).
    // Anyway you need to accept this restriction ... :(
    // ASSERT_MSG(sizeof(long int) >=8, "The type \"long int\" needs to be at least 8 byte wide !");

    ASSERT_MSG(sizeof(void*) >= 4, "The type \"void*\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(float) >= 4, "The type \"float\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(double) >= 4, "The type \"double\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(long double) >= 4, "The type \"long double\" needs to be at least 4 byte wide !");
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ < 201112L */

    // Use a user defined function at the end of the program
    atexit(At_Exit_Function);

    struct argparse_option cli_options [] =
    {
            OPT_HELP(),

            OPT_GROUP("Main functions"),
            OPT_STRING('i', "input", &GLOBAL_CLI_INPUT_FILE, "First input file", NULL, 0, 0),
            OPT_STRING('j', "input2", &GLOBAL_CLI_INPUT_FILE2, "Second input file", NULL, 0, 0),
            OPT_STRING('o', "output", &GLOBAL_CLI_OUTPUT_FILE, "Output file", NULL, 0, 0),

            OPT_GROUP("Additional functions"),
            OPT_BOOLEAN('f', "format", &GLOBAL_CLI_FORMAT_OUTPUT,
                    "Format the output for better readability in a normal editor", NULL, 0, 0),
            OPT_BOOLEAN('s', "sentence_offset", &GLOBAL_CLI_SENTENCE_OFFSET,
                    "Calculate sentence offsets", NULL, 0, 0),
            OPT_BOOLEAN('w', "word_offset", &GLOBAL_CLI_WORD_OFFSET,
                    "Calculate word offsets (On by default)", NULL, 0, 0),
            OPT_BOOLEAN('\0', "show_too_long_tokens", &GLOBAL_CLI_SHOW_TOO_LONG_TOKENS,
                    "Show too long tokens in the result file", NULL, 0, 0),
            OPT_BOOLEAN('\0', "no_part_matches", &GLOBAL_CLI_NO_PART_MATCHES,
                    "Don't show partial matches in the output file", NULL, 0, 0),
            OPT_BOOLEAN('\0', "no_full_matches", &GLOBAL_CLI_NO_FULL_MATCHES,
                    "Don't show full matches in the output file", NULL, 0, 0),
            OPT_BOOLEAN('k', "keep_single_token_results", &GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN,
                    "Keep results with only one token", NULL, 0, 0),
            OPT_BOOLEAN('c', "case_sensitive_token_cmp", &GLOBAL_CLI_CASE_SENSITIVE_TOKEN_COMPARISON,
                    "Do a case sensitive comparison of the tokens", NULL, 0, 0),

            OPT_GROUP("Debug / test functions"),
            OPT_BOOLEAN('T', "run_all_test_functions", &GLOBAL_RUN_ALL_TEST_FUNCTIONS,
                    "Execute all test functions", NULL, 0, 0),
            OPT_FLOAT('A', "abort", &GLOBAL_ABORT_PROCESS_PERCENT,
                     "Abort the calculation at this percent", NULL, 0, 0),
            OPT_BOOLEAN('n', "no_cpu_extensions", &GLOBAL_CLI_NO_CPU_EXTENSIONS,
                    "Don't use CPU extensions, even if there available on the host", NULL, 0, 0),
            OPT_BOOLEAN('\0', "no_timestamp", &GLOBAL_CLI_NO_TIMESTAMP,
                    "Don't save a timestamp in the output file", NULL, 0, 0),

            OPT_END()
    };

    // Parse CLI parameter
    struct argparse argparse_object;
    argparse_init(&argparse_object, cli_options, GLOBAL_USAGES, 0);
    argparse_describe(&argparse_object, GLOBAL_PROGRAM_DESCRIPTION, GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION);
    const int new_argc = argparse_parse(&argparse_object, argc, argv);
    UNUSED(new_argc);

    if (GLOBAL_RUN_ALL_TEST_FUNCTIONS)
    {
        Run_All_Test_Functions();

        // Show a test report
        TEST_REPORT();

        return (tt_fails != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
    }
    if (! isnan(GLOBAL_ABORT_PROCESS_PERCENT))
    {
        Check_CLI_Parameter_GLOBAL_ABORT_PROCESS_PERCENT();
        printf ("Abort percent value: %f\n", GLOBAL_ABORT_PROCESS_PERCENT);
    }
    if (GLOBAL_CLI_INPUT_FILE != NULL)
    {
        printf ("Input file 1: \"%s\"\n", GLOBAL_CLI_INPUT_FILE);
        Check_CLI_Parameter_CLI_INPUT_FILE();
    }
    else
    {
        PUTS_FFLUSH ("Missing first input file. Option: [-i / --input]");
        EXIT(EXIT_FAILURE);
    }
    if (GLOBAL_CLI_INPUT_FILE2 != NULL)
    {
        printf ("Input file 2: \"%s\"\n", GLOBAL_CLI_INPUT_FILE2);
        Check_CLI_Parameter_CLI_INPUT_FILE2();
    }
    else
    {
        PUTS_FFLUSH ("Missing second input file. Option: [-j / --input2]");
        EXIT(EXIT_FAILURE);
    }
    if (GLOBAL_CLI_OUTPUT_FILE != NULL)
    {
        printf ("Output file:  \"%s\"\n", GLOBAL_CLI_OUTPUT_FILE);
        Check_CLI_Parameter_CLI_OUTPUT_FILE();
    }
    else
    {
        PUTS_FFLUSH ("Missing output file. Option: [-o / --output]");
        EXIT(EXIT_FAILURE);
    }

    Check_CLI_Parameter_Logical_Consistency();
    puts("");

    printf("Available extensions: ");
    if (CPUID_IsMMXAvailable())     { printf(ANSI_TEXT_BOLD "MMX " ANSI_RESET_ALL); }
    if (CPUID_IsSSE2Available())    { printf(ANSI_TEXT_BOLD "SSE2 " ANSI_RESET_ALL); }
    if (CPUID_IsSSE4_1Available())  { printf(ANSI_TEXT_BOLD "SSE4.1 " ANSI_RESET_ALL); }
    if (CPUID_IsAVXAvailable())     { printf(ANSI_TEXT_BOLD "AVX " ANSI_RESET_ALL); }
    if (CPUID_IsAVX2Available())    { printf(ANSI_TEXT_BOLD "AVX2 " ANSI_RESET_ALL); }
    NEWLINE

    // Show the used CPU extensions
    if (! GLOBAL_CLI_NO_CPU_EXTENSIONS)
    {
#if defined(__AVX__) && defined(__AVX2__) && ! defined(NO_AVX2) && ! defined(NO_CPU_EXTENSIONS)
        puts("Using " ANSI_TEXT_BOLD "AVX2" ANSI_RESET_ALL " CPU extension.");
#elif defined(__AVX__) && ! defined(NO_AVX) && ! defined(NO_CPU_EXTENSIONS)
        puts("Using " ANSI_TEXT_BOLD "AVX" ANSI_RESET_ALL " CPU extension.");
#elif defined(__SSE__) && defined(__SSE2__) && defined(__SSE3__) && defined(__SSE4_1__) && ! defined(NO_SSE4_1) && ! defined(NO_CPU_EXTENSIONS)
        puts("Using " ANSI_TEXT_BOLD "SSE4.1" ANSI_RESET_ALL " CPU extension.");
#elif defined(__SSE__) && defined(__SSE2__) && ! defined(NO_SSE2) && ! defined(NO_CPU_EXTENSIONS)
        puts("Using " ANSI_TEXT_BOLD "SSE2" ANSI_RESET_ALL " CPU extension.");
#elif defined(__MMX__) && ! defined(NO_MMX) && ! defined (NO_CPU_EXTENSIONS)
        puts("Using " ANSI_TEXT_BOLD "MMX" ANSI_RESET_ALL " CPU extension.");
#else
        puts("Using " ANSI_TEXT_BOLD "no" ANSI_RESET_ALL " CPU extension.");
#endif
    }
    else
    {
        puts("Using " ANSI_TEXT_BOLD "no" ANSI_RESET_ALL " CPU extension.");
    }
#ifdef I386
    puts("Using " ANSI_TEXT_BOLD "32 bit" ANSI_RESET_ALL " mode.");
#else
    puts("Using " ANSI_TEXT_BOLD "64 bit" ANSI_RESET_ALL " mode.");
#endif /* I386 */

    printf("Comparison mode: " ANSI_TEXT_BOLD "%s" ANSI_RESET_ALL "\n",
            (GLOBAL_CLI_CASE_SENSITIVE_TOKEN_COMPARISON) ? "case sensitive" : "case insensitive");

    PUTS_FFLUSH("");

    // Execute the intersection process
    Exec_Intersection(GLOBAL_ABORT_PROCESS_PERCENT, NULL, NULL);

    return EXIT_SUCCESS;
}

//=====================================================================================================================

/**
 * @brief Run all test functions.
 */
static void
Run_All_Test_Functions (void)
{
    RUN(TEST_AppendNewString);
    RUN(TEST_AppendDataToNewestString);
    RUN(TEST_AppendDataToSpecificString);

    RUN(TEST_Intersection);
    RUN(TEST_Tokenize_String);

    RUN(TEST_cJSON_Parse_JSON_Fragment);
    RUN(TEST_cJSON_Get_Token_Array_From_JSON_Fragment);
    RUN(TEST_cJSON_Parse_Full_JSON_File);

    RUN(TEST_Number_Of_Tokenarrays);
    RUN(TEST_Max_Dataset_ID_Length);
    RUN(TEST_Max_Tokenarray_Length);
    RUN(TEST_Length_Of_The_First_25_Tokenarrays);

    RUN(TEST_MD5_Of_Test_Files);

    RUN(TEST_Number_Of_Tokens_Found);
    RUN(TEST_Number_Of_Sets_Found);
    RUN(TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files);
    RUN(TEST_Number_Of_Sets_Equal_With_Switched_Input_Files);
    RUN(TEST_Number_Of_Tokens_Equal_With_Switched_Input_Files_JSON_And_CSV);
    RUN(TEST_Number_Of_Sets_Equal_With_Switched_Input_Files_JSON_And_CSV);

#if defined(__AVX__) && defined(__AVX2__) && ! defined(NO_AVX2) && ! defined(NO_CPU_EXTENSIONS)
    RUN(TEST_AVX2_Extension);
#elif defined(__AVX__) && ! defined(NO_AVX) && ! defined(NO_CPU_EXTENSIONS)
    RUN(TEST_AVX_Extension);
#elif defined(__SSE__) && defined(__SSE2__) && defined(__SSE3__) && defined(__SSE4_1__) && ! defined(NO_SSE4_1) && ! defined(NO_CPU_EXTENSIONS)
    RUN(TEST_SSE4_1_Extension);
#elif defined(__SSE__) && defined(__SSE2__) && ! defined(NO_SSE2) && ! defined(NO_CPU_EXTENSIONS)
    RUN(TEST_SSE2_Extension);
#elif defined(__MMX__) && ! defined(NO_MMX) && ! defined (NO_CPU_EXTENSIONS)
    RUN(TEST_MMX_Extension);
#else
    RUN(TEST_Placeholder_For_No_Extensions);
#endif

    RUN(TEST_Case_Insensitive_Comparison);

    RUN(TEST_Number_Of_Free_Calls);
    RUN(TEST_ANSI_Esc_Seq);
    RUN(TEST_Any_Print);

#ifdef LINUX
    RUN(TEST_Is_Output_File_JSON_Compatible);
#endif /* LINUX */

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Show the dynamic memory status. (How many malloc / calloc calls were done and is the number equal with the
 * amount of free calls). -> Exists memory leaks ?
 */
static void
At_Exit_Function (void)
{
    if (Missing_Free_Calls() != 0) { NEWLINE; }
    Show_Dynamic_Memory_Status();
    return;
}

//---------------------------------------------------------------------------------------------------------------------
