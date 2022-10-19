/**
 * @file TEST_Document_Word_List.c
 *
 * @brief Some test functions for the Document_Word_List object.
 *
 * @date 22.05.2022
 * @author Gyps
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
#define NUMBER_OF_ARRAYS 10 ///< Number of arrays in the Document_Word_List objects
#else
#error "The macro \"NUMBER_OF_ARRAYS\" is already defined !"
#endif /* NUMBER_OF_ARRAYS */

#ifndef MAX_ARRAY_LENGTH
#define MAX_ARRAY_LENGTH 10000 ///< Max length of a subset, that will be used
#else
#error "The macro \"MAX_ARRAY_LENGTH\" is already defined !"
#endif /* MAX_ARRAY_LENGTH */

#ifndef RAND_UPPER_BOUND
#define RAND_UPPER_BOUND 10 ///< Upper bound for the pseudo random numbers
#else
#error "The macro \"RAND_UPPER_BOUND\" is already defined !"
#endif /* RAND_UPPER_BOUND */

#ifndef NUMBER_OF_RUNS
#define NUMBER_OF_RUNS 50 ///< Numer of runs in each test
#else
#error "The macro \"NUMBER_OF_RUNS\" is already defined !"
#endif /* NUMBER_OF_RUNS */

// #define checks only works with C11 and higher
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(NUMBER_OF_ARRAYS > 0, "The macro \"NUMBER_OF_ARRAYS\" needs to be at least one !");
_Static_assert(MAX_ARRAY_LENGTH > 0, "The macro \"MAX_ARRAY_LENGTH\" needs to be at least one !");
_Static_assert(RAND_UPPER_BOUND > 0, "The macro \"RAND_UPPER_BOUND\" needs to be at least one !");
_Static_assert(NUMBER_OF_RUNS > 0, "The macro \"NUMBER_OF_RUNS needs\" to be at least one !");
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

/**
 * @brief Execute all intersection approaches and measure the used time. (Print the used time to stdout)
 *
 * @param[in] data_1 First Document_Word_List
 * @param[in] data_2 Second Document_Word_List
 */
static void Use_All_Intersection_Modes
(
        const struct Document_Word_List* const restrict data_1,
        const struct Document_Word_List* const restrict data_2
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief First intersection test with test data from the first meeting (11.05.2022).
 */
extern _Bool TEST_Intersection (void)
{
    _Bool result = false;

    // Testdata (from the first meeting)
    struct Document_Word_List* test_data [4];
    test_data [0] = DocumentWordList_CreateObject(3, 4);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            3 * 4 * sizeof (uint_fast32_t));
    test_data [1] = DocumentWordList_CreateObject(2, 4);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            2 * 4 * sizeof (uint_fast32_t));
    test_data [2] = DocumentWordList_CreateObject(2, 6);
    ASSERT_ALLOC(test_data [0], "Cannot create new Document_Word_List !", sizeof (struct Document_Word_List) *
            2 * 6 * sizeof (uint_fast32_t));
    test_data [3] = DocumentWordList_CreateObject(1, 5);
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
        DocumentWordList_AppendData(test_data [0], test_0_test_data [i], 4);
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_1_test_data); ++ i)
    {
        DocumentWordList_AppendData(test_data [1], test_1_test_data [i], 4);
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_2_test_data); ++ i)
    {
        DocumentWordList_AppendData(test_data [2], test_2_test_data [i], 6);
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_3_test_data); ++ i)
    {
        DocumentWordList_AppendData(test_data [3], test_3_test_data [i], 5);
    }

    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_data); ++ i)
    {
        DocumentWordList_ShowDataAndAttributes(test_data [i]);
        puts("");
    }

    // The first object is the origial object, that contains the set, on which the intersections will be generated
    struct Document_Word_List* intersection_objects [COUNT_ARRAY_ELEMENTS(test_data) - 1];
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(intersection_objects); ++ i)
    {
        // Determine intersections
        // T 1,1 are the data, which will be used for the calculation
        intersection_objects [i] = DocumentWordList_IntersectWithData(test_data [i + 1],
                test_data [0]->data_struct.data [0], 4, INTERSECTION_MODE_2_NESTED_LOOPS);
        ASSERT_ALLOC(intersection_objects [i], "Cannot create an intersection object ! Given object size could be wrong !",
                sizeof (struct Document_Word_List));
    }

    PUTS_FFLUSH("\n");
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(intersection_objects); ++ i)
    {
        DocumentWordList_ShowDataAndAttributes(intersection_objects [i]);
        puts("");
    }

    // Delete both arrays
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_data); ++ i)
    {
        DocumentWordList_DeleteObject(test_data [i]);
        test_data [i] = NULL;
    }
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(intersection_objects); ++ i)
    {
        DocumentWordList_DeleteObject(intersection_objects [i]);
        intersection_objects[i] = NULL;
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Intersection tests with full pseudo random numbers.
 */
extern _Bool TEST_Intersection_With_Random_Data (void)
{
    _Bool result = false;

    for (size_t i = 1; i <= NUMBER_OF_RUNS; ++ i)
    {
        PRINTF_FFLUSH("%4zu / %4zu\n", i, (size_t) NUMBER_OF_RUNS);

        // For the first data set it is sufficient to create only one array, because from the first data set only the
        // first subset will be used
        struct Document_Word_List* list_one_with_random_data = Create_Document_Word_List_With_Random_Test_Data
                (1, (MAX_ARRAY_LENGTH / NUMBER_OF_RUNS) * i, RAND_UPPER_BOUND);
        struct Document_Word_List* list_two_with_random_data = Create_Document_Word_List_With_Random_Test_Data
                (NUMBER_OF_ARRAYS, (MAX_ARRAY_LENGTH / NUMBER_OF_RUNS) * i, RAND_UPPER_BOUND);

        // Determine the intersections with all modes and measure the execution time
        Use_All_Intersection_Modes (list_one_with_random_data, list_two_with_random_data);

        DocumentWordList_DeleteObject(list_one_with_random_data);
        DocumentWordList_DeleteObject(list_two_with_random_data);
        list_one_with_random_data = NULL;
        list_two_with_random_data = NULL;
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Determine intersections with pseudo random data + a specified set of data, that will be inserted in every
 * subset.
 *
 * With this approach the excepted result is clear and the functionality of the intersection calculation can be checked.
 */
extern _Bool TEST_Intersection_With_Random_Data_And_Specified_Result (void)
{
    _Bool result = false;

    for (size_t i = 1; i <= NUMBER_OF_RUNS; ++ i)
    {
        PRINTF_FFLUSH("%4zu / %4zu\n", i, (size_t) NUMBER_OF_RUNS);

        struct Document_Word_List* list_one_with_specified_data = DocumentWordList_CreateObject(1, 5);
        const uint_fast32_t array_data [] = { 0, 2, 4, 6, 8 };
        DocumentWordList_AppendData(list_one_with_specified_data, array_data, COUNT_ARRAY_ELEMENTS(array_data));

        struct Document_Word_List* list_two_with_random_data = Create_Document_Word_List_With_Random_Test_Data_Plus_Specified_Data
                (array_data, COUNT_ARRAY_ELEMENTS(array_data), NUMBER_OF_ARRAYS, (MAX_ARRAY_LENGTH / NUMBER_OF_RUNS) * i, RAND_UPPER_BOUND);

        // Determine the intersections with all modes and measure the execution time
        Use_All_Intersection_Modes(list_one_with_specified_data, list_two_with_random_data);

        DocumentWordList_DeleteObject(list_one_with_specified_data);
        DocumentWordList_DeleteObject(list_two_with_random_data);
        list_one_with_specified_data = NULL;
        list_two_with_random_data = NULL;
    }

    return result;
}

//=====================================================================================================================

/**
 * @brief Execute all intersection approaches and measure the used time. (Print the used time to stdout)
 *
 * @param[in] data_1 First Document_Word_List
 * @param[in] data_2 Second Document_Word_List
 */
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
            INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH,
            INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH
    };
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(used_modes); ++ i)
    {
        clock_t begin = (clock_t) -1;
        CLOCK_WITH_RETURN_CHECK(begin);
        struct Document_Word_List* intersection_data =
                DocumentWordList_IntersectWithData(data_2, data_1->data_struct.data [0], data_1->arrays_lengths[0],
                        used_modes [i]);
        clock_t end = (clock_t) -1;
        CLOCK_WITH_RETURN_CHECK(end);

        printf ("Mode: ");
        switch (used_modes [i])
        {
        case INTERSECTION_MODE_2_NESTED_LOOPS:
            printf ("2 nested loops:              ");
            break;
        case INTERSECTION_MODE_QSORT_AND_BINARY_SEARCH:
            printf ("QSort and binary search:     ");
            break;
        case INTERSECTION_MODE_HEAPSORT_AND_BINARY_SEARCH:
            printf ("Heap-Sort and binary search: ");
            break;
        default:
            ASSERT_MSG(false, "Default path executed !");
        }
        printf ("%10.4f sec.\n", (float)(end - begin) / CLOCKS_PER_SEC);

        //Show_Data_And_Attributes_From_Document_Word_List(intersection_data);
        // puts("");
        DocumentWordList_DeleteObject(intersection_data);
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

#ifdef NUMBER_OF_RUNS
#undef NUMBER_OF_RUNS
#endif /* NUMBER_OF_RUNS */
