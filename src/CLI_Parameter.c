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
        "\n    cJSON    1.7.15 from Dave Gamble (https://github.com/DaveGamble/cJSON)"
        "\n    argparse 1.1.0  from Yecheng Fu  (https://github.com/cofyc/argparse)"
        "\n    tinytest n/a    from Joe Walnes  (https://github.com/joewalnes/tinytest)";

// Variables for the parsed CLI parameter
const char* GLOBAL_CLI_INPUT_FILE = NULL;
const char* GLOBAL_CLI_INPUT_FILE2 = NULL;
const char* GLOBAL_CLI_OUTPUT_FILE = NULL;
_Bool GLOBAL_CLI_FORMAT_OUTPUT = false;
_Bool GLOBAL_CLI_SENTENCE_OFFSET = false;
_Bool GLOBAL_CLI_WORD_OFFSET = false;
_Bool GLOBAL_RUN_ALL_TEST_FUNCTIONS = false;
_Bool GLOBAL_CLI_SHOW_TOO_LONG_TOKENS = false;
_Bool GLOBAL_CLI_NO_PART_MATCHES = false;
_Bool GLOBAL_CLI_NO_FULL_MATCHES = false;
float GLOBAL_ABORT_PROCESS_PERCENT = NAN;



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die erste Eingabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE (void)
{
    if (GLOBAL_CLI_INPUT_FILE == NULL)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The first input file name is NULL !\n");
        exit(1);
    }
    if (IS_STRING_LENGTH_ZERO(GLOBAL_CLI_INPUT_FILE))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The first input file name length is zero !\n");
        exit(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* input_file = fopen (GLOBAL_CLI_INPUT_FILE, "r");

    if (input_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the first input file \"%s\" !\n", GLOBAL_CLI_INPUT_FILE);
        exit(1);
    }

    FCLOSE_WITH_NAME_AND_SET_TO_NULL(input_file, GLOBAL_CLI_INPUT_FILE);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die zweite Eingabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE2 (void)
{
    if (GLOBAL_CLI_INPUT_FILE2 == NULL)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The second input file name is NULL !\n");
        exit(1);
    }
    if (IS_STRING_LENGTH_ZERO(GLOBAL_CLI_INPUT_FILE2))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file name ! The second input file name length is zero !\n");
        exit(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* input_file = fopen (GLOBAL_CLI_INPUT_FILE2, "r");

    if (input_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the second input file \"%s\" !\n", GLOBAL_CLI_INPUT_FILE2);
        exit(1);
    }

    FCLOSE_WITH_NAME_AND_SET_TO_NULL(input_file, GLOBAL_CLI_INPUT_FILE2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die Ausgabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_OUTPUT_FILE (void)
{
    if (GLOBAL_CLI_OUTPUT_FILE == NULL)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid output file name ! The output file name is NULL !\n");
        exit(1);
    }
    if (IS_STRING_LENGTH_ZERO(GLOBAL_CLI_OUTPUT_FILE))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid output file name ! The output file name length is zero !\n");
        exit(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* output_file = fopen (GLOBAL_CLI_OUTPUT_FILE, "w");

    if (output_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the output file \"%s\" !\n", GLOBAL_CLI_OUTPUT_FILE);
        exit(1);
    }

    FCLOSE_WITH_NAME_AND_SET_TO_NULL(output_file, GLOBAL_CLI_OUTPUT_FILE);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunction for the abort percent value.
 */
void Check_CLI_Parameter_GLOBAL_ABORT_PROCESS_PERCENT (void)
{
    if (isinf(GLOBAL_ABORT_PROCESS_PERCENT))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS(stderr, "Abort percent value is +/-Inf !\n")
        exit (1);
    }
    // A isnan check is not necessary because a NaN value is the default value of this parameter and indicates, that no
    // abort percent value was given with the CLI parameter
    /*if (isnanf(GLOBAL_ABORT_PROCESS_PERCENT))
    {
        FPRINTF_FFLUSH_NO_VA_ARGS(stderr, "Abort percent value is NaN !\n")
        exit (1);
    }*/

    if (GLOBAL_ABORT_PROCESS_PERCENT < 0.0f || GLOBAL_ABORT_PROCESS_PERCENT > 100.0f)
    {
        FPRINTF_FFLUSH(stderr, "Abort percent value (%f) is not in a percent range \n",
                GLOBAL_ABORT_PROCESS_PERCENT)
        exit (1);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
