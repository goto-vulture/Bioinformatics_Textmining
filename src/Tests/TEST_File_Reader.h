/**
 * @file TEST_File_Reader.c
 *
 * @brief Here are tests for the File_Reader translation unit.
 *
 * @date 18.10.2022
 * @author: am4
 */

#ifndef TEST_FILE_READER_H
#define TEST_FILE_READER_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Check the number of token arrays in the test file.
 */
extern void TEST_Number_Of_Tokenarrays (void);

/**
 * @brief Check the max length of all data set IDs in the test file.
 */
extern void TEST_Max_Dataset_ID_Length (void);

/**
 * @brief Check the max length of all token arrays in the test file.
 */
extern void TEST_Max_Tokenarray_Length (void);

/**
 * @brief Check the length of the first 25 token arrays.
 */
extern void TEST_Length_Of_The_First_25_Tokenarrays (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_FILE_READER_H */
