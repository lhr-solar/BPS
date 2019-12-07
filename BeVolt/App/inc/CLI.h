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
#include "LED.h"
#include "CAN.h"
#include "WDTimer.h"
#include "config.h"


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

#endif
