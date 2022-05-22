/*
 * Document_Word_List_Test.c
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#include "Document_Word_List_Test.h"
#include "../Document_Word_List.h"
#include "../Misc.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Print_Tools.h"
#include "Create_Test_Data.h"



extern _Bool TEST_Intersection (void)
{
    _Bool result = false;

    // Testdaten (die aus der ersten Besprechung)
    struct Document_Word_List* test_data [4];
    test_data [0] = Create_Document_Word_List(3, 4);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            3 * 4 * sizeof (uint_fast32_t));
    test_data [1] = Create_Document_Word_List(2, 4);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            2 * 4 * sizeof (uint_fast32_t));
    test_data [2] = Create_Document_Word_List(2, 6);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            2 * 6 * sizeof (uint_fast32_t));
    test_data [3] = Create_Document_Word_List(1, 5);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            1 * 5 * sizeof (uint_fast32_t));

    const uint_fast32_t test_0_test_data [][4] =
    {
            { 1, 7, 12, 13 },
            { 2, 5, 7, 9 },
            { 4, 8, 7, 11 }
    };
    const uint_fast32_t test_1_test_data [][4] =
    {
            { 1, 11, 7, 5 },
            { 1, 12, 15, 14 }
    };
    const uint_fast32_t test_2_test_data [][6] =
    {
            { 1, 2, 3, 7, 12, 13 },
            { 4, 5, 6, 7, 8, 0 }
    };
    const uint_fast32_t test_3_test_data [][5] =
    {
            { 2, 7, 17, 20, 22 }
    };
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_0_test_data); ++ i)
    {
        Append_Data_To_Document_Word_List(test_data [0], test_0_test_data [i], 4);
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_1_test_data); ++ i)
    {
        Append_Data_To_Document_Word_List(test_data [1], test_1_test_data [i], 4);
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_2_test_data); ++ i)
    {
        Append_Data_To_Document_Word_List(test_data [2], test_2_test_data [i], 6);
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_3_test_data); ++ i)
    {
        Append_Data_To_Document_Word_List(test_data [3], test_3_test_data [i], 5);
    }

    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_data); ++ i)
    {
        Show_Data_And_Attributes_From_Document_Word_List(test_data [i]);
        puts("");
    }

    // Das erste Objekt ist das Original-Objekt, welches die Menge enthalt, worauf die Schnittmengen gebildet werden
    struct Document_Word_List* intersection_objects [COUNT_ARRAY_ELEMENTS(test_data) - 1];
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(intersection_objects); ++ i)
    {
        // Schnittmengen bilden
        // T 1,1 sind die Daten womit die Schnittmenge erzeugt wird
        intersection_objects [i] = Intersect_Data_With_Document_Word_List(test_data [i + 1],
                test_data [0]->data [0], 4, INTERSECTION_MODE_DEFAULTS);
        ASSERT_ALLOC(intersection_objects [i], "Cannot create an intersection object ! Given object size could be wrong !",
                sizeof (struct Document_Word_List));
    }

    PUTS_FFLUSH("\n");
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(intersection_objects); ++ i)
    {
        Show_Data_And_Attributes_From_Document_Word_List(intersection_objects [i]);
        puts("");
    }

    // Beide Arrays loeschen
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_data); ++ i)
    {
        Delete_Document_Word_List(test_data [i]);
        test_data [i] = NULL;
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(intersection_objects); ++ i)
    {
        Delete_Document_Word_List(intersection_objects [i]);
        intersection_objects[i] = NULL;
    }

    return result;
}

extern _Bool TEST_Intersection_With_Random_Data (void)
{
    _Bool result = false;

    struct Document_Word_List* list_one_with_random_data = Create_Document_Word_List_With_Random_Test_Data
            (5, 10, 10);
    struct Document_Word_List* list_two_with_random_data = Create_Document_Word_List_With_Random_Test_Data
            (5, 10, 10);

    Show_Data_And_Attributes_From_Document_Word_List(list_one_with_random_data);
    Show_Data_And_Attributes_From_Document_Word_List(list_two_with_random_data);

    // Eine Schnittmenge mit den Zufallszahlen ausfuehren und die Zeit bestimmen
    struct Document_Word_List* intersection_data =
            Intersect_Data_With_Document_Word_List(list_two_with_random_data, list_one_with_random_data->data [0],
                    list_one_with_random_data->arrays_lengths[0], INTERSECTION_MODE_DEFAULTS);

    Show_Data_And_Attributes_From_Document_Word_List(intersection_data);

    Delete_Document_Word_List(list_one_with_random_data);
    Delete_Document_Word_List(list_two_with_random_data);
    Delete_Document_Word_List(intersection_data);
    list_one_with_random_data = NULL;
    list_two_with_random_data = NULL;
    intersection_data = NULL;

    return result;
}
