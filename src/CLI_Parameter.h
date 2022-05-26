/**
 * @file CLI_Parameter.h
 *
 * @brief CLI-Parameter des Programms.
 *
 * Implementiert als globale Variablen, damit von ueberall ein Zugriff moeglich ist.
 *
 * @date 22.01.2022
 * @author: am1
 */

#ifndef CLI_PARAMETER_H
#define CLI_PARAMETER_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdbool.h>



extern const char* const GLOBAL_USAGES []; ///< Beschreibung ueber die Verwendung des CLI Interfaces

extern const char* const GLOBAL_PROGRAM_DESCRIPTION; ///< Allgemeine Programmbeschreibung
extern const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION; ///< Zusaetzliche Programmbeschreibung



// Variablen fuer die geparsten CLI-Parameter
/**
 * @brief CLI-Parameter: Erste Eingabedatei
 */
extern const char* GLOBAL_CLI_INPUT_FILE;



/**
 * @brief Testfunktion fuer den CLI Parameter, der die erste Eingabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
