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
 * @brief CLI-Parameter: Anzahl an C-Atomen, die bei den Tests verwendet werden sollen
 */
extern int GLOBAL_MAX_C_ATOMS_FOR_TESTS;

/**
 * @brief CLI-Parameter: Soll eine Testfunktion zur Laufzeit vom Nutzer ausgewaehlt werden ?
 */
extern _Bool GLOBAL_SELECT_TEST_FUNCTION;

/**
 * @brief CLI_Parameter: Eine bestimmte Testfunktion aufrufen, wobei der Index der Testfunktion ueber diesen
 * CLI-Parameter festgelegt wird.
 *
 * Vorteil im Vergleich zur Variante, wo man die Testfunktion waehrend der Ausfuehrung auswaehlen muss: Das Testen kann
 * direkt ohne Umwege erfolgen.
 */
extern int GLOBAL_RUN_TEST_FUNCTION_INDEX;



/**
 * @brief Testfunktion fuer die Anzahl an C-Atomen, die bei den Tests verwendet werden.
 */
extern void Check_CLI_Parameter_MAX_C_ATOMS_FOR_TESTS (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
