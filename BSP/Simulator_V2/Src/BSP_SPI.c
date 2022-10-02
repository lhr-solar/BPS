/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_SPI.h"
#include "config.h"
#include "common.h"


void SPI3_Init(bsp_os_t *spi3_os){
	// TODO
}

/**
 * @brief   Initializes the SPI port.
 * @param   port The SPI port to initialize.
 * @return  None
 */
void BSP_SPI_Init(spi_port_t port, bsp_os_t *spi_os, bool baremetal) {
    // TODO
}

/**
 * @brief   Transmits data to through SPI.
 * @note    Blocking statement
 * @param   port    the SPI port to write to
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI_Write(spi_port_t port, uint8_t *txBuf, uint32_t txLen) {
    // TODO
}

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
void BSP_SPI_Read(spi_port_t port, uint8_t *rxBuf, uint32_t rxLen) {
    // TODO
}

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the LTC6811 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the LTC6811
 *          go to standby.
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI_SetStateCS(spi_port_t port, uint8_t state) {
    // TODO
}

/**
 * @brief   Sets the LTC6811's SPI port clock to either the fast speed for
 *          communicating with the LTC6811 or the slow speed for clocking
 *          the I2C commnication with the mux
 * @param   speed  either SPI_SLOW or SPI_FAST
 * @return  None
 */
void BSP_SPI_SetClock(spi_speed_t speed) {
    // TODO
}
