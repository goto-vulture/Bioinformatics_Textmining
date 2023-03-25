/**
 * @file CPUID.h
 *
 * @brief Functions with inline asm to discover details of a x86 processor.
 *
 * Documentation:
 * @see https://www.sandpile.org/x86/cpuid.htm
 *
 * @date 25.03.2023
 * @author: am1
 */

#ifndef CPUID_H
#define CPUID_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined(__GNUC__)



#include <inttypes.h>



/**
 * @brief Get the vendor string of the x86 host CPU.
 *
 * Asserts:
 *      result != NULL
 *
 * @param[out] result Pointer to a int32_t array with exact 4 elements.
 */
extern void CPUID_GetVendorString
(
        int32_t (* const result)[4]
);

/**
 * @brief Get the bit, that shows whether the host CPU supports the MMX extension or not.
 *
 * @return true for MMX support, otherwise false
 */
extern _Bool CPUID_IsMMXAvailable
(
        void
);

/**
 * @brief Get the bit, that shows whether the host CPU supports the SSE2 extension or not.
 *
 * @return true for SSE2 support, otherwise false
 */
extern _Bool CPUID_IsSSE2Available
(
        void
);

/**
 * @brief Get the bit, that shows whether the host CPU supports the SSE4.1 extension or not.
 *
 * @return true for SSE4.1 support, otherwise false
 */
extern _Bool CPUID_IsSSE4_1Available
(
        void
);

/**
 * @brief Get the bit, that shows whether the host CPU supports the AVX extension or not.
 *
 * @return true for AVX support, otherwise false
 */
extern _Bool CPUID_IsAVXAvailable
(
        void
);

/**
 * @brief Get the bit, that shows whether the host CPU supports the AVX2 extension or not.
 *
 * @return true for AVX2 support, otherwise false
 */
extern _Bool CPUID_IsAVX2Available
(
        void
);

/**
 * @brief Get the bit, that shows whether the host CPU supports the AVX512F extension or not.
 *
 * @return true for AVX512F support, otherwise false
 */
extern _Bool CPUID_IsAVX512FAvailable
(
        void
);



#endif /* #if defined(__GNUC__) */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CPUID_H */
