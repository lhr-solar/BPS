// LTC6811.h
/**
 * Driver for LTC6811 chip related functions. The IC uses the robust Isolated
 * SPI (isoSPI) to interface with the STM32F4. The LTC6820 is required to convert
 * regular SPI to isoSPI.
 * MCU --SPI--> LTC6820 --isoSPI--> LTC6813
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

#ifndef LTC6811_H__
#define LTC6811_H__

#include <stdint.h>

/** LTC6811_Init
 * Initializes SPI for LTC6820 to convert to isoSPI
 * Initializes and configures LTC6811
 */
void LTC6811_Init(void);

/** LTC6811_SendCmd
 * Sends command data to LTC6811
 * @param unsigned int 16-bit data
 */
uint16_t *LTC6811_SendCmd(uint16_t *data);

/** LTC6811_Measure
 * Sends command to LTC6811 to gather and save all ADC values
 * @return unsigned int 16-bit measurements from all ADC's
 */
uint16_t *LTC6811_Measure(void);

#endif
