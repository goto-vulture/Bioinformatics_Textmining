/**
 * @file Intersection_Approaches.h
 *
 * @brief For the intersection operation three approaches are available:
 *
 * 1. The simplest way: Two nested loops
 * 2. Data sorting (with QSort from the C lib) and a binary search
 * 3. Data sorting (with heap sort) and a binary search
 *
 * Plots with runtime comparisions are in the "Plots" folder.
 *
 * Summary of the tests: In normal cases (with datasets of a few hundred integers) the simplest way is faster, because
 * the sorting of the data needs most of the time. Only with a larger amout of data the approaches with a data sorting
 * are faster.
 *
 * @date 23.05.2022
 * @author Gyps
 */

#ifndef INTERSECTION_APPROACHES_H
#define INTERSECTION_APPROACHES_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Document_Word_List.h"



/**
 * @brief Schnittmenge durch die naive Herangehensweise (Jeden mit jedem vergleichen -> zwei verschachtelte Schleifen)
 * bilden.
 *
 * @param[in] object Woerterliste
 * @param[in] data Daten, die fuer die Schnittmengenbildung gegen der Woerterliste verwendet wird
 * @param[in] data_length Groesse der Daten
 *
 * @return Neues dynamisch erzeugtes Objekt, welches die Schnittmenge zu jeder Untermenge der Woerterliste enthaelt
 */
extern struct Document_Word_List*
Intersection_Approach_2_Nested_Loops
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
);

/**
 * @brief Schnittmenge durch eine vorherige aufsteigende Sortierung der Elemente in der Woerterliste. Anschliessend
 * werden die Elemente, die in beiden Mengen vorkommen, mittels der binaeren Suche gesucht.
 *
 * Algorithmus fuer die Sortierung: QSort
 *
 * @param[in] object Woerterliste
 * @param[in] data Daten, die fuer die Schnittmengenbildung gegen der Woerterliste verwendet wird
 * @param[in] data_length Groesse der Daten
 *
 * @return Neues dynamisch erzeugtes Objekt, welches die Schnittmenge zu jeder Untermenge der Woerterliste enthaelt
 */
extern struct Document_Word_List*
Intersection_Approach_QSort_And_Binary_Search
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
);

/**
 * @brief Schnittmenge durch eine vorherige aufsteigende Sortierung der Elemente in der Woerterliste. Anschliessend
 * werden die Elemente, die in beiden Mengen vorkommen, mittels der binaeren Suche gesucht.
 *
 * Algorithmus fuer die Sortierung: HeapSort
 *
 * @param[in] object Woerterliste
 * @param[in] data Daten, die fuer die Schnittmengenbildung gegen der Woerterliste verwendet wird
 * @param[in] data_length Groesse der Daten
 *
 * @return Neues dynamisch erzeugtes Objekt, welches die Schnittmenge zu jeder Untermenge der Woerterliste enthaelt
 */
extern struct Document_Word_List*
Intersection_Approach_HeapSort_And_Binary_Search
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERSECTION_APPROACHES_H */
