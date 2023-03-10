/**
 * @file md5.h
 *
 * @brief Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm
 * and modified slightly to be functionally identical but condensed into control structures.
 */

#ifndef MD5_H
#define MD5_H ///< Include guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    uint64_t size;        // Size of input in bytes
    uint32_t buffer[4];   // Current accumulation of hash
    uint8_t input[64];    // Input to be used in the next step
    uint8_t digest[16];   // Result of algorithm
}MD5Context;

/**
 * @brief Initialize a context
 */
void md5Init(MD5Context* const ctx);

/**
 * @brief Add some amount of input to the context
 *
 * If the input fills out a block of 512 bits, apply the algorithm (md5Step)
 * and save the result in the buffer. Also updates the overall size.
 */
void md5Update(MD5Context* const ctx, const uint8_t* const input_buffer, const size_t input_len);

/**
 * @brief Pad the current input to get to 448 bytes, append the size in bits to the very end,
 * and save the result of the final iteration into digest.
 */
void md5Finalize(MD5Context* const ctx);

/**
 * @brief Step on 512 bits of input with the main MD5 algorithm.
 */
void md5Step(uint32_t* const buffer, const uint32_t* const input);

/**
 * @brief Functions that will return a pointer to the hash of the provided input.
 */
uint8_t* md5String(char *input);

/**
 * @brief Functions that will return a pointer to the hash of the provided input file.
 */
uint8_t* md5File(FILE *file);

/**
 * @brief Rotates a 32-bit word left by n bits.
 */
uint32_t rotateLeft(const uint32_t x, const uint32_t n);

/**
 * @brief Compare the MD5 sum of the file with the expected one.
 *
 * @param file_name File name
 * @param expected_md5_sum Expected MD5 sum of the file
 * @param err_occurred Occurred an error in the function ?
 * @return true, if the MD5 sum is equal, else false
 */
extern _Bool Check_Test_File_MD5_Sum
(
        const char* const restrict file_name,
        const char* const restrict expected_md5_sum,
        _Bool* const restrict err_occurred
);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MD5_H */
