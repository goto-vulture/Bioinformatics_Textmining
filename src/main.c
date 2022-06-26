/**
 * @file main.c
 *
 * @brief Die Bestimmung von sogenannten "Dominating Word Sets" aus Datensätzen im Bereich der Bioinformatik.
 *
 * @author x86 / Gyps
 */

/*         .-..-.__
 *       /`  |\_( "\__
 *       |   / __/`'--)
 *       |  /  | |
 *       /_/ _/_/
 * jgs  `|/))))
 *
 * For me, a vulture is more desirable than money.
 */

/*
 * C-Version ueberpruefen, um Schluesselwoerter u.U. durch leere Makros bzw. Alternativwerte zu ersetzen
 *
 * Das Makro __STDC_VERSION__ ist fuer C90 nicht definiert
 * ->   Schluesselwoerter inline und restrict durch leere Markos ersetzen, damit eine Uebersetzung auch mit C90
 *      moeglich ist.
 */
#ifndef __STDC_VERSION__
    /**
     * @brief Das Schluesselwort "inline" als Makro definieren, wenn der C90 Standard verwendet wird.
     *
     * In diesem Standard existiert "inline" als Schluesselwort noch nicht. Die Uebersetzung wuerde an diesen
     * Stellen mit einem Fehler abbrechen.
     * Die Loesung: "inline" wird ein leeres Makro
     */
    #ifndef inline
        #define inline
    #else
        #error "The macro \"inline\" is already defined !"
    #endif /* inline */

    /**
     * @brief Das Schluesselwort "restrict" als Makro definieren, wenn der C90 Standard verwendet wird.
     *
     * In diesem Standard existiert "restrict" als Schluesselwort noch nicht. Die Uebersetzung wuerde an diesen
     * Stellen mit einem Fehler abbrechen.
     * Die Loesung: "restrict" wird ein leeres Makro
     */
    #ifndef restrict
        #define restrict
    #else
        #error "The macro \"restrict\" is already defined !"
    #endif /* restrict */

    /**
     * @brief Die Problematik mit Wahrheitswerten vor C99 loesen, indem "_Bool" in diesen Faellen mittels Makro mit "int"
     * ersetzt wird.
     */
    #ifndef _Bool
        #define _Bool int
    #else
        #error "The macro \"_Bool\" is already defined !"
    #endif /* _Bool */

    /**
     * @brief In seltenen Faellen ist "true" als Makro nicht vordefiniert. Z.B. vor C99
     */
    #ifndef true
        #define true 1
    #else
        #error "The macro \"true\" is already defined !"
    #endif /* true */

    /**
     * @brief In seltenen Faellen ist "false" als Makro nicht vordefiniert. Z.B. vor C99
     */
    #ifndef false
        #define false 0
    #else
        #error "The macro \"false\" is already defined !"
    #endif /* false */
#endif /* __STDC_VERSION__ */

/**
 * @brief Ein einfache Zeichenkette die anzeigt, dass eine Information nicht vorhanden ist.
 */
#ifndef N_A
    #define N_A "N/A"
#else
    #error "The macro \"N_A\" is already defined !"
#endif /* N_A */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Error_Handling/Dynamic_Memory.h"
#include "Tests/TEST_Document_Word_List.h"
#include "File_Reader.h"
#include "Token_Int_Mapping.h"
#include "Print_Tools.h"
#include "Document_Word_List.h"
#include "Error_Handling/Assert_Msg.h"
#include "Intersection_Approaches.h"



//=====================================================================================================================

/**
 * @brief main-Funktion des Programms.
 *
 * Aktuell werden die CLI-Parameter (noch) nicht verwendet.
 *
 * @param[in] argc argc-Parameter
 * @param[in] argv Anzahl an argc-Parametern
 *
 * @return 0 bei erfolgreicher Ausfuehrung; Ansonsten != 0
 */
int main (const int argc, const char* argv [])
{
    (void) argc;
    (void) argv;

    const char filename [] = "/home/am1/Downloads/Sachen/test_ebm_tokens.txt";

    // TEST_Intersection();
    //TEST_Intersection_With_Random_Data();
    // TEST_Intersection_With_Random_Data_And_Specified_Result();

    struct Token_Container* token_container = Create_Token_Container_From_File (filename);

    //Show_Selected_Token_Container(token_container, 0);
    //Show_Selected_Token_Container(token_container, 1);

    printf ("Full token container size: %zu byte\n", Get_Token_Container_Size(token_container));

    for (uint_fast32_t i = 0; i < token_container->next_free_element; ++ i)
    {
        // Show_Selected_Token_Container(token_container, i);
    }
    printf ("Sum all tokens in container: %" PRIuFAST32 "\n\n", Count_All_Tokens_In_Token_Container(token_container));


    // ===== ===== ===== BEGINN Mapping Tabelle bilden ===== ===== =====
    struct Token_Int_Mapping* token_int_mapping = Create_Token_Int_Mapping();

    for (uint_fast32_t i = 0; i < token_container->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < token_container->tokens [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container, i, i2);
            _Bool element_added = Add_Token_To_Mapping(token_int_mapping, token, strlen(token));
            (void) element_added;
        }
    }
    // ===== ===== ===== ENDE Mapping Tabelle bilden ===== ===== =====

    // Datei einlesen und mithilfe der Mapping Tablle die Tokens in Zahlen umwandeln. Mittels der Zahlendarstellung
    // kann dann die Schnittmengenbestimmung durchgefuehrt werden
    const size_t length_of_longest_token_container = Get_Lengh_Of_Longest_Token_Container(token_container);
    PRINTF_FFLUSH("Length of longest token container: %zu\n", length_of_longest_token_container);

    // Show_C_Str_Array_Usage(token_int_mapping);

    struct Document_Word_List* document_word_list =
            Create_Document_Word_List(token_container->next_free_element, length_of_longest_token_container);

    uint_fast32_t* token_int_values = (uint_fast32_t*) MALLOC(length_of_longest_token_container *
            sizeof (uint_fast32_t));
    uint_fast32_t* intersection_values = (uint_fast32_t*) CALLOC(length_of_longest_token_container,
            sizeof (uint_fast32_t));
    uint_fast32_t count_intersection_values = 0;

    size_t next_free_value = 0;
    ASSERT_ALLOC(token_int_values, "Cannot allocate memory for token int mapping values !",
            length_of_longest_token_container * sizeof (uint_fast32_t));
    ASSERT_ALLOC(intersection_values, "Cannot allocate memory for token int mapping values !",
            length_of_longest_token_container * sizeof (uint_fast32_t));
    _Bool intersection_values_created = false;

    for (uint_fast32_t i = 0; i < token_container->next_free_element; ++ i)
    {
        memset(token_int_values, '\0', length_of_longest_token_container * sizeof (uint_fast32_t));
        next_free_value = 0;

        for (uint_fast32_t i2 = 0; i2 < token_container->tokens [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container, i, i2);
            token_int_values [next_free_value] = Token_To_Int(token_int_mapping, token, strlen(token));
            ++ next_free_value;
        }

        // Nur wenn Daten vorhanden sind, macht es Sinn diese zu kopieren
        if (next_free_value > 0)
        {
            Append_Data_To_Document_Word_List(document_word_list, token_int_values, next_free_value);

            if (! intersection_values_created /* == false */)
            {
                memcpy (intersection_values, token_int_values, length_of_longest_token_container *
                        sizeof (uint_fast32_t));

                intersection_values_created = true;
                count_intersection_values = next_free_value;
            }
        }
    }

    struct Document_Word_List* intersection_result = Intersection_Approach_2_Nested_Loops (document_word_list,
            intersection_values, count_intersection_values);

    for (size_t i = 0; i < intersection_result->number_of_arrays; ++ i)
    {
        printf("Token block: %zu\n[ ", i);
        for (size_t i2 = 0; i2 < intersection_result->arrays_lengths [i]; ++ i2)
        {
            // Token zum passenden Int-Wert finden -> also Mapping rueckgaengig machen
            char int_to_token_mem [32];
            memset (int_to_token_mem, '\0', sizeof (int_to_token_mem));

            Int_To_Token (token_int_mapping, intersection_result->data [i][i2], int_to_token_mem,
                    sizeof (int_to_token_mem) - 1);
            printf("%s", int_to_token_mem);

            if ((i2 + 1) < intersection_result->arrays_lengths [i])
            {
                printf(", ");
            }
        }
        puts(" ]");
    }

    /*const size_t result_obj = 1;
    for (size_t i = 0; i < intersection_result->arrays_lengths [result_obj]; ++ i)
    {
        char int_to_token_mem [32];
        memset (int_to_token_mem, '\0', sizeof (int_to_token_mem));

        Int_To_Token (token_int_mapping, intersection_result->data [result_obj][i], int_to_token_mem,
                sizeof (int_to_token_mem) - 1);

        printf ("%3" PRIuFAST32 " -> %30s\n", intersection_result->data [result_obj][i], int_to_token_mem);
    }
    printf ("%3zu results\n\n", intersection_result->arrays_lengths [result_obj]);*/

    Delete_Document_Word_List(intersection_result);

    FREE_AND_SET_TO_NULL(token_int_values);
    FREE_AND_SET_TO_NULL(intersection_values);

    Delete_Document_Word_List(document_word_list);
    Delete_Token_Int_Mapping(token_int_mapping);
    Delete_Token_Container (token_container);

    Show_Dynamic_Memory_Status();

    return EXIT_SUCCESS;
}

//=====================================================================================================================
