/**
 * @file Exec_Config.h
 *
 * @date 23.10.2022
 * @author: am4
 */

#ifndef EXEC_CONFIG_H
#define EXEC_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief A enum with all possible switches.
 *
 * For the result the classic trick with binary ORing should be used.
 */
enum Exec_Config_Types
{
    ETC             = 1 << 0,   ///< This is a placeholder without a technical meaning

    PART_MATCH      = 1 << 1,   ///< Show partial matching in the result file ?
    FULL_MATCH      = 1 << 2,   ///< Show full matching in the result file ?
    STOP_WORD_LIST  = 1 << 3,   ///< Using a stop word list to filter the input tokens ? (This is recommend)
    CHAR_OFFSET     = 1 << 4,   ///< Char offset in the result file ?
    SENTENCE_OFFSET = 1 << 5,   ///< Sentence offset in the result file ?
    CASE_SENSITIVE  = 1 << 6,   ///< Case sensitive comparison of the tokens ?

    // Additional settings
    SHORTEN_OUTPUT      = 1 << 7,   ///< Shorten the output file ? (E.g. with removing the formation of the result JSON file)
    NO_FILENAMES        = 1 << 8,   ///< Don't show the input file names in the general info block.
    NO_CREATION_TIME    = 1 << 9,   ///< Don't show the creation time in the general info block.
    NO_PROGRAM_VERSION  = 1 << 10   ///< Don't show the program version in the general info block.
};

/**
 * @brief All settings, except SENTENCE_OFFSET,
 *
 * @return The default settings encoded in a value.
 */
extern unsigned int Exec_Config_Default_Settings (void);

/**
 * @brief Create a value with all token matching types.
 *
 * @return The created value with the meaning, that all token matching types should be used.
 */
extern unsigned int Exec_Config_All_Matches (void);

/**
 * @brief Create the config to suppress all additional info in the result file.
 *
 * @return The config for no additional infos.
 */
extern unsigned int Exec_Config_No_Additional_Infos (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EXEC_CONFIG_H */
