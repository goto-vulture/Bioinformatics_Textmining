/*
 * @file _Generics.h
 *
 * @brief Here are macros, that uses the C11 _Generic keyword.
 *
 * _Generic give us the possibility to create a macro, that give the type name as string (TYPE_NAME).
 * Second idea: In normal cases macros are not type safe. With tie combination of _Generic and _Static_assert it is
 * possible to make type checks on the translation level.
 *
 * @date 16.10.2022
 * @author: am4
 */

#ifndef _GENERICS_H
#define _GENERICS_H ///< Include Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



// Is it a C11 compiler ? Because _Generic and _Static_assert are keywords that were introduced in the C11 standard.
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#ifndef TYPE
#define TYPE(type) type: #type
#else
#error "The macro \"TYPE\" is already defined !"
#endif /* TYPE */

#ifndef ALL_POSSIBLE_TYPES
#define ALL_POSSIBLE_TYPES(type)                                                                                        \
    TYPE(type),                                                                                                         \
	ALL_PTR(type)
#else
#error "The macro \"ALL_POSSIBLE_TYPES\" is already defined !"
#endif /* ALL_POSSIBLE_TYPES */

#ifndef ALL_POSSIBLE_TYPES_WITH_UNSIGNED
#define ALL_POSSIBLE_TYPES_WITH_UNSIGNED(type)                                                                          \
    ALL_POSSIBLE_TYPES(type),                                                                                           \
    ALL_POSSIBLE_TYPES(unsigned type)
#else
#error "The macro \"ALL_POSSIBLE_TYPES_WITH_UNSIGNED\" is already defined !"
#endif /* ALL_POSSIBLE_TYPES_WITH_UNSIGNED */

#ifndef ALL_PTR
#define ALL_PTR(type)                                                                                                   \
    TYPE_PTR(type),                                                                                                     \
    TYPE_PTR_PTR(type),                                                                                                 \
    TYPE_PTR_PTR_PTR(type),                                                                                             \
    TYPE_PTR_PTR_PTR_PTR(type),                                                                                         \
	TYPE_PTR_PTR_PTR_PTR_PTR(type)
#else
#error "The macro \"ALL_PTR\" is already defined !"
#endif /* ALL_PTR */

#ifndef TYPE_PTR
#define TYPE_PTR(type) TYPE(type*)
#else
#error "The macro \"TYPE_PTR\" is already defined !"
#endif /* TYPE_PTR */

#ifndef TYPE_PTR_PTR
#define TYPE_PTR_PTR(type) TYPE(type**)
#else
#error "The macro \"TYPE_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR(type) TYPE(type***)
#else
#error "The macro \"TYPE_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR_PTR(type) TYPE(type****)
#else
#error "The macro \"TYPE_PTR_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR_PTR_PTR(type) TYPE(type*****)
#else
#error "The macro \"TYPE_PTR_PTR_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR_PTR_PTR */

#ifndef INT_TYPES
#define INT_TYPES                                                                                                       \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(char),                                                                             \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(short int),                                                                        \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(int),                                                                              \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(long int),                                                                         \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(long long int)
#else
#error "The macro \"INT_TYPES\" is already defined !"
#endif /* INT_TYPES */

#ifndef FLOAT_TYPES
#define FLOAT_TYPES                                                                                                     \
    ALL_POSSIBLE_TYPES(float),                                                                                          \
    ALL_POSSIBLE_TYPES(double),                                                                                         \
    ALL_POSSIBLE_TYPES(long double)
#else
#error "The macro \"FLOAT_TYPES\" is already defined !"
#endif /* FLOAT_TYPES */

#ifndef VOID_TYPES
#define VOID_TYPES ALL_PTR(void)
#else
#error "The macro \"VOID_TYPES\" is already defined !"
#endif /* VOID_TYPES */

/**
 * @brief This macro "returns" the type name of standard C types.
 *
 * Useful for the following checks with _Static_assert.
 */
#ifndef TYPE_NAME
#define TYPE_NAME(x)                                                                                                    \
    _Generic((x),                                                                                                       \
    INT_TYPES,                                                                                                          \
    FLOAT_TYPES,                                                                                                        \
    VOID_TYPES,                                                                                                         \
    default: "Cannot determine type !")
#else
#error "The macro \"TYPE_NAME\" is already defined !"
#endif /* TYPE_NAME */



/**
 * @brief Is the value a specific type ?
 *
 * With this macro it is possible to make other macros more type safe.
 */
#ifndef IS_TYPE
#define IS_TYPE(value, exp_type)                                                                                        \
    _Static_assert(_Generic((value), exp_type: true, default: false),                                                   \
        #value " has the wrong type. Expected type: " #exp_type " !");
#else
#error "The macro \"IS_TYPE\" is already defined !"
#endif /* IS_TYPE */



/**
 * @brief IS_IN_TYPE_LIST_2 - IS_IN_TYPE_LIST_5 have the same functionality like IS_TYPE. But with a list of valid types.
 *
 * It is comparable with a white list for valid variable types.
 */
#ifndef IS_IN_TYPE_LIST_2
#define IS_IN_TYPE_LIST_2(value, exp_type_1, exp_type_2)                                                                \
    _Static_assert(_Generic((value), exp_type_1: true, exp_type_2: true, default: false),                               \
            #value " has the wrong type. Expected types: " #exp_type_1 " or " #exp_type_2 " !");
#else
#error "The macro \"IS_IN_TYPE_LIST_2\" is already defined !"
#endif /* IS_IN_TYPE_LIST_2 */

#ifndef IS_IN_TYPE_LIST_3
#define IS_IN_TYPE_LIST_3(value, exp_type_1, exp_type_2, exp_type_3)                                                    \
    _Static_assert(_Generic((value), exp_type_1: true, exp_type_2: true, exp_type_3: true, default: false),             \
            #value " has the wrong type. Expected types: " #exp_type_1 ", " #exp_type_2 " or " #exp_type_3 " !");
#else
#error "The macro \"IS_IN_TYPE_LIST_3\" is already defined !"
#endif /* IS_IN_TYPE_LIST_3 */

#ifndef IS_IN_TYPE_LIST_4
#define IS_IN_TYPE_LIST_4(value, exp_type_1, exp_type_2, exp_type_3, exp_type_4)                                                \
    _Static_assert(_Generic((value), exp_type_1: true, exp_type_2: true, exp_type_3: true, exp_type_4: true, default: false),   \
            #value " has the wrong type. Expected types: " #exp_type_1 ", " #exp_type_2 ", " #exp_type_3 " or " #exp_type_4 " !");
#else
#error "The macro \"IS_IN_TYPE_LIST_4\" is already defined !"
#endif /* IS_IN_TYPE_LIST_4 */

#ifndef IS_IN_TYPE_LIST_5
#define IS_IN_TYPE_LIST_5(value, exp_type_1, exp_type_2, exp_type_3, exp_type_4, exp_type_5)                                                    \
    _Static_assert(_Generic((value), exp_type_1: true, exp_type_2: true, exp_type_3: true, exp_type_4: true, exp_type_5: true, default: false), \
            #value " has the wrong type. Expected types: " #exp_type_1 ", " #exp_type_2 ", " #exp_type_3 ", " #exp_type_4 " or " #exp_type_5 " !");
#else
#error "The macro \"IS_IN_TYPE_LIST_5\" is already defined !"
#endif /* IS_IN_TYPE_LIST_5 */



/**
 * @brief Is the value not a specific type ?
 *
 * With this macro it is possible to make other macros more type safe.
 */
#ifndef IS_NOT_TYPE
#define IS_NOT_TYPE(value, not_allowed_type)                                                                            \
    _Static_assert(_Generic((value), not_allowed_type: false, default: true),                                           \
        #value " has the wrong type. The type " #not_allowed_type " is not allowed !");
#else
#error "The macro \"IS_NOT_TYPE\" is already defined !"
#endif /* IS_NOT_TYPE */



/**
 * @brief IS_NOT_IN_TYPE_LIST_2 - IS_NOT_IN_TYPE_LIST_5 have the same functionality like IS_NOT_TYPE. But with a list
 * of invalid types.
 *
 * It is comparable with a black list for invalid variable types.
 */
#ifndef IS_NOT_TYPE_LIST_2
#define IS_NOT_TYPE_LIST_2(value, not_allowed_type_1, not_allowed_type_2)                                               \
    _Static_assert(_Generic((value), not_allowed_type_1: false, not_allowed_type_2: false, default: true),              \
        #value " has the wrong type. The types " #not_allowed_type_1 " and " #not_allowed_type_2 " are not allowed !");
#else
#error "The macro \"IS_NOT_TYPE_LIST_2\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_2 */

#ifndef IS_NOT_TYPE_LIST_3
#define IS_NOT_TYPE_LIST_3(value, not_allowed_type_1, not_allowed_type_2, not_allowed_type_3)                                           \
    _Static_assert(_Generic((value), not_allowed_type_1: false, not_allowed_type_2: false, not_allowed_type_3: false, default: true),   \
        #value " has the wrong type. The types " #not_allowed_type_1 ", " #not_allowed_type_2 " and " #not_allowed_type_3 " are not allowed !");
#else
#error "The macro \"IS_NOT_TYPE_LIST_3\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_3 */

#ifndef IS_NOT_TYPE_LIST_4
#define IS_NOT_TYPE_LIST_4(value, not_allowed_type_1, not_allowed_type_2, not_allowed_type_3, not_allowed_type_4)                                                   \
    _Static_assert(_Generic((value), not_allowed_type_1: false, not_allowed_type_2: false, not_allowed_type_3: false, not_allowed_type_4: false, default: true),    \
        #value " has the wrong type. The types " #not_allowed_type_1 ", " #not_allowed_type_2 ", " not_allowed_type_3 " and " #not_allowed_type_4 " are not allowed !");
#else
#error "The macro \"IS_NOT_TYPE_LIST_4\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_4 */

#ifndef IS_NOT_TYPE_LIST_5
#define IS_NOT_TYPE_LIST_5(value, not_allowed_type_1, not_allowed_type_2, not_allowed_type_3, not_allowed_type_4, not_allowed_type_5)                                                       \
    _Static_assert(_Generic((value), not_allowed_type_1: false, not_allowed_type_2: false, not_allowed_type_3: false, not_allowed_type_4: false, not_allowed_type_5: false, default: true), \
        #value " has the wrong type. The types " #not_allowed_type_1 ", " #not_allowed_type_2 ", " #not_allowed_type_3 ", " #not_allowed_type_4 " and " #not_allowed_type_5 " are not allowed !");
#else
#error "The macro \"IS_NOT_TYPE_LIST_5\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_5 */

#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GENERICS_H */
