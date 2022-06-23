/*
 * Token_Int_Mapping.h
 *
 *  Created on: 23.06.2022
 *      Author: am1
 */

#ifndef TOKEN_INT_MAPPING_H
#define TOKEN_INT_MAPPING_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stddef.h>



#ifndef C_STR_ARRAYS
#define C_STR_ARRAYS 100
#else
#error "The macro \"C_STR_ARRAYS\" is already defined !"
#endif /* C_STR_ARRAYS */

//=====================================================================================================================

struct Token_Int_Mapping
{
    char* c_str_arrays [C_STR_ARRAYS];                  // C-String Arrays, die die Tokens beinhalten
                                                        // Flaches Speichermodell !

    size_t allocated_c_strings_in_array;                   // Anzahl an allokierten Tokens-Objekte
    uint_fast32_t c_str_array_lengths [C_STR_ARRAYS];   // Laenge der C-String Arrays
};

//=====================================================================================================================

extern struct Token_Int_Mapping*
Create_Token_Int_Mapping
(
        void
);

extern void
Delete_Token_Int_Mapping
(
        struct Token_Int_Mapping* object
);

extern void
Add_Token_To_Mapping
(
        struct Token_Int_Mapping* object,
        const char* new_token,
        const size_t new_token_length
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TOKEN_INT_MAPPING_H */
