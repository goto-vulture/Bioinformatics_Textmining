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
 * @return Status value (0: Success; != 0 Error)
 */
extern int
Exec_Intersection
(
        void
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EXEC_INTERSECTION */
