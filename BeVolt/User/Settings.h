// Definition.h
/**
 * Definition file to hold any aliases/constants referenced by multiple files
 * Not specific to a single board/unit
 * @authors Chase Block, Sijin Woo
 * @lastRevised 10/2/2018
 */
 
#ifndef DEFINITION_H__
#define DEFINITION_H__

// NUCLEO or custom
#define NUCLEO		1		// Change 1 if using nucleo, 0 if not

// Basic Parameters of BPS layout
#define NUM_BATTERY_MODULES			12		// Number of Battery Modules
#define NUM_VOLTAGE_BOARDS				1			// Number of Voltage Slave Boards
#define NUM_TEMPERATURE_BOARDS		3			// Number of Temperature Slave Boards

// Max and Min limits of lithium ion cells
#define MIN_VOLTAGE_LIMIT				2.7		// Under voltage limit (Voltage)		(2.5V)
#define MAX_VOLTAGE_LIMIT				4.0		// Over voltage limit (Voltage)		(4.2V)
#define MAX_DISCHARG_TEMPERATURE_LIMIT		73.00	// Max temperature limit (Celcius)		(75C)
#define MAX_CHARGE_TEMPERATURE_LIMIT			48.00	// Max temperature limit (Celcius)		(50C)
#define MAX_CURRENT_LIMIT				450			// Max current limit (Amperes)		(Max continuous discharge is 15A)

// PRIORITIES
#define UART1_PRIORITY		8			// Terminal (PuTTY) interface. Keep this at the lowest priority out of all interrupts.

#endif
