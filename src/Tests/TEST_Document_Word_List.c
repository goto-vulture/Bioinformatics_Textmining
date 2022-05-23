/*
 * Document_Word_List_Test.c
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#include "TEST_Document_Word_List.h"

#include <time.h>
#include "../Document_Word_List.h"
#include "../Misc.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Print_Tools.h"
#include "Create_Test_Data.h"



#ifndef NUMBER_OF_ARRAYS
#define NUMBER_OF_ARRAYS 20
#else
#error "The macro \"NUMBER_OF_ARRAYS\" is already defined !"
#endif /* NUMBER_OF_ARRAYS */

#ifndef MAX_ARRAY_LENGTH
#define MAX_ARRAY_LENGTH 25000
#else
#error "The macro \"MAX_ARRAY_LENGTH\" is already defined !"
#endif /* MAX_ARRAY_LENGTH */

#ifndef RAND_UPPER_BOUND
#define RAND_UPPER_BOUND 10
#else
#error "The macro \"RAND_UPPER_BOUND\" is already defined !"
#endif /* RAND_UPPER_BOUND */

static void Use_All_Intersection_Modes
(
        const struct Document_Word_List* const restrict data_1,
        const struct Document_Word_List* const restrict data_2
);

//---------------------------------------------------------------------------------------------------------------------

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
                test_data [0]->data [0], 4, INTERSECTION_MODE_2_NESTED_LOOPS);
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

//---------------------------------------------------------------------------------------------------------------------

extern _Bool TEST_Intersection_With_Random_Data (void)
{
    _Bool result = false;

    struct Document_Word_List* list_one_with_random_data = Create_Document_Word_List_With_Random_Test_Data
            (1, MAX_ARRAY_LENGTH, RAND_UPPER_BOUND);
    struct Document_Word_List* list_two_with_random_data = Create_Document_Word_List_With_Random_Test_Data
            (NUMBER_OF_ARRAYS, MAX_ARRAY_LENGTH, RAND_UPPER_BOUND);

    // Eine Schnittmenge mit den Zufallszahlen ausfuehren und die Zeit bestimmen (Alle Modi verwenden)
    Use_All_Intersection_Modes (list_one_with_random_data, list_two_with_random_data);

    Delete_Document_Word_List(list_one_with_random_data);
    Delete_Document_Word_List(list_two_with_random_data);
    list_one_with_random_data = NULL;
    list_two_with_random_data = NULL;

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool TEST_Intersection_With_Random_Data_And_Specified_Result (void)
{
    _Bool result = false;

    struct Document_Word_List* list_one_with_specified_data = Create_Document_Word_List(1, 5);
    const uint_fast32_t array_data [] = { 0, 2, 4, 6, 8 };
    Append_Data_To_Document_Word_List(list_one_with_specified_data, array_data, COUNT_ARRAY_ELEMENTS(array_data));

    struct Document_Word_List* list_two_with_random_data = Create_Document_Word_List_With_Random_Test_Data_Plus_Specified_Data
            (array_data, COUNT_ARRAY_ELEMENTS(array_data), NUMBER_OF_ARRAYS, MAX_ARRAY_LENGTH, RAND_UPPER_BOUND);

    // Eine Schnittmenge mit den Zufallszahlen ausfuehren und die Zeit bestimmen
    Use_All_Intersection_Modes(list_one_with_specified_data, list_two_with_random_data);

    Delete_Document_Word_List(list_one_with_specified_data);
    Delete_Document_Word_List(list_two_with_random_data);
    list_one_with_specified_data = NULL;
    list_two_with_random_data = NULL;

    return result;
}

//=====================================================================================================================

static void Use_All_Intersection_Modes
(
        const struct Document_Word_List* const restrict data_1,
        const struct Document_Word_List* const restrict data_2
)
{
    ASSERT_MSG(data_1 != NULL, "data 1 is NULL !");
    ASSERT_MSG(data_2 != NULL, "data 2 is NULL !");

    const enum Intersection_Mode used_modes [] =
    {
            INTERSECTION_MODE_2_NESTED_LOOPS,
            INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH
    };
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(used_modes); ++ i)
    {
        const clock_t begin = clock();
        ASSERT_MSG(begin != -1, "Time values are not available on this system !");
        struct Document_Word_List* intersection_data =
                Intersect_Data_With_Document_Word_List(data_2, data_1->data [0], data_1->arrays_lengths[0],
                        used_modes [i]);
        const clock_t end = clock();
        ASSERT_MSG(end != -1, "Time values are not available on this system !");

        printf ("Mode: ");
        switch (used_modes [i])
        {
        case INTERSECTION_MODE_2_NESTED_LOOPS:
            printf ("2 nested loops:          ");
            break;
        case INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH:
            printf ("QSort and binary search: ");
            break;
        default:
            ASSERT_MSG(false, "Default path executed !");
        }
        printf ("%10.4f sec.\n", (float)(end - begin) / CLOCKS_PER_SEC);

        //Show_Data_And_Attributes_From_Document_Word_List(intersection_data);
        // puts("");
        Delete_Document_Word_List(intersection_data);
        intersection_data = NULL;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef NUMBER_OF_ARRAYS
#undef NUMBER_OF_ARRAYS
#endif /* NUMBER_OF_ARRAYS */

#ifdef MAX_ARRAY_LENGTH
#undef MAX_ARRAY_LENGTH
#endif /* MAX_ARRAY_LENGTH */

#ifdef RAND_UPPER_BOUND
#undef RAND_UPPER_BOUND
#endif /* RAND_UPPER_BOUND */
