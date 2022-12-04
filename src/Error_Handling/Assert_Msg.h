/**
 * @file Assert_Msg.h
 *
 * @brief Here are asserts defined, which don't need the translation unit "tinytest".
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#ifndef ERROR_HANDLING_ASSERT_MSG_H
#define ERROR_HANDLING_ASSERT_MSG_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "_Generics.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief A assert with a user-defined error message.
 */
#ifndef ASSERT_MSG
    #define ASSERT_MSG(expr, message) assert (((void) (message), expr));                                                \
        IS_IN_TYPE_LIST_4(message, char*, const char*, char* const, const char* const)
#else
    #error "The macro \"ASSERT_MSG\" is already defined !"
#endif /* ASSERT_MSG */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief A assert with a user-defined error message in the form of format string.
 *
 * In an error case the error message will be shown before the assert will be executed because the standard assert
 * function only accepts C-String without format parameter.
 */
#ifndef ASSERT_FMSG
    #define ASSERT_FMSG(expr, format_string, ...)                                                                       \
    if (! (expr))                                                                                                       \
    {                                                                                                                   \
        fprintf (stderr, format_string, __VA_ARGS__);                                                                   \
        fputc ('\n', stderr);                                                                                           \
        fflush (stderr);                                                                                                \
        ASSERT_MSG(false, "See error message above !")                                                                  \
    }                                                                                                                   \
    IS_IN_TYPE_LIST_4(format_string, char*, const char*, char* const, const char* const)
#else
    #error "The macro \"ASSERT_FMSG\" is already defined !"
#endif /* ASSERT_FMSG */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief This assert is for error cases after an dynamic allocation operation.
 *
 * In an error case the number of bytes, that tried to allocate, will be printed. This additional information is not
 * necessary. But it can help to find bugs. E.g.: Maybe there is an bug in the memory size calculation.
 */
#ifndef ASSERT_ALLOC
    #define ASSERT_ALLOC(ptr, message, memory_size)                                                                     \
    if (ptr == NULL)                                                                                                    \
    {                                                                                                                   \
        fprintf (stderr, "Try to allocate %zu bytes !\n", (size_t) memory_size);                                        \
        fflush (stderr);                                                                                                \
        ASSERT_MSG(false, message)                                                                                      \
    }                                                                                                                   \
    IS_IN_TYPE_LIST_4(message, char*, const char*, char* const, const char* const)                                      \
    IS_INT(memory_size)
#else
    #error "The macro \"ASSERT_ALLOC\" is already defined !"
#endif /* ASSERT_ALLOC */

//---------------------------------------------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ERROR_HANDLING_ASSERT_MSG_H */
