#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "common.h"
#include "os.h"
#include "BSP_OS.h"


/**
 * @note    This is for the RTOS version of the BPS 
 */

/**
 * @brief   Initializes AmperesIO semaphore pending and posting
 * @param   spi3_os pointer to struct that holds the SPI3 specific pend()/post() functions
 * @return  None
 */
void SPI3_Init(bsp_os_t *spi3_os);

/**
 * @note    If other ICs need to be connected onto the SPI line, this code will have to be modified.
 */

/**
 * @brief   Initializes the SPI port connected to the LTC6820.
 *          This port communicates with the LTC6811 voltage and temperature
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          Look at analog devices website and LTC6811's or LTC6820's datasheets.
 * @param   None
 * @return  None
 */
void BSP_SPI_Init(bsp_os_t *spi_os);


/**
 * @brief   Transmits data to through SPI.
 *          With the way the LTC6811 communication works, the LTC6811 will not send
 *          anything during a transmit for uC to LTC6811. This is unlike what
 *          the SPI protocol expects where a transmit and receive happen
 *          simultaneously.
 * @note    Blocking statement
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI_Write(uint8_t *txBuf, uint32_t txLen);

/**
 * @brief   Gets the data from SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not be expecting anything from the uC.
 *          The SPI protocol requires the uC to transmit data in order to receive
 *          anything so the uC will send junk data.
 * @note    Blocking statement
 * @param   rxBuf   data array to store the data that is received.
 * @param   rxLen   length of data array.
 * @return  None
 */
void BSP_SPI_Read(uint8_t *rxBuf, uint32_t rxLen);

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the LTC6811 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the LTC6811
 *          go to standby.
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI_SetStateCS(uint8_t state);

#endif
