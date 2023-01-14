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

#ifdef NUM_BATTERY_MODULES
#pragma message ("Custom number of battery modules set to " XSTR(NUM_BATTERY_MODULES))
#endif

#ifdef NUM_TEMPERATURE_SENSORS
#pragma message ("Custom number of temperature sensors set to " XSTR(NUM_TEMPERATURE_SENSORS))
#endif

#ifdef NUM_TEMP_SENSORS_PER_MOD
#pragma message ("Custom number of temp sensors per battery module set to " XSTR(NUM_TEMP_SENSORS_PER_MOD))
#endif

#ifdef MIN_VOLTAGE_CHARGING_LIMIT
#pragma message ("Custom minimum voltage charge limit set to " XSTR(MIN_VOLTAGE_CHARGING_LIMIT))
#endif

#ifdef MIN_VOLTAGE_LIMIT
#pragma message ("Custom minimum voltage set to " XSTR(MIN_VOLTAGE_LIMIT))
#endif

#ifdef MAX_VOLTAGE_LIMIT
#pragma message ("Custom maximum voltage set to " XSTR(MAX_VOLTAGE_LIMIT))
#endif

#ifdef CHARGE_DISABLE_VOLTAGE
#pragma message ("Custom charge disable voltage set to " XSTR(CHARGE_DISABLE_VOLTAGE))
#endif

#ifdef MAX_DISCHARGE_TEMPERATURE_LIMIT
#pragma message ("Custom maximum discharge temperature set to " XSTR(MAX_DISCHARGE_TEMPERATURE_LIMIT))
#endif

#ifdef MAX_CHARGE_TEMPERATURE_LIMIT
#pragma message ("Custom maximum charge temperature set to " XSTR(MAX_CHARGE_TEMPERATURE_LIMIT))
#endif

#ifdef MAX_CURRENT_LIMIT
#pragma message ("Custom maximum current set to " XSTR(MAX_CURRENT_LIMIT))
#endif

#ifdef MAX_CHARGING_CURRENT
#pragma message ("Custom charge current set to " XSTR(MAX_CHARGING_CURRENT))
#endif

#ifdef BALANCING_TOLERANCE_START
#pragma message ("Custom balance tolerance start set to " XSTR(BALANCING_TOLERANCE_START))
#endif

#ifdef BALANCING_TOLERANCE_STOP
#pragma message ("Custom balance tolerance stop set to " XSTR(BALANCING_TOLERANCE_STOP))
#endif

// I don't think the other #defines in config.h should be editable