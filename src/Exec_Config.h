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
    WORD_OFFSET     = 1 << 6,   ///< Word offset in the result file ?
    CASE_SENSITIVE  = 1 << 7,   ///< Case sensitive comparison of the tokens ?

    // Additional settings
    SHORTEN_OUTPUT              = 1 << 8,   ///< Shorten the output file ? (E.g. with removing the formation of the result JSON file)
    NO_FILENAMES                = 1 << 9,   ///< Don't show the input file names in the general info block.
    NO_CREATION_TIME            = 1 << 10,  ///< Don't show the creation time in the general info block.
    NO_PROGRAM_VERSION          = 1 << 11,  ///< Don't show the program version in the general info block.
    KEEP_SINGLE_TOKEN_RESULTS   = 1 << 12,  ///< Keep results with only one token
    SHOW_TOO_LONG_TOKENS        = 1 << 13   ///< Show and save too long tokens in the result file
};

/**
 * Macros to detect the bits with more comfort
 */
// > Main settings <
#ifndef PART_MATCH_BIT
#define PART_MATCH_BIT(input) ((input) & PART_MATCH) ///< Is PART_MATCH bit set ?
#else
#error "The macro \"PART_MATCH_BIT\" is already defined !"
#endif /* PART_MATCH_BIT */

#ifndef FULL_MATCH_BIT
#define FULL_MATCH_BIT(input) ((input) & FULL_MATCH) ///< Is FULL_MATCH bit set ?
#else
#error "The macro \"FULL_MATCH_BIT\" is already defined !"
#endif /* FULL_MATCH_BIT */

#ifndef STOP_WORD_LIST_BIT
#define STOP_WORD_LIST_BIT(input) ((input) & STOP_WORD_LIST) ///< Is STOP_WORD_LIST bit set ?
#else
#error "The macro \"STOP_WORD_LIST_BIT\" is already defined !"
#endif /* STOP_WORD_LIST_BIT */

#ifndef CHAR_OFFSET_BIT
#define CHAR_OFFSET_BIT(input) ((input) & CHAR_OFFSET) ///< Is CHAR_OFFSET bit set ?
#else
#error "The macro \"CHAR_OFFSET_BIT\" is already defined !"
#endif /* CHAR_OFFSET_BIT */

#ifndef SENTENCE_OFFSET_BIT
#define SENTENCE_OFFSET_BIT(input) ((input) & SENTENCE_OFFSET) ///< Is SENTENCE_OFFSET bit set ?
#else
#error "The macro \"SENTENCE_OFFSET_BIT\" is already defined !"
#endif /* SENTENCE_OFFSET_BIT */

#ifndef WORD_OFFSET_BIT
#define WORD_OFFSET_BIT(input) ((input) & WORD_OFFSET) ///< Is WORD_OFFSET bit set ?
#else
#error "The macro \"WORD_OFFSET_BIT\" is already defined !"
#endif /* WORD_OFFSET_BIT */

#ifndef CASE_SENSITIVE_BIT
#define CASE_SENSITIVE_BIT(input) ((input) & CASE_SENSITIVE) ///< Is CASE_SENSITIVE bit set ?
#else
#error "The macro \"CASE_SENSITIVE_BIT\" is already defined !"
#endif /* CASE_SENSITIVE_BIT */

// > Additional settings <
#ifndef SHORTEN_OUTPUT_BIT
#define SHORTEN_OUTPUT_BIT(input) ((input) & SHORTEN_OUTPUT) ///< Is SHORTEN_OUTPUT bit set ?
#else
#error "The macro \"SHORTEN_OUTPUT_BIT\" is already defined !"
#endif /* SHORTEN_OUTPUT_BIT */

/**
 * @brief Another way to check, whether the SHORTEN_OUTPUT bit is set. The idea is a more usable name for this bit.
 *
 * A shorten output is the opposite of a enabled formatting.
 */
#ifndef FORMATTING_ENABLED
#define FORMATTING_ENABLED(input) !(SHORTEN_OUTPUT_BIT(input))
#else
#error "The macro \"FORMATTING_ENABLED\" is already defined !"
#endif /* FORMATTING_ENABLED */

#ifndef NO_FILENAMES_BIT
#define NO_FILENAMES_BIT(input) ((input) & NO_FILENAMES) ///< Is NO_FILENAMES bit set ?
#else
#error "The macro \"NO_FILENAMES_BIT\" is already defined !"
#endif /* NO_FILENAMES_BIT */

#ifndef NO_CREATION_TIME_BIT
#define NO_CREATION_TIME_BIT(input) ((input) & NO_CREATION_TIME) ///< Is NO_CREATION_TIME bit set ?
#else
#error "The macro \"NO_CREATION_TIME_BIT\" is already defined !"
#endif /* NO_CREATION_TIME_BIT */

#ifndef NO_PROGRAM_VERSION_BIT
#define NO_PROGRAM_VERSION_BIT(input) ((input) & NO_PROGRAM_VERSION) ///< Is NO_PROGRAM_VERSION bit set ?
#else
#error "The macro \"NO_PROGRAM_VERSION_BIT\" is already defined !"
#endif /* NO_PROGRAM_VERSION_BIT */

#ifndef KEEP_SINGLE_TOKEN_RESULTS_BIT
#define KEEP_SINGLE_TOKEN_RESULTS_BIT(input) ((input) & KEEP_SINGLE_TOKEN_RESULTS) ///< Is KEEP_SINGLE_TOKEN_RESULTS bit set ?
#else
#error "The macro \"KEEP_SINGLE_TOKEN_RESULTS_BIT\" is already defined !"
#endif /* KEEP_SINGLE_TOKEN_RESULTS_BIT */

#ifndef SHOW_TOO_LONG_TOKENS_BIT
#define SHOW_TOO_LONG_TOKENS_BIT(input) ((input) & SHOW_TOO_LONG_TOKENS) ///< Is SHOW_TOO_LONG_TOKENS bit set ?
#else
#error "The macro \"SHOW_TOO_LONG_TOKENS_BIT\" is already defined !"
#endif /* SHOW_TOO_LONG_TOKENS_BIT */



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
