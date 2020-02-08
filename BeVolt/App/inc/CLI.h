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

/** CLI_Init
 * Initializes the CLI with the values it needs
 * @param minions is a cell_asic struct describing the LTC6811
 */
void CLI_Init(cell_asic* minions);

/** CLI_InputParse
 * Parses the input string and stores in tokens[]
 * @param input is a pointer to the input string
 */
void CLI_InputParse(char *input); 

/** CLI_Startup
 * Prints the welcome messages and
 * asks for the override command
 * @return true or false
 */
bool CLI_Startup();

/** CLI_Help
 * Displays the help menu
 */
void CLI_Help();

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 */
void CLI_Voltage(void);

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 */
void CLI_Current(void);

/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 */
void CLI_Temperature(void);

/** CLI_LTC6811
 * Interacts with LTC6811 registers
 */
void CLI_LTC6811(void);

/** CLI_Contactor
 * Interacts with contactor status by
 * printing the status of the contactor
 */
void CLI_Contactor(void);

/** CLI_Charge
 * Checks and displays the desired
 * state of charge parameter(s)
 */
void CLI_Charge(void);

/** toggleLED
 * Helper function for CLI_LED
 * that toggles a given led
 * @param led is the led to toggle
 */
void toggleLED(led input);

/** CLI_LED
 * Interacts with the LEDs by 
 * checking error light status
 * running a full LED test
 * and turning a specific LED on/off
 */
void CLI_LED(void);

/** CLI_CAN
 * Interacts with CAN
 */
void CLI_CAN(void);

/** CLI_Display
 * Interacts with the display
 */
void CLI_Display(void);

/** CLI_Watchdog
 * Shows whether watchdog was tripped
 */
void CLI_Watchdog(void);

/** CLI_EEPROM
 * Interacts with EEPROM
 * by reading and writing to the EEPROM
 */
void CLI_EEPROM(void);

/** CLI_ADC
 * Prints the high precision and
 * low precision readings
 */
void CLI_ADC(void);

/** CLI_Critical
 * Shuts off contactor manually
 */  
void CLI_Critical(void);

/** CLI_Commands
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Commands(char *input);

#endif
