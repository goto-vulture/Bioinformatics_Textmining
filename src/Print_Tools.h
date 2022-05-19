/**
 * @file Print_Tools.h
 *
 * @brief Einige hilfreiche Ausgabe-Tools. Bereitgestellt als Makros.
 *
 * @date 13.03.2021
 * @author x86
 */

#ifndef PRINT_TOOLS_H
#define PRINT_TOOLS_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ausgabe mittels printf, wobei automatisch stdout geleert wird.
 *
 * Dies dient dazu, dass eine Ausgabe direkt auf dem Bildschirm erscheint.
 */
#ifndef PRINTF_FFLUSH
    #define PRINTF_FFLUSH(format_string, ...)                   \
    printf (format_string, __VA_ARGS__); fflush (stdout);
#else
    #error "The macro \"PRINTF_FFLUSH\" is already defined !"
#endif /* PRINTF_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ausgabe mittels fprintf, wobei automatisch der file Puffer geleert wird.
 *
 * Dies dient dazu, dass eine Ausgabe in die Datei direkt durchgefuehrt wird.
 */
#ifndef FPRINTF_FFLUSH
    #define FPRINTF_FFLUSH(file, format_string, ...)             \
    fprintf (file, format_string, __VA_ARGS__); fflush (file);
#else
    #error "The macro \"FPRINTF_FFLUSH\" is already defined !"
#endif /* FPRINTF_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ausgabe mittels printf, wobei automatisch stdout geleert wird.
 * Dies dient dazu, dass eine Ausgabe direkt auf dem Bildschirm erscheint.
 *
 * Der Hauptunterschied zwischen diesem Makro und der Verwendung eines Makros mit puts: Es wird hier nicht zwingend ein
 * Zeilenumbruch geschrieben !
 */
#ifndef PRINTF_NO_VA_ARGS_FFLUSH
    #define PRINTF_NO_VA_ARGS_FFLUSH(format_string)             \
    printf (format_string); fflush (stdout);
#else
    #error "The macro \"PRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* PRINTF_NO_VA_ARGS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ausgabe mittels fprintf, wobei automatisch der file Puffer geleert wird.
 * Dies dient dazu, dass eine Ausgabe direkt durchgefuehrt wird.
 *
 * Der Hauptunterschied zwischen diesem Makro und der Verwendung eines Makros mit puts: Es wird hier nicht zwingend ein
 * Zeilenumbruch geschrieben !
 */
#ifndef FPRINTF_NO_VA_ARGS_FFLUSH
    #define FPRINTF_FFLUSH_NO_VA_ARGS(file, format_string)      \
    fprintf (file, format_string); fflush (file);
#else
    #error "The macro \"FPRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* FPRINTF_NO_VA_ARGS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ausgabe mittels puts, wobei automatisch stdout geleert wird.
 *
 * Dies dient dazu, dass eine Ausgabe direkt auf dem Bildschirm erscheint. ("puts()": inklusive eines Zeilenumbruchs)
 */
#ifndef PUTS_FFLUSH
    #define PUTS_FFLUSH(string)                                 \
    puts (string); fflush (stdout);
#else
    #error "The macro \"PUTS_FFLUSH\" is already defined !"
#endif /* PUTS_FFLUSH */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Aktuelle Terminalzeile mittels \r leeren.
 *
 * Annahme, dass 96 Zeichen ausreichen.
 */
#ifndef CLEAN_LINE
    #define CLEAN_LINE()                                                                                        \
    printf ("\r"); PRINT_X_TIMES_SAME_CHAR(' ', 96) fflush (stdout);
#else
    #error "The macro \"CLEAN_LINE\" is already defined !"
#endif /* CLEAN_LINE */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Eine leere Zeile auf stdout ausgeben (mit Flush).
 */
#ifndef PRINT_NEWLINE
    #define PRINT_NEWLINE PUTS_FFLUSH("")
#else
    #error "The macro \"PRINT_NEWLINE\" is already defined !"
#endif /* PRINT_NEWLINE */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief X Mal das gleiche Zeichen auf stdout ausgeben (mit Flush am Ende).
 */
#ifndef PRINT_X_TIMES_SAME_CHAR
#define PRINT_X_TIMES_SAME_CHAR(character, times) \
    for (size_t i = 0; i < (times); ++ i) { printf ("%c", (character)); } fflush (stdout);
#else
    #error "The macro \"PRINT_X_TIMES_SAME_CHAR\" is already defined !"
#endif /* PRINT_X_TIMES_SAME_CHAR */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein 2D-String-Array aud stdout ausgeben.
 *
 * Solch eine Funktionalitaet wird benoetigt, um Fehler bei der Erstellung von textbasierten Zeichnungen schneller zu
 * finden.
 *
 * Asserts:
 *      drawing != NULL
 *      dim_1 > 0
 *      dim_2 > 0
 *
 * @param[in] drawing 2D-String-Array
 * @param[in] dim_1 Groesse der 1. Array Dimension (Anzahl an Zeichenketten)
 * @param[in] dim_2 Groesse der 2. Array Dimension (Maximale Anzahl an Zeichen pro Zeichenkette)
 */
extern void Print_2D_String_Array (const char* const restrict drawing [], const size_t dim_1, const size_t dim_2);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PRINT_TOOLS_H */
