/**
 * @file CLI_Parameter.c
 *
 * @brief CLI-Parameter des Programms.
 *
 * @date 22.01.2022
 * @author: am1
 */

#include "CLI_Parameter.h"
#include <stddef.h>
#include <stdlib.h>
#include "Print_Tools.h"



const char* const GLOBAL_USAGES [] =
{
     "Bioinformatics_Textmining [options] [[--] args]",
     "Bioinformatics_Textmining [options]",
     NULL,
};

const char* const GLOBAL_PROGRAM_DESCRIPTION            = "Programmbeschreibung";
const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION = "Weitere Programmbeschreibung";



// Variablen fuer die geparsten CLI-Parameter
const char* GLOBAL_CLI_INPUT_FILE = 0;
const char* GLOBAL_CLI_OUTPUT_FILE = 0;



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die erste Eingabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_INPUT_FILE (void)
{
    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testfunktion fuer den CLI Parameter, der die Ausgabedatei beschreibt.
 */
void Check_CLI_Parameter_CLI_OUTPUT_FILE (void)
{
    return;
}

//---------------------------------------------------------------------------------------------------------------------
