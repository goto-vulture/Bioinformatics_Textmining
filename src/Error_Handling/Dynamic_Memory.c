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
    const uint_fast64_t missing_free_calls = (GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls;

    printf ("malloc () calls:       %10" PRIuFAST64 "\n"
            "calloc () calls:       %10" PRIuFAST64 "\n"
            "realloc () calls:      %10" PRIuFAST64 "\n"
            "free () calls:         %10" PRIuFAST64 "\n"
            "Missing free () calls: %10" PRIuFAST64 " %s\n",
            GLOBAL_malloc_calls,
            GLOBAL_calloc_calls,
            GLOBAL_realloc_calls,
            GLOBAL_free_calls,
            missing_free_calls,
            (missing_free_calls == 0) ? ":D" : ":o");

    return;
}

//---------------------------------------------------------------------------------------------------------------------
