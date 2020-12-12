/* Copyright (c) 2020 UT Longhorn Racing Solar */

/** config.h
 * Config file to hold any aliases/constants referenced by multiple files
 * Not specific to a single board/unit
 */

#ifndef CONFIG_H__
#define CONFIG_H__

#include "common.h"

typedef enum {OFF = 0, ON} State;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum {SAFE = 0, DANGER = 1, OVERVOLTAGE = 2, UNDERVOLTAGE = 3, OPENWIRE = 4} SafetyStatus;

// NUCLEO or custom
#define COMMAND_SIZE			128
#define NUCLEO		0		// Change 1 if using nucleo, 0 if not

//--------------------------------------------------------------------------------
// Basic Parameters of BPS layout
#define NUM_MINIONS	4					 // Number of minion boards
												//

//--------------------------------------------------------------------------------
// Battery Pack layout
#define NUM_BATTERY_MODULES				31		// Number of battery modules
#define NUM_TEMPERATURE_SENSORS			62		// Number of temperature sensors
#define NUM_TEMP_SENSORS_PER_MOD		2		// Number of temperature sensors per battery module

//--------------------------------------------------------------------------------
// MAX and MIN limits of lithium ion cells
#define MIN_VOLTAGE_LIMIT				2.7		// Under voltage limit (Voltage)	(actual min: 2.5V)
#define MAX_VOLTAGE_LIMIT				4.0		// Over voltage limit (Voltage)		(actual max: 4.2V)

#define MAX_DISCHARGE_TEMPERATURE_LIMIT	73.00	// Max temperature limit (Celcius)	(actual max: 75C)
#define MAX_CHARGE_TEMPERATURE_LIMIT	48.00	// Max temperature limit (Celcius)	(actual max: 50C)

#define MAX_CURRENT_LIMIT				100000		// Max current limit (Milliamperes)		(Max continuous discharge is 15A per cell)
#define MAX_HIGH_PRECISION_CURRENT 		50000		// Max current detectable by the high-precision current sensor (mA)
#define MAX_CHARGING_CURRENT 			-20000		// Max current per cell is 1.5 Amps (Standard charge)

//--------------------------------------------------------------------------------
// Helpers
#define MILLI_SCALING_FACTOR			1000
#define STARTUP_WAIT_TIME					100000 // Number of iterations to wait for battery charging instructions on startup

//--------------------------------------------------------------------------------
// Voltage Sensor Configurations
// Defines how many voltage sensors are connected to each board
#define MAX_VOLT_SENSORS_PER_MINION_BOARD	8	// User defined. The LTC6811 can actually measure 12 modules.
#define NUM_PINS_PER_LTC 					9
#define TOTAL_VOLT_WIRES					NUM_PINS_PER_LTC*NUM_MINIONS - 1

//--------------------------------------------------------------------------------
// Temperature Sensor Configurations
// Define how many temperature sensors are connected to each board
#define MAX_TEMP_SENSORS_PER_MINION_BOARD	16

//--------------------------------------------------------------------------------
// HeartBeat Delay Ticks
// Define heartbeatDelay as # of desired while(1) loops per toggle
#define HEARTBEAT_DELAY 0x10

#endif
