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
#include "Print_Tools.h"



// Maximale Laenge eines Tokens (inkl. Terminator-Symbol)
#ifndef MAX_TOKEN_LENGTH
#define MAX_TOKEN_LENGTH 32
#else
#error "The macro \"MAX_TOKEN_LENGTH\" is already defined !"
#endif /* MAX_TOKEN_LENGTH */

// Allokationsgroesse eines Tokens-Objektes, wenn der Speicher nicht mehr ausreicht
#ifndef TOKENS_ALLOCATION_STEP_SIZE
#define TOKENS_ALLOCATION_STEP_SIZE 350
#else
#error "The macro \"TOKENS_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKENS_ALLOCATION_STEP_SIZE */

// Allokationsgroesse des Token_Containers, wenn der Speicher nicht mehr ausreicht
#ifndef TOKEN_CONTAINER_ALLOCATION_STEP_SIZE
#define TOKEN_CONTAINER_ALLOCATION_STEP_SIZE 100
#else
#error "The macro \"TOKEN_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* TOKEN_CONTAINER_ALLOCATION_STEP_SIZE */

#ifndef READ_FILE_BUFFER_SIZE
#define READ_FILE_BUFFER_SIZE 100000
#else
#error "The macro \"READ_FILE_BUFFER_SIZE\" is already defined !"
#endif /* READ_FILE_BUFFER_SIZE */

/**
 * @brief Eine Zeile aus einer bereits geoeffneten Datei lesen.
 *
 * Es wird der Zeiger auf den Puffer zurueckgegeben, um so Fehler (z.B. durch NULL) anzeigen zu koennen.
 *
 * @param[in] token_container Token_Container
 * @param[in] file FILE-Objekt auf die bereits geoeffnete Datei
 * @param[in] file_buffer Benutzerdefinierter Puffer fuer das Lesen der Datei
 * @param[in] buffer_length Groesse des Puffers
 * @param[in] next_char_in_buffer Index des naechsten freien Zeichen im Puffer
 *
 * @return Zeiger auf den Puffer
 */
static char*
Read_Line
(
        struct Token_Container* const restrict token_container,
        FILE* const file,
        char* const restrict file_buffer,
        const size_t buffer_length,
        size_t next_char_in_buffer
);

/**
 * @brief Tokens aus einem Puffer extrahieren und dem Token_Container hinzufuegen.
 *
 * Hinweis: Das Ermitteln der Tokens ist sehr rudimentaer und NICHT fuer die Verwendung geeignet ! Bisher wurde eine
 * schmandige und sehr einfache Variante implementiert, um die Entwicklung fortfuehren zu koennen !
 *
 * @param[in] token_container Token_Container
 * @param[in] file_buffer Datei-Puffer mit den Inhalt der Daten, die verarbeitet werden sollen
 * @param[in] next_char_in_buffer Index des naechsten freien Zeichen im Puffer
 */
static void
Extract_Tokens_From_Line
(
        struct Token_Container* const restrict token_container,
        const char* const restrict file_buffer,
        const size_t used_char_in_buffer
);

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
        const char* const file_name
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
        new_container->tokens [i].allocated_tokens = TOKENS_ALLOCATION_STEP_SIZE;
    }

    FILE* file = fopen(file_name, "r");
    ASSERT_FMSG(file != NULL, "Cannot open the file: \"%s\" !", file_name);

    char* file_buffer = (char*) MALLOC(READ_FILE_BUFFER_SIZE * sizeof (char));
    ASSERT_ALLOC(file_buffer, "Cannot create the read file buffer !", READ_FILE_BUFFER_SIZE * sizeof (char));

    uint_fast32_t line_counter = 0;
    while (Read_Line (new_container, file, file_buffer, READ_FILE_BUFFER_SIZE, 0) != NULL)
    {
        if (file_buffer [0] != '[')
        {
            continue;
        }

        ++ line_counter;

        // Muessen neue Token_Container erzeugt werden ?
        if ((line_counter % TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) == 0)
        {
            static size_t token_container_realloc_counter = 0;
            ++ token_container_realloc_counter;
            const size_t old_allocated_token_container = new_container->allocated_token_container;

            // Die aeusseren Token_Container Objekte anpassen (Anzahl erhoehen)
            struct Tokens* temp_ptr = (struct Tokens*) REALLOC(new_container->tokens,
                    (old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) * sizeof (struct Tokens));
            ASSERT_ALLOC(temp_ptr, "Cannot reallocate memory for Token_Container objects !",
                    (old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE) * sizeof (struct Tokens));
            memset(temp_ptr + old_allocated_token_container, '\0', sizeof (struct Tokens) * TOKEN_CONTAINER_ALLOCATION_STEP_SIZE);

            new_container->tokens = temp_ptr;
            new_container->allocated_token_container = old_allocated_token_container + TOKEN_CONTAINER_ALLOCATION_STEP_SIZE;

            // Fuer die neuen Objekte neue Tokens-Objekte erzeugen
            for (size_t i = old_allocated_token_container; i < new_container->allocated_token_container; ++ i)
            {
                new_container->tokens [i].data = (char*) MALLOC(MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
                ASSERT_ALLOC(new_container->tokens [i].data, "Cannot create data for a Token object !",
                        MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);
                memset(new_container->tokens [i].data, '\0', MAX_TOKEN_LENGTH * TOKENS_ALLOCATION_STEP_SIZE);

                new_container->tokens [i].max_token_length = MAX_TOKEN_LENGTH;
                new_container->tokens [i].allocated_tokens = TOKENS_ALLOCATION_STEP_SIZE;
            }

            PRINTF_FFLUSH("Token_Container realloc. From %zu to %zu objects (%zu times)\n",
                    old_allocated_token_container, new_container->allocated_token_container,
                    token_container_realloc_counter);
        }

        Extract_Tokens_From_Line
        (
                new_container,
                file_buffer,
                strlen(file_buffer)
        );
    }


    FCLOSE_AND_SET_TO_NULL(file);
    FREE_AND_SET_TO_NULL(file_buffer);

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

    return container->tokens [index_tokens].data + (max_token_size * index_token_in_tokens_object);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Groesse des Token_Containers (inkl. dynamisch allokierten Speicher) in Byte zurueckgeben.
 *
 * @param[in] container Token_Container
 *
 * @return Groesse des Token_Containers (inkl. dynamisch allokierten Speicher) in Byte
 */
extern size_t
Get_Token_Container_Size
(
        const struct Token_Container* const container
)
{
    size_t result = 0;

    const size_t max_token_size = container [0].tokens->max_token_length;
    result += sizeof (struct Tokens) * container->allocated_token_container;
    result += sizeof (struct Token_Container);

    for (size_t i = 0; i < container->allocated_token_container; ++ i)
    {
        result += (container->tokens [i].allocated_tokens * max_token_size);
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Inhalt eines Tokens-Objektes ausgeben.
 *
 * @param[in] container Token_Container
 * @param[in] selected_container Index des Tokens-Objektes im uebergebenen Token_Container
 */
extern void
Show_Selected_Token_Container
(
        const struct Token_Container* const container,
        const size_t selected_container
)
{
    const size_t token_size = container->tokens [selected_container].max_token_length;

    printf ("Container: %zu\n", selected_container);
    for (size_t i = 0; i < container->tokens [selected_container].next_free_element; ++ i)
    {
        printf ("%4zu: %s\n", i, &(container->tokens [selected_container].data [i * token_size]));
    }
    PUTS_FFLUSH("");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Anzahl alle Tokens im gesamten Token_Container (also inkl. aller Tokens-Objekte) ermitteln und zurueckgeben.
 *
 * @param[in] container Token_Container
 *
 * @return Anzahl aller Tokens im gesamten Token_Container (also inkl. aller Tokens-Objekte)
 */
extern uint_fast32_t
Count_All_Tokens_In_Token_Container
(
        const struct Token_Container* const container
)
{
    uint_fast32_t result = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        result += container->tokens [i].next_free_element;
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

extern size_t
Get_Lengh_Of_Longest_Token_Container
(
        const struct Token_Container* const container
)
{
    size_t result = 0;

    for (uint_fast32_t i = 0; i < container->next_free_element; ++ i)
    {
        if (container->tokens [i].next_free_element > result)
        {
            result = container->tokens [i].next_free_element;
        }
    }

    return result;
}

//=====================================================================================================================

/**
 * @brief Eine Zeile aus einer bereits geoeffneten Datei lesen.
 *
 * Es wird der Zeiger auf den Puffer zurueckgegeben, um so Fehler (z.B. durch NULL) anzeigen zu koennen.
 *
 * @param[in] token_container Token_Container
 * @param[in] file FILE-Objekt auf die bereits geoeffnete Datei
 * @param[in] file_buffer Benutzerdefinierter Puffer fuer das Lesen der Datei
 * @param[in] buffer_length Groesse des Puffers
 * @param[in] next_char_in_buffer Index des naechsten freien Zeichen im Puffer
 *
 * @return Zeiger auf den Puffer
 */
static char*
Read_Line
(
        struct Token_Container* const restrict token_container,
        FILE* const file,
        char* const restrict file_buffer,
        const size_t buffer_length,
        size_t next_char_in_buffer
)
{
    ASSERT_MSG(token_container != NULL, "Token_Container is NULL !");
    ASSERT_MSG(file_buffer != NULL, "file_buffer is NULL !");
    ASSERT_MSG(buffer_length > 0, "buffer_length is 0 !");
    ASSERT_FMSG(next_char_in_buffer < buffer_length, "Next char buffer index (%zu) is equal/larger than the buffer "
            "length (%zu) !", next_char_in_buffer, buffer_length);

    static uint_fast32_t current_file_line = 0;
    static size_t sum_char_read = 0;
    ++ current_file_line;

    char* return_value = fgets (file_buffer + next_char_in_buffer, (int) (buffer_length - next_char_in_buffer), file);

    //ASSERT_FMSG(return_value != NULL, "Cannot read the file line %" PRIuFAST32 " !", current_file_line);
    if (return_value != NULL)
    {
        sum_char_read += strlen(file_buffer);
        PRINTF_FFLUSH("Read line %5" PRIuFAST32 ". %8zu char. Sum char read %zu\n", current_file_line,
                strlen(file_buffer), sum_char_read);
    }

    // Reicht die Groesse des Puffers aus ? Bzw. konnte die aktuelle Zeile komplett gelesen werden
    //ASSERT_FMSG(file_buffer [buffer_length - 1] != '\0', "File buffer length %zu is not large enough to read the full "
    //        "line %" PRIuFAST32 " !", buffer_length, current_file_line);

    return return_value;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Tokens aus einem Puffer extrahieren und dem Token_Container hinzufuegen.
 *
 * Hinweis: Das Ermitteln der Tokens ist sehr rudimentaer und NICHT fuer die Verwendung geeignet ! Bisher wurde eine
 * schmandige und sehr einfache Variante implementiert, um die Entwicklung fortfuehren zu koennen !
 *
 * @param[in] token_container Token_Container
 * @param[in] file_buffer Datei-Puffer mit den Inhalt der Daten, die verarbeitet werden sollen
 * @param[in] next_char_in_buffer Index des naechsten freien Zeichen im Puffer
 */
static void
Extract_Tokens_From_Line
(
        struct Token_Container* const restrict token_container,
        const char* const restrict file_buffer,
        const size_t used_char_in_buffer
)
{
    ASSERT_MSG(token_container != NULL, "Token_Container is NULL !");
    ASSERT_MSG(file_buffer != NULL, "file_buffer is NULL !");
    ASSERT_MSG(used_char_in_buffer > 0, "used_char_in_buffer is 0 !");

    if (used_char_in_buffer <= 2) { return; }

    size_t file_buffer_cursor = 0;

    while (file_buffer_cursor < used_char_in_buffer)
    {
        const uint_fast32_t next_free_element_in_tokens =
                token_container->tokens [token_container->next_free_element].next_free_element;
        struct Tokens* current_tokens_obj = &(token_container->tokens [token_container->next_free_element]);

        // Wird weiterer Speicher fuer die neuen Tokens benoetigt ?
        if (next_free_element_in_tokens >= current_tokens_obj->allocated_tokens)
        {
            static size_t tokens_realloc_counter = 0;
            ++ tokens_realloc_counter;
            const size_t old_tokens_size = current_tokens_obj->allocated_tokens;
            const size_t token_size = current_tokens_obj->max_token_length;

            char* tmp_ptr = (char*) REALLOC(current_tokens_obj->data,
                    (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
            ASSERT_ALLOC(tmp_ptr, "Cannot reallocate memory for Tokens data !",
                    (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);
            //memset(tmp_ptr, '\0',
            //        (old_tokens_size + TOKENS_ALLOCATION_STEP_SIZE) * token_size);

            current_tokens_obj->data = tmp_ptr;
            current_tokens_obj->allocated_tokens += TOKENS_ALLOCATION_STEP_SIZE;

            PRINTF_FFLUSH("Tokens realloc. From %zu to %zu objects (%zu times)\n", old_tokens_size,
                    current_tokens_obj->allocated_tokens, tokens_realloc_counter);
        }


        // Beginn des naechsten Tokens suchen
        while (file_buffer [file_buffer_cursor] != '\'' && file_buffer [file_buffer_cursor] != '\"')
        {
            ++ file_buffer_cursor;
            if (file_buffer [file_buffer_cursor] == ']' || (file_buffer_cursor >= used_char_in_buffer))
            {
                goto end;
            }
        }
        ++ file_buffer_cursor;

        // Das Ende des Tokens suchen
        size_t end_token = file_buffer_cursor;
        while (file_buffer [end_token] != '\'' && file_buffer [end_token] != '\"')
        {
            ++ end_token;
            if (file_buffer [end_token] == ']' || (end_token >= used_char_in_buffer))
            {
                goto end;
            }
        }
        const size_t token_length = end_token - file_buffer_cursor;

        // Alle Tokens, die kleiner als 3 Zeichen sind, werden uebersprungen
        if (token_length < 3)
        {
            file_buffer_cursor = end_token + 1;
            continue;
        }

        const size_t token_size = token_container->tokens [token_container->next_free_element].max_token_length;
        strncpy (&(token_container->tokens [token_container->next_free_element].data [next_free_element_in_tokens * token_size]),
                &(file_buffer [file_buffer_cursor]),
                ((token_length >= MAX_TOKEN_LENGTH) ? MAX_TOKEN_LENGTH - 1 : token_length));

        token_container->tokens [token_container->next_free_element].data [((next_free_element_in_tokens + 1) * token_size) - 1] = '\0';
        token_container->tokens [token_container->next_free_element].next_free_element ++;
        file_buffer_cursor = end_token + 1;
    }
    end:
    token_container->next_free_element ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
