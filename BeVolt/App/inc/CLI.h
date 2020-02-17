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

// String hash values
#define HELP        3443615
#define MENU        3594497
#define VOLTAGE     485505740
#define CURRENT     117203550
#define TEMPERATURE 801551920
#define LTC         98843
#define REGISTER    985857826
#define CONTACTOR   85684648
#define SWITCH      72420721
#define CHARGE      990159610
#define LED         99339
#define LIGHTS      402681384
#define CAN         108816
#define CANBUS      403432629
#define DISPLAY     893981360
#define WATCHDOG    811524042
#define EEPROM      226595301
#define ADC_HASH    98336
#define CRITICAL    368282223
#define ABORT       110634416
#define ALL         107233

#define MODULE      994869698
#define TOTAL       102745028
#define SAFE        3110035
#define SAFETY      574365715
#define HIGH        3200606
#define LOW         117908
#define STATE       96828305
#define CHARGING    66671398
#define ON          3521
#define OFF         101295
#define SET         114722
#define RESET       110251087
#define TEST        3569518
#define READ        3075562
#define WRITE       96835935
#define ERROR       108701384

#define FAULT       110461410
#define UVOLT       110456310
#define OVOLT       110456304
#define OTEMP       106782339
#define OCURR       108793185
#define WDOG        3178363
#define EXTRA       93093008

#define PARTYTIME   958393139
#define PING        3177550

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
void CLI_Startup(void);

/** CLI_Help
 * Displays the help menu
 */
void CLI_Help(void);

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

/** CLI_Handler
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Handler(char *input);

#endif
