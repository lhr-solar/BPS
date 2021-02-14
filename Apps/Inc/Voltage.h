/* Copyright (c) 2020 UT Longhorn Racing Solar */

/** Voltage.h
 * Voltage file that holds all voltage related information of BeVolt's battery pack.
 */

#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <stdint.h>
#include "config.h"
#include "LTC6811.h"
#include "common.h"
#include "os.h"

typedef struct voltage_safety{
    SafetyStatus wire_checks[TOTAL_VOLT_WIRES];
    SafetyStatus module_checks[NUM_BATTERY_MODULES];
} VoltageSafety_t;

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 * @param voltageMutex pointer to mutex, meant to pass pointer to VoltageBuffer_Mutex 
 */
void Voltage_Init(cell_asic *boards);

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 */
void Voltage_UpdateMeasurements(void);

/** Voltage_CheckStatus
 * Checks if all modules are safe
 * @return SAFE or danger: UNDERVOLTAGE or OVERVOLTAGE
 */
SafetyStatus Voltage_CheckStatus(void);

/** Voltage_GetModulesInDanger
 * Finds all battery modules in danger and stores them into a list.
 * Each battery module corresponds to and index of the array. If the element in the
 * array is 1, then it means that module in the index is in danger.
 * @param struct that has safety status for all wires and modules in the system
 */
void Voltage_GetModulesInDanger(VoltageSafety_t* system);
 
/** Voltage_OpenWireSummary
 * Runs the open wire method with print=true
 * Gives a summary of the open wire status (which wires are open and on which boards)
 */
void Voltage_OpenWireSummary(void);

/** Voltage_OpenWire
 * Uses the built-in adow function to check for open wires
 * @return SafetyStatus
 */
SafetyStatus Voltage_OpenWire(void);

/** Voltage_GetOpenWire
 * Finds the pin locations of the open wires
 * @return hexadecimal string (1 means open wire, 0 means closed)
 */
uint32_t Voltage_GetOpenWire(void);

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery module (0-indexed)
 * @return voltage of module at specified index (fixed point of 0.00001)
 */
uint16_t Voltage_GetModuleMillivoltage(uint8_t moduleIdx);

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack (fixed point of 0.00001
 */
uint32_t Voltage_GetTotalPackVoltage(void);

#endif
