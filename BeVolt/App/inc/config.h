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

// Basic Parameters of BPS layout
#define NUM_BATTERY_MODULES			12		// Number of Battery Modules
#define NUM_VOLTAGE_BOARDS				1			// Number of Voltage Slave Boards
#define NUM_TEMPERATURE_BOARDS		3			// Number of Temperature Slave Boards

// Max and Min limits of lithium ion cells
#define MIN_VOLTAGE_LIMIT				2.7		// Under voltage limit (Voltage)		(actual min: 2.5V)
#define MAX_VOLTAGE_LIMIT				4.0		// Over voltage limit (Voltage)		(actual max: 4.2V)

#define MAX_DISCHARGE_TEMPERATURE_LIMIT		73.00	// Max temperature limit (Celcius)		(actual max: 75C)
#define MAX_CHARGE_TEMPERATURE_LIMIT			48.00	// Max temperature limit (Celcius)		(actual max: 50C)

#define MAX_CURRENT_LIMIT				100			// Max current limit (Amperes)		(Max continuous discharge is 15A)

#endif
