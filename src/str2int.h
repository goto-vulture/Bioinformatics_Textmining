/**
 * @file str2int.h
 *
 * @brief Konvertierungsfunktion C-String -> int.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#ifndef STR2INT_H
#define STR2INT_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>



/**
 * @brief Fehlercodes, die Fehlerzustaende bei der Verwendung von str2int(), anzeigen.
 */
enum str2int_errno
{
    STR2INT_SUCCESS = 0,    ///< Konvertierung war erfolgreich
    STR2INT_OVERFLOW,       ///< Overflow waehrend der Konvertierung
    STR2INT_UNDERFLOW,      ///< Underflow waehrend der Konvertierung
    STR2INT_INCONVERTIBLE   ///< Zeichenkette ist nicht konvertierbar
};



/**
 * @brief Konvertierung eines C-Strings zu einem mind. 32 Bit breiten Integer.
 *
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt. Im Fehlerfall wird der Ausgabespeicherbereich
 * (*out) nicht geaendert.
 *
 * @param[out] out Pointer auf das Ausgabe-Objekt.
 * @param[in] input_string C-String Eingabe.
 * @param[in] base Basis der Zahl (Angabe des Zahlensystems).
 *
 * @return Fehlercode. STR2INT_SUCCESS, wenn alles in Ordnung war; ansonsten != STR2INT_SUCCESS
 */
extern enum str2int_errno str2int (long int* out, const char* input_string, const int base);

/**
 * @brief Konvertierung eines C-Strings zu einem mind. 32 Bit breiten Integer.
 *
 * Das Ergebnis wird direkt ueber den Rueckgabewert angezeigt. Falls Fehler bei der Konvertierung aufgetreten sind,
 * dann ist der Rueckgabewert immer LONG_MIN.
 *
 * @param[in] input_string C-String Eingabe.
 * @param[in] base Basis der Zahl (Angabe des Zahlensystems).
 *
 * @return Der konvertierte C-String; oder LONG_MIN im Fehlerfall
 */
extern long int str2int_wo_errno (const char* input_string, const int base);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR2INT_H */
