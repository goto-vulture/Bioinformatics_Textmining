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

void md5Init(MD5Context *ctx);
void md5Update(MD5Context *ctx, uint8_t *input, size_t input_len);
void md5Finalize(MD5Context *ctx);
void md5Step(uint32_t *buffer, uint32_t *input);

uint8_t* md5String(char *input);
uint8_t* md5File(FILE *file);

uint32_t F(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t G(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t H(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t I(uint32_t X, uint32_t Y, uint32_t Z);

uint32_t rotateLeft(uint32_t x, uint32_t n);

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
