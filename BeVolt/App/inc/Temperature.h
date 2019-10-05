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
<<<<<<< HEAD:BeVolt/User/Temperature.h
#include "Settings.h"
#include "SPI.h"

=======
#include "config.h"
>>>>>>> master:BeVolt/App/inc/Temperature.h

/** Temperature_Init
 * Initializes device drivers including SPI and LTC2983 for Temperature Monitoring
 */
void Temperature_Init(void);

/** Temperature_StartMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return 1 if successfully stored, 0 if failed (FOR FUTURE EDIT, BUT YOU CAN'T TELL IF FAILED EXCEPT IF YOU HAVE TO WAIT FOR A LONG TIME)
 */
bool Temperature_StartMeasurements(void);

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

/** Temperature_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Temperature_GetModulesInDanger(void);

/** Temperature_GetModuleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
uint16_t Temperature_GetModuleTemperature(uint16_t moduleIdx);

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
uint16_t Temperature_GetTotalPackAvgTemperature(void);

 
 
#endif
