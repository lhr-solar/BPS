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
typedef enum SafetyStatus_e {SAFE = 0, DANGER = 1} SafetyStatus;
typedef enum SafetyStatusOpt_e {
    OPT_SAFE = SAFE,
    UNDERVOLTAGE = 1,
    OVERVOLTAGE,
    CHARGE_DISABLE,
    OPT_NONE
} SafetyStatusOpt;

//--------------------------------------------------------------------------------
// Basic Parameters of BPS layout

#ifndef NUM_MINIONS
#define NUM_MINIONS                     3       // Number of minion boards
#endif

//--------------------------------------------------------------------------------
// Battery Fault Checks
#define NUM_FAULT_POINTS                4

//--------------------------------------------------------------------------------
// Battery Pack layout
#ifndef NUM_BATTERY_MODULES
#define NUM_BATTERY_MODULES             31      // Number of battery modules
#endif

#ifndef MODULE_CELLS_IN_PARALLEL
#define MODULE_CELLS_IN_PARALLEL        9
#endif

#ifndef NUM_TEMP_SENSORS_PER_MOD
#define NUM_TEMP_SENSORS_PER_MOD        1       // Number of temperature sensors per battery module
#endif

#ifndef NUM_TEMPERATURE_SENSORS
#define NUM_TEMPERATURE_SENSORS         31      // Number of temperature sensors
#endif

//--------------------------------------------------------------------------------
// Voltage Sensor Configurations
// Defines how many voltage sensors are connected to each board
#ifndef MAX_VOLT_SENSORS_PER_MINION_BOARD
#define MAX_VOLT_SENSORS_PER_MINION_BOARD    12     // Note: use this value for reference, we don't actually check against it
#endif

#ifndef PER_MINION_BOARD_VOLT_SENSORS
#define PER_MINION_BOARD_VOLT_SENSORS   {11, 9, 11}    // Number of voltage sensors per minion board. Should be an array with same length as NUM_MINIONS
#endif

#define MAX_VOLT_WIRES                    (MAX_VOLT_SENSORS_PER_MINION_BOARD * NUM_MINIONS)

//--------------------------------------------------------------------------------
// Temperature Sensor Configurations
// Define how many temperature sensors are connected to each board
#ifndef MAX_TEMP_SENSORS_PER_MINION_BOARD
#define MAX_TEMP_SENSORS_PER_MINION_BOARD    16     // Note: use this value for reference, we don't actually check against it
#endif

#ifndef PER_MINION_BOARD_TEMP_SENSORS
#define PER_MINION_BOARD_TEMP_SENSORS   {16, 0, 15}     // Number of voltage sensors per minion board. Should be an array with same length as NUM_MINIONS
#endif

#define MAX_TEMP_SENSORS                (MAX_TEMP_SENSORS_PER_MINION_BOARD * NUM_MINIONS)

//--------------------------------------------------------------------------------
// Globals created using voltage and sensor configuration arrays
// Internal only -- don't modify unless necessary

__attribute__ ((weak)) const int
VoltageSensorsCfg[] = PER_MINION_BOARD_VOLT_SENSORS;

// check if VOLT_TAP_DIST array size is equal to the number of minions
_Static_assert((sizeof(VoltageSensorsCfg)/sizeof(*VoltageSensorsCfg)) == NUM_MINIONS, 
               "Error: the number of minions specified and the number used for PER_MINION_BOARD_VOLT_SENSORS are different.");

__attribute__ ((weak)) const int
TemperatureSensorsCfg[] = PER_MINION_BOARD_TEMP_SENSORS;

// check if VOLT_TAP_DIST array size is equal to the number of minions
_Static_assert((sizeof(TemperatureSensorsCfg)/sizeof(*TemperatureSensorsCfg)) == NUM_MINIONS, 
               "Error: the number of minions specified and the number used for PER_MINION_BOARD_TEMP_SENSORS are different.");



//--------------------------------------------------------------------------------
// MAX and MIN limits of lithium ion cells
#ifndef MIN_VOLTAGE_CHARGING_LIMIT
#define MIN_VOLTAGE_CHARGING_LIMIT      2550    // Under voltage limit for charging (milliVolts) 
#endif

#ifndef MIN_VOLTAGE_LIMIT
#define MIN_VOLTAGE_LIMIT               2700    // Under voltage limit (milliVolts)     (actual min: 2.5V)
#endif

#ifndef MAX_VOLTAGE_LIMIT
#define MAX_VOLTAGE_LIMIT               4200    // Over voltage limit (milliVolts)      (actual max: 4.2V)
#endif

#ifndef CHARGE_DISABLE_VOLTAGE
#define CHARGE_DISABLE_VOLTAGE          4100    // Voltage to stop charging at
#endif

// make sure we don't enable charging if we're too close to the voltage limit
#if MAX_VOLTAGE_LIMIT - 100 < CHARGE_DISABLE_VOLTAGE
#error "Charging maximum voltage is too close to voltage trip limit!"
#endif

#ifndef MAX_DISCHARGE_TEMPERATURE_LIMIT
#define MAX_DISCHARGE_TEMPERATURE_LIMIT 60000   // Max temperature limit (milliCelcius)     (recommended release: 60.00C)
#endif

#ifndef MAX_CHARGE_TEMPERATURE_LIMIT
#define MAX_CHARGE_TEMPERATURE_LIMIT    45000   // Max temperature limit (milliCelcius)     (recommended release: 45.00C)
#endif

#ifndef CHARGE_DISABLE_TEMPERATURE
#define CHARGE_DISABLE_TEMPERATURE      43500   // Temperature to stop charging at
#endif

// make sure we don't enable charging if we're too close to the temperature limit
#if MAX_CHARGE_TEMPERATURE_LIMIT - 1000 < CHARGE_DISABLE_TEMPERATURE
#error "Charging maximum temperature is too close to temperature trip limit!"
#endif

#ifndef COLD_CHARGE_TEMPERATURE
#define COLD_CHARGE_TEMPERATURE         25000   // Temperature for reduced charging rate
#endif

#ifndef COLD_DISCHARGE_TEMPERATURE
#define COLD_DISCHARGE_TEMPERATURE      10000   // Temperature for reduced discharging rate
#endif

#ifndef MAX_CURRENT_LIMIT
#define MAX_CURRENT_LIMIT               60000   // Max current limit (Milliamperes)        (Max continuous discharge is 7A per cell)
#endif

#ifndef MAX_COLD_CURRENT_LIMIT
#define MAX_COLD_CURRENT_LIMIT          16000   // Max current limit (Milliamperes)        (Max continuous discharge is 2A per cell)
#endif

#ifndef MAX_CHARGING_CURRENT
#define MAX_CHARGING_CURRENT            -25000  // Max current per cell is 3.3 Amps (Standard charge)
#endif

#ifndef MAX_COLD_CHARGING_CURRENT
#define MAX_COLD_CHARGING_CURRENT       -10000  // Max current per cell is 1.4 Amps (Cold charge)
#endif

#ifndef BALANCING_TOLERANCE_START
#define BALANCING_TOLERANCE_START       50      // mV differential from lowest module that will start battery balancing
#endif

#ifndef BALANCING_TOLERANCE_STOP
#define BALANCING_TOLERANCE_STOP        20      // mV differential from lowest module that will stop battery balancing
#endif
//--------------------------------------------------------------------------------
// Helpers

#define PWM_PERIOD                      4000    // Number of clock cycles per PWM period

//--------------------------------------------------------------------------------
// Output Data Rates for various sensor measurement CAN IDs
// Helps reduce CANBus traffic by sending data at a slower rate
// at 125kbps (and assuming max-length messages), this equates ~900 messages per second
#define ODR_VOLTAGE_DATA_ARRAY_PERIOD_MS        500     // Period in milliseconds to send voltage data
#define ODR_TEMPERATURE_DATA_ARRAY_PERIOD_MS    500     // Period in milliseconds to send temperature data
#define ODR_CURRENT_DATA_PERIOD_MS              200     // Period in milliseconds to send current data
#define ODR_STATE_OF_CHARGE_DATA_PERIOD_MS      2000    // Period in milliseconds to send state of charge data
#define ODR_CHARGING_ENABLED_PERIOD_MS          200     // Period in milliseconds to send charging enabled data

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

// set CAN loopback mode. default is false.
// DO NOT CHANGE VALUE HERE -- pass in when calling 'make'
#ifndef BPS_CAN_LOOPBACK
#define BPS_CAN_LOOPBACK false 
#endif

// mirror all CAN messages over UART via printf. default is false.
// DO NOT CHANGE VALUE HERE -- pass in when calling 'make'
#ifndef BPS_CAN_MIRROR_OUTPUT_USB
#define BPS_CAN_MIRROR_OUTPUT_USB false
#endif

// enable print outputs over USB UART.
// DO NOT CHANGE VALUE HERE -- pass in when calling 'make'
#ifndef BPS_ENABLE_PRINT_OUTPUT
#define BPS_ENABLE_PRINT_OUTPUT true
#endif

#endif
