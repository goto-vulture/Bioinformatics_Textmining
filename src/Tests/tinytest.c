/**
 * @file tinytest.c
 *
 * @brief TinyTest: A really really really tiny and simple no-hassle C unit-testing framework.
 *
 * @date 2010
 * @author Joe Walnes <joe@walnes.com> http://joewalnes.com
 *
 * Angepasst von: x86 / Gyps.
 */

#include "tinytest.h"
#include <string.h>





int tt_passes                       = 0;        // Anzahl an Tests, die erfolgreich durchgefuehrt wurden
int tt_fails                        = 0;        // Anzahl an Tests, die fehlgeschlagen sind
int tt_current_test_failed          = 0;        // Ist der akuelle Test fehlgeschlagen ?
const char* tt_current_msg          = NULL;     // Nachricht des aktuellen Tests
const char* tt_current_expression   = NULL;     // Aktueller Assert-Ausdruck
const char* tt_current_file         = NULL;     // Aktueller Dateiname
int tt_current_line                 = 0;        // Aktuelle Zeile

char tt_failed_function_names [32][128];        // Funktionsnamen, bei denen der Test fehlgeschlagen ist
int tt_current_failed_function_name = 0;        // Aktuller Index fuer das Array, welches die Namen der fehlgeschlagenen
                                                // Funktionen sichert



//---------------------------------------------------------------------------------------------------------------------

void tt_execute (const char *name, void (*test_function) (void))
{
    printf ("\n\n===== ===== ===== START RUN (%s) ===== ===== =====\n", name);

    // Variable wird durch die Makros ASSERT_EQUALS und ASSERT_STRING_EQUALS angepasst !
    tt_current_test_failed = 0;

    // Test-Funktion ausfuehren
    test_function ();

    if (tt_current_test_failed)
    {
        printf ("failure: %s:%d: In test %s():\n    %s (%s)\n", tt_current_file, tt_current_line, name, tt_current_msg,
                tt_current_expression);
        tt_fails ++;

        // Solane es noch weitere freie C-Strings gibt, werden die Namen der Funktionen, bei denen der Test
        // fehlgeschlagen ist, aufgezeichnet
        if ((size_t) tt_current_failed_function_name <
                (sizeof (tt_failed_function_names) / sizeof (tt_failed_function_names [0]) - 1))
        {
            // Zeiger auf das aktuelle Objekt erstellen, um den folgenden Code kuerzer formulieren zu koennen
            char* current_container_name = tt_failed_function_names [tt_current_failed_function_name];
            const size_t current_container_name_size = sizeof (tt_failed_function_names [tt_current_failed_function_name]);

            memset (current_container_name, '\0', current_container_name_size);
            strncpy (current_container_name, name, current_container_name_size);

            // Nullterminierung garantieren
            current_container_name [current_container_name_size - 1] = '\0';

            tt_current_failed_function_name ++;
        }
    }
    else
    {
        tt_passes ++;
    }

    printf ("===== ===== ===== END RUN (%s) ===== ===== =====\n\n", name);
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

int tt_assert (const char *file, int line, const char *msg, const char *expression, int pass)
{
    tt_current_msg = msg;
    tt_current_expression = expression;
    tt_current_file = file;
    tt_current_line = line;
    tt_current_test_failed = !pass;
    return pass;
}

//---------------------------------------------------------------------------------------------------------------------

int tt_report (void)
{
    if (tt_fails)
    {
        for (int i = 0; i < tt_current_failed_function_name; ++ i)
        {
            printf ("Failed function %2d: %s\n", i + 1, tt_failed_function_names [i]);
        }

        printf ("\n%c%sFAILED%c%s [%s] (passed:%d, failed:%d, total:%d)\n",
        TT_COLOR_CODE, TT_COLOR_RED, TT_COLOR_CODE, TT_COLOR_RESET, tt_current_file, tt_passes, tt_fails,
                tt_passes + tt_fails);

        return -1;
    }
    else
    {
        printf ("\n%c%sPASSED%c%s [%s] (total:%d)\n",
        TT_COLOR_CODE, TT_COLOR_GREEN, TT_COLOR_CODE, TT_COLOR_RESET, tt_current_file, tt_passes);
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
