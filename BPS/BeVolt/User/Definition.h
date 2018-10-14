// Definition.h
/**
 * Definition file to hold any aliases/constants referenced by multiple files
 * Not specific to a single board/unit
 * @authors Chase Block, Sijin Woo
 * @lastRevised 10/2/2018
 */
 
#define NUM_BATTERY_MODULES			31
#define NUM_VOLTAGE_BOARDS				3
#define NUM_TEMPERATURE_BOARDS		3

// PRIORITIES
#define UART1_PRIORITY		8			// Terminal (PuTTY) interface. Keep this at the lowest priority out of all interrupts.
