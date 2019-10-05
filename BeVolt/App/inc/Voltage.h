/** Voltage.h
 * Voltage file that holds all voltage related information of BeVolt's
 * battery pack.
 */

#ifndef VOLTAGE_H__
#define VOLTAGE_H__

#include <stdint.h>
#include "config.h"

enum VoltageStatus {OverVoltage, UnderVoltage};

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_Init(void);

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_UpdateMeasurements(void);

/** Voltage_IsSafe
 * Checks if all modules are safe
 * @return SAFE or DANGER
 */
SafetyStatus Voltage_IsSafe(void);

/** Voltage_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Voltage_GetModulesInDanger(void);

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain module in the battery pack
 * @param index of module
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleVoltage(uint16_t moduleIdx);

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint16_t Voltage_GetTotalPackVoltage(void);

#endif
