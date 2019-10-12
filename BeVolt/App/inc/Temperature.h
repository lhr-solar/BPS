/** Temperature.h
 * Temperature file that holds all Temperature related information of BeVolt's
 * battery pack.
 */

// NOTE: All units are in Celsius

#ifndef TEMPERATURE_H__
#define TEMPERATURE_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LTC2983.h"
#include "SPI.h"
#include "config.h"

/** Temperature_Init
 * Initializes device drivers including SPI and LTC2983 for Temperature Monitoring
 */
void Temperature_Init(void);

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return SUCCESS or ERROR
 */
Status Temperature_UpdateMeasurements(void);

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SUCCESS or ERROR
 */
Status Temperature_IsSafe(uint8_t isCharging);

/** Temperature_SetChargeState
 * Lithium Ion Cells have two separate max temperature limits. There is a limit
 * when the battery is charging and another limit when the battery is discharging.
 * We need to account for these two limits by setting which limit should be used.
 * @param 1 if pack is charging, 0 if discharging
 */
void Temperature_SetChargeState(uint8_t isCharging);

/** Temperature_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
int16_t *Temperature_GetModulesInDanger(void);

/** Temperature_GetModuleTemperature
 * Gets the temperature of a certain module in the battery pack. Since there
 * are NUM_TEMP_SENSORS_PER_MOD sensors per module, just average all of the sensors
 * for that module so each module only has one temperature reading
 * @param index of module
 * @return temperature of module at specified index
 */
int16_t Temperature_GetModuleTemperature(uint16_t moduleIdx);

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int16_t Temperature_GetTotalPackAvgTemperature(void);

 
 
#endif
