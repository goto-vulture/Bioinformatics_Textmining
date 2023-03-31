/**
 * @file TEST_cJSON_Parser.h
 *
 * @brief In this translation unit are a few tests for the cJSON parser usage.
 *
 * @date 06.07.2022
 * @author am1
 */

#ifndef TEST_CJSON_PARSER_H
#define TEST_CJSON_PARSER_H ///< Include-Guard

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Check, whether a JSON fragment can be parsed.
 */
extern void TEST_cJSON_Parse_JSON_Fragment (void);

/**
 * @brief Check, whether a token array can be parsed from a JSON file.
 */
extern void TEST_cJSON_Get_Token_Array_From_JSON_Fragment (void);

/**
 * @brief Checking the parsing of a whole file.
 */
extern void TEST_cJSON_Parse_Full_JSON_File (void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEST_CJSON_PARSER_H */
