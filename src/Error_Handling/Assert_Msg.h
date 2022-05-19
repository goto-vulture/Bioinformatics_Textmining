/**
 * @file Assert_Msg.h
 *
 * @brief Hier sind Asserts definiert, die nicht die Uebersetzungseinheit "tinytest" benoetigen.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#ifndef ERROR_HANDLING_ASSERT_MSG_H
#define ERROR_HANDLING_ASSERT_MSG_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <assert.h>
#include <stdio.h>
#include <stdbool.h>



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein Assert mit benutzerdefinierter Fehlermeldung.
 */
#ifndef ASSERT_MSG
    #define ASSERT_MSG(expr, message) assert (((void) (message), expr));
#else
    #error "The macro \"ASSERT_MSG\" is already defined !"
#endif /* ASSERT_MSG */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein Assert mit benutzerdefinierter Fehlermeldung in Form eines Format-Strings.
 *
 * Der Einfachheit halber wird die Fehlermeldung - inkl. der Parameter - auf stderr ausgegeben.
 * Zusaetzlich wird ein Zeilenumbruch auf stderr ausgegeben, damit die Format-String Fehlermeldung und die
 * Fehlermeldung vom ASSERT-Makro nicht in einer Zeile stehen.
 */
#ifndef ASSERT_FMSG
    #define ASSERT_FMSG(expr, format_string, ...)                       \
    if (! (expr))                                                       \
    {                                                                   \
        fprintf (stderr, format_string, __VA_ARGS__);                   \
        fputc ('\n', stderr);                                           \
        fflush (stderr);                                                \
        ASSERT_MSG(false, "See error message above !")                  \
    }
#else
    #error "The macro \"ASSERT_FMSG\" is already defined !"
#endif /* ASSERT_FMSG */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein Assert fuer Fehlerfaelle bei der Allokation von Speicher.
 *
 * Vor der eigentlichen Assert-Meldung wird die Groesse des Speichers angegeben, der allokiert werden sollte. Die
 * Angabe der Speichergroesse ist technisch nicht zwingend notwendig, macht aber die Fehlermeldungen besser, da man
 * weiss bei welcher Allokationsgroesse es Probleme gab.
 */
#ifndef ASSERT_ALLOC
    #define ASSERT_ALLOC(ptr, message, memory_size)                                 \
    if (ptr == NULL)                                                                \
    {                                                                               \
        fprintf (stderr, "Try to allocate %zu bytes !\n", (size_t) memory_size);    \
        fflush (stderr);                                                            \
        ASSERT_MSG(false, message)                                                  \
    }
#else
    #error "The macro \"ASSERT_ALLOC\" is already defined !"
#endif /* ASSERT_ALLOC */

//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ERROR_HANDLING_ASSERT_MSG_H */
