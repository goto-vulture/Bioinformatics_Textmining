/**
 * @file OS_Specific_Configurations.h
 *
 * @brief OS-specific configurations of the program.
 *
 * @date 14.03.2021
 * @author x86 / Gyps
 */

#ifndef OS_SPECIFIC_CONFIGURATIONS_H
#define OS_SPECIFIC_CONFIGURATIONS_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/*
 * Is it a Linux system ?
 */
#if defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix)

    #ifndef LINUX
        #define LINUX ///< For Linux systems
    #else
        #error "The Macro \"LINUX\" is already defined !"
    #endif /* LINUX */
 
#endif /* defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix) */



/*
 * Configurations on a windows system.
 * It is possible to detect a Windows system with 64 bit only with the 32 bit defines, because a 64 bit system define
 * both types; 32 and 64 bit.
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

    #ifndef WINDOWS
        #define WINDOWS ///< For windows system
    #else
        #error "The macro \"WINDOWS\" is already defined !"
    #endif /* WINDOWS */

    /**
     * Placeholder macro, because with Windows and with some compilers a special format string for the type size_t is
     * necessary.
     * The correct type depends on the bit width of the system.
     */
    #ifdef _WIN64
        #ifndef PRI_SIZET
            #define PRI_SIZET PRIu64
        #else
            #error "The macro \"PRI_SIZET\" is already defined !"
        #endif /* PRI_SIZET */
        
        #ifndef WINDOWS_64
            #define WINDOWS_64 ///< Windows and 64 bit
        #else
            #error "The macro \"WINDOWS_64\" is already defined !"
        #endif /* WINDOWS_64 */
    #else
        #ifndef PRI_SIZET
            #define PRI_SIZET PRIu32
        #else
            #error "The macro \"PRI_SIZET\" is already defined !"
        #endif /* PRI_SIZET */
        
        #ifndef WINDOWS_32
            #define WINDOWS_32 ///< Windows and 32 bit
        #else
            #error "The macro \"WINDOWS_32\" is already defined !"
        #endif /* WINDOWS_32 */
    #endif
#else

    /**
     * On other systems the "Windows size_t" macro will be replace with the normal format string ("zu").
     */
    #ifndef PRI_SIZET
        #define PRI_SIZET "zu"
    #else
        #error "The macro \"PRI_SIZET\" is already defined !"
    #endif /* PRI_SIZET */
#endif /* defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) */



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OS_SPECIFIC_CONFIGURATIONS_H */
