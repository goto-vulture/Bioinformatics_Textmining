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
 *
 * @param[in] no_missing_free_call_print Print the status even when no free calls are missing
 */
void Show_Dynamic_Memory_Status (const _Bool no_missing_free_call_print)
{
    const int_fast64_t missing_free_calls = (int_fast64_t) ((GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls);
    if (missing_free_calls == 0 && ! no_missing_free_call_print) { return; }

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
