/**
 * This is a C file because we will include it in compilation,
 * so all these messages are only executed once.
 * 
 * The general format is as follows:
 * 
 * #ifdef COMPILER_DEFINE_HERE
 * #pragma message ("XYZ WAS SPECIFIED BY COMPILER")
 * #endif
 * 
 * config.h will also have similar formatting.
 */

// For displaying the value
// Two defines needed to print the actual value and not the name of the define.
#define XSTR(x) STR(x)
#define STR(x) #x

#include "config.h"

#ifdef NUM_MINIONS
#pragma message ("NUM_MINIONS set to " XSTR(NUM_MINIONS))
#else
#pragma message ("NUM_MINIONS set to " XSTR(NUM_MINIONS))
#endif

#ifdef NUM_BATTERY_MODULES
#pragma message ("NUM_BATTERY_MODULES set to " XSTR(NUM_BATTERY_MODULES))
#else
#pragma message ("NUM_BATTERY_MODULES set to " XSTR(NUM_BATTERY_MODULES))
#endif

#ifdef NUM_TEMPERATURE_SENSORS
#pragma message ("NUM_TEMPERATURE_SENSORS set to " XSTR(NUM_TEMPERATURE_SENSORS))
#else
#pragma message ("NUM_TEMPERATURE_SENSORS set to " XSTR(NUM_TEMPERATURE_SENSORS))
#endif

#ifdef NUM_TEMP_SENSORS_PER_MOD
#pragma message ("NUM_TEMP_SENSORS_PER_MOD set to " XSTR(NUM_TEMP_SENSORS_PER_MOD))
#else
#pragma message ("NUM_TEMP_SENSORS_PER_MOD set to " XSTR(NUM_TEMP_SENSORS_PER_MOD))
#endif

#ifdef MIN_VOLTAGE_CHARGING_LIMIT
#pragma message ("MIN_VOLTAGE_CHARGING_LIMIT set to " XSTR(MIN_VOLTAGE_CHARGING_LIMIT))
#else
#pragma message ("MIN_VOLTAGE_CHARGING_LIMIT set to " XSTR(MIN_VOLTAGE_CHARGING_LIMIT))
#endif

#ifdef MIN_VOLTAGE_LIMIT
#pragma message ("MIN_VOLTAGE_LIMIT set to " XSTR(MIN_VOLTAGE_LIMIT))
#else
#pragma message ("MIN_VOLTAGE_LIMIT set to " XSTR(MIN_VOLTAGE_LIMIT))
#endif

#ifdef MAX_VOLTAGE_LIMIT
#pragma message ("MAX_VOLTAGE_LIMIT set to " XSTR(MAX_VOLTAGE_LIMIT))
#else
#pragma message ("MAX_VOLTAGE_LIMIT set to " XSTR(MAX_VOLTAGE_LIMIT))
#endif

#ifdef CHARGE_DISABLE_VOLTAGE
#pragma message ("CHARGE_DISABLE_VOLTAGE set to " XSTR(CHARGE_DISABLE_VOLTAGE))
#else
#pragma message ("CHARGE_DISABLE_VOLTAGE set to " XSTR(CHARGE_DISABLE_VOLTAGE))
#endif

#ifdef MAX_DISCHARGE_TEMPERATURE_LIMIT
#pragma message ("MAX_DISCHARGE_TEMPERATURE_LIMIT set to " XSTR(MAX_DISCHARGE_TEMPERATURE_LIMIT))
#else
#pragma message ("MAX_DISCHARGE_TEMPERATURE_LIMIT set to " XSTR(MAX_DISCHARGE_TEMPERATURE_LIMIT))
#endif

#ifdef MAX_CHARGE_TEMPERATURE_LIMIT
#pragma message ("MAX_CHARGE_TEMPERATURE_LIMIT set to " XSTR(MAX_CHARGE_TEMPERATURE_LIMIT))
#else
#pragma message ("MAX_CHARGE_TEMPERATURE_LIMIT set to " XSTR(MAX_CHARGE_TEMPERATURE_LIMIT))
#endif

#ifdef MAX_CURRENT_LIMIT
#pragma message ("MAX_CURRENT_LIMIT set to " XSTR(MAX_CURRENT_LIMIT))
#else
#pragma message ("MAX_CURRENT_LIMIT set to " XSTR(MAX_CURRENT_LIMIT))
#endif

#ifdef MAX_CHARGING_CURRENT
#pragma message ("MAX_CHARGING_CURRENT set to " XSTR(MAX_CHARGING_CURRENT))
#else
#pragma message ("MAX_CHARGING_CURRENT set to " XSTR(MAX_CHARGING_CURRENT))
#endif

#ifdef BALANCING_TOLERANCE_START
#pragma message ("BALANCING_TOLERANCE_START start set to " XSTR(BALANCING_TOLERANCE_START))
#else
#pragma message ("BALANCING_TOLERANCE_START start set to " XSTR(BALANCING_TOLERANCE_START))
#endif

#ifdef BALANCING_TOLERANCE_STOP
#pragma message ("BALANCING_TOLERANCE_STOP stop set to " XSTR(BALANCING_TOLERANCE_STOP))
#else
#pragma message ("BALANCING_TOLERANCE_STOP stop set to " XSTR(BALANCING_TOLERANCE_STOP))
#endif

// I don't think the other #defines in config.h should be editable