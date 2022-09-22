/*
 * @file Defines.h
 *
 * @brief Here are the defines, that cannot be assigned to a specific translation unit or if there must be global
 * visible.
 *
 * @date22.09.2022
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



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DEFINES_H */
