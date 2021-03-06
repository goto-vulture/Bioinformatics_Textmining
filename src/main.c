/**
 * @file main.c
 *
 * @brief The calculation of so called "Dominating Word Sets" from data sets in the field of bioinformatics.
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
 * Check the used C version, to replace keywords in the language if necessary. (E.g.: The C version is an older version
 * and contains some keywords which were included in newer releases of the language.
 *
 * The possiblility is high, that another components in the code exists, which are not compatible with older versions (I
 * used only the C11 standard while the development)
 * At least the conflict with the keywords can be solved with the following preprocessor statements.
 *
 * The macro __STDC_VERSION__ is for C90 not defined
 * ->   Replace the keywords inline and restrict with empty marcos to make a translation with the C90 standard possible.
 */
#ifndef __STDC_VERSION__
    /**
     * @brief Define the keyword "inline" as macro to replace them with empty code. (In C90 this keyword don't exists)
     */
    #ifndef inline
        #define inline
    #else
        #error "The macro \"inline\" is already defined !"
    #endif /* inline */

    /**
     * @brief Here is the same situation like with the keyword "inline". Define the keyword "restrict" as macro to
     * replace them with empty code. (In C90 this keyword also don't exists)
     */
    #ifndef restrict
        #define restrict
    #else
        #error "The macro \"restrict\" is already defined !"
    #endif /* restrict */

    /**
     * @brief Solve the problem with boolean values before C99. DIY "_Bool" with a macro. A "_Bool" will be replaced
     * with "int".
     */
    #ifndef _Bool
        #define _Bool int
    #else
        #error "The macro \"_Bool\" is already defined !"
    #endif /* _Bool */

    /**
     * @brief In rare situations "true" is not defined (e.g. before C99) -> DIY
     */
    #ifndef true
        #define true 1
    #else
        #error "The macro \"true\" is already defined !"
    #endif /* true */

    /**
     * @brief In rare situations "false" is not defined (e.g. before C99) -> DIY
     */
    #ifndef false
        #define false 0
    #else
        #error "The macro \"false\" is already defined !"
    #endif /* false */

/*
 * Is C11 available: Use _Statc_asserts to check the system before compiling the program.
 */
#elif __STDC_VERSION__ == 201112L
    #include <limits.h>
    #include <stddef.h>
    #include <time.h>

    _Static_assert(CHAR_BIT == 8, "A char must be exact 8 bit long !");
    _Static_assert(sizeof(size_t) >= 4, "The type \"size_t\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(time_t) >= 4, "The type \"time_t\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(int) >= 4, "The type \"int\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(long int) >= 8, "The type \"long int\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(void*) >= 4, "The type \"void*\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(float) >= 4, "The type \"float\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(double) >= 4, "The type \"double\" needs to be at least 4 byte wide !");
    _Static_assert(sizeof(long double) >= 4, "The type \"long double\" needs to be at least 4 byte wide !");
#endif /* __STDC_VERSION__ */

/**
 * @brief A simple string, that shows, if an information was not given.
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
#include "argparse.h"
#include "CLI_Parameter.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Tests/TEST_Document_Word_List.h"
#include "File_Reader.h"
#include "Token_Int_Mapping.h"
#include "Print_Tools.h"
#include "Document_Word_List.h"
#include "Error_Handling/Assert_Msg.h"
#include "Intersection_Approaches.h"

#include "Tests/tinytest.h"
#include "Tests/TEST_cJSON_Parser.h"



/**
 * @brief Run all test functions.
 */
static void
Run_All_Test_Functions
(
        void
);

/**
 * @brief Show the dynamic memory status. (How many malloc / calloc calls were done and is the number equal with the
 * amount of free calls). -> Exists memory leaks ?
 */
static void
At_Exit_Function
(
     void
);

//=====================================================================================================================

/**
 * @brief main function of the program.
 * *
 * @param[in] argc argc parameter
 * @param[in] argv Number of argc paramer
 *
 * @return 0, if the program execution was successful; Else != 0
 */
int main (const int argc, const char* argv [])
{
    // Runtime check of the system
    // This check is necessary, because a _Static_assert test is only available in C11
#if __STDC_VERSION__ < 201112L
    ASSERT_MSG(CHAR_BIT == 8, "A char must be exact 8 bit long !");
    ASSERT_MSG(sizeof(size_t) >= 4, "The type \"size_t\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(time_t) >= 4, "The type \"time_t\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(int) >= 4, "The type \"int\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(long int) >= 8, "The type \"long int\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(void*) >= 4, "The type \"void*\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(float) >= 4, "The type \"float\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(double) >= 4, "The type \"double\" needs to be at least 4 byte wide !");
    ASSERT_MSG(sizeof(long double) >= 4, "The type \"long double\" needs to be at least 4 byte wide !");
#endif /* __STDC_VERSION__ < 201112L */

    // Use a user defined function at the end of the program
    atexit(At_Exit_Function);

    // ===== ===== ===== BEGINN CLI-Parameter parsen ===== ===== =====
    struct argparse_option cli_options [] =
    {
            OPT_HELP(),

            OPT_GROUP("Hauptfunktionen"),
            OPT_STRING('i', "input", &GLOBAL_CLI_INPUT_FILE, "Erste Eingabedatei", NULL, 0, 0),
            OPT_STRING('j', "input2", &GLOBAL_CLI_INPUT_FILE2, "Zweite Eingabedatei", NULL, 0, 0),
            OPT_STRING('o', "output", &GLOBAL_CLI_OUTPUT_FILE, "Ausgabedatei", NULL, 0, 0),

            OPT_GROUP("Debug- / Testfunktionen"),
            OPT_BOOLEAN('T', "run_all_test_functions", &GLOBAL_RUN_ALL_TEST_FUNCTIONS,
                    "Ausfuehrung aller Testfunktionen", NULL, 0, 0),

            OPT_END()
    };

    // CLI-Parameter parsen
    struct argparse argparse_object;
    argparse_init(&argparse_object, cli_options, GLOBAL_USAGES, 0);
    argparse_describe(&argparse_object, GLOBAL_PROGRAM_DESCRIPTION, GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION);
    const int new_argc = argparse_parse(&argparse_object, argc, argv);
    (void) new_argc;

    /* Old (for test purposes) way to run the calculations */
    /* if (GLOBAL_INPUT_FILE == NULL && GLOBAL_OUTPUT_FILE == NULL)
    else if (GLOBAL_INPUT_FILE == NULL && GLOBAL_OUTPUT_FILE == NULL)
    {
        GLOBAL_INPUT_FILE = (char*) "/home/am1/Downloads/Sachen/test_ebm_tokens.txt";
        GLOBAL_OUTPUT_FILE = (char*) "/home/am1/Downloads/Sachen/output.txt";
    }

    PRINTF_FFLUSH ("Input file:  \"%s\"\nOutput file: \"%s\"\n\n", GLOBAL_INPUT_FILE, GLOBAL_OUTPUT_FILE);
    // ===== ===== ===== ENDE CLI-Parameter parsen ===== ===== =====

    struct Token_List_Container* token_container = Create_Token_Container_From_File (GLOBAL_INPUT_FILE);

    printf ("Full token container size: %zu byte\n", Get_Token_Container_Size(token_container));
    printf ("Sum all tokens in container: %" PRIuFAST32 "\n\n", Count_All_Tokens_In_Token_Container(token_container));

    // ===== ===== ===== BEGINN Mapping Tabelle bilden ===== ===== =====
    struct Token_Int_Mapping* token_int_mapping = Create_Token_Int_Mapping();

    for (uint_fast32_t i = 0; i < token_container->next_free_element; ++ i)
    {
        for (uint_fast32_t i2 = 0; i2 < token_container->token_lists [i].next_free_element; ++ i2)
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

    // Mehrdimensionaler Container fuer Int-Werte, die fuer die Schnittmengenbestimmung verwendet werden
    struct Document_Word_List* source_int_values =
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

    // ===== ===== ===== BEGINN Daten fuer die Berechnung vorbereiten ===== ===== =====
    for (uint_fast32_t i = 0; i < token_container->next_free_element; ++ i)
    {
        memset(token_int_values, '\0', length_of_longest_token_container * sizeof (uint_fast32_t));
        next_free_value = 0;

        // Token aus der Eingabedatei mittels des Mapping-Objektes in die passenden Int-Werte konvertieren
        for (uint_fast32_t i2 = 0; i2 < token_container->token_lists [i].next_free_element; ++ i2)
        {
            char* token = Get_Token_From_Token_Container (token_container, i, i2);
            token_int_values [next_free_value] = Token_To_Int(token_int_mapping, token, strlen(token));
            ++ next_free_value;
        }

        // Nur wenn Daten vorhanden sind, macht es Sinn diese zu kopieren
        if (next_free_value > 0)
        {
            // Damit das Einfuegen schneller funktioniert, geschieht dies ueber einen Block an Speicher und nicht fuer
            // jeden einzelnen Wert
            Append_Data_To_Document_Word_List(source_int_values, token_int_values, next_free_value);

            // Den ersten Datensatz als Datensatz verwenden, woran die Schnittmengenbestimmung durchgefuehrt wird
            if (! intersection_values_created)
            {
                memcpy (intersection_values, token_int_values, length_of_longest_token_container *
                        sizeof (uint_fast32_t));

                intersection_values_created = true;
                count_intersection_values = next_free_value;
            }
        }
    }
    // ===== ===== ===== ENDE Daten fuer die Berechnung vorbereiten ===== ===== =====

    // Schnittmengenbestimmung durchfuehren
    struct Document_Word_List* intersection_result = Intersection_Approach_2_Nested_Loops (source_int_values,
            intersection_values, count_intersection_values);

    // ===== ===== ===== BEGINN Ergebnisse ausgeben / in die Datei schreiben ===== ===== =====
    FILE* result_file = fopen(GLOBAL_OUTPUT_FILE, "w");
    ASSERT_FMSG(result_file != NULL, "Cannot open/create the result file: \"%s\" !", GLOBAL_OUTPUT_FILE);

    for (size_t i = 0; i < intersection_result->number_of_arrays; ++ i)
    {
        fprintf(stdout, "Token block: %zu\n[ ", i);
        fprintf(result_file, "Token block: %zu\n[ ", i);
        for (size_t i2 = 0; i2 < intersection_result->arrays_lengths [i]; ++ i2)
        {
            // Token zum passenden Int-Wert finden -> also Mapping rueckgaengig machen
            char int_to_token_mem [32];
            memset (int_to_token_mem, '\0', sizeof (int_to_token_mem));

            Int_To_Token (token_int_mapping, intersection_result->data [i][i2], int_to_token_mem,
                    sizeof (int_to_token_mem) - 1);
            fprintf(stdout, "%s", int_to_token_mem);
            fprintf(result_file, "%s", int_to_token_mem);

            if ((i2 + 1) < intersection_result->arrays_lengths [i])
            {
                fprintf(stdout, ", ");
                fprintf(result_file, ", ");
            }
        }
        fputs(" ]\n", stdout);
        fputs(" ]\n", result_file);
    }

    FCLOSE_AND_SET_TO_NULL(result_file);
    // ===== ===== ===== ENDE Ergebnisse ausgeben / in die Datei schreiben ===== ===== =====

    Delete_Document_Word_List(intersection_result);

    FREE_AND_SET_TO_NULL(token_int_values);
    FREE_AND_SET_TO_NULL(intersection_values);

    Delete_Document_Word_List(source_int_values);
    Delete_Token_Int_Mapping(token_int_mapping);
    Delete_Token_Container (token_container); */

    if (GLOBAL_RUN_ALL_TEST_FUNCTIONS)
    {
        Run_All_Test_Functions();

        // Show a test report
        TEST_REPORT();

        return (tt_fails != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
    }
    if (GLOBAL_CLI_INPUT_FILE != NULL)
    {
        printf ("Input file: \"%s\"\n", GLOBAL_CLI_INPUT_FILE);
        Check_CLI_Parameter_CLI_INPUT_FILE();
    }
    else
    {
        PUTS_FFLUSH ("Missing first input file. Option: [-i / --input]");
        exit(EXIT_FAILURE);
    }
    if (GLOBAL_CLI_INPUT_FILE2 != NULL)
    {
        printf ("Input file2: \"%s\"\n", GLOBAL_CLI_INPUT_FILE2);
        Check_CLI_Parameter_CLI_INPUT_FILE2();
    }
    else
    {
        PUTS_FFLUSH ("Missing second input file. Option: [-j / --input2]");
        exit(EXIT_FAILURE);
    }
    if (GLOBAL_CLI_OUTPUT_FILE != NULL)
    {
        printf ("Output file: \"%s\"\n", GLOBAL_CLI_OUTPUT_FILE);
        Check_CLI_Parameter_CLI_OUTPUT_FILE();
    }
    else
    {
        PUTS_FFLUSH ("Missing output file. Option: [-o / --output]");
        exit(EXIT_FAILURE);
    }
    // ===== ===== ===== ENDE CLI-Parameter parsen ===== ===== =====

    struct Token_List_Container* token_container_input_1 = Create_Token_Container_From_File (GLOBAL_CLI_INPUT_FILE);
    Print_Token_List_Status_Infos (token_container_input_1);

    struct Token_List_Container* token_container_input_2 = Create_Token_Container_From_File (GLOBAL_CLI_INPUT_FILE2);
    Print_Token_List_Status_Infos (token_container_input_2);

    Delete_Token_Container(token_container_input_1);
    token_container_input_1 = NULL;
    Delete_Token_Container(token_container_input_2);
    token_container_input_2 = NULL;

    return EXIT_SUCCESS;
}

//=====================================================================================================================

/**
 * @brief Run all test functions.
 */
static void
Run_All_Test_Functions
(
        void
)
{
    RUN(TEST_cJSON_Parse_JSON_Fragment);
    RUN(TEST_cJSON_Get_Token_Array_From_JSON_Fragment);
    RUN(TEST_cJSON_Parse_Full_JSON_File);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Show the dynamic memory status. (How many malloc / calloc calls were done and is the number equal with the
 * amount of free calls). -> Exists memory leaks ?
 */
static void
At_Exit_Function
(
        void
)
{
    puts ("\n");
    Show_Dynamic_Memory_Status();
    return;
}

//---------------------------------------------------------------------------------------------------------------------
