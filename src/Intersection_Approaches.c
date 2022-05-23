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
    const size_t multiple_guard_realloc_step = 1000;
    size_t size_multiple_guard = multiple_guard_realloc_step;

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
                        size_multiple_guard += multiple_guard_realloc_step;
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

    printf ("Used size for the multiple guard: %zu\n", size_multiple_guard * sizeof (_Bool));
    FREE_AND_SET_TO_NULL(multiple_guard);

    return intersection_result;
}

//---------------------------------------------------------------------------------------------------------------------