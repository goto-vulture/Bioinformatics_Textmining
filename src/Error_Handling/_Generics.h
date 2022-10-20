/**
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

#ifndef TYPE_TYPE
#define TYPE_TYPE(type) type: #type
#else
#error "The macro \"TYPE_TYPE\" is already defined !"
#endif /* TYPE_TYPE */

#ifndef TYPE_VALID
#define TYPE_VALID(type) type: true
#else
#error "The macro \"TYPE_VALID\" is already defined !"
#endif /* TYPE_VALID */

#ifndef TYPE_INVALID
#define TYPE_INVALID(type) type: false
#else
#error "The macro \"TYPE_INVALID\" is already defined !"
#endif /* TYPE_INVALID */

#ifndef TYPE_PTR_FMT_STR
#define TYPE_PTR_FMT_STR(type) type: "%p"
#else
#error "The macro \"TYPE_PTR_FMT_STR\" is already defined !"
#endif /* TYPE_PTR_FMT_STR */



#ifndef ALL_POSSIBLE_TYPES
#define ALL_POSSIBLE_TYPES(type, type_def)                                                                              \
    type_def(type),                                                                                                     \
	ALL_PTR(type, TYPE_DEF)
#else
#error "The macro \"ALL_POSSIBLE_TYPES\" is already defined !"
#endif /* ALL_POSSIBLE_TYPES */

#ifndef ALL_POSSIBLE_TYPES_WITH_UNSIGNED
#define ALL_POSSIBLE_TYPES_WITH_UNSIGNED(type, type_def)                                                                \
    ALL_POSSIBLE_TYPES(type, type_def),                                                                                 \
    ALL_POSSIBLE_TYPES(unsigned type, type_def)
#else
#error "The macro \"ALL_POSSIBLE_TYPES_WITH_UNSIGNED\" is already defined !"
#endif /* ALL_POSSIBLE_TYPES_WITH_UNSIGNED */

#ifndef ALL_PTR
#define ALL_PTR(type, type_def)                                                                                         \
    TYPE_PTR(type, type_def),                                                                                           \
    TYPE_PTR_PTR(type, type_def),                                                                                       \
    TYPE_PTR_PTR_PTR(type, type_def),                                                                                   \
    TYPE_PTR_PTR_PTR_PTR(type, type_def),                                                                               \
    TYPE_PTR_PTR_PTR_PTR_PTR(type, type_def)
#else
#error "The macro \"ALL_PTR\" is already defined !"
#endif /* ALL_PTR */

#ifndef TYPE_PTR
#define TYPE_PTR(type, type_def) type_def(type*)
#else
#error "The macro \"TYPE_PTR\" is already defined !"
#endif /* TYPE_PTR */

#ifndef TYPE_PTR_PTR
#define TYPE_PTR_PTR(type, type_def) type_def(type**)
#else
#error "The macro \"TYPE_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR(type, type_def) type_def(type***)
#else
#error "The macro \"TYPE_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR_PTR(type, type_def) type_def(type****)
#else
#error "The macro \"TYPE_PTR_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR_PTR_PTR(type, type_def) type_def(type*****)
#else
#error "The macro \"TYPE_PTR_PTR_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR_PTR_PTR */

#ifndef INT_TYPES
#define INT_TYPES                                                                                                       \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(char, TYPE_TYPE),                                                                  \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(short int, TYPE_TYPE),                                                             \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(int, TYPE_TYPE),                                                                   \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(long int, TYPE_TYPE),                                                              \
    ALL_POSSIBLE_TYPES_WITH_UNSIGNED(long long int, TYPE_TYPE)
#else
#error "The macro \"INT_TYPES\" is already defined !"
#endif /* INT_TYPES */

#ifndef FLOAT_TYPES
#define FLOAT_TYPES                                                                                                     \
    ALL_POSSIBLE_TYPES(float, TYPE_TYPE),                                                                               \
    ALL_POSSIBLE_TYPES(double, TYPE_TYPE),                                                                              \
    ALL_POSSIBLE_TYPES(long double, TYPE_TYPE)
#else
#error "The macro \"FLOAT_TYPES\" is already defined !"
#endif /* FLOAT_TYPES */

#ifndef VOID_TYPES
#define VOID_TYPES ALL_PTR(void, TYPE_TYPE)
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
 * @brief Check whether x is a pointer. "type_def" defines, if a pointer should be valid or invalid.
 */
#ifndef PTR_CHECK
#define PTR_CHECK(x, type_def, default_result)                                                                          \
    _Generic((x),                                                                                                       \
    ALL_PTR(char, type_def),                                                                                            \
    ALL_PTR(unsigned char, type_def),                                                                                   \
    ALL_PTR(short int, type_def),                                                                                       \
    ALL_PTR(unsigned short int, type_def),                                                                              \
    ALL_PTR(int, type_def),                                                                                             \
    ALL_PTR(unsigned  int, type_def),                                                                                   \
    ALL_PTR(long int, type_def),                                                                                        \
    ALL_PTR(unsigned long int, type_def),                                                                               \
    ALL_PTR(long long int, type_def),                                                                                   \
    ALL_PTR(unsigned long long int, type_def),                                                                          \
    ALL_PTR(float, type_def),                                                                                           \
    ALL_PTR(double, type_def),                                                                                          \
    ALL_PTR(long double, type_def),                                                                                     \
    ALL_PTR(void, type_def),                                                                                            \
    default: default_result)
#else
#error "The macro \"PTR_CHECK\" is already defined !"
#endif /* PTR_CHECK */

/**
 * @brief Is the given value a pointer ?
 */
#ifndef IS_PTR
#define IS_PTR(value)                                                                                                   \
    _Static_assert(PTR_CHECK(value, TYPE_VALID, false), #value " needs to be a pointer !")
#else
#error "The macro \"IS_PTR\" is already defined !"
#endif /* IS_PTR */

/**
 * @brief Is the given value not a pointer ?
 */
#ifndef IS_NO_PTR
#define IS_NO_PTR(value)                                                                                                \
    _Static_assert(PTR_CHECK(value, TYPE_INVALID, true), #value " is a pointer ! Pointer are not allowed !");
#else
#error "The macro \"IS_NO_PTR\" is already defined !"
#endif /* IS_NO_PTR */

/**
 * @brief Is the given value a integer value ?
 */
#ifndef IS_INT
#define IS_INT(value)                                                                                                   \
    _Static_assert(PTR_CHECK(value, TYPE_INVALID, true), #value " is a pointer ! Pointer are not allowed !");
#else
#error "The macro \"IS_INT\" is already defined !"
#endif /* IS_INT */

/**
 * @brief Is the given value not a integer value ?
 */
#ifndef IS_NO_INT
#define IS_NO_INT(value)                                                                                                \
    _Static_assert(PTR_CHECK(value, TYPE_VALID, false), #value " needs to be a pointer !");
#else
#error "The macro \"IS_NO_INT\" is already defined !"
#endif /* IS_NO_INT */



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
 * @brief This macro returns the format string specifier of the given variable, if possible.
 */
#ifndef GET_FORMAT_STR
#define GET_FORMAT_STR(value)                                                                                           \
    _Generic((value),                                                                                                   \
    char: "%c",                                                                                                         \
    unsigned char: "%hhu",                                                                                              \
    short int: "%hi",                                                                                                   \
    unsigned short int: "%hu",                                                                                          \
    int: "%d",                                                                                                          \
    unsigned int: "%lu",                                                                                                \
    long int: "%ld",                                                                                                    \
    unsigned long int: "%lu",                                                                                           \
    long long int: "%lld",                                                                                              \
    unsigned long long int: "%llu",                                                                                     \
    float: "%f",                                                                                                        \
    double: "%f",                                                                                                       \
    long double: "%Lf",                                                                                                 \
    ALL_PTR(char, TYPE_PTR_FMT_STR),                                                                                    \
    ALL_PTR(unsigned char, TYPE_PTR_FMT_STR),                                                                           \
    ALL_PTR(short int, TYPE_PTR_FMT_STR),                                                                               \
    ALL_PTR(unsigned short int, TYPE_PTR_FMT_STR),                                                                      \
    ALL_PTR(int, TYPE_PTR_FMT_STR),                                                                                     \
    ALL_PTR(unsigned int, TYPE_PTR_FMT_STR),                                                                            \
    ALL_PTR(long int, TYPE_PTR_FMT_STR),                                                                                \
    ALL_PTR(unsigned long int, TYPE_PTR_FMT_STR),                                                                       \
    ALL_PTR(long long int, TYPE_PTR_FMT_STR),                                                                           \
    ALL_PTR(unsigned long long int, TYPE_PTR_FMT_STR),                                                                  \
    ALL_PTR(float, TYPE_PTR_FMT_STR),                                                                                   \
    ALL_PTR(double, TYPE_PTR_FMT_STR),                                                                                  \
    ALL_PTR(long double, TYPE_PTR_FMT_STR),                                                                             \
    ALL_PTR(void, TYPE_PTR_FMT_STR),                                                                                    \
    default: "N/A")
#else
#error "The macro \"GET_FORMAT_STR\" is already defined !"
#endif /* GET_FORMAT_STR */



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

#else //================================================================================================================

/*
 * Create empty macros to make the code compatible, when the compiler does not supports C11.
 */
#ifndef TYPE_TYPE
#define TYPE_TYPE(type)
#else
#error "The macro \"TYPE_TYPE\" is already defined !"
#endif /* TYPE_TYPE */

#ifndef TYPE_VALID
#define TYPE_VALID(type)
#else
#error "The macro \"TYPE_VALID\" is already defined !"
#endif /* TYPE_VALID */

#ifndef TYPE_INVALID
#define TYPE_INVALID(type)
#else
#error "The macro \"TYPE_INVALID\" is already defined !"
#endif /* TYPE_INVALID */

#ifndef ALL_POSSIBLE_TYPES
#define ALL_POSSIBLE_TYPES(type)
#else
#error "The macro \"ALL_POSSIBLE_TYPES\" is already defined !"
#endif /* ALL_POSSIBLE_TYPES */

#ifndef ALL_POSSIBLE_TYPES_WITH_UNSIGNED
#define ALL_POSSIBLE_TYPES_WITH_UNSIGNED(type)
#else
#error "The macro \"ALL_POSSIBLE_TYPES_WITH_UNSIGNED\" is already defined !"
#endif /* ALL_POSSIBLE_TYPES_WITH_UNSIGNED */

#ifndef ALL_PTR
#define ALL_PTR(type)
#else
#error "The macro \"ALL_PTR\" is already defined !"
#endif /* ALL_PTR */

#ifndef TYPE_PTR
#define TYPE_PTR(type)
#else
#error "The macro \"TYPE_PTR\" is already defined !"
#endif /* TYPE_PTR */

#ifndef TYPE_PTR_PTR
#define TYPE_PTR_PTR(type)
#else
#error "The macro \"TYPE_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR(type)
#else
#error "The macro \"TYPE_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR_PTR(type)
#else
#error "The macro \"TYPE_PTR_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR_PTR */

#ifndef TYPE_PTR_PTR_PTR_PTR_PTR
#define TYPE_PTR_PTR_PTR_PTR_PTR(type)
#else
#error "The macro \"TYPE_PTR_PTR_PTR_PTR_PTR\" is already defined !"
#endif /* TYPE_PTR_PTR_PTR_PTR_PTR */

#ifndef INT_TYPES
#define INT_TYPES
#else
#error "The macro \"INT_TYPES\" is already defined !"
#endif /* INT_TYPES */

#ifndef FLOAT_TYPES
#define FLOAT_TYPES
#else
#error "The macro \"FLOAT_TYPES\" is already defined !"
#endif /* FLOAT_TYPES */

#ifndef VOID_TYPES
#define VOID_TYPES
#else
#error "The macro \"VOID_TYPES\" is already defined !"
#endif /* VOID_TYPES */

#ifndef TYPE_NAME
#define TYPE_NAME(x)
#else
#error "The macro \"TYPE_NAME\" is already defined !"
#endif /* TYPE_NAME */

#ifndef PTR_CHECK
#define PTR_CHECK(x, type_def, default_result)
#else
#error "The macro \"PTR_CHECK\" is already defined !"
#endif /* PTR_CHECK */

#ifndef IS_PTR
#define IS_PTR(value)
#else
#error "The macro \"IS_PTR\" is already defined !"
#endif /* IS_PTR */

#ifndef IS_NO_PTR
#define IS_NO_PTR(value)
#else
#error "The macro \"IS_NO_PTR\" is already defined !"
#endif /* IS_NO_PTR */

#ifndef IS_INT
#define IS_INT(value)
#else
#error "The macro \"IS_INT\" is already defined !"
#endif /* IS_INT */

#ifndef IS_NO_INT
#define IS_NO_INT(value)
#else
#error "The macro \"IS_NO_INT\" is already defined !"
#endif /* IS_NO_INT */

#ifndef IS_TYPE
#define IS_TYPE(value, exp_type)
#else
#error "The macro \"IS_TYPE\" is already defined !"
#endif /* IS_TYPE */

#ifndef IS_IN_TYPE_LIST_2
#define IS_IN_TYPE_LIST_2(value, exp_type_1, exp_type_2)
#else
#error "The macro \"IS_IN_TYPE_LIST_2\" is already defined !"
#endif /* IS_IN_TYPE_LIST_2 */

#ifndef IS_IN_TYPE_LIST_3
#define IS_IN_TYPE_LIST_3(value, exp_type_1, exp_type_2, exp_type_3)
#else
#error "The macro \"IS_IN_TYPE_LIST_3\" is already defined !"
#endif /* IS_IN_TYPE_LIST_3 */

#ifndef IS_IN_TYPE_LIST_4
#define IS_IN_TYPE_LIST_4(value, exp_type_1, exp_type_2, exp_type_3, exp_type_4)
#else
#error "The macro \"IS_IN_TYPE_LIST_4\" is already defined !"
#endif /* IS_IN_TYPE_LIST_4 */

#ifndef IS_IN_TYPE_LIST_5
#define IS_IN_TYPE_LIST_5(value, exp_type_1, exp_type_2, exp_type_3, exp_type_4, exp_type_5)
#else
#error "The macro \"IS_IN_TYPE_LIST_5\" is already defined !"
#endif /* IS_IN_TYPE_LIST_5 */

#ifndef IS_NOT_TYPE
#define IS_NOT_TYPE(value, not_allowed_type)
#else
#error "The macro \"IS_NOT_TYPE\" is already defined !"
#endif /* IS_NOT_TYPE */

#ifndef IS_NOT_TYPE_LIST_2
#define IS_NOT_TYPE_LIST_2(value, not_allowed_type_1, not_allowed_type_2)
#else
#error "The macro \"IS_NOT_TYPE_LIST_2\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_2 */

#ifndef IS_NOT_TYPE_LIST_3
#define IS_NOT_TYPE_LIST_3(value, not_allowed_type_1, not_allowed_type_2, not_allowed_type_3)
#else
#error "The macro \"IS_NOT_TYPE_LIST_3\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_3 */

#ifndef IS_NOT_TYPE_LIST_4
#define IS_NOT_TYPE_LIST_4(value, not_allowed_type_1, not_allowed_type_2, not_allowed_type_3, not_allowed_type_4)
#else
#error "The macro \"IS_NOT_TYPE_LIST_4\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_4 */

#ifndef IS_NOT_TYPE_LIST_5
#define IS_NOT_TYPE_LIST_5(value, not_allowed_type_1, not_allowed_type_2, not_allowed_type_3, not_allowed_type_4, not_allowed_type_5)
#else
#error "The macro \"IS_NOT_TYPE_LIST_5\" is already defined !"
#endif /* IS_NOT_TYPE_LIST_5 */

#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _GENERICS_H */
