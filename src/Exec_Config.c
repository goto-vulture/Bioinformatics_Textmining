/**
 * @file Exec_Config.c
 *
 * @date 23.10.2022
 * @author am4
 */

#include "Exec_Config.h"



/**
 * @brief All settings, except SENTENCE_OFFSET,
 *
 * @return The default settings encoded in a value.
 */
extern unsigned int Exec_Config_Default_Settings (void)
{
    return PART_MATCH | FULL_MATCH | STOP_WORD_LIST | CHAR_OFFSET | CASE_SENSITIVE;
}

/**
 * @brief Create a value with all token matching types.
 *
 * @return The created value with the meaning, that all token matching types should be used.
 */
extern unsigned int Exec_Config_All_Matches (void)
{
    return PART_MATCH | FULL_MATCH;
}
