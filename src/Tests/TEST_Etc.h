/**
 * @file TEST_etc.h
 *
 * @brief Here are TEST functions, that cannot be placed in a translation unit for specific struct objects.
 *
 * @date 01.11.2022
 * @author: leer
 */

#ifndef TEST_ETC_H
#define TEST_ETC_H ///< Include guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Check the number of free calls at the end of a calculation.
 *
 * If the dynamic memory management works, than there are exact the same number of free calls compared with the sum of
 * malloc and calloc calls.
 */
extern void TEST_Number_Of_Free_Calls (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_ETC_H */
