/**
 * @file Document_Word_List.h
 *
 * @date 22.05.2022
 * @author Gyps
 */

#ifndef DOCUMENT_WORD_LIST_H
#define DOCUMENT_WORD_LIST_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stddef.h>



//=====================================================================================================================

enum Intersection_Mode
{
    INTERSECTION_MODE_2_NESTED_LOOPS = 1 << 0,
    INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH = 1 << 1,
    INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH = 1 << 2
};

struct Document_Word_List
{
    uint_fast32_t** data;           ///< 2 dimensionales Array mit den Daten
    size_t* arrays_lengths;         ///< 1 dimensionales Array mit den Laengen der Datenarrays

    uint_fast32_t next_free_array;  ///< Naechstes freies Array
    size_t max_array_length;        ///< Maximale Laenge aller Dateiarrays
    size_t number_of_arrays;        ///< Anzahl an Arrays

    _Bool intersection_data;        ///< Ist der Datensatz durch eine Schnittmengenoperation entstanden ?
};

//=====================================================================================================================

/**
 * @brief Neue Woerterliste erstellen.
 *
 * @param[in] number_of_arrays Anzahl an Arrays (Untermengen)
 * @param[in] max_array_length Maximale Laenge der Arrays bzw. Untermengen
 *
 * @return Neu dynamisch erzeugte Woerterliste
 */
extern struct Document_Word_List*
Create_Document_Word_List
(
        const size_t number_of_arrays,
        const size_t max_array_length
);

/**
 * @brief Woerterliste loeschen.
 *
 * @param[in] object Woerterliste, die geloescht werden soll
 */
extern void
Delete_Document_Word_List
(
        struct Document_Word_List* object
);

/**
 * @brief Daten zu einer Woerterliste hinzufuegen.
 *
 * @param[in] object Woerterliste, an der die neuen Daten angebracht werden
 * @param[in] new_data Neue Daten
 * @param[in] data_length Laenge der neuen Daten
 */
extern void
Append_Data_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const size_t data_length
);

/**
 * @brief Daten einer Woerterliste auf stdout ausgeben
 *
 * @param[in] object Woerterliste
 */
extern void
Show_Data_From_Document_Word_List
(
        const struct Document_Word_List* const object
);

/**
 * @brief Daten einer Woerterliste + Attributinformationen auf stdout ausgeben
 *
 * @param[in] object Woerterliste
 */
extern void
Show_Data_And_Attributes_From_Document_Word_List
(
        const struct Document_Word_List* const object
);

/**
 * @brief Schnittmenge zwischen einer Woerterliste und einem Datensatz bilden.
 *
 * @param[in] object Woerterliste (1. Menge fuer die Schnittmengenberechnung)
 * @param[in] data Datensatz (2. Menge fuer die Schnittmengenberechnung)
 * @param[in] data_length Groesse des Datensatzes
 * @param[in] mode Modus fuer die Schnittstellenberechnung (Welches Verfahren soll verwendet werden ?)
 *
 * @return Neu erzeugte Woerterliste mit den Schnittmengen in den jeweiligen Untermengen
 */
extern struct Document_Word_List*
Intersect_Data_With_Document_Word_List
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length,

    const enum Intersection_Mode mode
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DOCUMENT_WORD_LIST_H */
