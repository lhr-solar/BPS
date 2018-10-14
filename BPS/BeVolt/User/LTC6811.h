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

// ADC Command Configurations (binary)
// Conversion Mode: MD[1:0] bits
//	00 : 422Hz
//	01 : 27kHz (Fast)
//	10 : 7kHz (Normal)
//	11 : 26Hz (Filtered)
#define ADC_CONVERSION_MODE			0b10

// Discharge Permitted?
//	0 : Disabled
//	1 : Enabled
#define ADC_DCP									0

// Cell channels to measure (binary)
// 	000 : All cells
//	001 : Cell 1 and 7
//	010 : Cell 2 and 8
//	011 : Cell 3 and 9
//	100 : Cell 4 and 10
//	101 : Cell 5 and 11
//	110 : Cell 6 and 12
#define CELL_CH_TO_CONVERT				0b000

/** LTC6811_Init
 * Initializes SPI for LTC6820 to convert to isoSPI
 * Initializes and configures LTC6811
 */
void LTC6811_Init(void);

/** LTC6811_Measure
 * Sends command to LTC6811 to gather and save all ADC values
 * @param ptr to unsigned int 16-bit buffer that will hold new measurements from all ADC's 
 * @param number of modules
 */
void LTC6811_Measure(uint16_t modules, uint32_t size);

/** LTC6811_SendCmd
 * Sends command to LTC6811
 * Packet format:
 * [16 bits : cmd][16 bits : pec]
 * @param unsigned int 16-bit cmd
 */
void LTC6811_SendCmd(uint16_t txCmd);

/** LTC6811_Write
 * Sends command then sends data to all LTC6811's
 * Packet format:
 * [16 bits : cmd][16 bits : pec][CMD_LEN - 32 bits : data]
 * @param unsigned 16-bit command
 * @param unsigned 8 bit data packet to be sent
 */
void LTC6811_Write(uint16_t txCmd, uint8_t *data);

/** LTC6811_Read
 * Sends command to LTC6811 then reads all the data that the LTC6811 sends back
 * @param unsigned 16-bit command
 * @param pointer to unsigned 8-bit array that will hold data that is received
 * @param max size of rx array
 * @return pecError (just to check if any noise affected communication)
 */
int8_t LTC6811_Read(uint16_t txCmd, uint8_t *rxData, uint32_t rxSize);

#endif
