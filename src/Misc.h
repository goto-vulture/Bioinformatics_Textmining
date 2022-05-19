/**
 * @file Misc.h
 *
 * @brief Hier sind einige Makros definiert, die sich nicht einer bestimmten Uebersetzungseinheit zuordnen lassen. Z.B. das
 * Berechnen der Groesse eines Arrays, welches statisch definiert wurde.
 *
 * @date 14.03.2021
 * @author x86 / Gyps
 */

#ifndef MISC_H
#define MISC_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Dieses Makro zaehlt die Elemente in einem Array.
 *
 * ! Dies funktioniert nur dann, wenn die Groesse des Arrays bereits zur Uebersetzungszeit bekannt ist !
 * !!! Wenn dies nicht der Fall ist (wie z.B. bei Speicherbereiche, die durch malloc() angelegt wurden, dann liefert
 *     dieses Makro FALSCHE ERGEBNISSE !!!
 */
#ifndef COUNT_ARRAY_ELEMENTS
#define COUNT_ARRAY_ELEMENTS(array) ((sizeof (array)) / (sizeof (array [0])))
#else
#error "The macro \"COUNT_ARRAY_ELEMENTS\" is already defined !"
#endif /* COUNT_ARRAY_ELEMENTS */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Dieses Makro soll die Erstellung einer Zeichenkettendarstellung von Objekten vereinfachen.
 *
 * Bei der Konvertierung ist das Procedere immer das gleiche. Nur das Stringliteral unterscheidet sich. Daher bietet
 * sich dafuer ein Makro an.
 * Natuerlich wird dadurch vorausgesetzt, dass die Variablen sowie das Goto-Label vorher korrekt erzeugt wurden. Dieses
 * Makro nimmt nur eine Zeichenkette entgegen - KEINEN Formatstring mit weiteren Parametern.
 *
 * => Fuer die Verwendung eines Formatstrings: Siehe das Makro "TO_STRING_HELPER_VA_ARGS"
 */
#ifndef TO_STRING_HELPER
#define TO_STRING_HELPER(new_string)                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, new_string);   \
        next_free_byte += used_char;                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER\" is already defined !"
#endif /* TO_STRING_HELPER */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Dieses Makro soll die Erstellung einer Zeichenkettendarstellung von Objekten vereinfachen.
 * Diesmal mit einem Formatstring.
 *
 * Diese Unterteilung ist - leider - notwendig, da bei einem Makro, welches __VA_ARGS__ verwendet, ein Kommata zu viel
 * erzeugt wird, wenn __VA_ARGS__ leer ist.
 * Es gibt zwar compilerspezifische Erweiterungen, die dies unterbinden. Eine standardkonforme Loesung existiert aber
 * nicht !
 */
#ifndef TO_STRING_HELPER_VA_ARGS
#define TO_STRING_HELPER_VA_ARGS(format_string, ...)                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, format_string, __VA_ARGS__);   \
        next_free_byte += used_char;                                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER_VA_ARGS\" is already defined !"
#endif /* TO_STRING_HELPER_VA_ARGS */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Minimales Vergleichsmakro um aus zwei Werten den kleineren auszuwaehlen.
 *
 * ! Bei dem Makroansatz ist es wichtig, dass das Makro OHNE Seiteneffekte aufgerufen wird !
 *
 * Ein Aufruf wie
 * MIN (x ++, y ++)
 * darf nicht verwendet werden, da durch die einfache Textersetzung des Makros die Inkrementiertung des kleineren
 * Wertes ZWEIMAL stattfinden wuerde !
 *
 * Es empfiehlt sich daher, dass das Makro nur mit den "reinen" Werten (also OHNE Ausdruecke wie "x + 2" aufgerufen
 * wird), um dieses Problem im vorn­he­r­ein sicher zu vermeiden. :)
 */
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#else
#error "The macro \"MIN\" is already defined !"
#endif /* MIN */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Minimales Vergleichsmakro um aus zwei Werten den groesseren auszuwaehlen.
 *
 * ! Bei dem Makroansatz ist es wichtig, dass das Makro OHNE Seiteneffekte aufgerufen wird !
 *
 * Ein Aufruf wie
 * MAX (x ++, y ++)
 * darf nicht verwendet werden, da durch die einfache Textersetzung des Makros die Inkrementiertung des groesseren
 * Wertes ZWEIMAL stattfinden wuerde !
 *
 * Es empfiehlt sich daher, dass das Makro nur mit den "reinen" Werten (also OHNE Ausdruecke wie "x + 2" aufgerufen
 * wird), um dieses Problem im vorn­he­r­ein sicher zu vermeiden. :)
 */
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#else
#error "The macro \"MAX\" is already defined !"
#endif /* MAX */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein uebergebenes Array an Zeigern mit den Nullpointer initialisieren.
 */
#ifndef SET_POINTER_ARRAY_TO_NULL
#define SET_POINTER_ARRAY_TO_NULL(array, number_of_elements)                                                            \
    for (size_t array_index = 0; array_index < (number_of_elements); array_index ++)                                    \
    {                                                                                                                   \
        (array) [array_index] = NULL;                                                                                   \
    }
#else
#error "The macro \"SET_POINTER_ARRAY_TO_NULL\" is already defined !"
#endif /* SET_POINTER_ARRAY_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MISC_H */
