/** config.h
 * Config file to hold any aliases/constants referenced by multiple files
 * Not specific to a single board/unit
 */
 
#ifndef CONFIG_H__
#define CONFIG_H__

#include "stm32f4xx.h"

typedef enum {SAFE = 0, DANGER = 1} SafetyStatus;
typedef ErrorStatus Status;

// NUCLEO or custom
#define NUCLEO		0		// Change 1 if using nucleo, 0 if not

//--------------------------------------------------------------------------------
// Basic Parameters of BPS layout
#define NUM_VOLTAGE_BOARDS				1			// Number of Voltage Slave Boards
#define NUM_TEMPERATURE_BOARDS			3			// Number of Temperature Slave Boards

//--------------------------------------------------------------------------------
// Battery Pack layout
#define NUM_BATTERY_MODULES				12		// Number of battery modules
#define NUM_TEMPERATURE_SENSORS			62		// Number of temperature sensors
#define NUM_TEMP_SENSORS_PER_MOD		2		// Number of temperature sensors per battery module

//--------------------------------------------------------------------------------
// MAX and MIN limits of lithium ion cells
#define MIN_VOLTAGE_LIMIT				2.7		// Under voltage limit (Voltage)	(actual min: 2.5V)
#define MAX_VOLTAGE_LIMIT				4.0		// Over voltage limit (Voltage)		(actual max: 4.2V)

#define MAX_DISCHARGE_TEMPERATURE_LIMIT	73.00	// Max temperature limit (Celcius)	(actual max: 75C)
#define MAX_CHARGE_TEMPERATURE_LIMIT	48.00	// Max temperature limit (Celcius)	(actual max: 50C)

#define MAX_CURRENT_LIMIT				100		// Max current limit (Amperes)		(Max continuous discharge is 15A per cell)

//--------------------------------------------------------------------------------
// Voltage Board Sensor Configurations
// Define how many voltage sensors are connected to each board
#define NUM_SENSORS_ON_VOLT_BOARD_1		12		// Number of voltage sensors used on board 1 (12 channels max)
#define NUM_SENSORS_ON_VOLT_BOARD_2		12		// Number of voltage sensors used on board 2 (12 channels max)
#define NUM_SENSORS_ON_VOLT_BOARD_3		7		// Number of voltage sensors used on board 3 (12 channels max)

//--------------------------------------------------------------------------------
// Temperature Board Sensor Configurations
// Define how many temperature sensors are connected to each board
#define NUM_SENSORS_ON_TEMP_BOARD_1		1		// Number of temperature channels used in board 1 (20 channels max)
#define NUM_SENSORS_ON_TEMP_BOARD_2		20		// Number of temperature channels used in board 2 (20 channels max)
#define NUM_SENSORS_ON_TEMP_BOARD_3		20		// Number of temperature channels used in board 3 (20 channels max)
#define NUM_SENSORS_ON_TEMP_BOARD_4		20		// Number of temperature channels used in board 4 (20 channels max)
#define NUM_SENSORS_ON_TEMP_BOARD_5		20		// Number of temperature channels used in board 5 (20 channels max)
#define NUM_SENSORS_ON_TEMP_BOARD_6		20		// Number of temperature channels used in board 6 (20 channels max)

typedef enum board {
	TEMP_CS1,
	TEMP_CS2,
	TEMP_CS3,
	TEMP_CS4,
	TEMP_CS5,
	TEMP_CS6
} board;

#endif
