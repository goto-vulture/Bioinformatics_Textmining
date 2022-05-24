/*
 * Intersection_Approaches.c
 *
 *  Created on: 23.05.2022
 *      Author: t430
 */

#include "Intersection_Approaches.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Misc.h"
#include "Error_Handling/Assert_Msg.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Print_Tools.h"



#ifndef MULTIPLE_GUARD_ALLOC_STEP
#define MULTIPLE_GUARD_ALLOC_STEP 100
#else
#error "The macro \"MULTIPLE_GUARD_ALLOC_STEP\" is already defined !"
#endif /* MULTIPLE_GUARD_ALLOC_STEP */

static struct Document_Word_List*
Init
(
        const struct Document_Word_List* const restrict object
);
static void
Find_Intersection_Data
(
        const struct Document_Word_List* const restrict intersection_result,
        const struct Document_Word_List* const restrict object,
        const uint_fast32_t* const restrict data,
        const size_t data_length
);

static int
Compare_Function
(
        const void* const a,
        const void* const b
);
static _Bool
Binary_Search
(
        const uint_fast32_t* const data,
        const size_t data_size,
        const uint_fast32_t search_value
);
static void
Heapsort
(
        uint_fast32_t* const data,
        const size_t data_size
);

//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Intersection_Approach_2_Nested_Loops
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
)
{
    struct Document_Word_List* intersection_result = Create_Document_Word_List (object->number_of_arrays,
            object->max_array_length);
    ASSERT_ALLOC(intersection_result, "Cannot create new Document Word List for intersection !",
            sizeof (struct Document_Word_List) + object->number_of_arrays * object->max_array_length *
            sizeof (uint_fast32_t));
    intersection_result->next_free_array = object->next_free_array;
    intersection_result->intersection_data = true;

    // Array, welches erkennt, ob in der Schnittmenge ein Wert mehrfach hinzugefuegt werden soll
    size_t size_multiple_guard = MULTIPLE_GUARD_ALLOC_STEP;

    _Bool* multiple_guard = (_Bool*) CALLOC(size_multiple_guard, sizeof (_Bool));
    ASSERT_ALLOC(multiple_guard, "Cannot create the multiple guard !", size_multiple_guard * sizeof (_Bool));

    // Fuer den ersten Versuch die naive Variant fuer die Bestimmung der Schnittmenge
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        memset(multiple_guard, '\0', size_multiple_guard * sizeof (_Bool));

        for (size_t i2 = 0; i2 < object->arrays_lengths [i]; ++ i2)
        {
            // Jedes Element des Arrays mit jedem Element der Testdaten miteinander vergleichen
            for (size_t i3 = 0; i3 < data_length; ++ i3)
            {
                if (object->data [i][i2] == data [i3])
                {
                    // Reicht der Speicher aus ?
                    if (data [i3] > size_multiple_guard)
                    {
                        size_multiple_guard += MULTIPLE_GUARD_ALLOC_STEP;
                        multiple_guard = (_Bool*) REALLOC(multiple_guard, size_multiple_guard * sizeof (_Bool));
                        ASSERT_ALLOC(multiple_guard, "Cannot increase the memory for the multiple guard !",
                                size_multiple_guard * sizeof (_Bool));
                    }

                    if (multiple_guard [data [i3]] == false)
                    {
                        intersection_result->data [i][intersection_result->arrays_lengths [i]] = data [i3];
                        intersection_result->arrays_lengths [i] ++;
                        multiple_guard [data [i3]] = true;
                    }
                }
            }
        }
    }

    // printf ("Used size for the multiple guard: %zu\n", size_multiple_guard * sizeof (_Bool));
    FREE_AND_SET_TO_NULL(multiple_guard);

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Intersection_Approach_QSort_And_Binary_Search
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
)
{
    /*struct Document_Word_List* intersection_result = Create_Document_Word_List (object->number_of_arrays,
            object->max_array_length);
    ASSERT_ALLOC(intersection_result, "Cannot create new Document Word List for intersection !",
            sizeof (struct Document_Word_List) + object->number_of_arrays * object->max_array_length *
            sizeof (uint_fast32_t));
    intersection_result->next_free_array = object->next_free_array;
    intersection_result->intersection_data = true;*/

    struct Document_Word_List* intersection_result = Init (object);

    // Alle Daten der Woerter-Liste aufsteigend sortieren
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        qsort (object->data [i], object->arrays_lengths [i], sizeof (uint_fast32_t), Compare_Function);
        //Heapsort(object->data [i], object->arrays_lengths [i]);
    }

    Find_Intersection_Data (intersection_result, object, data, data_length);

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Intersection_Approach_HeapSort_And_Binary_Search
(
    const struct Document_Word_List* const restrict object,
    const uint_fast32_t* const restrict data,
    const size_t data_length
)
{
    struct Document_Word_List* intersection_result = Init (object);

    // Alle Daten der Woerter-Liste aufsteigend sortieren
    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        // qsort (object->data [i], object->arrays_lengths [i], sizeof (uint_fast32_t), Compare_Function);
        Heapsort(object->data [i], object->arrays_lengths [i]);
    }

    Find_Intersection_Data (intersection_result, object, data, data_length);

    return intersection_result;
}

//=====================================================================================================================

static struct Document_Word_List*
Init
(
        const struct Document_Word_List* const restrict object
)
{
    struct Document_Word_List* intersection_result = Create_Document_Word_List (object->number_of_arrays,
            object->max_array_length);
    ASSERT_ALLOC(intersection_result, "Cannot create new Document Word List for intersection !",
            sizeof (struct Document_Word_List) + object->number_of_arrays * object->max_array_length *
            sizeof (uint_fast32_t));
    intersection_result->next_free_array = object->next_free_array;
    intersection_result->intersection_data = true;

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Find_Intersection_Data
(
        const struct Document_Word_List* const restrict intersection_result,
        const struct Document_Word_List* const restrict object,
        const uint_fast32_t* const restrict data,
        const size_t data_length
)
{
    // Array, welches erkennt, ob in der Schnittmenge ein Wert mehrfach hinzugefuegt werden soll
    size_t size_multiple_guard = MULTIPLE_GUARD_ALLOC_STEP;

    _Bool* multiple_guard = (_Bool*) CALLOC(size_multiple_guard, sizeof (_Bool));
    ASSERT_ALLOC(multiple_guard, "Cannot create the multiple guard !", size_multiple_guard * sizeof (_Bool));

    for (size_t i = 0; i < object->number_of_arrays; ++ i)
    {
        memset(multiple_guard, '\0', size_multiple_guard * sizeof (_Bool));

        // Binaere Suche durchfuehren
        for (size_t i2 = 0; i2 < data_length; ++ i2)
        {
            if (Binary_Search(object->data[i], object->arrays_lengths [i], data [i2]) == true)
            {
                // Reicht der Speicher aus ?
                if (data [i2] > size_multiple_guard)
                {
                    size_multiple_guard += MULTIPLE_GUARD_ALLOC_STEP;
                    multiple_guard = (_Bool*) REALLOC(multiple_guard, size_multiple_guard * sizeof (_Bool));
                    ASSERT_ALLOC(multiple_guard, "Cannot increase the memory for the multiple guard !",
                            size_multiple_guard * sizeof (_Bool));
                }

                if (multiple_guard [data [i2]] == false)
                {
                    intersection_result->data [i][intersection_result->arrays_lengths [i]] = data [i2];
                    intersection_result->arrays_lengths [i] ++;
                    multiple_guard [data [i2]] = true;
                }
            }
        }
    }

    FREE_AND_SET_TO_NULL(multiple_guard);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static int
Compare_Function
(
        const void* const a,
        const void* const b
)
{
    const uint_fast32_t* const val_a = (uint_fast32_t*) a;
    const uint_fast32_t* const val_b = (uint_fast32_t*) b;

    if (*val_a < *val_b) { return -1; }
    if (*val_a > *val_b) { return +1; }
    return 0;
}

//---------------------------------------------------------------------------------------------------------------------

static _Bool
Binary_Search
(
        const uint_fast32_t* const data,
        const size_t data_size,
        const uint_fast32_t search_value
)
{
    ASSERT_MSG(data != NULL, "data is NULL !");
    ASSERT_MSG(data_size != 0, "data size is 0 !");

    _Bool result = false;

    // Diese Werte MUESSEN mit Vorzeichen sein, da in machen Faellen right -1 annimmt !
    int_fast32_t left = 0;
    int_fast32_t right = (int_fast32_t) (data_size - 1);

    while (left <= right)
    {
        // Hier wird der moegliche negative Wert fuer right durch die Ganzzahldivision auf -0 ausgewertet
        // Ausdruck dieses Sonderfalls: 0 + ((-1 + 0) / 2) => 0 + (-0) => 0
        const int_fast32_t middle = left + ((right - left) / 2);
        if (data [middle] == search_value)
        {
            result = true;
            break;
        }
        else
        {
            if (data [middle] > search_value)
            {
                right = middle - 1;
            }
            else
            {
                left = middle + 1;
            }
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

// Abgeleitet von der Implementierung auf Wikipedia: https://de.wikipedia.org/wiki/Heapsort#Implementierung
static void
Heapsort
(
        uint_fast32_t* const data,
        const size_t data_size
)
{
    ASSERT_MSG(data != NULL, "data is NULL !");
    ASSERT_MSG(data_size != 0, "data size is 0 !");

    uint_fast32_t value = 0;
    uint_fast32_t parent = 0;
    uint_fast32_t child = 0;
    uint_fast32_t root = data_size / 2;
    uint_fast32_t n = data_size;

    while (1)
    {
        if (root > 0)
        {
            -- root;
            parent = root;
            value = data [root];
        }
        else
        {
            -- n;
            if (n > 0)
            {
                value = data [n];
                data [n] = data [0];
                parent = 0;
            }
            else
            {
                break;
            }
        }

        while ((child = (parent + 1) << 1) < n)
        {
            if (data [child - 1] > data [child])
            {
                -- child;
            }
            data [parent] = data [child];
            parent = child;
        }

        if (child == n)
        {
            -- child;
            if (data [child] >= value)
            {
                data [parent] = data [child];
                data [child] = value;
                continue;
            }
            child = parent;
        }
        else
        {
            if (data [parent] >= value)
            {
                data [parent] = value;
                continue;
            }
            child = (parent - 1) >> 1;
        }

        while (child != root)
        {
            parent = (child - 1) >> 1;
            if (data [parent] >= value)
            {
                break;
            }
            data [child] = data [parent];
            child = parent;
        }

        data [child]= value;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef MULTIPLE_GUARD_ALLOC_STEP
#undef MULTIPLE_GUARD_ALLOC_STEP
#endif /* MULTIPLE_GUARD_ALLOC_STEP */
