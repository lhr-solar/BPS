// LTC2983.h
/**
 * Driver for LTC2983 chip related functions. The IC uses SPI to communicate to
 * to MCU. IsoSPI is not required since temperature sensors are galvanically isolated.
 * regular SPI to isoSPI.
 * If using SPI: MCU --SPI--> LTC2983
 * If using isoSPI: MCU --SPI--> LTC6820 --isoSPI--> LTC2983
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

#ifndef LTC2983_H__
#define LTC2983_H__

#include <stdint.h>
#include <stdbool.h>

/** LTC2983_Init
 * Initializes SPI pins
 * Initializes and configures LTC2983 chip 
 */
void LTC2983_Init(void);

/** LTC2983_Ready
 * Checks if LTC2984 is ready after startup
 * @return unsigned int 16-bit data
 */
bool LTC2983_Ready(void);

/** LTC2983_MeasureSingleChannel
 * Sends command to LTC2983 to gather and save all ADC values
 * @param 16 bit channel size in a 1 byte array
 * @return unsigned int 16-bit measurements from all ADCs
 */
int32_t LTC2983_MeasureSingleChannel(void /*uint8_t *channelAddr*/);

/** LTC2983_Measure
 * Sends command to LTC2983 to gather and save all ADC values
 * @return unsigned int 16-bit measurements from all ADCs
 */
uint16_t *LTC2983_Measure(void);


uint32_t LTC2983_ReadChannel(uint16_t channel);

#endif
