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



#ifndef CHAR_OFFSET_TYPE
#define CHAR_OFFSET_TYPE unsigned short int     ///< Char offset type
#else
#error "The macro \"CHAR_OFFSET_TYPE\" is already defined !"
#endif /* CHAR_OFFSET_TYPE */

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



#ifndef SENTENCE_OFFSET_TYPE
#define SENTENCE_OFFSET_TYPE unsigned char      ///< Type for the sentence offsets
#else
#error "The macro \"SENTENCE_OFFSET_TYPE\" is already defined !"
#endif /* SENTENCE_OFFSET_TYPE */

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



#ifndef WORD_OFFSET_TYPE
#define WORD_OFFSET_TYPE unsigned char      ///< Type for the word offsets
#else
#error "The macro \"WORD_OFFSET_TYPE\" is already defined !"
#endif /* WORD_OFFSET_TYPE */

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
    #define WORD_OFFSET_TYPE_FSTR_SPECIFIER "cu" ///< Format specifier for the sentence offsets
    #else
    #error "The macro \"WORD_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
    #endif /* WORD_OFFSET_TYPE_FSTR_SPECIFIER */

    #ifndef WORD_OFFSET_TYPE_MAX
    #define WORD_OFFSET_TYPE_MAX UCHAR_MAX      ///< Max value for the sentence offset type
    #else
    #error "The macro \"WORD_OFFSET_TYPE_MAX\" is already defined !"
    #endif /* WORD_OFFSET_TYPE_MAX */
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */



#ifndef INT_MAPPING_TYPE
#define INT_MAPPING_TYPE unsigned int           ///< Int mapping type
#else
#error "The macro \"INT_MAPPING_TYPE\" is already defined !"
#endif /* INT_MAPPING_TYPE */

#ifndef INT_MAPPING_TYPE_FSTR_SPECIFIER
#define INT_MAPPING_TYPE_FSTR_SPECIFIER "u"     ///< Specifier for the int mapping type
#else
#error "The macro \"INT_MAPPING_TYPE_FSTR_SPECIFIER\" is already defined !"
#endif /* INT_MAPPING_TYPE_FSTR_SPECIFIER */

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
#define VERSION 0.1
#else
#error "The macro \"VERSION\" is already defined !"
#endif /* VERSION */

#ifndef VERSION_STR     ///< Current program version as c string.
#define VERSION_STR "0.1"
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



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEFINES_H */
