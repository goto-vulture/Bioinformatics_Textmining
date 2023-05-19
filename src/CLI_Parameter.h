/**
 * @file CLI_Parameter.h
 *
 * @brief CLI parameter of the program.
 *
 * Implemented with global variables. A access at every point is for the usage necessary.
 *
 * @date 22.01.2022
 * @author: am1
 */

#ifndef CLI_PARAMETER_H
#define CLI_PARAMETER_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */





extern const char* const GLOBAL_USAGES []; ///< Description of the CLI interface usage

extern const char* const GLOBAL_PROGRAM_DESCRIPTION; ///< General program description
extern const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION; ///< Additional program description



// Variables for the parsed CLI parameter
extern const char* GLOBAL_CLI_INPUT_FILE; ///< First input file

extern const char* GLOBAL_CLI_INPUT_FILE2; ///< Second input file

extern const char* GLOBAL_CLI_OUTPUT_FILE; ///< Output file

extern _Bool GLOBAL_CLI_FORMAT_OUTPUT; ///< Format the output ?

extern _Bool GLOBAL_RUN_ALL_TEST_FUNCTIONS; ///< Run all test functions ?

extern _Bool GLOBAL_CLI_SENTENCE_OFFSET; ///< Create sentence offsets in the calculation ?

extern _Bool GLOBAL_CLI_WORD_OFFSET; ///< Create word offsets in the calculation ?

extern _Bool GLOBAL_CLI_SHOW_TOO_LONG_TOKENS; ///< Show too long tokens in the result file

extern _Bool GLOBAL_CLI_NO_PART_MATCHES; ///< Do not show part matches in the result file

extern _Bool GLOBAL_CLI_NO_FULL_MATCHES; ///< Do not show full matches in the result file

extern _Bool GLOBAL_CLI_NO_TIMESTAMP; ///< Don't save the creation timestamp of the export file in the General JSON block

extern _Bool GLOBAL_CLI_NO_CPU_EXTENSIONS; ///< Don't use any CPU extensions, even there are available on the host

extern _Bool GLOBAL_CLI_CASE_SENSITIVE_TOKEN_COMPARISON; ///< Do a case-insensitive token comparison ?

/**
 * @brief Shall be results with only one token written in the result file ? In normal cases such information are
 * useless.
 */
extern _Bool GLOBAL_CLI_KEEP_RESULTS_WITH_ONE_TOKEN;

/**
 * @brief On which percent in the calculation should be aborted ? This is for development and debugging purposes useful
 * to limit the calculation process.
 */
extern float GLOBAL_ABORT_PROCESS_PERCENT;



/**
 * @brief Check, whether the given CLI parameter have a logical consistency.
 *
 * E.g. --no_part_matches and --no_full_matches makes no sense.
 */
extern void Check_CLI_Parameter_Logical_Consistency (void);

/**
 * @brief @brief Test function for the CLI parameter, that is used as first input file name.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE (void);

/**
 * @brief Test function for the CLI parameter, that is used as second input file name.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE2 (void);

/**
 * @brief Test function for the CLI parameter, that is used as output file name.
 */
extern void Check_CLI_Parameter_CLI_OUTPUT_FILE (void);

/**
 * @brief Test function for the abort percent value.
 */
extern void Check_CLI_Parameter_GLOBAL_ABORT_PROCESS_PERCENT (void);

/**
 * @brief Set all CLI parameter to the default values.
 *
 * This is only useful for the TEST functions.
 */
extern void Set_CLI_Parameter_To_Default_Values (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
