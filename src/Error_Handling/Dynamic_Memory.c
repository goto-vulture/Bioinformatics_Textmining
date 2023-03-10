/**
 * @file Dynamic_Memory.c
 *
 * @brief Print the number of malloc (), calloc (), realloc () and free () calls.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#include "Dynamic_Memory.h"
#include <stdio.h>



// Global variables to count the malloc (), calloc (), realloc () and free () calls
uint_fast64_t GLOBAL_malloc_calls   = 0;
uint_fast64_t GLOBAL_calloc_calls   = 0;
uint_fast64_t GLOBAL_realloc_calls  = 0;
uint_fast64_t GLOBAL_free_calls     = 0;



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Show the current malloc (), calloc (), realloc () and free () calls, that were measured with the MALLOC,
 * CALLOC and FREE macros.
 */
void Show_Dynamic_Memory_Status (void)
{
    const int_fast64_t missing_free_calls = Missing_Free_Calls();
    // Show the status only in case of missing (or too much) free calls
    if (missing_free_calls == 0)
    {
        return;
    }

    const char k []         = { 'K', '\0' };
    const char null_str []  = { '\0' };

    printf ("malloc () calls:       %10" PRIuFAST64 " %s\n"
            "calloc () calls:       %10" PRIuFAST64 " %s\n"
            "realloc () calls:      %10" PRIuFAST64 " %s\n"
            "free () calls:         %10" PRIuFAST64 " %s\n"
            "Missing free () calls: %10" PRIdFAST64 " %s\n",
            (GLOBAL_malloc_calls > 1000) ? GLOBAL_malloc_calls / 1000 : GLOBAL_malloc_calls,
            (GLOBAL_malloc_calls > 1000) ? k : null_str,
            (GLOBAL_calloc_calls > 1000) ? GLOBAL_calloc_calls / 1000 : GLOBAL_calloc_calls,
            (GLOBAL_calloc_calls > 1000) ? k : null_str,
            (GLOBAL_realloc_calls > 1000) ? GLOBAL_realloc_calls / 1000 : GLOBAL_realloc_calls,
            (GLOBAL_realloc_calls > 1000) ? k : null_str,
            (GLOBAL_free_calls > 1000) ? GLOBAL_free_calls / 1000 :  GLOBAL_free_calls,
            (GLOBAL_free_calls > 1000) ? k : null_str,
            missing_free_calls,
            (missing_free_calls == 0) ? ":D" : (missing_free_calls < 0) ? ":oo" : ":o");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Calculate the number of missing free calls.
 *
 * Because a free call on a NULL pointer does not have any effect, a negative result (too much calls) is possible
 * -> int_fast64_t instead of uint_fast64_t as return type.
 *
 * @return Number of missing free calls.
 */
extern int_fast64_t Missing_Free_Calls (void)
{
    return (int_fast64_t) ((GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls);
}

//---------------------------------------------------------------------------------------------------------------------
