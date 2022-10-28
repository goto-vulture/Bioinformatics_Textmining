/**
 * @file TEST_File_Reader.c
 *
 * @brief Here are tests for the File_Reader translation unit.
 *
 * @date 18.10.2022
 * @author: am4
 */

#include "TEST_File_Reader.h"

#include <stdbool.h>
#include <ctype.h>
#include "../Misc.h"
#include "../File_Reader.h"
#include "md5.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"
#include "tinytest.h"



#ifndef TEST_FILE_READER_TEST_FILE
#define TEST_FILE_READER_TEST_FILE "./src/Tests/Test_Data/test_ebm.json" ///< The test file for the file reader checks
#else
#error "The macro \"TEST_FILE_READER_TEST_FILE\" is already defined !"
#endif /* TEST_FILE_READER_TEST_FILE */

/**
 * @brief MD5 sum of the test file for the file reader tests
 */
#ifndef TEST_FILE_READER_TEST_FILE_MD5
#define TEST_FILE_READER_TEST_FILE_MD5 "d1205477fc08c6e278d905edfdd537fb"
#else
#error "The macro \"TEST_FILE_READER_TEST_FILE_MD5\" is already defined !"
#endif /* TEST_FILE_READER_TEST_FILE_MD5 */

#ifndef LENGTH_MD5_SUM
#define MD5_SUM_LENGTH 16 ///< Length (number of bytes) of a MD5 sum
#else
#error "The macro \"LENGTH_MD5_SUM\" is already defined !"
#endif /* LENGTH_MD5_SUM */

#ifndef NUMBER_OF_TOKENARRAYS
#define NUMBER_OF_TOKENARRAYS 191 ///< Expected number of token arrays
#else
#error "The macro \"NUMBER_OF_TOKENARRAYS\" is already defined !"
#endif /* NUMBER_OF_TOKENARRAYS */

#ifndef MAX_DATASET_ID_LENGTH
#define MAX_DATASET_ID_LENGTH 8 ///< Expected max data set length
#else
#error "The macro \"MAX_DATASET_ID_LENGTH\" is already defined !"
#endif /* MAX_DATASET_ID_LENGTH */

#ifndef MAX_TOKENARRAY_LENGTH
#define MAX_TOKENARRAY_LENGTH 566 ///< Expected max token array length
#else
#error "The macro \"MAX_TOKENARRAY_LENGTH\" is already defined !"
#endif /* MAX_TOKENARRAY_LENGTH */

// #define checks only works with C11 and higher
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(MD5_SUM_LENGTH == 16, "The macro \"MD5_SUM_LENGTH\" needs to be exact 16 !");
_Static_assert(NUMBER_OF_TOKENARRAYS > 0, "The macro \"NUMBER_OF_TOKENARRAYS\" needs to be at least one !");
_Static_assert(MAX_DATASET_ID_LENGTH > 0, "The macro \"MAX_DATASET_ID_LENGTH\" needs to be at least one !");
_Static_assert(MAX_TOKENARRAY_LENGTH > 0, "The macro \"MAX_TOKENARRAY_LENGTH\" need to be at least one !");
_Static_assert(sizeof(TEST_FILE_READER_TEST_FILE) > 0 + 1, "The macro \"TEST_FILE_READER_TEST_FILE\" is empty !");
_Static_assert(sizeof(TEST_FILE_READER_TEST_FILE_MD5) == 32 + 1,
        "The macro \"TEST_FILE_READER_TEST_FILE_MD5\" needs to be exact 32 char (+ '\0') !");
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the number of token arrays in the test file.
 */
extern void TEST_Number_Of_Tokenarrays (void)
{
    _Bool err_occurred = true;
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5,
            &err_occurred);
    ASSERT_MSG(err_occurred == false, "Error occurred while checking a MD5 sum of a file !");
    ASSERT_FMSG(md5_sum_check_result == true, "MD5 sum of the file (%s) is not equal with the expected sum (%s) !",
            TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);

    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (TEST_FILE_READER_TEST_FILE);

    ASSERT_EQUALS(true, true); // Placeholder test
    TokenListContainer_DeleteObject(token_container_input_1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the max length of all data set IDs in the test file.
 */
extern void TEST_Max_Dataset_ID_Length (void)
{
    _Bool err_occurred = true;
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5,
            &err_occurred);
    ASSERT_MSG(err_occurred == false, "Error occurred while checking a MD5 sum of a file !");
    ASSERT_FMSG(md5_sum_check_result == true, "MD5 sum of the file (%s) is not equal with the expected sum (%s) !",
            TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);

    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (TEST_FILE_READER_TEST_FILE);

    ASSERT_EQUALS(TokenListContainer_GetLengthOfLongestDatasetID(token_container_input_1), MAX_DATASET_ID_LENGTH);
    TokenListContainer_DeleteObject(token_container_input_1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the max length of all token arrays in the test file.
 */
extern void TEST_Max_Tokenarray_Length (void)
{
    _Bool err_occurred = true;
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5,
            &err_occurred);
    ASSERT_MSG(err_occurred == false, "Error occurred while checking a MD5 sum of a file !");
    ASSERT_FMSG(md5_sum_check_result == true, "MD5 sum of the file (%s) is not equal with the expected sum (%s) !",
            TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);

    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (TEST_FILE_READER_TEST_FILE);

    ASSERT_EQUALS(TokenListContainer_GetLenghOfLongestTokenList(token_container_input_1), MAX_TOKENARRAY_LENGTH);
    TokenListContainer_DeleteObject(token_container_input_1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the length of the first 25 token arrays.
 */
extern void TEST_Length_Of_The_First_25_Tokenarrays (void)
{
    _Bool err_occurred = true;
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5,
            &err_occurred);
    ASSERT_MSG(err_occurred == false, "Error occurred while checking a MD5 sum of a file !");
    ASSERT_FMSG(md5_sum_check_result == true, "MD5 sum of the file (%s) is not equal with the expected sum (%s) !",
            TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);

    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (TEST_FILE_READER_TEST_FILE);

    ASSERT_EQUALS(true, true); // Placeholder test
    TokenListContainer_DeleteObject(token_container_input_1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
