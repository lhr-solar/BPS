/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** Temperature.h
 * Temperature file that holds all Temperature related information of BeVolt's battery pack.
 */

// NOTE: All units are in Celsius

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "common.h"
#include "config.h"
#include "LTC6811.h"

#define MUX1 0x90
#define MUX2 0x92
#define AUX_I2C_START 0x6
#define AUX_I2C_ACK 0
#define AUX_I2C_NACK 0x8
#define AUX_I2C_NACK_STOP 0x9
#define AUX_I2C_BLANK 0
#define AUX_I2C_NO_TRANSMIT 0x7

#define TEMP_ERR_OUT_BOUNDS 42424242

//TODO: tune PID with actual pack and fans, and then change values below to appropiate value
#define TEMPERATURE_PID_PROPORTIONAL 4
#define TEMPERATURE_PID_INTEGRAL 250
#define TEMPERATURE_PID_I_ZONE 5000
#define TEMPERATURE_PID_I_MAX_ACCUM 500000
#define TEMPERATURE_HOLD_FAN_SPEED 5
#define TEMPERATURE_PID_MILLICELCIUS_CONVERT 1000
#define TEMPERATURE_PID_MAX_INPUT 255999 + PID_DESIRED_TEMPERATURE

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 * @return SUCCESS or ERROR
 */
void Temperature_Init(cell_asic *boards);

/** Temperature_ChannelConfig
 * Configures which temperature channel you're sampling from
 * Assumes there are only 2 muxes; 0 index based - 0 is sensor 1
 * @precondition tempChannel should be < MAX_TEMP_SENSORS_PER_MINION_BOARD
 * @param channel number (0-indexed based)
 * @return SUCCESS or ERROR
 * @note we clear the otherMux every time a channel is switched even on the same mux
 * Maybe change depending on speed/optimization
 */
ErrorStatus Temperature_ChannelConfig(uint8_t tempChannel);

/** convertVoltageToTemperature
 * Converts mv to temperature based on the temperature sensor equation
 * Equation : T(in C) = (((13.582 - sqrt((-13.582)*(-13.582) + 4 * 0.00433 * (2230.8 - milliVolt)))/ (2.0 * -0.00433)) + 30)
 * @param mV from ADC
 * @return temperature in Celsius (Fixed Point with .001 resolution) 
 */
int32_t milliVoltToCelsius(uint32_t milliVolt);

/** Temperature_UpdateSingleChannel
 * Stores and updates the new measurements received on one particular temperature sensor
 * @param channel < MAX_TEMP_SENSORS_PER_MINION_BOARD, 0-indexed
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_UpdateSingleChannel(uint8_t channel);

/** Temperature_UpdateAllMeasurements
 * Stores and updates the new measurements received on all temperature sensors
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_UpdateAllMeasurements(void);

/** Temperature_CheckStatus
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SAFE, opt = CHARGE_DISABLE / OPT_SAFE
 * @return DANGER, opt = UNDERVOLTAGE / OVERVOLTAGE
 */
SafetyStatus Temperature_CheckStatus(uint8_t isCharging, SafetyStatusOpt *opt);

/** Temperature_SetChargeState
 * Lithium Ion Cells have two separate max temperature limits. There is a limit
 * when the battery is charging and another limit when the battery is discharging.
 * We need to account for these two limits by setting which limit should be used.
 * ChargingState is used for GetModulesInDanger when we can't use the current module 
 * to check if charging or discharging
 * @param 1 if pack is charging, 0 if discharging
 */
void Temperature_SetChargeState(uint8_t isCharging);

/** Temperature_GetModulesInDanger
 * Finds all modules that are in danger and stores them into a list
 * This function is called when you can't use the current module to see if it is charging.
 * @return pointer to index of modules that are in danger
 */
uint8_t *Temperature_GetModulesInDanger(void);

/** Temperature_GetSingleTempSensor
 * Gets the single sensor from a particular board
 * @param index of sensor (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int32_t Temperature_GetSingleTempSensor(uint8_t sensorIdx);

/** Temperature_GetModuleTemperature
 * Gets the avg temperature of a certain battery module in the battery pack. 
 * There is currently (2023-2024) one temperature per module, so this function does nothing.
 * @precondition: moduleIdx must be < NUM_BATTERY_MODULE
 * @param index of module (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int32_t Temperature_GetModuleTemperature(uint8_t moduleIdx);

/** Temperature_SampleADC
 * Starts ADC conversion on GPIO1 on LTC6811's auxiliary registers on all boards
 * @param ADCMode sets the sampling rate
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_SampleADC(uint8_t ADCMode);

/** Temperature_GetMaxTemperature
 * Gets the maximum measured temperature in the most recent batch of temperature measurements
 * @return the maximum measured temperature in the most recent batch of temperature measurements
 */
int32_t Temperature_GetMaxTemperature(void);

/** Temperature_GetMinTemperature
 * Gets the minimum measured temperature in the most recent batch of temperature measurements
 * @return the minimum measured temperature in the most recent batch of temperature measurements
 */
int32_t Temperature_GetMinTemperature(void);

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int32_t Temperature_GetTotalPackAvgTemperature(void);

#endif
