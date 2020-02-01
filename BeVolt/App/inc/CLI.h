/* CLI.c
 * Command Line Interface file to 
 * define and route commands
 */

#ifndef CLI_H__
#define CLI_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "Contactor.h"
#include "CAN.h"
#include "WDTimer.h"
#include "LED.h"
#include "config.h"
#include "SoC.h"
#include "CAN.h"
#include "SysTick.h"
#include "UART.h"


void CLI_Init(void);

void CLI_Handler(void);

/** CLI_Commands
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input command
 */
void CLI_Commands(char *input);

/** CLI_Startup
 * Asks if batteries needs to be charged
 * @return true or false
 */
bool CLI_Startup(void);

void CLI_InputParse(char *input);

char* CLI_GetToken(uint8_t idx);

void CLI_Help();

void CLI_LED();

#endif
