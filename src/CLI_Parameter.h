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



#include <stdbool.h>



extern const char* const GLOBAL_USAGES []; ///< Description of the CLI interface usage

extern const char* const GLOBAL_PROGRAM_DESCRIPTION; ///< General program description
extern const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION; ///< Additional program description



extern char* GLOBAL_INPUT_FILE; ///< Input file
extern char* GLOBAL_OUTPUT_FILE; ///< Output file
extern _Bool GLOBAL_RUN_ALL_TEST_FUNCTIONS; ///< Run all test functions ?



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
