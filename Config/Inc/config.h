/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** config.h
 * Config file to hold any aliases/constants referenced by multiple files
 * Not specific to a single board/unit
 */

#ifndef CONFIG_H__
#define CONFIG_H__

#include "common.h"

typedef enum State_e {OFF = 0, ON} State;
typedef enum ErrorStatus_e {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum SafetyStatus_e {SAFE = 0, DANGER = 1, OVERVOLTAGE = 2, UNDERVOLTAGE = 3} SafetyStatus;

//--------------------------------------------------------------------------------
// Basic Parameters of BPS layout

#define NUM_MINIONS	3					 // Number of minion boards

//--------------------------------------------------------------------------------
// Battery Fault Checks
#define NUM_FAULT_POINTS				4

//--------------------------------------------------------------------------------
// Battery Pack layout
#ifndef NUM_BATTERY_MODULES
#define NUM_BATTERY_MODULES				((12 * NUM_MINIONS) > 32) ?	32 : (12 * NUM_MINIONS)	// Number of battery modules
#endif

#ifndef NUM_TEMP_SENSORS_PER_MOD
#define NUM_TEMP_SENSORS_PER_MOD		1		// Number of temperature sensors per battery module
#endif

#ifndef NUM_TEMPERATURE_SENSORS
#define NUM_TEMPERATURE_SENSORS			((16 * NUM_MINIONS) > 32) ?	32 : (16 * NUM_MINIONS) // Number of temperature sensors
#endif

//--------------------------------------------------------------------------------
// MAX and MIN limits of lithium ion cells
#ifndef MIN_VOLTAGE_CHARGING_LIMIT
#define MIN_VOLTAGE_CHARGING_LIMIT      2550        // Under voltage limit for charging (milliVolts) 
#endif

#ifndef MIN_VOLTAGE_LIMIT
#define MIN_VOLTAGE_LIMIT				2700		// Under voltage limit (milliVolts)	(actual min: 2.5V)
#endif

#ifndef MAX_VOLTAGE_LIMIT
#define MAX_VOLTAGE_LIMIT				4100		// Over voltage limit (milliVolts)		(actual max: 4.2V)
#endif

#ifndef CHARGE_DISABLE_VOLTAGE
#define CHARGE_DISABLE_VOLTAGE          4000        // Voltage to stop charging at
#endif

// make sure we don't enable charging if we're too close to the voltage limit
#if MAX_VOLTAGE_LIMIT - 100 < CHARGE_DISABLE_VOLTAGE
#error "Charging maximum voltage is too close to voltage trip limit!"
#endif

#ifndef MAX_DISCHARGE_TEMPERATURE_LIMIT
#define MAX_DISCHARGE_TEMPERATURE_LIMIT	60000	    // Max temperature limit (milliCelcius)	(recommended release: 60.00C)
#endif

#ifndef MAX_CHARGE_TEMPERATURE_LIMIT
#define MAX_CHARGE_TEMPERATURE_LIMIT	45000	    // Max temperature limit (milliCelcius)	(recommended release: 45.00C)
#endif

#ifndef PID_DESIRED_TEMPERATURE
#define PID_DESIRED_TEMPERATURE         38000       // Desired temperature   (milliCelcius) 
#endif

#ifndef MAX_CURRENT_LIMIT
#define MAX_CURRENT_LIMIT				75000		// Max current limit (Milliamperes)		(Max continuous discharge is 15A per cell)
#endif

#ifndef MAX_CHARGING_CURRENT
#define MAX_CHARGING_CURRENT 			-20000		// Max current per cell is 1.5 Amps (Standard charge)
#endif

#ifndef BALANCING_TOLERANCE_START
#define BALANCING_TOLERANCE_START       50          //mV differential from lowest module that will start battery balancing
#endif

#ifndef BALANCING_TOLERANCE_STOP
#define BALANCING_TOLERANCE_STOP        20          //mV differential from lowest module that will stop battery balancing
#endif
//--------------------------------------------------------------------------------
// Helpers
#define STARTUP_WAIT_TIME				100000 // Number of iterations to wait for battery charging instructions on startup

#define PWM_PERIOD                      4000   // Number of clock cycles per PWM period
//--------------------------------------------------------------------------------
// Voltage Sensor Configurations
// Defines how many voltage sensors are connected to each board
#define MAX_VOLT_SENSORS_PER_MINION_BOARD	12	// User defined. The LTC6811 can actually measure 12 modules.
#define TOTAL_VOLT_WIRES					(MAX_VOLT_SENSORS_PER_MINION_BOARD * NUM_MINIONS)

//--------------------------------------------------------------------------------
// Temperature Sensor Configurations
// Define how many temperature sensors are connected to each board
#define MAX_TEMP_SENSORS_PER_MINION_BOARD	16

//--------------------------------------------------------------------------------
// HeartBeat Delay Ticks
// Define heartbeatDelay as # of desired while(1) loops per toggle
#define HEARTBEAT_DELAY 0x10

//--------------------------------------------------------------------------------
//Maximum PEC Code Errors
//Defines the number of PEC code errors allowed before the BPS is tripped
#define MAX_PEC_ERRORS 5

// used to determine if we are charging
#define AMPS_NOISE_LIMIT 250 // milliamps

#endif
