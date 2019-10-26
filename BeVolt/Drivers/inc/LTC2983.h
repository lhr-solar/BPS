// LTC2983.h
/**
 * Driver for LTC2983 chip related functions. The IC uses SPI to communicate to
 * to MCU. IsoSPI is not required since temperature sensors are galvanically isolated.
 * regular SPI to isoSPI.
 * If using SPI: MCU --SPI--> LTC2983
 * If using isoSPI: MCU --SPI--> LTC6820 --isoSPI--> LTC2983
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#ifndef LTC2983_H
#define LTC2983_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "SPI.h"
#include "config.h"


/** LTC2983_Init
 * Initializes and configures LTC2983 chip 
 * @preconditions		SPI_Init8 and GPIO Pins: PC6-8 & PB13-15 are initialized
 */
void LTC2983_Init(void);



/** Board_Select
 * Uses specified CS pins for selected board.
 * @param enumerated type Board. List in Config.h
 * @param state of the CS line (1 or 0). Note that SPI CS lines are HIGH or 1 during idle
 * @note Make sure the CS line returns to 1 if you're finished transmitting or receiving SPI messages
 */
void Board_Select(board Board, bool state);


/** LTC2983_Ready
 * Checks if LTC2984 is ready after startup
 * @return TRUE if ready, FALSE if busy
 */
bool LTC2983_Ready(void);


/** LTC2983_MeasureSingleChannel
 * Sends command to LTC2983 to gather and save all ADC values
 * @param Channel number to read (1 - 20)
 * @return signed 32-bit measurements from specified channel.
 *       returns a -1 if invalid ADC measurement
 */
int32_t LTC2983_MeasureSingleChannel(uint8_t channel);



/** LTC2983_StartMeasuringADC
 * Starts direct ADC conversion for all channels of selected board
 * @note Conversions are initiated consecutively so if you read the Command
 *		Status Register or the INTERRUPT pin, then it won't be ready until All
 *		channels are finished converting.
 * @param Selected Temperature Board CS (enumerated type in "Settings.h")
 */
void LTC2983_StartMeasuringADC(board temperatureBoard);



/**	LTC2983_ReadConversions
 * Reads all channels from temperature board and stores in buffer
 * @param pointer to a buffer to store conversion results
 * @param temperature board CS (enumerated type in "Settings.h")
 * @param number of Channels, use "Settings.h" definitions
 * @preconditions All channels on the board finished conversion before running this function
 */
void LTC2983_ReadConversions(int32_t *Buf, board temperatureBoard, uint8_t numOfChannels);




/** LTC2983_ConversionValidity
 * Checks the error bits of ADC conversion
 * @param Most significant byte of received data conversion
 * @note Used braces with conditional for future updates
 *			 in case return values or setting flags
 */
void LTC2983_ConversionValidity(uint8_t errorBits);




/** LTC2983_ReadChannel
 * Reads the 24 bit ADC value at a specified channel
 * @preconditions Specified channel to be read finished conversion before running this function
 * @param Selected temperature board CS (enumerated type in "Settings.h")
 * @param unsigned int 16-bit channel number
 * @return signed 32-bit (unconverted) data for channel
 */
int32_t LTC2983_ReadChannel(board temperatureBoard, uint8_t channel);


#endif

