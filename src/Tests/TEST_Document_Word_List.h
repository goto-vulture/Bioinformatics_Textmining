/*
 * Document_Word_List_Test.h
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#ifndef DOCUMENT_WORD_LIST_TEST_H
#define DOCUMENT_WORD_LIST_TEST_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdbool.h>



/**
 * @brief Erster Schnittmengen-Test mit den Testdaten aus der ersten Besprechung am 11.05.2022
 */
extern _Bool TEST_Intersection (void);

/**
 * @brief Schnittmengenberechnung mit komplett zufaelligen Daten.
 */
extern _Bool TEST_Intersection_With_Random_Data (void);

/**
 * @brief Schnittmengenberechnung mit vorher spezifizierten Daten fuer jede Untermenge. Das Ziel: Die Ergebnisse sind
 * dadurch vorher bekannt. Damit kann ueberprueft werden, dass die Verfahren richtig laufen.
 */
extern _Bool TEST_Intersection_With_Random_Data_And_Specified_Result (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DOCUMENT_WORD_LIST_TEST_H */
