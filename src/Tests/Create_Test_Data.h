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



extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data
(
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CREATE_TEST_DATA */
