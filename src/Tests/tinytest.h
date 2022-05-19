/**
 * @file tinytest.h
 *
 * @brief TinyTest: A really really really tiny and simple no-hassle C unit-testing framework.
 *
 * Features:
 *   - No library dependencies. Not even itself. Just a header file.
 *   - Simple ANSI C. Should work with virtually every C or C++ compiler on
 *     virtually any platform.
 *   - Reports assertion failures, including expressions and line numbers.
 *   - Stops test on first failed assertion.
 *   - ANSI color output for maximum visibility.
 *   - Easy to embed in apps for runtime tests (e.g. environment tests).
 *
 * Example Usage:
 *
 *    #include "tinytest.h"
 *    #include "mylib.h"
 *
 *    void test_sheep()
 *    {
 *      ASSERT("Sheep are cool", are_sheep_cool());
 *      ASSERT_EQUALS(4, sheep.legs);
 *    }
 *
 *    void test_cheese()
 *    {
 *      ASSERT("Cheese is tangy", cheese.tanginess > 0);
 *      ASSERT_STRING_EQUALS("Wensleydale", cheese.name);
 *    }
 *
 *    int main()
 *    {
 *      RUN(test_sheep);
 *      RUN(test_cheese);
 *      return TEST_REPORT();
 *    }
 *
 * To run the tests, compile the tests as a binary and run it.
 *
 * Project home page: http://github.com/joewalnes/tinytest
 *
 * @date 2010
 * @author Joe Walnes <joe@walnes.com> http://joewalnes.com
 *
 * Angepasst von: x86 / Gyps.
 */

#ifndef _TINYTEST_INCLUDED
#define _TINYTEST_INCLUDED ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>
#include <stdlib.h>
#include "../String_Tools.h"



/**
 * @brief Haupt-Assert Makro.
 */
#ifndef ASSERT
#define ASSERT(msg, expression) if (!tt_assert(__FILE__, __LINE__, (msg), (#expression), (expression) ? 1 : 0)) return
#else
#error "The macro \"ASSERT\" is already defined !"
#endif /* ASSERT */

/**
 * @brief Assert fuer die Gleichheit ganzzahliger Werte.
 *
 * Ein Vergleich von Gleitkommazahlen kann durchgefuehrt werden; ist aber wegen der potenziellen Ungenauigkeit und
 * Fehleranfaelligkeit von Vergleichen mittels == nicht empfohlen.
 */
#ifndef ASSERT_EQUALS
#define ASSERT_EQUALS(expected, actual) ASSERT((#actual), (expected) == (actual))
#else
#error "The macro \"ASSERT_EQUALS\" is already defined !"
#endif /* ASSERT_EQUALS */

/**
 * @brief Assert fuer die Gleichheit von C-Strings.
 */
#ifndef ASSERT_STRING_EQUALS
#define ASSERT_STRING_EQUALS(expected, actual)                                                      \
    if (strcmp((expected),(actual)) != 0)                                                           \
    {                                                                                               \
        PRINTF_FFLUSH("Compare \"%s\" (expected) and \"%s\" (actual).\n", (expected), (actual));    \
        ASSERT((#actual), strcmp((expected),(actual)) == 0);                                        \
    }
#else
#error "The macro \"ASSERT_STRING_EQUALS\" is already defined !"
#endif /* ASSERT_STRING_EQUALS */

/**
 * @brief Assert fuer die Gleichheit von C-Strings ohne Beachtung der Gross- und Kleinschreibung.
 */
#ifndef ASSERT_STRING_CASE_INSENSITIVE_EQUALS
#define ASSERT_STRING_CASE_INSENSITIVE_EQUALS(expected, actual)                                                     \
    if (Compare_Strings_Case_Insensitive((expected),(actual)) != 0)                                                 \
    {                                                                                                               \
        PRINTF_FFLUSH("Compare \"%s\" (expected) and \"%s\" (actual) case insensitive.\n", (expected), (actual));   \
        ASSERT((#actual), Compare_Strings_Case_Insensitive((expected),(actual)) == 0);                              \
    }
#else
#error "The macro \"ASSERT_STRING_CASE_INSENSITIVE_EQUALS\" is already defined !"
#endif /* ASSERT_STRING_CASE_INSENSITIVE_EQUALS */

/**
 * @brief Test-Funktion starten.
 */
#ifndef RUN
#define RUN(test_function) tt_execute((#test_function), (test_function))
#else
#error "The macro \"RUN\" is already defined !"
#endif /* RUN */

/**
 * @brief Test-Funktion (mit manueller Zeichenkette als Funktionsname) starten.
 */
#ifndef RUN_2
#define RUN_2(test_function, manual_test_function_name) tt_execute((manual_test_function_name), (test_function))
#else
#error "The macro \"RUN_2\" is already defined !"
#endif /* RUN_2 */

/**
 * @brief Bericht aller bisher stattgefundenen Asserts ausgeben.
 */
#ifndef TEST_REPORT
#define TEST_REPORT() tt_report()
#else
#error "The macro \"TEST_REPORT\" is already defined !"
#endif /* TEST_REPORT */

#ifndef TT_COLOR_CODE
#define TT_COLOR_CODE 0x1B ///< Einleitungswert fuer das Terminal, dass ein Farbcode folgt ("Escape"-Steuerzeichen)
#else
#error "The macro \"TT_COLOR_CODE\" is already defined !"
#endif /* TT_COLOR_CODE */
#ifndef TT_COLOR_RED
#define TT_COLOR_RED "[1;31m" ///< Farbcode fuer rote Ausgaben im Terminal (ANSI-Escape-Sequenz)
#else
#error "The macro \"TT_COLOR_RED\" is already defined !"
#endif /* TT_COLOR_RED */
#ifndef TT_COLOR_GREEN
#define TT_COLOR_GREEN "[1;32m" ///< Farbcode fuer gruene Ausgaben im Termial (ANSI-Escape-Sequenz)
#else
#error "The macro \"TT_COLOR_GREEN\" is already defined !"
#endif /* TT_COLOR_GREEN */
#ifndef TT_COLOR_RESET
#define TT_COLOR_RESET "[0m" ///< Aktell gesetzten Farbcode zuruecksetzen (ANSI-Escape-Sequenz)
#else
#error "The macro \"TT_COLOR_RESET\" is already defined !"
#endif /* TT_COLOR_RESET */



extern int tt_passes;                           ///< Anzahl an Tests, die erfolgreich durchgefuehrt wurden
extern int tt_fails;                            ///< Anzahl an Tests, die fehlgeschlagen sind
extern int tt_current_test_failed;              ///< Ist der akuelle Test fehlgeschlagen ?
extern const char* tt_current_msg;              ///< Nachricht des aktuellen Tests
extern const char* tt_current_expression;       ///< Aktueller Assert-Ausdruck
extern const char* tt_current_file;             ///< Aktueller Dateiname
extern int tt_current_line;                     ///< Aktuelle Zeile

extern char tt_failed_function_names [32][128]; ///< Funktionsnamen, bei denen der Test fehlgeschlagen ist
/**
 * @brief Aktuller Index fuer das Array, welches die Namen der fehlgeschlagenen Funktionen sichert
 */
extern int tt_current_failed_function_name;



/**
 * @brief Zu testende Funktion starten.
 *
 * Die Testfunktion (Signatur: void NAME (void)) wird uebergeben und gestartet. In der Testfunktion befinden sich
 * Makros, die - falls ein Test fehlschlaegt - eine globale Variable anpassen. Dadurch erkennt die tt_execute()
 * Funktion, dass mind. ein Test fehlschlug.
 *
 * ! Das Starten eins Tests erfolgt ueber das Makro "RUN" !
 * Diese Funktion muss nie direkt ausgefuehrt werden !
 *
 * @param[in] name Name des Testes. Wird fuer Statusausgaben verwendet. I.d.R. ist diese Variable der Name der zu
 * testenden Funktion.
 * @param[in] test_function Die zu testende Funktion. Grundsignatur: void NAME (void)
 *
 * @return N/A
 */
extern void tt_execute (const char *name, void (*test_function) (void));

/**
 * @brief Die Assert-Funktion.
 *
 * In dieser Funktion werden die Statusvariablen auf den passenden Wert gesetzt.
 *
 * ! Wie bei der Funktion tt_execute(): Diese Funktion muss nie direkt aufgerufen werden ! Der Aufruf geschieht ueber
 *   passenden Makros (ASSERT, ASSERT_EQUALS, ...)
 *
 * @param[in] file Name der Quelltestdatei, in der der Test ausgefuehrt wird (Fuer praezisere Fehlerausgaben)
 * @param[in] line Die Zeilennummer, wo der Test ausgefuehrt wird (Fuer praezisere Fehlerausgaben)
 * @param[in] msg Nachricht die ausgegeben werden soll, wenn der Test fehlschlug
 * @param[in] expression Der Assert-Ausdruck als Zeichenkette (wird mittels einer Praeprozessor-Anweisung in den Makros
 * erzeugt)
 * @param[in] pass War der Test (siehe: expression Variable) erfolgreich ?
 *
 * @return Ergebnis des Assert-Ausdrucks
 */
extern int tt_assert (const char *file, int line, const char *msg, const char *expression, int pass);

/**
 * @brief Bericht aller bisher stattgefundenen Asserts ausgeben.
 *
 * Wie viele Asserts waren erfolgreich; wie viele sind fehlgeschlagen ?
 *
 * @return N/A
 */
extern int tt_report (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TINYTEST_INCLUDED */
