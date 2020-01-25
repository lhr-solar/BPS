/** Voltage.h
 * Voltage file that holds all voltage related information of BeVolt's
 * battery pack.
 */

#ifndef VOLTAGE_H__
#define VOLTAGE_H__

#include <stdint.h>
#include "config.h"
#include "LTC6811.h"
#include "common.h"

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_Init(cell_asic *boards);

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_UpdateMeasurements(void);

/** Voltage_IsSafe
 * Checks if all modules are safe
 * @return SAFE or danger: UNDERVOLTAGE or OVERVOLTAGE
 */
SafetyStatus Voltage_IsSafe(void);

/** Voltage_GetModulesInDanger
 * Finds all battery modules that in danger and stores them into a list.
 * Each battery module corresponds to and index of the array. If the element in the
 * array is 1, then it means that module in the index is in danger.
 * @return pointer to index of modules that are in danger
 */
SafetyStatus *Voltage_GetModulesInDanger(void);
 
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
