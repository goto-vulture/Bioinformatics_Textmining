/**
 * @file main.c
 *
 * @brief Die Bestimmung von sogenannten "Dominating Word Sets" aus Datensätzen im Bereich der Bioinformatik.
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
 * C-Version ueberpruefen, um Schluesselwoerter u.U. durch leere Makros bzw. Alternativwerte zu ersetzen
 *
 * Das Makro __STDC_VERSION__ ist fuer C90 nicht definiert
 * ->   Schluesselwoerter inline und restrict durch leere Markos ersetzen, damit eine Uebersetzung auch mit C90
 *      moeglich ist.
 */
#ifndef __STDC_VERSION__
    /**
     * @brief Das Schluesselwort "inline" als Makro definieren, wenn der C90 Standard verwendet wird.
     *
     * In diesem Standard existiert "inline" als Schluesselwort noch nicht. Die Uebersetzung wuerde an diesen
     * Stellen mit einem Fehler abbrechen.
     * Die Loesung: "inline" wird ein leeres Makro
     */
    #ifndef inline
        #define inline
    #else
        #error "The macro \"inline\" is already defined !"
    #endif /* inline */

    /**
     * @brief Das Schluesselwort "restrict" als Makro definieren, wenn der C90 Standard verwendet wird.
     *
     * In diesem Standard existiert "restrict" als Schluesselwort noch nicht. Die Uebersetzung wuerde an diesen
     * Stellen mit einem Fehler abbrechen.
     * Die Loesung: "restrict" wird ein leeres Makro
     */
    #ifndef restrict
        #define restrict
    #else
        #error "The macro \"restrict\" is already defined !"
    #endif /* restrict */

    /**
     * @brief Die Problematik mit Wahrheitswerten vor C99 loesen, indem "_Bool" in diesen Faellen mittels Makro mit "int"
     * ersetzt wird.
     */
    #ifndef _Bool
        #define _Bool int
    #else
        #error "The macro \"_Bool\" is already defined !"
    #endif /* _Bool */

    /**
     * @brief In seltenen Faellen ist "true" als Makro nicht vordefiniert. Z.B. vor C99
     */
    #ifndef true
        #define true 1
    #else
        #error "The macro \"true\" is already defined !"
    #endif /* true */

    /**
     * @brief In seltenen Faellen ist "false" als Makro nicht vordefiniert. Z.B. vor C99
     */
    #ifndef false
        #define false 0
    #else
        #error "The macro \"false\" is already defined !"
    #endif /* false */
#endif /* __STDC_VERSION__ */

/**
 * @brief Ein einfache Zeichenkette die anzeigt, dass eine Information nicht vorhanden ist.
 */
#ifndef N_A
    #define N_A "N/A"
#else
    #error "The macro \"N_A\" is already defined !"
#endif /* N_A */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Error_Handling/Dynamic_Memory.h"
#include "Tests/TEST_Document_Word_List.h"
#include "CLI_Parameter.h"
#include "argparse.h"



//=====================================================================================================================

/**
 * @brief main-Funktion des Programms.
 *
 * Aktuell werden die CLI-Parameter (noch) nicht verwendet.
 *
 * @param[in] argc argc-Parameter
 * @param[in] argv Anzahl an argc-Parametern
 *
 * @return 0 bei erfolgreicher Ausfuehrung; Ansonsten != 0
 */
int main (const int argc, const char* argv [])
{
    // ===== ===== ===== BEGINN CLI-Parameter parsen ===== ===== =====
    struct argparse_option cli_options [] =
    {
            OPT_HELP(),

            OPT_GROUP("Hauptfunktionen"),
            OPT_STRING('i', "input", &GLOBAL_CLI_INPUT_FILE, "Erste Eingabedatei", NULL, 0, 0),
            OPT_STRING('j', "input2", &GLOBAL_CLI_INPUT_FILE2, "Zweite Eingabedatei", NULL, 0, 0),
            OPT_STRING('o', "output", &GLOBAL_CLI_OUTPUT_FILE, "Ausgabedatei", NULL, 0, 0),

            OPT_END()
    };

    // CLI-Parameter parsen
    struct argparse argparse_object;
    argparse_init(&argparse_object, cli_options, GLOBAL_USAGES, 0);
    argparse_describe(&argparse_object, GLOBAL_PROGRAM_DESCRIPTION, GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION);
    const int new_argc = argparse_parse(&argparse_object, argc, argv);

    if (GLOBAL_CLI_INPUT_FILE != 0)
    {
        printf ("Input file: \"%s\"\n", GLOBAL_CLI_INPUT_FILE);
        Check_CLI_Parameter_CLI_INPUT_FILE();
    }
    if (GLOBAL_CLI_INPUT_FILE2 != 0)
    {
        printf ("Input file2: \"%s\"\n", GLOBAL_CLI_INPUT_FILE2);
        Check_CLI_Parameter_CLI_INPUT_FILE2();
    }
    if (GLOBAL_CLI_OUTPUT_FILE != 0)
    {
        printf ("Output file: \"%s\"\n", GLOBAL_CLI_OUTPUT_FILE);
        Check_CLI_Parameter_CLI_OUTPUT_FILE();
    }
    // ===== ===== ===== ENDE CLI-Parameter parsen ===== ===== =====

    Show_Dynamic_Memory_Status();

    return EXIT_SUCCESS;
}

//=====================================================================================================================
