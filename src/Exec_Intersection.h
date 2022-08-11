/**
 * @file Exec_Intersection.c
 *
 * @date 11.08.2022
 * @author am4
 */

#ifndef EXEC_INTERSECTION
#define EXEC_INTERSECTION ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Execute the intersection process.
 *
 * Execution steps:
 * - Read files and extract the tokens                                      (Two Token_List_Container)
 * - Create a token int mapping list                                        (One Token_Int_Mapping)
 * - Use the token int mapping for the creation of a mapped token container (Two Document_Word_List)
 * - Create the intersections and save the information in the output file
 *
 * There will be NO input value tests, because NaN, +Inf, ... are good possibilities to say the function, that the
 * calculation should not stopped before the calculation is completed.
 *
 * Every invalid value will be interpreted in this way!
 *
 * Asserts:
 *      N/A
 *
 * @param[in] abort_progress_percent After this progress percent value the process will be stopped
 *
 * @return Status value (0: Success; != 0 Error)
 */
extern int
Exec_Intersection
(
        const float abort_progress_percent
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EXEC_INTERSECTION */
