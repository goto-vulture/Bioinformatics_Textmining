/*
 * File_Reader.c
 *
 *  Created on: 21.06.2022
 *      Author: am1
 */

#include "File_Reader.h"
#include <string.h>
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"



// Maximale Laenge eines Tokens (inkl. Terminator-Symbol)
#ifndef MAX_TOKEN_LENGTH
#define MAX_TOKEN_LENGTH 32
#else
#error "The macro \"MAX_TOKEN_LENGTH\" is already defined !"
#endif /* MAX_TOKEN_LENGTH */

// Allokationsgroesse eines Tokens-Objektes, wenn der Speicher nicht mehr ausreicht
#ifndef TOKENS_ALLOCATION_STEP_SIZE
#define TOKENS_ALLOCATION_STEP_SIZE 100
#else
#error "The macro \"TOKENS_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKENS_ALLOCATION_STEP_SIZE */

// Allokationsgroesse des Token_Containers, wenn der Speicher nicht mehr ausreicht
#ifndef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#define TOKEN_CONTAINER_ALLOCATION_STEP_SIZE 10
#else
#error "The macro \"TOKEN_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKEN_CONTAINER_ALLOCATION_STEP_SIZE */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Aus den Inhalt einer Datei die Tokens einlesen.
 *
 * Aktuell ist der "Parsingmechanismus" sehr simpel, sodass die Eingabe JSON Dateien bereits vorverarbeitet werden
 * muessen.
 *
 * Ein moegliches Python-Skript, welches die passende Ausgabe aus einer JSON-Datei erzeugt:
 *
    import json

    f = open('test_ebm.json')

    data = json.load(f)

    for i in data:
        print(i)
        print(data[i]["tokens"])
        print("")

    f.close()

 * Das Ziel ist, dass das Programm direkt mit JSON-Dateien arbeiten kann. Da dies aber nichts mit der
 * Grundfunktionalitaet zu tun hat, wird dies nach hinten verschoben.
 *
 * @param[in] file_name Vorverarbeitete Datei, aus denen die Tokens geladen werden
 *
 * @return Adresse auf ein neuen dynamisch erzeugten Token_Container
 */
extern struct Token_Container*
Create_Token_Container_From_File
(
        const char* file_name
)
{
    ASSERT_MSG(file_name != NULL, "File name is NULL !");

    // Den (aeusseren) Container erzeugen
    struct Token_Container* new_container =
            (struct Token_Container*) CALLOC(1, sizeof (struct Token_Container));
    ASSERT_ALLOC(new_container, "Cannot create new Token_Container !", 1 * sizeof (struct Token_Container));

    // Die inneren Container erzeugen
    new_container->allocated_token_container = TOKEN_CONTAINER_ALLOCATION_STEP_SIZE;
    new_container->tokens = (struct Tokens*) CALLOC(new_container->allocated_token_container, sizeof (struct Tokens));
    ASSERT_ALLOC(new_container->tokens, "Cannot create new Token objects !", new_container->allocated_token_container *
            sizeof (struct Tokens));

    // Speicher fuer die inneren Container erzeugen
    for (size_t i = 0; i < new_container->allocated_token_container; ++ i)
    {
        new_container->tokens [i].data = (char*) MALLOC(MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
        ASSERT_ALLOC(new_container->tokens [i].data, "Cannot create data for a Token object !",
                MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
        memset(new_container->tokens [i].data, '\0', MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);

        new_container->tokens [i].max_token_length = MAX_TOKEN_LENGTH;
    }

    return new_container;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Token_Container loeschen.
 *
 * @param[in] object Container, der geloescht wird
 */
extern void
Delete_Token_Container
(
        struct Token_Container* object
)
{
    ASSERT_MSG(object != NULL, "Token_Container is NULL !");

    // Von innen nach aussen die Elemente loeschen
    for (size_t i = 0; i < object->allocated_token_container; ++ i)
    {
        if (&(object->tokens [i]) != NULL)
        {
            FREE_AND_SET_TO_NULL(object->tokens [i].data);
        }
    }

    FREE_AND_SET_TO_NULL(object->tokens);
    FREE_AND_SET_TO_NULL(object);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Ein bestimmtes Token aus dem Container auslesen.
 *
 * Damit die Speicherverwaltung moeglichst effizient ist, wird der Speicher fuer das innenliegende Tokens-Objekt in
 * einem Block alloziert. Dies macht die Ermittlung eines spezifischen Objektes etwas aufwaendiger. Daher diese
 * Funktion
 *
 * @param[in] container Token_Container
 * @param[in] index_tokens Index des Token_Containers
 * @param[in] index_token_in_tokens_object Index des Tokens, im - durch den 2. Parameter - angegebenen Token_Container
 *
 * @return Zeiger auf den Beginn des Tokens (Token ist Nullterminiert !)
 */
extern char*
Get_Token_From_Token_Container
(
        const struct Token_Container* const container,
        const uint_fast32_t index_tokens,
        const uint_fast32_t index_token_in_tokens_object
)
{
    ASSERT_MSG(container != NULL, "Token_Container is NULL !");
    ASSERT_FMSG(index_tokens < container->next_free_element, "Tokens object id is invalid ! Max. valid: %"
            PRIuFAST32 "; Got: %" PRIuFAST32 " !", container->next_free_element - 1, index_tokens);
    ASSERT_FMSG(index_token_in_tokens_object < container->tokens [index_tokens].next_free_element,
            "Token in Tokens object %" PRIuFAST32 " is invalid ! Max. valid: %" PRIuFAST32 "; Got: %" PRIuFAST32 " !",
            index_tokens, container->tokens [index_tokens].next_free_element - 1, index_token_in_tokens_object);

    const size_t max_token_size = container->tokens [index_tokens].max_token_length;

    return container->tokens [index_tokens].data + max_token_size;
}

//---------------------------------------------------------------------------------------------------------------------
