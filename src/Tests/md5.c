/**
 * @file md5.c
 *
 * @brief Derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm
 * and modified slightly to be functionally identical but condensed into control structures.
 */

#include "md5.h"
#include <ctype.h>

/*
 * Constants defined by the MD5 algorithm
 */
#ifndef A
#define A 0x67452301
#else
#error "The macro \"A\" is already defined !"
#endif /* A */
#ifndef B

#define B 0xefcdab89
#else
#error "The macro \"B\" is already defined !"
#endif /* B */

#ifndef C
#define C 0x98badcfe
#else
#error "The macro \"C\" is already defined !"
#endif /* C */

#ifndef D
#define D 0x10325476
#else
#error "The macro \"D\" is already defined !"
#endif /* D */

static const uint32_t S[] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                              5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                              4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                              6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

static const uint32_t K[] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                              0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                              0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                              0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                              0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                              0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                              0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                              0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                              0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                              0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                              0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                              0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                              0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                              0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                              0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                              0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

/*
 * Bit-manipulation functions defined by the MD5 algorithm
 */
#ifndef F
#define F(X, Y, Z) ((X & Y) | (~X & Z))
#else
#error "The macro \"F\" is already defined !"
#endif /* F */

#ifndef G
#define G(X, Y, Z) ((X & Z) | (Y & ~Z))
#else
#error "The macro \"G\" is already defined !"
#endif /* G */

#ifndef H
#define H(X, Y, Z) (X ^ Y ^ Z)
#else
#error "The macro \"H\" is already defined !"
#endif /* H */

#ifndef I
#define I(X, Y, Z) (Y ^ (X | ~Z))
#else
#error "The macro \"I\" is already defined !"
#endif /* I */

/*
 * Padding used to make the size (in bits) of the input congruent to 448 mod 512
 */
static const uint8_t PADDING[] = { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
 * @brief Initialize a context
 */
void md5Init(MD5Context* const ctx){
    ctx->size = (uint64_t)0;

    ctx->buffer[0] = (uint32_t)A;
    ctx->buffer[1] = (uint32_t)B;
    ctx->buffer[2] = (uint32_t)C;
    ctx->buffer[3] = (uint32_t)D;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Add some amount of input to the context
 *
 * If the input fills out a block of 512 bits, apply the algorithm (md5Step)
 * and save the result in the buffer. Also updates the overall size.
 */
void md5Update(MD5Context* const ctx, const uint8_t* const input_buffer, const size_t input_len){
    uint32_t input[16];
    unsigned int offset = ctx->size % 64;
    ctx->size += (uint64_t)input_len;

    // Copy each byte in input_buffer into the next space in our context input
    for(size_t i = 0; i < input_len; ++i){
        ctx->input[offset++] = (uint8_t)*(input_buffer + i);

        // If we've filled our context input, copy it into our local array input
        // then reset the offset to 0 and fill in a new buffer.
        // Every time we fill out a chunk, we run it through the algorithm
        // to enable some back and forth between cpu and i/o
        if(offset % 64 == 0){
            for(unsigned int j = 0; j < 16; ++j){
                // Convert to little-endian
                // The local variable `input` our 512-bit chunk separated into 32-bit words
                // we can use in calculations
                input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
                           (uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
                           (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
                           (uint32_t)(ctx->input[(j * 4)]);
            }
            md5Step(ctx->buffer, input);
            offset = 0;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Pad the current input to get to 448 bytes, append the size in bits to the very end,
 * and save the result of the final iteration into digest.
 */
void md5Finalize(MD5Context* const ctx){
    uint32_t input[16];
    unsigned int offset = ctx->size % 64;
    unsigned int padding_length = offset < 56 ? 56 - offset : (56 + 64) - offset;

    // Fill in the padding andndo the changes to size that resulted from the update
    md5Update(ctx, PADDING, padding_length);
    ctx->size -= (uint64_t)padding_length;

    // Do a final update (internal to this function)
    // Last two 32-bit words are the two halves of the size (converted from bytes to bits)
    for(unsigned int j = 0; j < 14; ++j){
        input[j] = (uint32_t)(ctx->input[(j * 4) + 3]) << 24 |
                   (uint32_t)(ctx->input[(j * 4) + 2]) << 16 |
                   (uint32_t)(ctx->input[(j * 4) + 1]) <<  8 |
                   (uint32_t)(ctx->input[(j * 4)]);
    }
    input[14] = (uint32_t)(ctx->size * 8);
    input[15] = (uint32_t)((ctx->size * 8) >> 32);

    md5Step(ctx->buffer, input);

    // Move the result into digest (convert from little-endian)
    for(unsigned int i = 0; i < 4; ++i){
        ctx->digest[(i * 4) + 0] = (uint8_t)((ctx->buffer[i] & 0x000000FF));
        ctx->digest[(i * 4) + 1] = (uint8_t)((ctx->buffer[i] & 0x0000FF00) >>  8);
        ctx->digest[(i * 4) + 2] = (uint8_t)((ctx->buffer[i] & 0x00FF0000) >> 16);
        ctx->digest[(i * 4) + 3] = (uint8_t)((ctx->buffer[i] & 0xFF000000) >> 24);
    }
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Step on 512 bits of input with the main MD5 algorithm.
 */
void md5Step(uint32_t* const buffer, const uint32_t* const input){
    uint32_t AA = buffer[0];
    uint32_t BB = buffer[1];
    uint32_t CC = buffer[2];
    uint32_t DD = buffer[3];

    uint32_t E;

    unsigned int j;

    for(unsigned int i = 0; i < 64; ++i){
        switch(i / 16){
            case 0:
                E = F(BB, CC, DD);
                j = i;
                break;
            case 1:
                E = G(BB, CC, DD);
                j = ((i * 5) + 1) % 16;
                break;
            case 2:
                E = H(BB, CC, DD);
                j = ((i * 3) + 5) % 16;
                break;
            default:
                E = I(BB, CC, DD);
                j = (i * 7) % 16;
                break;
        }

        uint32_t temp = DD;
        DD = CC;
        CC = BB;
        BB = BB + rotateLeft(AA + E + K[i] + input[j], S[i]);
        AA = temp;
    }

    buffer[0] += AA;
    buffer[1] += BB;
    buffer[2] += CC;
    buffer[3] += DD;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Functions that will return a pointer to the hash of the provided input.
 */
uint8_t* md5String(char *input){
    MD5Context ctx;
    md5Init(&ctx);
    md5Update(&ctx, (uint8_t *)input, strlen(input));
    md5Finalize(&ctx);

    uint8_t *result = malloc(16);
    if (result == NULL) { return NULL; }
    memcpy(result, ctx.digest, 16);
    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Functions that will return a pointer to the hash of the provided input file.
 */
uint8_t* md5File(FILE *file){
    char *input_buffer = malloc(1024);
    if (input_buffer == NULL) { return NULL; }
    size_t input_size = 0;

    MD5Context ctx;
    md5Init(&ctx);

    while((input_size = fread(input_buffer, 1, 1024, file)) > 0){
        md5Update(&ctx, (uint8_t *)input_buffer, input_size);
    }

    md5Finalize(&ctx);

    free(input_buffer);

    uint8_t *result = malloc(16);
    if (result == NULL) { return NULL; }
    memcpy(result, ctx.digest, 16);
    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Rotates a 32-bit word left by n bits.
 */
uint32_t rotateLeft(const uint32_t x, const uint32_t n){
    return (x << n) | (x >> (32 - n));
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    FILE* test_file = fopen(file_name, "r");
    if (test_file == NULL && err_occurred != NULL)
    {
        *err_occurred = true;
        return false;
    }

    uint8_t* created_md5 = md5File(test_file);
    if (created_md5 == NULL && err_occurred != NULL)
    {
        *err_occurred = true;
        return false;
    }

    fclose(test_file);
    test_file = NULL;

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
        printf("Checked MD5 sum of the file \"%s\": ", file_name);
        Print_Hash(expected_md5_sum_hex);
        puts("");
    }

    // ! NO "FREE_AND_SET_TO_NULL" because this memory block was created in the MD5 lib with a simple malloc() call !
    free (created_md5);
    created_md5 = NULL;
    //FREE_AND_SET_TO_NULL(created_md5);

    if (err_occurred != NULL)
    {
        *err_occurred = false;
    }
    return result;
}

//=====================================================================================================================

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

    for (size_t i = 0; i < (sizeof (input_char) / sizeof (input_char [0])); i ++)
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



#ifdef A
#undef A
#endif /* A */

#ifdef B
#undef B
#endif /* B */

#ifdef C
#undef C
#endif /* C */

#ifdef D
#undef D
#endif /* D */

#ifdef F
#undef F
#endif /* F */

#ifdef G
#undef G
#endif /* G */

#ifdef H
#undef H
#endif /* H */

#ifdef I
#undef I
#endif /* I */
