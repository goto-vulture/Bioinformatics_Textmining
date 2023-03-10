/**
 * @file TEST_Two_Dim_C_String_Array.h
 *
 * @date 16.02.2023
 * @author: leer
 */

#ifndef TEST_TWO_DIM_C_STRING_ARRAY_H
#define TEST_TWO_DIM_C_STRING_ARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "../Two_Dim_C_String_Array.h"



/**
 * @brief Test Appending a new string.
 */
extern void TEST_AppendNewString (void);

/**
 * @brief Test Appending a string to the newest string.
 */
extern void TEST_AppendDataToNewestString (void);

/**
 * @brief Test Appending a string to a specific string.
 */
extern void TEST_AppendDataToSpecificString (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_TWO_DIM_C_STRING_ARRAY_H */
