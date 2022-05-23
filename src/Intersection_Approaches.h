/*
 * Intersection_Approaches.h
 *
 *  Created on: 23.05.2022
 *      Author: t430
 */

#ifndef INTERSECTION_APPROACHES_H
#define INTERSECTION_APPROACHES_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Document_Word_List.h"



extern struct Document_Word_List*
Intersection_Approach_2_Nested_Loops
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
);

extern struct Document_Word_List*
Intersection_Approach_QSort_And_Binary_Search
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
