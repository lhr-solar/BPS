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
#include "SPI.h"
#include "Voltage.h"
#include "config.h"
#include "LTC6811.h"

#define MUX1 0x91
#define MUX2 0x93
#define AUX_I2C_START 0x06
#define AUX_I2C_ACK 0
#define AUX_I2C_NACK_STOP 0x9

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 */
ErrorStatus Temperature_Init(void);

/** Temperature_ChannelConfig
 * Configures which temperature channel you're sampling from
 * Assumes there are only 2 muxes; 0 index based - 0 is sensor 1
 * @param channel number
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_ChannelConfig(uint8_t tempChannel);

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return SUCCESS or ERROR
 */
SafetyStatus Temperature_UpdateMeasurements(void);

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SUCCESS or ERROR
 */
SafetyStatus Temperature_IsSafe(uint8_t isCharging);

/** Temperature_SetChargeState
 * Lithium Ion Cells have two separate max temperature limits. There is a limit
 * when the battery is charging and another limit when the battery is discharging.
 * We need to account for these two limits by setting which limit should be used.
 * ChargingState is used for GetModulesInDanger when we can't use the current module to check if charging or discharging
 * @param 1 if pack is charging, 0 if discharging
 */
void Temperature_SetChargeState(uint8_t isCharging);

/** Temperature_GetModulesInDanger
 * Finds all modules that are in danger and stores them into a list
 * This function is called when you can't use the current module to see if it is charging.
 * @return pointer to index of modules that are in danger
 */
uint8_t *Temperature_GetModulesInDanger(void);

/** Temperature_GetModuleTemperature
 * Gets the temperature of a certain module in the battery pack. Since there
 * are NUM_TEMP_SENSORS_ON_MINION sensors per module, just average all of the sensors
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

 /** Temperature_GetRawADC
 * Starts ADC conversion on GPIO1 on LTC6811's auxiliary registers and returns ADC value
 * @param sets the sampling rate
 * @return raw adc value from GPIO1 from one of the 16 temperature sensors
 */
ErrorStatus Temperature_GetRawADC(uint8_t ADCMode);

#endif
