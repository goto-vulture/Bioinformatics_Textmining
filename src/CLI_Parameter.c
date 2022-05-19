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
     "IsomerY [options] [[--] args]",
     "IsomerY [options]",
     NULL,
};

const char* const GLOBAL_PROGRAM_DESCRIPTION            = "Programmbeschreibung";
const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION = "Weitere Programmbeschreibung";



// Variablen fuer die geparsten CLI-Parameter
int GLOBAL_MAX_C_ATOMS_FOR_TESTS = 0;

_Bool GLOBAL_SELECT_TEST_FUNCTION = false;

int GLOBAL_RUN_TEST_FUNCTION_INDEX = -1;



//---------------------------------------------------------------------------------------------------------------------

/*
 * Testfunktion fuer die Anzahl an C-Atomen, die bei den Tests verwendet werden.
 */
void Check_CLI_Parameter_MAX_C_ATOMS_FOR_TESTS (void)
{
    const int max_c_atom_for_tests = 14;

    if (GLOBAL_MAX_C_ATOMS_FOR_TESTS > max_c_atom_for_tests)
    {
        FPRINTF_FFLUSH(stderr, "Too much c atoms for the tests ! Current maximum: %d\n", max_c_atom_for_tests)
        exit(1);
    }
    return;
}

//---------------------------------------------------------------------------------------------------------------------
