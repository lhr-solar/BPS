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
#include "WDTimer.h"
#include "LED.h"
#include "config.h"
#include "SoC.h"
#include "CAN.h"
#include "SysTick.h"
#include "UART.h"
#include "Images.h"

/** CLI_Watchdog
 * Determines whether system had previously
 * reset on startup and if so outputs the
 * most recent Watchdog error
 */
void CLI_Watchdog(void);

/** CLI_EEPROM
 * Interacts with EEPROM
 */
void CLI_EEPROM(void);

/** CLI_Peripherals
 * Interacts with the ADC
 */
void CLI_ADC(void);

/** CLI_Critical
 * Shuts off contactor if battery sensors
 * detect unsafe levels
 */  
void CLI_Critical(void);

/** CLI_Commands
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Commands(char *input);

#endif
