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

/*
 * Check the used C version, to replace keywords in the language if necessary. (E.g.: The C version is an older version
 * and contains some keywords which were included in newer releases of the language.
 *
 * The possiblility is high, that another components in the code exists, which are not compatible with older versions (I
 * used only the C11 standard while the development)
 * At least the conflict with the keywords can be solved with the following preprocessor statements.
 *
 * The macro __STDC_VERSION__ is for C90 not defined
 * ->   Replace the keywords inline and restrict with empty marcos to make a translation with the C90 standard possible.
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
#elif __STDC_VERSION__ == 201112L
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

#include "Tests/tinytest.h"
#include "Tests/TEST_cJSON_Parser.h"



/**
 * @brief Run all test functions.
 */
static void
Run_All_Test_Functions
(
        void
);

/**
 * @brief Show the dynamic memory status. (How many malloc / calloc calls were done and is the number equal with the
 * amount of free calls). -> Exists memory leaks ?
 */
static void
At_Exit_Function
(
     void
);

//=====================================================================================================================

/**
 * @brief main function of the program.
 * *
 * @param[in] argc argc parameter
 * @param[in] argv Number of argc paramer
 *
 * @return 0, if the program execution was successful; Else != 0
 */
int main (const int argc, const char* argv [])
{
    // Runtime check of the system
    // This check is necessary, because a _Static_assert test is only available in C11
#if __STDC_VERSION__ < 201112L
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
#endif /* __STDC_VERSION__ < 201112L */

    // Use a user defined function at the end of the program
    atexit(At_Exit_Function);

    struct argparse_option cli_options [] =
    {
            OPT_HELP(),

            OPT_GROUP("Main functions"),
            OPT_STRING('i', "input", &GLOBAL_CLI_INPUT_FILE, "First input file", NULL, 0, 0),
            OPT_STRING('j', "input2", &GLOBAL_CLI_INPUT_FILE2, "Second input file", NULL, 0, 0),
            OPT_STRING('o', "output", &GLOBAL_CLI_OUTPUT_FILE, "Output file", NULL, 0, 0),

            OPT_GROUP("Debug / test functions"),
            OPT_BOOLEAN('T', "run_all_test_functions", &GLOBAL_RUN_ALL_TEST_FUNCTIONS,
                    "Execute all test functions", NULL, 0, 0),
            OPT_FLOAT('A', "abort", &GLOBAL_ABORT_PROCESS_PERCENT,
                     "At which percent the calculation should be aborted ?", NULL, 0, 0),

            OPT_END()
    };

    // Parse CLI parameter
    struct argparse argparse_object;
    argparse_init(&argparse_object, cli_options, GLOBAL_USAGES, 0);
    argparse_describe(&argparse_object, GLOBAL_PROGRAM_DESCRIPTION, GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION);
    const int new_argc = argparse_parse(&argparse_object, argc, argv);
    (void) new_argc;

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
        exit(EXIT_FAILURE);
    }
    if (GLOBAL_CLI_INPUT_FILE2 != NULL)
    {
        printf ("Input file 2: \"%s\"\n", GLOBAL_CLI_INPUT_FILE2);
        Check_CLI_Parameter_CLI_INPUT_FILE2();
    }
    else
    {
        PUTS_FFLUSH ("Missing second input file. Option: [-j / --input2]");
        exit(EXIT_FAILURE);
    }
    if (GLOBAL_CLI_OUTPUT_FILE != NULL)
    {
        printf ("Output file:  \"%s\"\n", GLOBAL_CLI_OUTPUT_FILE);
        Check_CLI_Parameter_CLI_OUTPUT_FILE();
    }
    else
    {
        PUTS_FFLUSH ("Missing output file. Option: [-o / --output]");
        exit(EXIT_FAILURE);
    }
    puts("");

    // Execute the intersection process
    Exec_Intersection((! isnan(GLOBAL_ABORT_PROCESS_PERCENT)) ? GLOBAL_ABORT_PROCESS_PERCENT : NAN);

    return EXIT_SUCCESS;
}

//=====================================================================================================================

/**
 * @brief Run all test functions.
 */
static void
Run_All_Test_Functions
(
        void
)
{
    RUN(TEST_cJSON_Parse_JSON_Fragment);
    RUN(TEST_cJSON_Get_Token_Array_From_JSON_Fragment);
    RUN(TEST_cJSON_Parse_Full_JSON_File);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Show the dynamic memory status. (How many malloc / calloc calls were done and is the number equal with the
 * amount of free calls). -> Exists memory leaks ?
 */
static void
At_Exit_Function
(
        void
)
{
    puts ("\n");
    Show_Dynamic_Memory_Status();
    return;
}

//---------------------------------------------------------------------------------------------------------------------
