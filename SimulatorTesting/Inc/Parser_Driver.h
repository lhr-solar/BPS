#ifndef PARSER_DRIVER_H
#define PARSER_DRIVER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "BSP_Total.h"

typedef enum { PARSE_SUCCESS, PARSE_FAIL, PARSE_UNEXPECTED } ParseResult_e;
static FILE* logFile;

void ParseInit(char* path);

/**
 * @brief These parse functions will go through the specified log file
 *        and search for the respective resources. If the log exists for
 *        the resource, and has appropriate values / outputs, the function
 *        returns a success. If the output is not correct, the parse fails.
 *        Otherwise, the parse result is unexpected, so we return that.
 * 
 * @return ParseResult_e 
 */

ParseResult_e ParseCAN();
ParseResult_e ParseContactor();
ParseResult_e ParseADC();
ParseResult_e ParseFans();
// do we need the communication protocal parsers??
ParseResult_e ParseI2C();
ParseResult_e ParseSPI();
ParseResult_e ParseUART();
// see above comment
ParseResult_e ParseLights();
ParseResult_e ParseTiming();

#endif