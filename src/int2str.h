/**
 * @file int2str.h
 *
 * @brief Konvertierungsfunktion C-String -> int.
 *
 * @date 09.12.2021
 * @author x86 / Gyps
 */

#ifndef INT2STR_H
#define INT2STR_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>



/**
 * @brief Fehlercodes, die Fehlerzustaende bei der Verwendung von int2str(), anzeigen.
 */
enum int2str_errno
{
    INT2STR_SUCCESS = 0,    ///< Konvertierung war erfolgreich
    INT2STR_INCONVERTIBLE   ///< Zahl ist nicht konvertierbar
};



/**
 * @brief long int (mind. 32 Bit) zu C-String konvertieren.
 *
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt.
 *
 * @param[out] output_string C-String fuer die Ausgabe
 * @param[in] output_string_size Maximale Groesse des Ausgabestrings.
 * @param[in] input Integer, der konvertiert wird.
 *
 * @return Fehlercode. 0, wenn alles in Ordnung war; ansonsten != 0
 */
extern enum int2str_errno int2str (char* const output_string, const size_t output_string_size, const long int input);

/**
 * @brief long int (mind. 32 Bit) zu C-String konvertieren.
 *
 * Diese Funktion verwendet ein statischen C-String fuer das Ergebnis, sodass der Aufrufer vorher keinen Speicher
 * bereitstellen muss.
 *
 * @param[in] input Integer, der konvertiert wird.
 *
 * @return Adresse auf den statischen Ergebnis-C-String; oder NULL, wenn die Konvertierung nicht gelungen ist
 */
extern char* int2str_wo_errno (const long int input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INT2STR_H */
