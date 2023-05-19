/*
 * @file Defines.h
 *
 * @brief Here are the defines, that cannot be assigned to a specific translation unit or if there must be global
 * visible.
 *
 * @date 22.09.2022
 * @author: am1
 */

#ifndef DEFINES_H
#define DEFINES_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Error_Handling/_Generics.h"



//---------------------------------------------------------------------------------------------------------------------

typedef unsigned int DATA_TYPE;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #ifndef DATA_TYPE_FSTR_SPECIFIER
    #define DATA_TYPE_FSTR_SPECIFIER GET_FORMAT_STR((DATA_TYPE) 42)
    #else
    #error "The macro \"DATA_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* DATA_TYPE_FSTR_SPECIFIER */

    #ifndef DATA_TYPE_MAX
    #define DATA_TYPE_MAX GET_MAX((DATA_TYPE) 42)
    #else
    #error "The macro \"DATA_TYPE_MAX\" is already defined !"
    #endif /* DATA_TYPE_MAX */
#else
#ifndef DATA_TYPE_FSTR_SPECIFIER
    #define DATA_TYPE_FSTR_SPECIFIER "lu"
    #else
    #error "The macro \"DATA_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* DATA_TYPE_FSTR_SPECIFIER */

    #ifndef DATA_TYPE_MAX
    #define DATA_TYPE_MAX UINT_MAX
    #else
    #error "The macro \"DATA_TYPE_MAX\" is already defined !"
    #endif /* DATA_TYPE_MAX */
#endif /* #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

typedef unsigned short int CHAR_OFFSET_TYPE;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    /**
     * @brief Get suitable format specifier with a _Generic expression.
     */
    #ifndef CHAR_OFFSET_TYPE_FSTR_SPECIFIER
    #define CHAR_OFFSET_TYPE_FSTR_SPECIFIER GET_FORMAT_STR((CHAR_OFFSET_TYPE) 42)
    #else
    #error "The macro \"CHAR_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* CHAR_OFFSET_TYPE_FSTR_SPECIFIER */

    /**
     * @brief Max value for the char offset type. (Get the value with a _Generic expression)
     */
    #ifndef CHAR_OFFSET_TYPE_MAX
    #define CHAR_OFFSET_TYPE_MAX GET_MAX((CHAR_OFFSET_TYPE) 42)
    #else
    #error "The macro \"CHAR_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* CHAR_OFFSET_TYPE_MAX */
#else
    #ifndef CHAR_OFFSET_TYPE_FSTR_SPECIFIER
    #define CHAR_OFFSET_TYPE_FSTR_SPECIFIER "hu"    ///< Specifier for the char offset type
    #else
    #error "The macro \"CHAR_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* CHAR_OFFSET_TYPE_FSTR_SPECIFIER */

    #ifndef CHAR_OFFSET_TYPE_MAX
    #define CHAR_OFFSET_TYPE_MAX USHRT_MAX          ///< Max value for the char offset type
    #else
    #error "The macro \"CHAR_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* CHAR_OFFSET_TYPE_MAX */
#endif /* #defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

typedef unsigned char SENTENCE_OFFSET_TYPE;

// If C11 available -> auto determining the printf specifier and max value
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    /**
     * @brief Get suitable format specifier with a _Generic expression.
     */
    #ifndef SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER
    #define SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER GET_FORMAT_STR((SENTENCE_OFFSET_TYPE) 42)
    #else
    #error "The macro \"SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER */

    /**
     * @brief Max value for the sentence offset type. (Get the value with a _Generic expression)
     */
    #ifndef SENTENCE_OFFSET_TYPE_MAX
    #define SENTENCE_OFFSET_TYPE_MAX GET_MAX((SENTENCE_OFFSET_TYPE) 42)
    #else
    #error "The macro \"SENTENCE_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* SENTENCE_OFFSET_TYPE_MAX */
#else
    #ifndef SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER
    #define SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER "cu"///< Type for the sentence offsets
    #else
    #error "The macro \"SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* SENTENCE_OFFSET_TYPE_FSTR_SPECIFIER */

    #ifndef SENTENCE_OFFSET_TYPE_MAX
    #define SENTENCE_OFFSET_TYPE_MAX UCHAR_MAX      ///< Max value for the sentence offset type
    #else
    #error "The macro \"SENTENCE_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* SENTENCE_OFFSET_TYPE_MAX */
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

typedef unsigned short int WORD_OFFSET_TYPE;

// If C11 available -> auto determining the printf specifier and max value
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    /**
     * @brief Get suitable format specifier with a _Generic expression.
     */
    #ifndef WORD_OFFSET_TYPE_FSTR_SPECIFIER
    #define WORD_OFFSET_TYPE_FSTR_SPECIFIER GET_FORMAT_STR((WORD_OFFSET_TYPE) 42)
    #else
    #error "The macro \"WORD_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* WORD_OFFSET_TYPE_FSTR_SPECIFIER */

    /**
     * @brief Max value for the word offset type. (Get the value with a _Generic expression)
     */
    #ifndef WORD_OFFSET_TYPE_MAX
    #define WORD_OFFSET_TYPE_MAX GET_MAX((WORD_OFFSET_TYPE) 42)
    #else
    #error "The macro \"WORD_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* WORD_OFFSET_TYPE_MAX */
#else
    #ifndef WORD_OFFSET_TYPE_FSTR_SPECIFIER
    #define WORD_OFFSET_TYPE_FSTR_SPECIFIER "hu" ///< Format specifier for the sentence offsets
    #else
    #error "The macro \"WORD_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* WORD_OFFSET_TYPE_FSTR_SPECIFIER */

    #ifndef WORD_OFFSET_TYPE_MAX
    #define WORD_OFFSET_TYPE_MAX USHRT_MAX      ///< Max value for the sentence offset type
    #else
    #error "The macro \"WORD_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* WORD_OFFSET_TYPE_MAX */
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

typedef unsigned int INT_MAPPING_TYPE;

// If C11 available -> auto determining the printf specifier
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    /**
     * @brief Get suitable format specifier with a _Generic expression.
     */
    #ifndef INT_MAPPING_TYPE_FSTR_SPECIFIER
    #define INT_MAPPING_TYPE_FSTR_SPECIFIER GET_FORMAT_STR((INT_MAPPING_TYPE) 42)
    #else
    #error "The macro \"INT_MAPPING_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* INT_MAPPING_TYPE_FSTR_SPECIFIER */

    /**
     * @brief Max value for the int mapping type. (Get the value with a _Generic expression)
     */
    #ifndef INT_MAPPING_TYPE_MAX
    #define INT_MAPPING_TYPE_MAX GET_MAX((INT_MAPPING_TYPE) 42)
    #else
    #error "The macro \"INT_MAPPING_TYPE_MAX\" is already defined !"
    #endif /* INT_MAPPING_TYPE_MAX */
#else
    #ifndef INT_MAPPING_TYPE_FSTR_SPECIFIER
    #define INT_MAPPING_TYPE_FSTR_SPECIFIER "u"     ///< Specifier for the int mapping type
    #else
    #error "The macro \"INT_MAPPING_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* INT_MAPPING_TYPE_FSTR_SPECIFIER */

    #ifndef INT_MAPPING_TYPE_MAX
    #define INT_MAPPING_TYPE_MAX UINT_MAX
    #else
    #error "The macro \"INT_MAPPING_TYPE_MAX\" is already defined !"
    #endif /* INT_MAPPING_TYPE_MAX */
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

#ifndef DATASET_ID_LENGTH
#define DATASET_ID_LENGTH 16                    ///< Length of a data set ID.
#else
#error "The macro \"DATASET_ID_LENGTH\" is already defined !"
#endif /* DATASET_ID_LENGTH */

#ifndef __STDC_NO_VLA__
    #ifndef MAX_VLA_LENGTH
    #define MAX_VLA_LENGTH (size_t) (4096)      ///< Max length of a VLA (variable length array)
    #else
    #error "The macro \"MAX_VLA_LENGTH\" is already defined !"
    #endif /* MAX_VLA_LENGTH */
#endif /* __STDC_NO_VLA__ */

/**
 * @brief VLA arrays will be created (in normal cases) on the stack. This memory is limited, therefore the macro
 * MAX_VLA_LENGTH exists. But switching between VLAs and malloc/calloc memory is inefficient, so with this macro it is
 * possible to disable size checks for the VLA usage.
 * @see MAX_VLA_LENGTH
 */
#ifndef UNSAFE_VLA_USAGE
#define UNSAFE_VLA_USAGE
#else
#error "The macro \"UNSAFE_VLA_USAGE\" is already defined !"
#endif /* UNSAFE_VLA_USAGE */

#ifndef VERSION         ///< Current program version.
#define VERSION 0.6
#else
#error "The macro \"VERSION\" is already defined !"
#endif /* VERSION */

#ifndef VERSION_STR     ///< Current program version as c string.
#define VERSION_STR "0.6"
#else
#error "The macro \"VERSION_STR\" is already defined !"
#endif /* VERSION_STR */



/**
 * @brief Check, whether the macro values are valid.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(DATASET_ID_LENGTH > 1, "The marco \"DATASET_ID_LENGTH\" needs to be at lest 2 (one char for the ID and "
        "one for the end symbol ('\0') !");
_Static_assert(MAX_VLA_LENGTH >= 100, "The macro \"MAX_VLA_LENGTH\" needs to be at least 100 !");
_Static_assert(sizeof(CHAR_OFFSET_TYPE_FSTR_SPECIFIER) > 0 + 1, "The macro \"CHAR_OFFSET_TYPE_FSTR_SPECIFIER\" needs "
        "at least one char (plus '\0') !");
_Static_assert(sizeof(INT_MAPPING_TYPE_FSTR_SPECIFIER) > 0 + 1, "The macro \"INT_MAPPING_TYPE_FSTR_SPECIFIER\" needs "
        "at least one char (plus '\0') !");
_Static_assert(sizeof(VERSION_STR) > 0 + 1, "The macro \"VERSION_STR\" needs at least one char (plus '\0') !");

IS_CONST_STR(VERSION_STR)
IS_TYPE(VERSION, double) // float is not the right type for the expected type !
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

#ifndef SIMD_VALUE_SIZE_BIT
#define SIMD_VALUE_SIZE_BIT 32      ///< Width of a single value
#else
#error "The macro \"SIMD_VALUE_SIZE_BIT\" is already defined !"
#endif /* SIMD_VALUE_SIZE_BIT */

#ifndef MMX_REG_WIDTH_BIT
#define MMX_REG_WIDTH_BIT 64        ///< Width of a MMX register
#else
#error "The macro \"MMX_REG_WIDTH_BIT\" is already defined !"
#endif /* MMX_REG_WIDTH_BIT */

#ifndef SSE_REG_WIDTH_BIT
#define SSE_REG_WIDTH_BIT 128       ///< Width of a SSE register
#else
#error "The macro \"SSE_REG_WIDTH_BIT\" is already defined !"
#endif /* SSE_REG_WIDTH_BIT */

#ifndef AVX_REG_WIDTH_BIT
#define AVX_REG_WIDTH_BIT 256       ///< Width of a AVX register
#else
#error "The macro \"AVX_REG_WIDTH_BIT\" is already defined !"
#endif /* AVX_REG_WIDTH_BIT */

#ifndef AVX512_REG_WIDTH_BIT
#define AVX512_REG_WIDTH_BIT 512    ///< Width of a AVX register
#else
#error "The macro \"AVX512_REG_WIDTH_BIT\" is already defined !"
#endif /* AVX512_REG_WIDTH_BIT */



/**
 * @brief Check, whether the macro values are valid.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(SIMD_VALUE_SIZE_BIT == 32,   "The macro \"SIMD_VALUE_SIZE_BIT\" needs to be exact 32 !");
_Static_assert(MMX_REG_WIDTH_BIT == 64,     "The macro \"MMX_REG_WIDTH_BIT\" needs to be exact 64 !");
_Static_assert(SSE_REG_WIDTH_BIT == 128,    "The macro \"SSE_REG_WIDTH_BIT\" needs to be exact 128 !");
_Static_assert(AVX_REG_WIDTH_BIT == 256,    "The macro \"AVX_REG_WIDTH_BIT\" needs to be exact 256 !");
_Static_assert(AVX512_REG_WIDTH_BIT == 512, "The macro \"AVX512_REG_WIDTH_BIT\" needs to be exact 512 !");

IS_TYPE(SIMD_VALUE_SIZE_BIT, int)
IS_TYPE(MMX_REG_WIDTH_BIT, int)
IS_TYPE(SSE_REG_WIDTH_BIT, int)
IS_TYPE(AVX_REG_WIDTH_BIT, int)
IS_TYPE(AVX512_REG_WIDTH_BIT, int)
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */

//---------------------------------------------------------------------------------------------------------------------

/**
 * Macros to manually disable CPU extensions.
 *
 * Comment in the macros, if you want to disable this extension.
 */
#ifndef NO_AVX2
//#define NO_AVX2
#else
#error "The macro \"NO_AVX2\" is already defined !"
#endif /* NO_AVX2 */

#ifndef NO_AVX
//#define NO_AVX
#else
#error "The macro \"NO_AVX\" is already defined !"
#endif /* NO_AVX */

#ifndef NO_SSE4_1
//#define NO_SSE4_1
#else
#error "The macro \"NO_SSE4_1\" is already defined !"
#endif /* NO_SSE4_1 */

#ifndef NO_SSE2
//#define NO_SSE2
#else
#error "The macro \"NO_SSE2\" is already defined !"
#endif /* NO_SSE2 */

#ifndef NO_MMX
//#define NO_MMX
#else
#error "The macro \"NO_MMX\" is already defined !"
#endif /* NO_MMX */

/**
 * @brief Disable ALL CPU extensions.
 */
#ifndef NO_CPU_EXTENSIONS
//#define NO_CPU_EXTENSIONS
#else
#error "The macro \"NO_CPU_EXTENSIONS\" is already defined !"
#endif /* NO_CPU_EXTENSIONS */

// On a 32 bit system any CPU extension is not usable
#if defined(__GNUC__)
#if defined(i386) || defined(__i386) || defined(__i386__) && (! defined(__x86_64__) && ! defined(_M_X64))
#define NO_CPU_EXTENSIONS
/**
 * @brief Summary macro, that the system is a 32 bit x86 system
 */
#ifndef I386
#define I386
#else
#error "The macro \"I386\" is already defined !"
#endif /* I386 */
#endif /* defined(i386) || defined(__i386) || defined(__i386__) && (! defined(__x86_64__) && ! defined(_M_X64)) */
#endif /* defined(__GNUC__) */

//---------------------------------------------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEFINES_H */
