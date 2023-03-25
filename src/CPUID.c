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

#if defined(__GNUC__)

// The following instructions are x86-only
#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)

#include "Error_Handling/Assert_Msg.h"



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



/**
 * @brief Constant values for the interesting registers after a CPUID instruction.
 */
enum Register
{
    UNKNOWN_REGISTER = 0,

    EAX,
    EBX,
    ECX,
    EDX
};

/**
 * @brief Get a specific bit after a CPUID instruction from a selected register.
 *
 * @param[in] eax_value Start value for EAX (With EAX different results are possible)
 * @param[in] shift Number of the flag (number of the bit)
 * @param[in] selected_reg Register, that holds the flags after the CPUID instruction
 *
 * @return True, whether the flag was set, otherwise False
 */
static _Bool CPUID_IsFlagSet
(
        const int32_t eax_value,
        const int shift,
        const enum Register selected_reg
);



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
        int32_t (* const result) [4]
)
{
    ASSERT_MSG(result != NULL, "Pointer to the result array is NULL !");

    // The int32_t array will be used as char* array to get the 12 char vendor string. The fourth element is designed to
    // be a terminating null byte
    *result[3] = 0;

    __asm__ volatile
    (
            "movq $0, %%rax" NT
            "cpuid" NT

            : "=b" ((*result)[0]), "=d" ((*result)[1]), "=c" ((*result)[2])
            :
            : "rax"
    );

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
    return CPUID_IsFlagSet(1, 23, EDX);
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
    return CPUID_IsFlagSet(1, 26, EDX);
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
    return CPUID_IsFlagSet(1, 19, ECX);
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
    return CPUID_IsFlagSet(7, 5, EBX);
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
    return CPUID_IsFlagSet(7, 16, EBX);
}

//=====================================================================================================================

/**
 * @brief Get a specific bit after a CPUID instruction from a selected register.
 *
 * @param[in] eax_value Start value for EAX (With EAX different results are possible)
 * @param[in] shift Number of the flag (number of the bit)
 * @param[in] selected_reg Register, that holds the flags after the CPUID instruction
 *
 * @return True, whether the flag was set, otherwise False
 */
static _Bool CPUID_IsFlagSet
(
        const int32_t eax_value,
        const int shift,
        const enum Register selected_reg
)
{
    int32_t result = 0;

    switch (selected_reg)
    {
    case EAX:
        __asm__ volatile
        (
                "mov %1, %%eax" NT
                "cpuid" NT

                : "=a" (result)
                : "r" (eax_value)
                : "rax"
        );
    break;
    case EBX:
        __asm__ volatile
        (
                "mov %1, %%eax" NT
                "cpuid" NT

                : "=b" (result)
                : "r" (eax_value)
                : "rax"
        );
    break;
    case ECX:
        __asm__ volatile
        (
                "mov %1, %%eax" NT
                "cpuid" NT

                : "=c" (result)
                : "r" (eax_value)
                : "rax"
        );
    break;
    case EDX:
        __asm__ volatile
        (
                "mov %1, %%eax" NT
                "cpuid" NT

                : "=d" (result)
                : "r" (eax_value)
                : "rax"
        );
    break;
    case UNKNOWN_REGISTER:
        ASSERT_MSG(false, "Unknown register given !");
        break;

    default:
        ASSERT_MSG(false, "switch case default path executed !");
    }

    return (_Bool) (result & (1 << shift));
}



#ifdef NT
#undef NT
#endif /* NT */



#else /* defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64) */

#warning "Translation unit, that contains only functions designed for a x86 host CPU included."

//---------------------------------------------------------------------------------------------------------------------

extern void CPUID_GetVendorString
(
        int32_t (* const result) [4]
)
{
    ASSERT_MSG(false, "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU");
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsMMXAvailable
(
        void
)
{
    ASSERT_MSG(false, "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU");
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsSSE2Available
(
        void
)
{
    ASSERT_MSG(false, "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU");
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsSSE4_1Available
(
        void
)
{
    ASSERT_MSG(false, "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU");
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsAVX2Available
(
        void
)
{
    ASSERT_MSG(false, "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU");
    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern _Bool CPUID_IsAVX512FAvailable
(
        void
)
{
    ASSERT_MSG(false, "You try to call a function, that contains x86-only instructions, with a non-x86 host CPU");
    return;
}

//---------------------------------------------------------------------------------------------------------------------

#endif /* defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64) */

#else /* #if defined(__GNUC__) */

#error "This translation unit uses inline assembly code in the GCC syntax, which is not portable to other compilers. If you don't need functions from this translation unit: comment the include line out."

#endif /* #if defined(__GNUC__) */
