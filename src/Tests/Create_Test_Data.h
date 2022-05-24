/*
 * Create_Test_Data.h
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#ifndef CREATE_TEST_DATA_H
#define CREATE_TEST_DATA_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>
#include "../Document_Word_List.h"



/**
 * @brief Woerterliste mit Zufallszahlen erzeugen.
 *
 * @param[in] number_of_arrays Anzahl an Untermengen
 * @param[in] max_array_length Maximale Laenge einer Untermenge
 * @param[in] rand_upper_bound Obere Grenze fuer die Zufallszahlen
 *
 * @return Die neu erzeugte Woerterliste
 */
extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data
(
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
);

/**
 * @brief Woerterliste mit Zufallszahlen erzeugen + spezifizierte Menge an Daten, die in jeder Untermenge der
 * Woerterliste vorkommen soll.
 *
 * Der Sinn hinter dieser Funktion ist, dass die Schnittmenge immer den der spezifizierten Daten entspricht. Damit kann
 * man testen, ob die Schnittmengenbildung korrekt funktioniert.
 *
 * @param[in] specified_data Spezifizierte Daten
 * @paran[in] specified_data_length Laenge der dpezifizierten Daten
 * @param[in] number_of_arrays Anzahl an Untermengen
 * @param[in] max_array_length Maximale Laenge einer Untermenge
 * @param[in] rand_upper_bound Obere Grenze fuer die Zufallszahlen
 *
 * @return Die neu erzeugte Woerterliste
 */
extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data_Plus_Specified_Data
(
        const uint_fast32_t* specified_data,
        const size_t specified_data_length,
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CREATE_TEST_DATA */
