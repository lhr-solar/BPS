/** Voltage.h
 * Voltage file that holds all voltage related information of BeVolt's
 * battery pack.
 */

#ifndef VOLTAGE_H__
#define VOLTAGE_H__

#include <stdint.h>

enum VoltageStatus {OverVoltage, UnderVoltage};

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @return 1 if successfully stored, -1 if failed and an error occurred
 */
int8_t Voltage_Init(void);

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Voltage_UpdateMeasurements(void);

/** Voltage_IsSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Voltage_IsSafe(void);

/** Voltage_SetLimits
 * Sets the max and min voltage limit the cells can reach before danger
 * @param max voltage limit
 * @param min voltage limit
 */
void Voltage_SetLimits(uint16_t ceiling, uint16_t floor);

/** Voltage_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Voltage_GetModulesInDanger(void);
 
/** Voltage_OpenWire
 * Uses the built-in adow function to check for open wires
 * @return 1 if open wire, 0 if good
 */
long Voltage_OpenWire(void);

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain module in the battery pack
 * @param index of module
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleVoltage(uint16_t moduleIdx);

/** *Voltage_GetOpenWire
 * Finds the pin locations of the open wires
 * @return array of battery modules (1 means disconnect, 0 means safe)
 */
uint8_t *Voltage_GetOpenWire(void);

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint16_t Voltage_GetTotalPackVoltage(void);

#endif
