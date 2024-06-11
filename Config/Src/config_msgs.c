/**
 * This is a C file because we will include it in compilation,
 * so all these messages are only executed once.
 * 
 * The general format is as follows:
 * 
 * #ifdef COMPILER_DEFINE_HERE
 * #pragma message(PRE(">") "XYZ WAS SPECIFIED BY COMPILER")
 * #endif
 * 
 * config.h will also have similar formatting.
 */

// For displaying the value
// Two defines needed to print the actual value and not the name of the define.
#include "config.h"

#define PRE(s)  "\r    "s"  "   // \r removes the filepath and 'note: '#pragma message:...' parts
#define STR(x)  #x
#define XSTR(x) STR(x)

#include "config.h"

#pragma message(PRE("👪") "NUM_MINIONS set to                        " XSTR(NUM_MINIONS))

#pragma message(PRE("⚡") "NUM_BATTERY_MODULES set to                " XSTR(NUM_BATTERY_MODULES))

#pragma message(PRE("⚡") "PER_MINION_BOARD_VOLT_SENSORS set to      " XSTR((PER_MINION_BOARD_VOLT_SENSORS)))

#pragma message(PRE("🔥") "NUM_TEMPERATURE_SENSORS set to            " XSTR(NUM_TEMPERATURE_SENSORS))

#pragma message(PRE("🔥") "PER_MINION_BOARD_TEMP_SENSORS set to      " XSTR((PER_MINION_BOARD_TEMP_SENSORS)))

#pragma message(PRE("🔥") "NUM_TEMP_SENSORS_PER_MOD set to           " XSTR(NUM_TEMP_SENSORS_PER_MOD))

#pragma message(PRE("🔌") "MIN_VOLTAGE_CHARGING_LIMIT set to         " XSTR(MIN_VOLTAGE_CHARGING_LIMIT))

#pragma message(PRE("🔌") "CHARGE_DISABLE_VOLTAGE set to             " XSTR(CHARGE_DISABLE_VOLTAGE))

#pragma message(PRE("🔋") "MIN_VOLTAGE_LIMIT set to                  " XSTR(MIN_VOLTAGE_LIMIT))

#pragma message(PRE("🔋") "MAX_VOLTAGE_LIMIT set to                  " XSTR(MAX_VOLTAGE_LIMIT))

#pragma message(PRE("🥵") "MAX_DISCHARGE_TEMPERATURE_LIMIT set to    " XSTR(MAX_DISCHARGE_TEMPERATURE_LIMIT))

#pragma message(PRE("🥵") "MAX_CHARGE_TEMPERATURE_LIMIT set to       " XSTR(MAX_CHARGE_TEMPERATURE_LIMIT))

#pragma message(PRE("🥵") "CHARGE_DISABLE_TEMPERATURE set to         " XSTR(CHARGE_DISABLE_TEMPERATURE))

#pragma message(PRE("🥶") "COLD_CHARGE_TEMPERATURE set to            " XSTR(COLD_CHARGE_TEMPERATURE))

#pragma message(PRE("🥶") "COLD_DISCHARGE_TEMPERATURE set to         " XSTR(COLD_DISCHARGE_TEMPERATURE))

#pragma message(PRE("📉") "MAX_CURRENT_LIMIT set to                  " XSTR(MAX_CURRENT_LIMIT))

#pragma message(PRE("📉") "MAX_COLD_CURRENT_LIMIT set to             " XSTR(MAX_COLD_CURRENT_LIMIT))

#pragma message(PRE("📈") "MAX_CHARGING_CURRENT set to               " XSTR(MAX_CHARGING_CURRENT))

#pragma message(PRE("📈") "MAX_COLD_CHARGING_CURRENT set to          " XSTR(MAX_CHARGING_CURRENT))

#pragma message(PRE("🔧") "BALANCING_TOLERANCE_START start set to    " XSTR(BALANCING_TOLERANCE_START))

#pragma message(PRE("🔧") "BALANCING_TOLERANCE_STOP stop set to      " XSTR(BALANCING_TOLERANCE_STOP))

#pragma message(PRE("📞") "BPS_CAN_LOOPBACK set to                   " XSTR(BPS_CAN_LOOPBACK))

#pragma message(PRE("📞") "BPS_CAN_MIRROR_OUTPUT_USB set to          " XSTR(BPS_CAN_MIRROR_OUTPUT_USB))

#pragma message(PRE("🔇") "BPS_ENABLE_PRINT_OUTPUT set to            " XSTR(BPS_ENABLE_PRINT_OUTPUT))

// I don't think the other #defines in config.h should be editable
