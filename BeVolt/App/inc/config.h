/** config.h
 * Config file to hold any aliases/constants referenced by multiple files
 * Not specific to a single board/unit
 */

#ifndef CONFIG_H__
#define CONFIG_H__

#include <stdint.h>
#include "stm32f4xx.h"

typedef enum {SAFE = 0, DANGER = 1, OVERVOLTAGE = 2, UNDERVOLTAGE = 3} SafetyStatus;

// NUCLEO or custom
#define NUCLEO		0		// Change 1 if using nucleo, 0 if not

//--------------------------------------------------------------------------------
// Basic Parameters of BPS layout
#define NUM_MINIONS						4		// Number of minion boards
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

#define MAX_CURRENT_LIMIT				100		// Max current limit (Amperes)		(Max continuous discharge is 15A per cell)
#define MAX_HIGH_PRECISION_CURRENT 		50		// Max current detectable by the high-precision current sensor

//--------------------------------------------------------------------------------
// Helpers
#define MILLI_SCALING_FACTOR			1000

//--------------------------------------------------------------------------------
// Voltage Sensor Configurations
// Defines how many voltage sensors are connected to each board
#define MAX_VOLT_SENSORS_PER_MINION_BOARD	8	// User defined. The LTC6811 can actually measure 12 modules.

//--------------------------------------------------------------------------------
// Temperature Sensor Configurations
// Define how many temperature sensors are connected to each board
#define MAX_TEMP_SENSORS_PER_MINION_BOARD	16

#endif
