/*
 * Create_Test_Data.c
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#include "Create_Test_Data.h"
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Misc.h"
#include "../Print_Tools.h"


static void Create_Random_Data
(
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound,
        struct Document_Word_List* const data_container
);



//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data
(
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
)
{
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");

    struct Document_Word_List* result_object = Create_Document_Word_List(number_of_arrays, max_array_length);
    ASSERT_ALLOC(result_object, "Cannot create new Document_Word_List with random test data !",
            sizeof (struct Document_Word_List));

    Create_Random_Data(number_of_arrays, max_array_length, rand_upper_bound, result_object);

    return result_object;
}

//---------------------------------------------------------------------------------------------------------------------

extern struct Document_Word_List*
Create_Document_Word_List_With_Random_Test_Data_Plus_Specified_Data
(
        const uint_fast32_t* specified_data,
        const size_t specified_data_length,
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound
)
{
    ASSERT_MSG(specified_data !=  NULL, "Specified data is NULL !");
    ASSERT_MSG(specified_data_length != 0, "Specified data length is 0 !");
    ASSERT_MSG(number_of_arrays != 0, "Number of arrays is 0 !");
    ASSERT_MSG(max_array_length != 0, "Max array length is 0 !");
    ASSERT_FMSG(specified_data_length <= max_array_length,
            "More specified data per array (%zu) than the max array length (%zu) !", specified_data_length,
            max_array_length);

    struct Document_Word_List* result_object = Create_Document_Word_List(number_of_arrays, max_array_length);
    ASSERT_ALLOC(result_object, "Cannot create new Document_Word_List with random test data !",
            sizeof (struct Document_Word_List));

    Create_Random_Data(number_of_arrays, max_array_length, rand_upper_bound, result_object);

    // Nachdem die Testdaten erzeugt wurden, werden die vorher festgeleten Testdaten in jedem Array an zufaelliger
    // Position eingebaut
    for (size_t i = 0; i < number_of_arrays; ++ i)
    {
        uint_fast32_t* used_positions = (uint_fast32_t*) CALLOC(specified_data_length, sizeof (uint_fast32_t));
        ASSERT_ALLOC(used_positions, "Try to create the array for the used positions information !",
                specified_data_length * sizeof (uint_fast32_t));

        for (size_t i2 = 0; i2 < specified_data_length; ++ i2)
        {
            while (1)
            {
                used_positions [i2] = (uint_fast32_t) rand () % max_array_length;
                _Bool new_position = true;

                // Wurde diese Position bereits vergeben ?
                for (size_t i3 = 0; i3 < i2; ++ i3)
                {
                    if (used_positions [i2] == used_positions [i3])
                    {
                        new_position = false;
                        break;
                    }
                }
                if (new_position == true)
                {
                    break;
                }
            }

            result_object->data [i][used_positions [i2]] = specified_data [i2];
        }

        FREE_AND_SET_TO_NULL(used_positions);
    }

    return result_object;
}

//=====================================================================================================================

static void Create_Random_Data
(
        const size_t number_of_arrays,
        const size_t max_array_length,
        const int rand_upper_bound,
        struct Document_Word_List* const data_container
)
{
    uint_fast64_t created_test_data = 0;

    // Hiermit wird sichergestellt, dass der Seed fuer die Zufallszahlen nur einmal gesetzt wird
    static _Bool init_done = false;
    if (init_done == false)
    {
        srand((unsigned int) time(NULL));
    }
    init_done = true;

    for (size_t i = 0; i < number_of_arrays; ++ i)
    {
        size_t used_array_length = ((size_t) rand () % (max_array_length + 1));
        if (used_array_length == 0)
        {
            ++ used_array_length;
        }

        size_t* new_test_data = (size_t*) MALLOC(sizeof (uint_fast32_t) * used_array_length);
        for (size_t i2 = 0; i2 < used_array_length; ++ i2)
        {
            new_test_data [i2] = (size_t) rand () % (size_t) rand_upper_bound;
            ++ created_test_data;
        }
        Append_Data_To_Document_Word_List (data_container, new_test_data, used_array_length);
        FREE_AND_SET_TO_NULL(new_test_data);
    }

    PRINTF_FFLUSH("Created test data: %" PRIuFAST64 "\n", created_test_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

