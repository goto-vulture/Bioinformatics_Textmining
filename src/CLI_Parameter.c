/**
 * @file CLI_Parameter.c
 *
 * @brief CLI parameter of the program.
 *
 * Implemented with global variables. A access at every point is for the usage necessary.
 *
 * @date 22.01.2022
 * @author: am1
 */

#include "CLI_Parameter.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "Print_Tools.h"
#include "String_Tools.h"
#include "Defines.h"
#include "Misc.h"
#include "Error_Handling/Dynamic_Memory.h"



const char* const GLOBAL_USAGES [] =
{
     "Bioinformatics_Textmining [options] [[--] args]",
     "Bioinformatics_Textmining [options]",
     NULL,
};

const char* const GLOBAL_PROGRAM_DESCRIPTION            =
        "\nThe program do a determination of so-called \"dominating word sets\" from data sets in the field of bioinformatics (v" VERSION_STR ").";
const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION =
        "\nThis idea will be develop as a project for the Ruhr University Bochum (RUB)."
        "\nIf you have any questions or suggestions: Get in contact with me: goto-vulture@gmx.de"
        "\n"
        "\nUsed libs"
        "\n    cJSON    1.7.15 MIT from Dave Gamble (https://github.com/DaveGamble/cJSON)"
        "\n    argparse 1.1.0  MIT from Yecheng Fu  (https://github.com/cofyc/argparse)"
        "\n    tinytest n/a    n/a from Joe Walnes  (https://github.com/joewalnes/tinytest)";



// Default values of the CLI parameter
#ifndef GLOBAL_CLI_INPUT_FILE_DEFAULT
#define GLOBAL_CLI_INPUT_FILE_DEFAULT NULL
#else
#error "The macro \"GLOBAL_CLI_INPUT_FILE_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_INPUT_FILE_DEFAULT */

#ifndef GLOBAL_CLI_INPUT_FILE_DEFAULT_2
#define GLOBAL_CLI_INPUT_FILE_DEFAULT_2 NULL
#else
#error "The macro \"GLOBAL_CLI_INPUT_FILE_DEFAULT_2\" is already defined !"
#endif /* GLOBAL_CLI_INPUT_FILE_DEFAULT_2 */

#ifndef GLOBAL_CLI_OUTPUT_FILE_DEFAULT
#define GLOBAL_CLI_OUTPUT_FILE_DEFAULT NULL
#else
#error "The macro \"GLOBAL_CLI_OUTPUT_FILE_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_OUTPUT_FILE_DEFAULT */

#ifndef GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT
#define GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT */

#ifndef GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT
#define GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT */

#ifndef GLOBAL_CLI_WORD_OFFSET_DEFAULT
#define GLOBAL_CLI_WORD_OFFSET_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_WORD_OFFSET_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_WORD_OFFSET_DEFAULT */

#ifndef GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT
#define GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT false
#else
#error "The macro \"GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT\" is already defined !"
#endif /* GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT */

#ifndef GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT
#define GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT */

#ifndef GLOBAL_CLI_NO_PART_MATCHES_DEFAULT
#define GLOBAL_CLI_NO_PART_MATCHES_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_NO_PART_MATCHES_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_NO_PART_MATCHES_DEFAULT */

#ifndef GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT
#define GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT */

#ifndef GLOBAL_CLI_NO_TIMESTAMP_DEFAULT
#define GLOBAL_CLI_NO_TIMESTAMP_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_NO_TIMESTAMP_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_NO_TIMESTAMP_DEFAULT */

#ifndef GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT
#define GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT */

#ifndef GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT
#define GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT false
#else
#error "The macro \"GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT\" is already defined !"
#endif /* GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT */

#ifndef GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT
#define GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT (float) NAN
#else
#error "The macro \"GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT\" is already defined !"
#endif /* GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT */

// Variables for the parsed CLI parameter
const char* GLOBAL_CLI_INPUT_FILE               = GLOBAL_CLI_INPUT_FILE_DEFAULT;
const char* GLOBAL_CLI_INPUT_FILE2              = GLOBAL_CLI_INPUT_FILE_DEFAULT_2;
const char* GLOBAL_CLI_OUTPUT_FILE              = GLOBAL_CLI_OUTPUT_FILE_DEFAULT;
_Bool GLOBAL_CLI_FORMAT_OUTPUT                  = GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT;
_Bool GLOBAL_CLI_SENTENCE_OFFSET                = GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT;
_Bool GLOBAL_CLI_WORD_OFFSET                    = GLOBAL_CLI_WORD_OFFSET_DEFAULT;
_Bool GLOBAL_RUN_ALL_TEST_FUNCTIONS             = GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT;
_Bool GLOBAL_CLI_SHOW_TOO_LONG_TOKENS           = GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT;
_Bool GLOBAL_CLI_NO_PART_MATCHES                = GLOBAL_CLI_NO_PART_MATCHES_DEFAULT;
_Bool GLOBAL_CLI_NO_FULL_MATCHES                = GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT;
_Bool GLOBAL_CLI_NO_TIMESTAMP                   = GLOBAL_CLI_NO_TIMESTAMP_DEFAULT;
_Bool GLOBAL_CLI_NO_CPU_EXTENSIONS              = GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT;
_Bool GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN    = GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT;
float GLOBAL_ABORT_PROCESS_PERCENT              = GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT;



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check, whether the given CLI parameter have a logical consistency.
 *
 * E.g. --no_part_matches and --no_full_matches makes no sense.
 */
void Check_CLI_Parameter_Logical_Consistency (void)
{
    if (GLOBAL_CLI_NO_PART_MATCHES && GLOBAL_CLI_NO_FULL_MATCHES)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS(stderr, "\nThe --no_part_matches in combination with --no_full_matches makes no sense,"
                " because the results will alwas be empty sets !\n");
        EXIT(1);
    }

    const size_t length_input_file_1 = (GLOBAL_CLI_INPUT_FILE != NULL) ? strlen (GLOBAL_CLI_INPUT_FILE) : 0;
    const size_t length_input_file_2 = (GLOBAL_CLI_INPUT_FILE2 != NULL) ? strlen (GLOBAL_CLI_INPUT_FILE2) : 0;
    const size_t length_output_file = (GLOBAL_CLI_OUTPUT_FILE != NULL) ? strlen (GLOBAL_CLI_OUTPUT_FILE) : 0;

    if (length_input_file_1 == length_output_file &&
            strncmp(GLOBAL_CLI_INPUT_FILE, GLOBAL_CLI_OUTPUT_FILE, length_input_file_1) == 0)
    {
        FPRINTF_FFLUSH(stderr, "\nInput file 1 and the output file are the same files (%s) !\n", GLOBAL_CLI_INPUT_FILE);
        EXIT(1);
    }
    if (length_input_file_2 == length_output_file &&
            strncmp(GLOBAL_CLI_INPUT_FILE2, GLOBAL_CLI_OUTPUT_FILE, length_input_file_2) == 0)
    {
        FPRINTF_FFLUSH(stderr, "\nInput file 2 and the output file are the same files (%s) !\n", GLOBAL_CLI_INPUT_FILE2);
        EXIT(1);
    }
    if (length_input_file_1 == length_input_file_2 &&
            strncmp(GLOBAL_CLI_INPUT_FILE, GLOBAL_CLI_INPUT_FILE2, length_input_file_2) == 0)
    {
        FPRINTF_FFLUSH(stderr, "\nInput file 1 and 2 are the same files (%s) !\n", GLOBAL_CLI_INPUT_FILE);
        EXIT(1);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief @brief Test function for the CLI parameter, that is used as first input file name.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE (void)
{
    if (GLOBAL_CLI_INPUT_FILE == NULL)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The first input file name is NULL !\n");
        EXIT(1);
    }
    if (IS_STRING_LENGTH_ZERO(GLOBAL_CLI_INPUT_FILE))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The first input file name length is zero !\n");
        EXIT(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* input_file = fopen (GLOBAL_CLI_INPUT_FILE, "r");

    if (input_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the first input file \"%s\" !\n", GLOBAL_CLI_INPUT_FILE);
        EXIT(1);
    }

    FCLOSE_WITH_NAME_AND_SET_TO_NULL(input_file, GLOBAL_CLI_INPUT_FILE);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Test function for the CLI parameter, that is used as second input file name.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE2 (void)
{
    if (GLOBAL_CLI_INPUT_FILE2 == NULL)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The second input file name is NULL !\n");
        EXIT(1);
    }
    if (IS_STRING_LENGTH_ZERO(GLOBAL_CLI_INPUT_FILE2))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The second input file name length is zero !\n");
        EXIT(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* input_file = fopen (GLOBAL_CLI_INPUT_FILE2, "r");

    if (input_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the second input file \"%s\" !\n", GLOBAL_CLI_INPUT_FILE2);
        EXIT(1);
    }

    FCLOSE_WITH_NAME_AND_SET_TO_NULL(input_file, GLOBAL_CLI_INPUT_FILE2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Test function for the CLI parameter, that is used as output file name.
 */
void Check_CLI_Parameter_CLI_OUTPUT_FILE (void)
{
    if (GLOBAL_CLI_OUTPUT_FILE == NULL)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid output file name ! The output file name is NULL !\n");
        EXIT(1);
    }
    if (IS_STRING_LENGTH_ZERO(GLOBAL_CLI_OUTPUT_FILE))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid output file name ! The output file name length is zero !\n");
        EXIT(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* output_file = fopen (GLOBAL_CLI_OUTPUT_FILE, "w");

    if (output_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the output file \"%s\" !\n", GLOBAL_CLI_OUTPUT_FILE);
        EXIT(1);
    }

    FCLOSE_WITH_NAME_AND_SET_TO_NULL(output_file, GLOBAL_CLI_OUTPUT_FILE);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Test function for the abort percent value.
 */
void Check_CLI_Parameter_GLOBAL_ABORT_PROCESS_PERCENT (void)
{
    if (isinf(GLOBAL_ABORT_PROCESS_PERCENT))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS(stderr, "Abort percent value is +/-Inf !\n")
        EXIT (1);
    }
    // A isnan check is not necessary because a NaN value is the default value of this parameter and indicates, that no
    // abort percent value was given with the CLI parameter
    /*if (isnanf(GLOBAL_ABORT_PROCESS_PERCENT))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS(stderr, "Abort percent value is NaN !\n")
        EXIT (1);
    }*/

    if (GLOBAL_ABORT_PROCESS_PERCENT < 0.0f || GLOBAL_ABORT_PROCESS_PERCENT > 100.0f)
    {
        FPRINTF_FFLUSH(stderr, "Abort percent value (%f) is not in a percent range \n",
                GLOBAL_ABORT_PROCESS_PERCENT)
        EXIT (1);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Set all CLI parameter to the default values.
 *
 * This is only useful for the TEST functions.
 */
extern void Set_CLI_Parameter_To_Default_Values (void)
{
    GLOBAL_CLI_INPUT_FILE                   = GLOBAL_CLI_INPUT_FILE_DEFAULT;
    GLOBAL_CLI_INPUT_FILE2                  = GLOBAL_CLI_INPUT_FILE_DEFAULT_2;
    GLOBAL_CLI_OUTPUT_FILE                  = GLOBAL_CLI_OUTPUT_FILE_DEFAULT;
    GLOBAL_CLI_FORMAT_OUTPUT                = GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT;
    GLOBAL_CLI_SENTENCE_OFFSET              = GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT;
    GLOBAL_CLI_WORD_OFFSET                  = GLOBAL_CLI_WORD_OFFSET_DEFAULT;
    GLOBAL_RUN_ALL_TEST_FUNCTIONS           = GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT;
    GLOBAL_CLI_SHOW_TOO_LONG_TOKENS         = GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT;
    GLOBAL_CLI_NO_PART_MATCHES              = GLOBAL_CLI_NO_PART_MATCHES_DEFAULT;
    GLOBAL_CLI_NO_FULL_MATCHES              = GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT;
    GLOBAL_CLI_NO_TIMESTAMP                 = GLOBAL_CLI_NO_TIMESTAMP_DEFAULT;
    GLOBAL_CLI_NO_CPU_EXTENSIONS            = GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT;
    GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN  = GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT;
    GLOBAL_ABORT_PROCESS_PERCENT            = GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

// Delete macros for the default values of the CLI parameter
#ifdef GLOBAL_CLI_INPUT_FILE_DEFAULT
#undef GLOBAL_CLI_INPUT_FILE_DEFAULT
#endif /* GLOBAL_CLI_INPUT_FILE_DEFAULT */

#ifdef GLOBAL_CLI_INPUT_FILE_DEFAULT_2
#undef GLOBAL_CLI_INPUT_FILE_DEFAULT_2
#endif /* GLOBAL_CLI_INPUT_FILE_DEFAULT_2 */

#ifdef GLOBAL_CLI_OUTPUT_FILE_DEFAULT
#undef GLOBAL_CLI_OUTPUT_FILE_DEFAULT
#endif /* GLOBAL_CLI_OUTPUT_FILE_DEFAULT */

#ifdef GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT
#undef GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT
#endif /* GLOBAL_CLI_FORMAT_OUTPUT_DEFAULT */

#ifdef GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT
#undef GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT
#endif /* GLOBAL_CLI_SENTENCE_OFFSET_DEFAULT */

#ifdef GLOBAL_CLI_WORD_OFFSET_DEFAULT
#undef GLOBAL_CLI_WORD_OFFSET_DEFAULT
#endif /* GLOBAL_CLI_WORD_OFFSET_DEFAULT */

#ifdef GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT
#undef GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT
#endif /* GLOBAL_RUN_ALL_TEST_FUNCTIONS_DEFAULT */

#ifdef GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT
#undef GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT
#endif /* GLOBAL_CLI_SHOW_TOO_LONG_TOKENS_DEFAULT */

#ifdef GLOBAL_CLI_NO_PART_MATCHES_DEFAULT
#undef GLOBAL_CLI_NO_PART_MATCHES_DEFAULT
#endif /* GLOBAL_CLI_NO_PART_MATCHES_DEFAULT */

#ifdef GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT
#undef GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT
#endif /* GLOBAL_CLI_NO_FULL_MATCHES_DEFAULT */

#ifdef GLOBAL_CLI_NO_TIMESTAMP_DEFAULT
#undef GLOBAL_CLI_NO_TIMESTAMP_DEFAULT
#endif /* GLOBAL_CLI_NO_TIMESTAMP_DEFAULT */

#ifdef GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT
#undef GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT
#endif /* GLOBAL_CLI_NO_CPU_EXTENSIONS_DEFAULT */

#ifdef GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT
#undef GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT
#endif /* GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN_DEFAULT */

#ifdef GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT
#undef GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT
#endif /* GLOBAL_ABORT_PROCESS_PERCENT_DEFAULT */
