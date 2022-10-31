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

extern _Bool GLOBAL_CLI_NO_PART_MATCHES; ///< Do not show part matches in the result file

extern _Bool GLOBAL_CLI_NO_FULL_MATCHES; ///< Do not show full matches in the result file

/**
 * @brief On which percent in the calculation should be aborted ? This is for development and debugging purposes useful
 * to limit the calculation process.
 */
extern float GLOBAL_ABORT_PROCESS_PERCENT;



/**
 * @brief Testfunktion fuer den CLI Parameter, der die erste Eingabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE (void);

/**
 * @brief Testfunktion fuer den CLI Parameter, der die zweite Eingabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_INPUT_FILE2 (void);

/**
 * @brief Testfunktion fuer den CLI Parameter, der die Ausgabedatei beschreibt.
 */
extern void Check_CLI_Parameter_CLI_OUTPUT_FILE (void);

/**
 * @brief Testfunction for the abort percent value.
 */
extern void Check_CLI_Parameter_GLOBAL_ABORT_PROCESS_PERCENT (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
