/*
 * File_Reader.h
 *
 *  Created on: 21.06.2022
 *      Author: am1
 */

#ifndef FILE_READER_H
#define FILE_READER_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>
#include <stddef.h>



//=====================================================================================================================

struct Token_Container
{
    // Objekt fuer die Tokens
    struct Tokens
    {
        char* data;                         // Tokens (befinden sich auf einem linearen Stueck Speicher)
        size_t max_token_length;            // Maximale Laenge eines Tokens - Inkl. Nullterminator
        uint_fast32_t next_free_element;    // Naechstes freies Element im Tokenspeicher (Konkrete Adresse muss zur
                                            // Laufzeit berechnet werden)
        size_t allocated_tokens;            // Anzahl an Tokens, die allokiert wurden
    }* tokens;

    uint_fast32_t next_free_element;        // Naechstes freies Tokens-Objekt
    size_t allocated_token_container;       // Anzahl an allokierten Tokens-Objekte
};

//=====================================================================================================================

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
);

/**
 * @brief Token_Container loeschen.
 *
 * @param[in] object Container, der geloescht wird
 */
extern void
Delete_Token_Container
(
        struct Token_Container* object
);

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
);

extern size_t
Get_Token_Container_Size
(
        const struct Token_Container* const container
);

extern void
Show_Selected_Token_Container
(
        const struct Token_Container* const container,
        const size_t selected_container
);


// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FILE_READER_H */
