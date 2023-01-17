/**
 * @file Dynamic_Memory.h
 *
 * @brief Here the number of dynamic memory operations will be counted with global variables. (Yes I know: They are evil
 * bytes. It is necessary to be careful; they can bite ! But up to now I had a acceptable relationship with them :3)
 *
 * The goal is to determine whether memory leaks are possible. If this is the case there will be more allocation calls
 * than free calls.
 *
 * To work properly, it is necessary to use the macros, that are here defined. This is also required for the free calls !
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#ifndef DYNAMIC_MEMORY_H
#define DYNAMIC_MEMORY_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>   // uint_fast64_t
#include <stdlib.h>     // malloc, calloc
#include "_Generics.h"
#include "Assert_Msg.h"


// Global variables to count the malloc (), calloc (), realloc () and free () calls
extern uint_fast64_t GLOBAL_malloc_calls;   ///< Number of executed malloc calls
extern uint_fast64_t GLOBAL_calloc_calls;   ///< Number of executed calloc calls
extern uint_fast64_t GLOBAL_realloc_calls;  ///< Number of executed realloc calls
extern uint_fast64_t GLOBAL_free_calls;     ///< Number of executed free calls



/**
 * @brief Show the current malloc (), calloc (), realloc () and free () calls, that were measured with the MALLOC,
 * CALLOC and FREE macros.
 *
 * @param[in] no_missing_free_call_print Print the status even when no free calls are missing
 */
extern void Show_Dynamic_Memory_Status (const _Bool no_missing_free_call_print);



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute and count malloc calls.
 */
#ifndef MALLOC
    #define MALLOC(memory_size)                                                                                         \
        malloc (memory_size);                                                                                           \
        ++ GLOBAL_malloc_calls;                                                                                         \
        IS_INT(memory_size)
#else
    #error "The macro \"MALLOC\" is already defined !"
#endif /* MALLOC */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute and count calloc calls.
 */
#ifndef CALLOC
    #define CALLOC(number_of_elements, element_size)                                                                    \
        calloc (number_of_elements, element_size);                                                                      \
        ++ GLOBAL_calloc_calls;                                                                                         \
        IS_INT(number_of_elements)                                                                                      \
        IS_INT(element_size)
#else
    #error "The macro \"CALLOC\" is already defined !"
#endif /* CALLOC */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute and count realloc calls.
 *
 * realloc do a malloc and a free call in the background.
 * => So the malloc, realloc and free counter needs to be increased.
 *
 * If the pointer is NULL, then realloc() behaves like malloc()
 */
#ifndef REALLOC
    #define REALLOC(pointer, element_size)                                                                              \
        realloc (pointer, element_size);                                                                                \
        ++ GLOBAL_malloc_calls;                                                                                         \
        if (pointer != NULL)                                                                                            \
        {                                                                                                               \
            ++ GLOBAL_realloc_calls;                                                                                    \
            ++ GLOBAL_free_calls;                                                                                       \
        }                                                                                                               \
        IS_INT(element_size)
#else
    #error "The macro \"REALLOC\" is already defined !"
#endif /* REALLOC */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Delete the object, increase the free counter and set the pointer to NULL.
 *
 * A nullptr check before the free call is NOT necessary. In case of nullptr the free function do nothing !
 *
 * See: https://stackoverflow.com/questions/4190703/is-it-safe-to-delete-a-null-pointer
 */
#ifndef FREE_AND_SET_TO_NULL
    #define FREE_AND_SET_TO_NULL(pointer)                                                                               \
        /* if (pointer != NULL) */                                                                                      \
        {                                                                                                               \
            free (pointer);                                                                                             \
            pointer = NULL;                                                                                             \
            ++ GLOBAL_free_calls;                                                                                       \
        }
#else
    #error "The macro \"FREE_AND_SET_TO_NULL\" is already defined !"
#endif /* FREE_AND_SET_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Close an file and set the FILE pointer to NULL.
 */
#ifndef FCLOSE_AND_SET_TO_NULL
    #define FCLOSE_AND_SET_TO_NULL(pointer)                                                                             \
        if (fclose (pointer) == EOF)                                                                                    \
        {                                                                                                               \
            ASSERT_MSG(stderr, "Cannot close the file ! EOF was returned !")                                            \
        }                                                                                                               \
        pointer = NULL;                                                                                                 \
        IS_TYPE(pointer, FILE*)
#else
    #error "The macro \"FCLOSE_AND_SET_TO_NULL\" is already defined !"
#endif /* FCLOSE_AND_SET_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Close an file and set the FILE pointer to NULL.
 */
#ifndef FCLOSE_WITH_NAME_AND_SET_TO_NULL
    #define FCLOSE_WITH_NAME_AND_SET_TO_NULL(pointer, file_name)                                                        \
        if (fclose (pointer) == EOF)                                                                                    \
        {                                                                                                               \
            ASSERT_FMSG(stderr, "Cannot close the file \"%s\"! EOF was returned !", file_name)                          \
        }                                                                                                               \
        pointer = NULL;                                                                                                 \
        IS_TYPE(pointer, FILE*)                                                                                         \
        IS_IN_TYPE_LIST_4(file_name, char*, const char*, char* const, const char* const)
#else
    #error "The macro \"FCLOSE_WITH_NAME_AND_SET_TO_NULL\" is already defined !"
#endif /* FCLOSE_WITH_NAME_AND_SET_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Delete an object with a user-defined delete function.
 */
#ifndef FREE_WITH_FUNCTION_AND_SET_TO_NULL
    #define FREE_WITH_FUNCTION_AND_SET_TO_NULL(free_function, pointer)                                                  \
        free_function (pointer);                                                                                        \
        pointer = NULL;
#else
    #error "The macro \"FREE_WITH_FUNCTION_AND_SET_TO_NULL\" is already defined !"
#endif /* FREE_WITH_FUNCTION_AND_SET_TO_NULL */

//---------------------------------------------------------------------------------------------------------------------



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DYNAMIC_MEMORY_H */
