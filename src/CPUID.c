/**
 * @file CPUID.c
 *
 * @brief Functions with inline asm to discover details of a x86 processor.
 *
 * Documentation:
 * @see https://www.sandpile.org/x86/cpuid.htm
 *
 * @date 25.03.2023
 * @author: am1
 */

#include "CPUID.h"
#include "Error_Handling/Assert_Msg.h"

#if defined(__GNUC__)

// The following instructions are x86-only
#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)



/**
 * @brief Newline-Tabulator String for formatting the Inline-Assembler instructions.
 *
 * It is mandatory to write "\n\t" at the end of each line to tell the compiler, that a instructions ends at the end of
 * this line.
 */
#ifndef NT
#define NT "\n\t"
#else
#error "The macro \"NT\" is already defined !"
#endif /* NT */



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Execute the CPUID instruction with given eax, ebx, ecx and edx values.
 *
 * @param[in] input Input values for the eax, ebx, ecx and edx register
 *
 * @return Result values of the eax, ebx, ecx and edx register
 */
extern struct CPUID_Register CPUID_ExecWithGivenValues
(
        volatile struct CPUID_Register input
)
{
    volatile struct CPUID_Register result = { .eax = 0, .ebx = 0, .ecx = 0, .edx = 0 };

    __asm__ volatile
    (
            "mov %4, %%eax" NT
            "mov %5, %%ebx" NT
            "mov %6, %%ecx" NT
            "mov %7, %%edx" NT
            "cpuid" NT

            : "=a" (result.eax), "=b" (result.ebx), "=c" (result.ecx), "=d" (result.edx)
            : "r" (input.eax), "r" (input.ebx), "r" (input.ecx), "r" (input.edx)
            :
    );

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

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
        volatile int32_t (* const result) [4]
)
{
    ASSERT_MSG(result != NULL, "Pointer to the result array is NULL !");

    volatile struct CPUID_Register input = { .eax = 0, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    // Yes the order is correct: ebx | edx | ecx
    (*result)[0] = result_register.ebx;
    (*result)[1] = result_register.edx;
    (*result)[2] = result_register.ecx;

    // The int32_t array will be used as char* array to get the 12 char vendor string. The fourth element is designed to
    // be a terminating null byte
    (*result)[3] = 0;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the AMD easter egg string.
 *
 * Asserts:
 *      result != NULL
 *
 * @param[out] result Pointer to a int32_t array with exact 5 elements.
 */
extern void CPUID_GetAMDEasterEggString
(
        volatile int32_t (* const result)[5]
)
{
    ASSERT_MSG(result != NULL, "Pointer to the result array is NULL !");

    volatile struct CPUID_Register input = { .eax = (int32_t) 0x8FFFFFFF, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    (*result)[0] = result_register.eax;
    (*result)[1] = result_register.ebx;
    (*result)[2] = result_register.ecx;
    (*result)[3] = result_register.edx;

    // The int32_t array will be used as char* array to get the 16 char easter egg string. The fifth element is
    // designed to be a terminating null byte
    (*result)[4] = 0;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the bit, that shows whether the host CPU supports the MMX extension or not.
 *
 * @return true for MMX support, otherwise false
 */
extern _Bool CPUID_IsMMXAvailable
(
        void
)
{
    volatile struct CPUID_Register input = { .eax = 1, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    return result_register.edx & (1 << 23);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the bit, that shows whether the host CPU supports the SSE2 extension or not.
 *
 * @return true for SSE2 support, otherwise false
 */
extern _Bool CPUID_IsSSE2Available
(
        void
)
{
    volatile struct CPUID_Register input = { .eax = 1, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    return result_register.edx & (1 << 26);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the bit, that shows whether the host CPU supports the SSE4.1 extension or not.
 *
 * @return true for SSE4.1 support, otherwise false
 */
extern _Bool CPUID_IsSSE4_1Available
(
        void
)
{
    volatile struct CPUID_Register input = { .eax = 1, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    return result_register.ecx & (1 << 19);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the bit, that shows whether the host CPU supports the AVX extension or not.
 *
 * @return true for AVX support, otherwise false
 */
extern _Bool CPUID_IsAVXAvailable
(
        void
)
{
    volatile struct CPUID_Register input = { .eax = 1, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    return result_register.ecx & (1 << 28);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the bit, that shows whether the host CPU supports the AVX2 extension or not.
 *
 * @return true for AVX2 support, otherwise false
 */
extern _Bool CPUID_IsAVX2Available
(
        void
)
{
    volatile struct CPUID_Register input = { .eax = 7, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    return result_register.ebx & (1 << 5);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Get the bit, that shows whether the host CPU supports the AVX512F extension or not.
 *
 * @return true for AVX512F support, otherwise false
 */
extern _Bool CPUID_IsAVX512FAvailable
(
        void
)
{
    volatile struct CPUID_Register input = { .eax = 7, .ebx = 0, .ecx = 0, .edx = 0 };
    const volatile struct CPUID_Register result_register = CPUID_ExecWithGivenValues(input);

    return result_register.ebx & (1 << 16);
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef NT
#undef NT
#endif /* NT */

#else /* defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64) */

#warning "Translation unit, that contains only functions designed for a x86 host CPU included."

/**
 * @brief Error message if a x86 specific function will be executed on a non-x86 host CPU.
 */
#ifndef ERR_MSG
#define ERR_MSG "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU"
#else
#error "The macro \"ERR_MSG\" is already defined !"
#endif /* ERR_MSG */

//---------------------------------------------------------------------------------------------------------------------

extern struct CPUID_Register CPUID_ExecWithGivenValues
(
        struct CPUID_Register input
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void CPUID_GetVendorString
(
        int32_t (* const result) [4]
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void CPUID_GetAMDEasterEggString
(
        int32_t (* const result)[5]
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsMMXAvailable
(
        void
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsSSE2Available
(
        void
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsSSE4_1Available
(
        void
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsAVXAvailable
(
        void
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsAVX2Available
(
        void
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsAVX512FAvailable
(
        void
)
{
    ASSERT_MSG(false, ERR_MSG);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef ERR_MSG
#undef ERR_MSG
#endif /* ERR_MSG */

#endif /* defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64) */

#else /* #if defined(__GNUC__) */

#error "This translation unit uses inline assembly code in the GCC syntax, which is not portable to other compilers. If you don't need functions from this translation unit: comment the include line out."

#endif /* #if defined(__GNUC__) */
