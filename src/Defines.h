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



#ifndef CHAR_OFFSET_TYPE
#define CHAR_OFFSET_TYPE unsigned short int     ///< Char offset type
#else
#error "The macro \"CHAR_OFFSET_TYPE\" is already defined !"
#endif /* CHAR_OFFSET_TYPE */

#ifndef CHAR_OFFSET_TYPE_FSTR_SPECIFIER
#define CHAR_OFFSET_TYPE_FSTR_SPECIFIER "hu"    ///< Specifier for the char offset type
#else
#error "The macro \"CHAR_OFFSET_TYPE_FSTR_SPECIFIER\" is already defined !"
#endif /* CHAR_OFFSET_TYPE_FSTR_SPECIFIER */

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



/**
 * @brief Check, whether the macro values are valid.
 */
#if __STDC_VERSION__ >= 201112L
_Static_assert(DATASET_ID_LENGTH > 1, "The marco \"DATASET_ID_LENGTH\" needs to be at lest 2 (one char for the ID and "
        "one for the end symbol ('\0') !");
_Static_assert(MAX_VLA_LENGTH >= 100, "The macro \"MAX_VLA_LENGTH\" needs to be at least 100 !");
#endif /* __STDC_VERSION__ */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEFINES_H */
