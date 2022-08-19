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
 *      -- Token_List_Container is a container for token lists
 *      -- Every token list represents the tokens of one tokens array in the source JSON file
 *
 * - Create a token int mapping list                                        (One Token_Int_Mapping)
 *      -- The token int mapping give every token a unique integer value (This idea was chosen to speed up the
 *         intersection process, because integer comparisons are significant faster than string comparisons, especially
 *         with long strings)
 *      -- Also every token will be unique in the mapping object
 *      -- To avoid searching in the full data, a pseudo hash function was implemented. There are 100 data buckets
 *      -- The "hash sum" of a string is the sum of all char mod 100
 *      -- The integer values have the chosen bucket encoded in it itself
 *      -- the first two lowest digits (in decimal system) encodes the bucket. E.g.: xxx10 means, that this integer can
 *         found in the 11. bucket, when it exists in the mapping data
 *
 * - Use the token int mapping for the creation of a mapped token container (Two Document_Word_List)
 *      -- A Document_Word_List contains a 2 dimensional integer array
 *      -- In this array is the data for the intersection or for the intersection result (Yes Document_Word_List will
 *         be used for the intersection input data and for the intersection result !)
 *      -- Every array in the 2 dimensional array represents the data of one tokens array
 *      -- This means, that the results are focused on the tokens array from the input data
 *      -- This is necessary, because one of the goals of this project is to find the intersections between the tokens
 *         arrays in the input data
 *
 * - Create the intersections and save the information in the output file
 *      -- At the end the intersection between two Document_Word_List objects will be calculated
 *      -- The results will be written in the result file
 *
 *
 *
 * In this function will be NO input value tests, because NaN, +Inf, ... are good possibilities to say the function,
 * that the calculation should not stopped before the it is completed.
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
