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

/**
 * @brief Check the functionality of the most important ANSI esc sequences.
 *
 * This test cannot fail, because it is not possible for the program to check whether the output on the terminal has
 * the expected view.
 */
extern void TEST_ANSI_Esc_Seq (void);

/**
 * @brief Test, whether the Tokenize_String function do the tokenisation calculation properly.
 */
extern void TEST_Tokenize_String (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_ETC_H */
