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
#include "Error_Handling/Dynamic_Memory.h"



const char* const GLOBAL_USAGES [] =
{
     "Bioinformatics_Textmining [options] [[--] args]",
     "Bioinformatics_Textmining [options]",
     NULL,
};

const char* const GLOBAL_PROGRAM_DESCRIPTION            = "Program description";
const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION = "Additional program description";

// Variables for the parsed CLI parameter
const char* GLOBAL_CLI_INPUT_FILE = NULL;
const char* GLOBAL_CLI_INPUT_FILE2 = NULL;
const char* GLOBAL_CLI_OUTPUT_FILE = NULL;
_Bool GLOBAL_CLI_FORMAT_OUTPUT = false;
_Bool GLOBAL_CLI_SENTENCE_OFFSET = false;
_Bool GLOBAL_CLI_WORD_OFFSET = false;
_Bool GLOBAL_RUN_ALL_TEST_FUNCTIONS = false;
_Bool GLOBAL_CLI_NO_PART_MATCHES = false;
_Bool GLOBAL_CLI_NO_FULL_MATCHES = false;
float GLOBAL_ABORT_PROCESS_PERCENT = NAN;



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die erste Eingabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE (void)
{
    if (GLOBAL_CLI_INPUT_FILE == NULL || strlen (GLOBAL_CLI_INPUT_FILE) == 0)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file ! Either the file name is NULL or the name length is "
                "zero !\n");
        exit(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* input_file = fopen (GLOBAL_CLI_INPUT_FILE, "r");

    if (input_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the first input file \"%s\" !\n", GLOBAL_CLI_INPUT_FILE);
        exit(1);
    }

    FCLOSE_AND_SET_TO_NULL(input_file);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die zweite Eingabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE2 (void)
{
    if (GLOBAL_CLI_INPUT_FILE2 == NULL || strlen (GLOBAL_CLI_INPUT_FILE2) == 0)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid input file ! Either the file name is NULL or the name length is "
                "zero !\n");
        exit(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* input_file = fopen (GLOBAL_CLI_INPUT_FILE2, "r");

    if (input_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the second input file \"%s\" !\n", GLOBAL_CLI_INPUT_FILE2);
        exit(1);
    }

    FCLOSE_AND_SET_TO_NULL(input_file);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die Ausgabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_OUTPUT_FILE (void)
{
    if (GLOBAL_CLI_OUTPUT_FILE == NULL || strlen (GLOBAL_CLI_OUTPUT_FILE) == 0)
    {
        FPRINTF_FFLUSH_NO_VA_ARGS (stderr, "Invalid output file name ! Either the file name is NULL or the name length "
                "is zero !\n");
        exit(1);
    }

    // Testweise die Eingabedatei oeffnen
    FILE* output_file = fopen (GLOBAL_CLI_OUTPUT_FILE, "w");

    if (output_file == NULL)
    {
        FPRINTF_FFLUSH (stderr, "Cannot open the output file \"%s\" !\n", GLOBAL_CLI_OUTPUT_FILE);
        exit(1);
    }

    FCLOSE_AND_SET_TO_NULL(output_file);

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
