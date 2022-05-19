/**
 * @file Dynamic_Memory.h
 *
 * @brief Hier werden die Anzahl an dynamischen Speicheroperaionen mittels globalen Variablen (ja ich weiss ganz boese:
 * man muss aufpassen, dass diese nicht zubeissen ! Aber bisher hatte ich viel Glueck mit denen. ;)) und Makros
 * mitgezaehlt.
 *
 * Das Ziel ist festzustellen, ob das Vorhandensein von Speicherlecks moeglich ist. Wenn ja, dann muss es mehr
 * Allokationen als free-Aufrufe geben.
 *
 * Damit dies sicher geschehen kann, muessen immer die hier definierten Makros verwendet werden ! Auch fuer die free-
 * Aufrufe !
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#ifndef DYNAMIC_MEMORY_H
#define DYNAMIC_MEMORY_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stdlib.h>



// Globale Variablen fuer das Zaehlen der malloc (), calloc (), realloc () und free () Aufrufe
extern uint_fast64_t GLOBAL_malloc_calls;   ///< Anazhl an durchgefuehrten malloc-Aufrufen
extern uint_fast64_t GLOBAL_calloc_calls;   ///< Anazhl an durchgefuehrten calloc-Aufrufen
extern uint_fast64_t GLOBAL_realloc_calls;  ///< Anazhl an durchgefuehrten realloc-Aufrufen
extern uint_fast64_t GLOBAL_free_calls;     ///< Anazhl an durchgefuehrten free-Aufrufen



/**
 * @brief Aktuelle Anzahl der durch die Makros MALLOC, CALLOC und FREE getaetigten malloc (), calloc () und free ()
 * Aufrufe sowie die Anzahl an fehlenden free () Aufrufe ausgeben.
 */
extern void Show_Dynamic_Memory_Status (void);



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief malloc-Aufrufe mitzaehlen
 */
#ifndef MALLOC
    #define MALLOC(memory_size)                                                                                       \
        malloc (memory_size);                                                                                         \
        ++ GLOBAL_malloc_calls;
#else
    #error "The macro \"MALLOC\" is already defined !"
#endif /* MALLOC */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief calloc-Aufrufe mitzaehlen
 */
#ifndef CALLOC
    #define CALLOC(number_of_elements, element_size)                                                                  \
        calloc (number_of_elements, element_size);                                                                    \
        ++ GLOBAL_calloc_calls;
#else
    #error "The macro \"CALLOC\" is already defined !"
#endif /* CALLOC */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief realloc-Aufrufe mitzaehlen
 *
 * realloc fuehrt im Hintergrund einen malloc-Aufruf aus; allerdings auch automatisch einen free-Aufruf auf den alten
 * Speicherbereich !
 * => Daher muessen zwe Zaehler pro realloc-Aufruf inkrementiert werden.
 */
#ifndef REALLOC
    #define REALLOC(number_of_elements, element_size)                                                                 \
        realloc (number_of_elements, element_size);                                                                   \
        ++ GLOBAL_malloc_calls;                                                                                       \
        ++ GLOBAL_realloc_calls;                                                                                      \
        ++ GLOBAL_free_calls;
#else
    #error "The macro \"REALLOC\" is already defined !"
#endif /* REALLOC */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Das uebergebene Objekt wird geloescht und auf nullptr gesetzt
 *
 * Eine Abfrage auf nullptr, vor dem Loeschen, ist NICHT erforderlich, da dies die free-Funktion bzw. dies sowieso vor
 * dem eigentlichen Loeschprozess machen ! Falls der Zeiger wirklich ein Nullzeiger sein sollte, dann wird von diesen
 * beiden Funktion einfach nichts gemacht !
 *
 * Siehe: https://stackoverflow.com/questions/4190703/is-it-safe-to-delete-a-null-pointer
 */
#ifndef FREE_AND_SET_TO_NULL
    #define FREE_AND_SET_TO_NULL(pointer)                                                                             \
        /* if (pointer != NULL) */                                                                                    \
        {                                                                                                             \
            free (pointer);                                                                                           \
            pointer = NULL;                                                                                           \
            ++ GLOBAL_free_calls;                                                                                     \
        }
#else
    #error "The macro \"FREE_AND_SET_TO_NULL\" is already defined !"
#endif /* FREE_AND_SET_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Der uebergebene Filestreampointer wird geschlossen und auf nullptr gesetzt.
 */
#ifndef FCLOSE_AND_SET_TO_NULL
    #define FCLOSE_AND_SET_TO_NULL(pointer)                                                                           \
        fclose (pointer);                                                                                             \
        pointer = NULL;
#else
    #error "The macro \"FCLOSE_AND_SET_TO_NULL\" is already defined !"
#endif /* FCLOSE_AND_SET_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Das Loeschen eines Objektes mittels der passenden Loesch-Funktion.
 *
 * Wie fuer Makros typisch: Es ist bloss eine Textersetzung ! Wenn eine falsche Loesch-Funktion angegeben wird, dann
 * wird sich der Compiler beschweren, und das zu Recht. ;)
 */
#ifndef FREE_WITH_FUNCTION_AND_SET_TO_NULL
    #define FREE_WITH_FUNCTION_AND_SET_TO_NULL(free_function, pointer)                                                \
        free_function (pointer);                                                                                      \
        pointer = NULL;
#else
    #error "The macro \"FREE_WITH_FUNCTION_AND_SET_TO_NULL\" is already defined !"
#endif /* FREE_WITH_FUNCTION_AND_SET_TO_NULL */
//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DYNAMIC_MEMORY_H */
