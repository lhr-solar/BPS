/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/* CLI.c
 * Command Line Interface file to define and route commands
 */

#ifndef CLI_H
#define CLI_H

#include "common.h"
#include "config.h"
#include "BSP_Lights.h"
#include "Voltage.h"

// String hash values
#define CLI_CHAR_BASE       33
#define CLI_VOLTAGE         CLI_CHAR_BASE + 0  
#define CLI_CURRENT         CLI_CHAR_BASE + 1  
#define CLI_TEMPERATURE     CLI_CHAR_BASE + 2  
#define CLI_CHARGE          CLI_CHAR_BASE + 3  
#define CLI_DISPLAY         CLI_CHAR_BASE + 4  
#define CLI_WATCHDOG        CLI_CHAR_BASE + 5  
#define CLI_EEPROM          CLI_CHAR_BASE + 6  
#define CLI_ADC             CLI_CHAR_BASE + 7  
#define CLI_OPENWIRE        CLI_CHAR_BASE + 8
#define CLI_FAULT           CLI_CHAR_BASE + 9
#define CLI_ALL             CLI_CHAR_BASE + 10  
#define CLI_CRITICAL        CLI_CHAR_BASE + 11  
#define CLI_CAN             CLI_CHAR_BASE + 12  
#define CLI_LTC             CLI_CHAR_BASE + 13  
#define CLI_CONTACTOR       CLI_CHAR_BASE + 14  
#define CLI_PING            CLI_CHAR_BASE + 15
#define CLI_REGISTER        CLI_CHAR_BASE + 16  
#define CLI_SWITCH          CLI_CHAR_BASE + 17  
#define CLI_LED             CLI_CHAR_BASE + 18  
// #define CLI_LIGHTS          (CLI_CHAR_BASE + 19)  
// #define CLI_CANBUS          (CLI_CHAR_BASE + 20)  
// #define CLI_ABORT           (CLI_CHAR_BASE + 21)  
// 
// #define CLI_MODULE         (CLI_CHAR_BASE + 19)  
// #define CLI_TOTAL          (CLI_CHAR_BASE + 20)  
// #define CLI_SAFE           (CLI_CHAR_BASE + 21)  
// #define CLI_SAFETY         (CLI_CHAR_BASE + 22)  
// #define CLI_HIGH           (CLI_CHAR_BASE + 23)  
// #define CLI_LOW            (CLI_CHAR_BASE + ul)  
// #define CLI_CHARGING_STATE (CLI_CHAR_BASE + ul)  
// #define CLI_ON             (CLI_CHAR_BASE + ul)  
// #define CLI_OFF            (CLI_CHAR_BASE + ul)  
// #define CLI_SET            (CLI_CHAR_BASE + ul)  
// #define CLI_RESET          (CLI_CHAR_BASE + ul)  
// #define CLI_TEST           (CLI_CHAR_BASE + ul)  
// #define CLI_READ           (CLI_CHAR_BASE + ul)  
// #define CLI_WRITE          (CLI_CHAR_BASE + ul)  
// #define CLI_ERROR          (CLI_CHAR_BASE + ul)  
// 
// #define CLI_RUN		    
// #define CLI_UVOLT          (CLI_CHAR_BASE + ul)
// #define CLI_OVOLT          (CLI_CHAR_BASE + ul)
// #define CLI_OTEMP          (CLI_CHAR_BASE + ul)
// #define CLI_OCURR          (CLI_CHAR_BASE + ul)
// #define CLI_WDOG           (CLI_CHAR_BASE + ul)
// #define CLI_EXTRA          (CLI_CHAR_BASE + ul)
// 
// 
// #define CLI_TRIP   		
// #define CLI_CLEAR		   (CLI_CHAR_BASE + ul) 
// 
// #define CLI_SHUTDOWN       (CLI_CHAR_BASE + ul)
// #define CLI_PARTYTIME      (CLI_CHAR_BASE + ul)

// Useful values
#define NUM_TEMP_SENSORS_LAST_MINION    14


/** CLI_Init
 * Initializes the CLI with the values it needs
 * @param minions is a cell_asic struct describing the LTC6811
 */
//void CLI_Init(cell_asic* minions);
void CLI_Init(void);

/** CLI_InputParse
 * Parses the input string and stores in cmd[]
 * @param input is a pointer to the input string
 * @param parsedTokens is an array to pass the parsed words back
 */
void CLI_InputParse(char* input, int* parsedTokens); 

/** CLI_StringHash
 * Calculates the hashed value of the input
 * @param string is the string to be hashed
 * @return the integer hash value
 */ 
uint32_t CLI_StringHash(char* string);

/** CLI_Startup
 * Prints the welcome messages
 */
//void CLI_Startup(void);

/** CLI_Help
 * Displays the help menu
 */
void CLI_Help(void);

/** CLI_Voltage
 * Checks and displays the desired voltage parameter(s)
 * @param cmd is the array of command bytes
 */
void CLI_Voltage(char *cmd);

/** CLI_Current
 * Checks and displays the desired current parameter(s)
 * @param cmd is the array of command bytes
 */
void CLI_Current(char *cmd);

/** CLI_Temperature
 * Checks and displays the desired temperature parameter(s)
 * @param cmd is the array of command bytes
 */
void CLI_Temperature(char *cmd);

/** CLI_LTC6811
 * Prints register information. All registers are of the same type
 * Prints each register's respective three registers: tx_data, rx_date, and rx_pec_match
 */
void CLI_LTC6811(void);

/** CLI_Contactor
 * Interacts with contactor status by printing the status of the contactor
 * @param cmd is the array of command bytes
 */
void CLI_Contactor(char *cmd);

/** CLI_Charge
 * Checks and displays the desired state of charge parameter(s)
 * @param cmd is the array of command bytes
 */
void CLI_Charge(char *cmd);

/** setLED
 * Helper function for CLI_LED that sets a given LED to a state
 * @param led is the LED to toggle
 * @param state is the 'on' of 'off' state
 * 				represented by a 1/0 or hashes
 */
void setLED(Light input, int state);

/** CLI_LED
 * Interacts with the LEDs by checking error light status, running a full LED test, and turning a specific LED on/off
 * @param cmd is the array of command bytes
 */
void CLI_Led(char *cmd);

/** CLI_CAN
 * Interacts with CAN by reading and writing to bus
 * @param cmd is the array of command bytes
 */
void CLI_Can(char *cmd);

/** CLI_Display
 * Interacts with the display
 */
void CLI_Display(void);

///** CLI_Watchdog
// * Shows whether watchdog was tripped
// * @param cmd is the array of command bytes
// */
//void CLI_Watchdog(char *cmd);
//
///** CLI_EEPROM
// * Interacts with EEPROM by reading and writing to the EEPROM
// * @param cmd is the array of command bytes
// */
//void CLI_EEPROM(char *cmd);
//
///** CLI_ADC
// * Prints the high precision and
// * low precision readings
// */
void CLI_Adc(void);

/** CLI_Critical
 * Shuts off contactor manually
 */  
void CLI_Critical(void);

/** CLI_All
 * Displays all information about BPS modules: voltage, current, temperature, charge, contactor
 */
void CLI_All(void);

void CLI_Ping(void);

/** CLI_Handler
 * Routes the command given to the proper measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Handler(char* cmd);

#endif
