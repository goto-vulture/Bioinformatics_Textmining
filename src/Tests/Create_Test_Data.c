/*
 * Create_Test_Data.c
 *
 *  Created on: 22.05.2022
 *      Author: pc178
 */

#include "Create_Test_Data.h"
#include <stdlib.h>
#include <time.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Misc.h"



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

    srand((unsigned int) time(NULL));

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
        }
        Append_Data_To_Document_Word_List (result_object, new_test_data, used_array_length);
        FREE_AND_SET_TO_NULL(new_test_data);
    }

    return result_object;
}
