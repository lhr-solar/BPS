/** Temperature.h
 * Temperature file that holds all Temperature related information of BeVolt's
 * battery pack.
 */

// NOTE: All units are in Celsius

#ifndef TEMPERATURE_H__
#define TEMPERATURE_H__

#include "common.h"
#include "config.h"
#include "LTC6811.h"

#define MUX1 0x90
#define MUX2 0x92
#define AUX_I2C_BLANK 0
#define AUX_I2C_SLAVE_ACK 0x07
#define AUX_I2C_SLAVE_ACK_STOP 0x1
#define AUX_I2C_START 0x06
#define AUX_I2C_ACK 0
#define AUX_I2C_NACK_STOP 0x9
#define AUX_I2C_NACK 0x8

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 */
ErrorStatus Temperature_Init(cell_asic *boards);

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
ErrorStatus Temperature_UpdateMeasurements(void);

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
 * @precondition: boardIdx must < NUM_MINONS; sensorIdx must be < MAX_TEMP_SENSORS_PER_MINON_BOARD
 * @param index of module
 * @return temperature of module at specified index
 * @note make sure there is a temperature sensor connected at that index or else garbage data will be returned
 */
int16_t Temperature_GetModuleTemperature(uint8_t moduleIdx);

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

/** convertVoltageToTemperature
 * Converts mv to temperature based on the temperature sensor equation
 * Equation : T(in C) = (((13.582 - sqrt((-13.582)*(-13.582) + 4 * 0.00433 * (2230.8 - milliVolt)))/ (2.0 * -0.00433)) + 30)
 * @param mV from ADC
 * @return temperature in Celsius (Fixed Point with .001 resolution) 
 */
int milliVoltToCelsius(float milliVolt);
#endif
