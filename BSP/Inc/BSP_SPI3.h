#ifndef __BSP_SPI3_H
#define __BSP_SPI3_H

#include "common.h"

/**
 * @note    If other ICs need to be connected onto the SPI line, this code will have to be modified.
 */

/**
 * @brief   Initializes the SPI port connected to the AS8510.
 *          This port communicates with the AS8510 voltage and current
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          We are only using the AS8510 to monitor current.
 * @param   None
 * @return  None
 */
void BSP_SPI3_Init(void);

/**
 * @brief   Transmits data to through SPI.
 * @note    Blocking statement
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI3_Write(uint8_t *txBuf, uint32_t txLen);

/**
 * @brief   Gets the data from SPI.
 * @note    Blocking statement
 * @param   rxBuf   data array to store the data that is received.
 * @param   rxLen   length of data array.
 * @return  None
 */
void BSP_SPI3_Read(uint8_t *rxBuf, uint32_t rxLen);

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the AS8510 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the AS8510
 *          ignore the data on the line.
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI3_SetStateCS(uint8_t state);

#endif
