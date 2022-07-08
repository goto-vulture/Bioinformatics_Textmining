/**
 * @file CLI_Parameter.h
 *
 * @brief CLI parameter of the program.
 *
 * Implemented with global variables. A access at every point is for the usage necessary.
 *
 * @date 22.01.2022
 * @author: am1
 */

#ifndef CLI_PARAMETER_H
#define CLI_PARAMETER_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdbool.h>



extern const char* const GLOBAL_USAGES []; ///< Description of the CLI interface usage

extern const char* const GLOBAL_PROGRAM_DESCRIPTION; ///< General program description
extern const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION; ///< Additional program description



// Variablen fuer die geparsten CLI-Parameter
/**
 * @brief CLI-Parameter: Erste Eingabedatei
 */
extern const char* GLOBAL_CLI_INPUT_FILE;

/**
 * @brief CLI-Parameter: Zweite Eingabedatei
 */
extern const char* GLOBAL_CLI_INPUT_FILE2;

/**
 * @brief CLI-Parameter: Ausgabedatei
 */
extern const char* GLOBAL_CLI_OUTPUT_FILE;



/**
 * @brief Testfunktion fuer den CLI Parameter, der die erste Eingabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE (void);

/**
 * @brief Testfunktion fuer den CLI Parameter, der die zweite Eingabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE2 (void);

/**
 * @brief Testfunktion fuer den CLI Parameter, der die Ausgabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_OUTPUT_FILE (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
