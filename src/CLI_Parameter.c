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
_Bool GLOBAL_RUN_ALL_TEST_FUNCTIONS = false;



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
