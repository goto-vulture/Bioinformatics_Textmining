/*
 * Document_Word_List.h
 *
 *  Created on: 22.05.2022
 *      Author: pc178
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



enum Intersection_Mode
{
    INTERSECTION_MODE_DEFAULTS = 1 << 0
};

struct Document_Word_List
{
    uint_fast32_t** data;
    size_t* arrays_lengths;

    uint_fast32_t next_free_array;
    size_t max_array_length;
    size_t number_of_arrays;

    _Bool intersection_data;
};



extern struct Document_Word_List*
Create_Document_Word_List
(
        const size_t number_of_arrays,
        const size_t max_array_length
);

extern void
Delete_Document_Word_List
(
        struct Document_Word_List* object
);

extern void
Append_Data_To_Document_Word_List
(
        struct Document_Word_List* const object,
        const uint_fast32_t* const new_data,
        const size_t data_length
);

extern void
Show_Data_From_Document_Word_List
(
        const struct Document_Word_List* const object
);

extern void
Show_Data_And_Attributes_From_Document_Word_List
(
        const struct Document_Word_List* const object
);

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
