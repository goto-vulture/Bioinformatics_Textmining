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

/**
 * @brief Compare the MD5 sum of the file with the expected one.
 *
 * @param file_name File name
 * @param expected_md5_sum Expected MD5 sum of the file
 * @return true, if the MD5 sum is equal, else false
 */
static _Bool Check_Test_File_MD5_Sum
(
        const char* const restrict file_name,
        const char* const restrict expected_md5_sum
);

/**
 * @brief Print a MD5 sum in hexadecimal notation.
 *
 * @param md5_hash MD5 sum
 */
static void Print_Hash
(
        const uint8_t* const md5_hash
);

/**
 * @brief Convert two char, that represents a byte, to a binary result.
 *
 * Got code from here:
 * @link https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
 *
 * @param char_1 First char of the byte representation as string
 * @param char_2 Second char of the byte representation as string
 * @return Binary result
 */
static uint8_t Hex_Char_To_Byte
(
        const char char_1,
        const char char_2
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the number of token arrays in the test file.
 */
extern void TEST_Number_Of_Tokenarrays (void)
{
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);
    ASSERT_EQUALS(md5_sum_check_result, true);

    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (TEST_FILE_READER_TEST_FILE);

    ASSERT_EQUALS(true, true);
    TokenListContainer_DeleteObject(token_container_input_1);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Check the max length of all data set IDs in the test file.
 */
extern void TEST_Max_Dataset_ID_Length (void)
{
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);
    ASSERT_EQUALS(md5_sum_check_result, true);

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
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);
    ASSERT_EQUALS(md5_sum_check_result, true);

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
    const _Bool md5_sum_check_result = Check_Test_File_MD5_Sum(TEST_FILE_READER_TEST_FILE, TEST_FILE_READER_TEST_FILE_MD5);
    ASSERT_EQUALS(md5_sum_check_result, true);

    struct Token_List_Container* token_container_input_1 = TokenListContainer_CreateObject (TEST_FILE_READER_TEST_FILE);

    ASSERT_EQUALS(true, true);
    TokenListContainer_DeleteObject(token_container_input_1);

    return;
}

//=====================================================================================================================

/**
 * @brief Compare the MD5 sum of the file with the expected one.
 *
 * @param file_name File name
 * @param expected_md5_sum Expected MD5 sum of the file
 * @return true, if the MD5 sum is equal, else false
 */
static _Bool Check_Test_File_MD5_Sum
(
        const char* const restrict file_name,
        const char* const restrict expected_md5_sum
)
{
    FILE* test_file = fopen(file_name, "r");
    ASSERT_FMSG(test_file != NULL, "Cannot open the test file \"%s\" !", file_name);

    uint8_t* created_md5 = md5File(test_file);
    ASSERT_FMSG(created_md5 != NULL, "Cannot create the MD5 sum of the test file \"%s\" !", file_name);

    FCLOSE_AND_SET_TO_NULL(test_file);

    // Convert expected md5 sum (char array) to uint8_t array
    uint8_t expected_md5_sum_hex [MD5_SUM_LENGTH];
    memset (expected_md5_sum_hex, '\0', sizeof(expected_md5_sum_hex));
    uint8_t current_byte = 0;

    for (int i = 0; i < MD5_SUM_LENGTH * 2; i += 2) // MD5_SUM_LENGTH * 2, because every byte are encoded with 2 char
    {
        expected_md5_sum_hex [current_byte] = Hex_Char_To_Byte (expected_md5_sum [i], expected_md5_sum [i + 1]);
        ++ current_byte;
    }

    // Compare MD5 sums
    _Bool result = true;
    for (int i = 0; i < MD5_SUM_LENGTH; ++ i)
    {
        if (expected_md5_sum_hex [i] != created_md5 [i])
        {
            result = false;
            break;
        }
    }

    if (! result)
    {
        printf("Expected result: ");
        Print_Hash(expected_md5_sum_hex);
        printf("\nGot:             ");
        Print_Hash(created_md5);
        puts("");
    }
    else
    {
        printf("Checked MD5 sum of the file \"%s\": ", TEST_FILE_READER_TEST_FILE);
        Print_Hash(expected_md5_sum_hex);
        puts("");
    }

    FREE_AND_SET_TO_NULL(created_md5);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Print a MD5 sum in hexadecimal notation.
 *
 * @param md5_hash MD5 sum
 */
static void Print_Hash
(
        const uint8_t* const md5_hash
)
{
    for (unsigned char i = 0; i < MD5_SUM_LENGTH; ++ i)
    {
        printf("%02x", md5_hash [i]);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Convert two char, that represents a byte, to a binary result.
 *
 * Got code from here:
 * @link https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
 *
 * @param char_1 First char of the byte representation as string
 * @param char_2 Second char of the byte representation as string
 * @return Binary result
 */
static uint8_t Hex_Char_To_Byte
(
        const char char_1,
        const char char_2
)
{
    uint8_t result = 0;

    const char input_char [2] = { char_1, char_2 };

    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(input_char); i ++)
    {
        // get current character then increment
        uint8_t byte = (uint8_t) tolower(input_char [i]);
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9')
        {
            byte = (uint8_t) (byte - '0');
        }
        else if (byte >= 'a' && byte <='f')
        {
            byte = (uint8_t) (byte - 'a' + 10);
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        result = (uint8_t) ((result << 4) | (byte & 0xF));
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
