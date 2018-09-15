// LTC6813.h
/**
 * Driver for LTC6813-1 chip related functions. The IC uses the robust Isolated
 * SPI (isoSPI) to interface with the STM32F4. The LTC6820 is required to convert
 * regular SPI to isoSPI.
 * MCU --SPI--> LTC6820 --isoSPI--> LTC6813
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

#ifndef __LTC6813_H__
#define __LTC6813_H__

/** LTC6813_Init
 * Initializes SPI for LTC6820 to convert to isoSPI
 * Initializes and configures LTC6813
 */
void LTC6813_Init(void);

/** LTC6813_SendCmd
 * Sends command data to LTC6813
 * @param unsigned int 16-bit data
 */
uint16_t *LTC6813_SendCmd(uint16_t *data);

/** LTC6813_Measure
 * Sends command to LTC6813 to gather and save all ADC values
 * @return unsigned int 16-bit measurements from all ADC's
 */
uint16_t *LTC6813_Measure(void);

#endif
