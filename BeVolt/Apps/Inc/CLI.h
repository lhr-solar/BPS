/* CLI.c
 * Command Line Interface file to 
 * define and route commands
 */

#ifndef CLI_H__
#define CLI_H__

#include "common.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "BSP_Contactor.h"
#include "BSP_WDTimer.h"
#include "BSP_Lights.h"
#include "config.h"
#include "SoC.h"
#include "CANbus.h"
#include "BSP_CAN.h"
#include "Images.h"

// String hash values
#define CLI_HELP_HASH						0x348B9F
#define CLI_MENU_HASH           0x36D901
#define CLI_VOLTAGE_HASH        0x1CF03ACC
#define CLI_CURRENT_HASH        0x6FC625E
#define CLI_TEMPERATURE_HASH    0x2FC6B634
#define CLI_LTC_HASH            0x1821B
#define CLI_REGISTER_HASH       0x3AC2FF22
#define CLI_CONTACTOR_HASH      0x51B71A8
#define CLI_SWITCH_HASH         0x4510D71
#define CLI_CHARGE_HASH         0x3B04A2FA
#define CLI_LED_HASH            0x1840B
#define CLI_LIGHTS_HASH         0x18006E28
#define CLI_CAN_HASH            0x1A910
#define CLI_CANBUS_HASH         0x180BE4B5
#define CLI_DISPLAY_HASH        0x354912B0
#define CLI_WATCHDOG_HASH       0x305EDFCA
#define CLI_EEPROM_HASH         0xD8191E5
#define CLI_ADC_HASH            0x18020
#define CLI_CRITICAL_HASH       0x15F38A6F
#define CLI_ABORT_HASH          0x69825B0
#define CLI_ALL_HASH            0x1A2E1

#define CLI_MODULE_HASH         0x3B4C81C2
#define CLI_TOTAL_HASH          0x61FC3C4
#define CLI_SAFE_HASH           0x2F7493
#define CLI_SAFETY_HASH         0x223C2013
#define CLI_HIGH_HASH           0x30D65E
#define CLI_LOW_HASH            0x1CC94
#define CLI_CHARGING_STATE      0x3F95326
#define CLI_ON_HASH             0xDC1
#define CLI_OFF_HASH            0x18BAF
#define CLI_SET_HASH            0x1C022
#define CLI_RESET_HASH          0x6924C4F
#define CLI_TEST_HASH           0x36776E
#define CLI_READ_HASH           0x2EEDEA
#define CLI_WRITE_HASH          0x5C5995F
#define CLI_ERROR_HASH          0x67AA6C8

#define CLI_FAULT_HASH          0x69581E2
#define CLI_RUN_HASH		    		0x1AB8B
#define CLI_UVOLT_HASH          0x6956DF6
#define CLI_OVOLT_HASH          0x6956DF0
#define CLI_OTEMP_HASH          0x65D5E83
#define CLI_OCURR_HASH          0x67C0D61
#define CLI_WDOG_HASH           0x307F7B
#define CLI_EXTRA_HASH          0x58C7C90

#define CLI_TRIP_HASH   				0x3481FB
#define CLI_CLEAR_HASH					0x674180D

#define CLI_SHUTDOWN_HASH       0xE7D4586
#define CLI_PARTYTIME_HASH      0x391FEB33
#define CLI_PING_HASH           0x307C4E

// Useful values
#define NUM_TEMP_SENSORS_LAST_MINION    14

/** CLI_Init
 * Initializes the CLI with the values it needs
 * @param minions is a cell_asic struct describing the LTC6811
 */
void CLI_Init(cell_asic* minions);

/** CLI_InputParse
 * Parses the input string and stores in hashTokens[]
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
void CLI_Startup(void);

/** CLI_Help
 * Displays the help menu
 */
void CLI_Help(void);

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 * @param hashTokens is the array of hashed tokens
 */
void CLI_Voltage(int* hashTokens);

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 * @param hashTokens is the array of hashed tokens
 */
void CLI_Current(int* hashTokens);

/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 * @param hashTokens is the array of hashed tokens
 */
void CLI_Temperature(int* hashTokens);

/** CLI_LTC6811
 * Prints register information
 * All registers are of the same type
 * Prints each register's respective three registers
 * (tx_data, rx_date, and rx_pec_match)
 */
void CLI_LTC6811(void);

/** CLI_Contactor
 * Interacts with contactor status by
 * printing the status of the contactor
 * @param hashTokens is the array of hashed tokens
 */
void CLI_Contactor(int* hashTokens);

/** CLI_Charge
 * Checks and displays the desired
 * state of charge parameter(s)
 * @param hashTokens is the array of hashed tokens
 */
void CLI_Charge(int* hashTokens);

/** setLED
 * Helper function for CLI_LED
 * that sets a given LED to a state
 * @param led is the LED to toggle
 * @param state is the 'on' of 'off' state
 * 				represented by a 1/0 or hashes
 */
void setLED(Led input, int state);

/** CLI_LED
 * Interacts with the LEDs by 
 * checking error light status
 * running a full LED test
 * and turning a specific LED on/off
 * @param hashTokens is the array of hashed tokens
 */
void CLI_LED(int* hashTokens);

/** CLI_CAN
 * Interacts with CAN by
 * reading and writing to bus
 * @param hashTokens is the array of hashed tokens
 */
void CLI_CAN(int* hashTokens);

/** CLI_Display
 * Interacts with the display
 */
void CLI_Display(void);

/** CLI_Watchdog
 * Shows whether watchdog was tripped
 * @param hashTokens is the array of hashed tokens
 */
void CLI_Watchdog(int* hashTokens);

/** CLI_EEPROM
 * Interacts with EEPROM
 * by reading and writing to the EEPROM
 * @param hashTokens is the array of hashed tokens
 */
void CLI_EEPROM(int* hashTokens);

/** CLI_ADC
 * Prints the high precision and
 * low precision readings
 */
void CLI_ADC(void);

/** CLI_Critical
 * Shuts off contactor manually
 */  
void CLI_Critical(void);

/** CLI_All
 * Displays all information about BPS modules
 * (voltage, current, temperature, charge, contactor)
 */
void CLI_All(void);

/** CLI_Handler
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Handler(char* input);

#endif
