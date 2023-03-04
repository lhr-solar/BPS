/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * BSP_SPI.c - Simulates SPI communication on BPS board
 * NOTE: This does not fully simulate all SPI communication.
*/

#include "BSP_SPI.h"
#include "config.h"
#include "common.h"
#include <inttypes.h>
#include "Simulator.h"

static bool initialized = false;

/**
 * @brief   Initializes the SPI port.
 * @param   port The SPI port to initialize.
 * @return  None
 */
void BSP_SPI_Init(spi_port_t port, bsp_os_t *spi_os, bool baremetal) {
    initialized = true;
    Simulator_Log(LOG_INFO,"SPI initialized\n");
    Simulator_Log(LOG_INFO,"SPI3 initialized\n");
}

/**
 * @brief   Transmits data to through SPI.
 * @note    Blocking statement
 * @param   port    the SPI port to write to
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
bool BSP_SPI_Write(spi_port_t port, uint8_t *txBuf, uint32_t txLen) {
    char* spi = port ? "SPI3 LTC2315" : "SPI LTC6811";
    char* msg;
    if(!initialized) {
        asprintf(&msg, "Used %s without initialization!\n", spi);
        Simulator_Log_Location(LOG_ERROR, msg);
        free(msg);
        Fault_BitMap |= Fault_CRC;
        EnterFaultState();
        return false;
    }
    char* data;
    memcpy(&data, txBuf, txLen);
    asprintf(&msg, "Wrote to %s with data {", spi);
    for (uint32_t i = 0; i < txLen; i++){
        strcat(msg, (char*)&txBuf[i]);
    }
    strcat(msg, "}\n");
    Simulator_Log(LOG_INFO, msg);
    free(msg);
    return true;
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
bool BSP_SPI_Read(spi_port_t port, uint8_t *rxBuf, uint32_t rxLen) {
    char* spi = port ? "SPI3 LTC2315" : "SPI LTC6811";
    char* msg;
    if(!initialized) {
        asprintf(&msg, "Used %s without initialization!\n", spi);
        Simulator_Log_Location(LOG_ERROR, msg);
        free(msg);
        Fault_BitMap |= Fault_CRC;
        EnterFaultState();
        return false;
    }
    asprintf(&msg, "%s {read}\n", spi);
    Simulator_Log(LOG_INFO, msg);
    free(msg);
    return true;
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
    char* spi = port ? "SPI3 LTC2315" : "SPI LTC6811";
    char* msg;
    if(!initialized) {
        asprintf(&msg, "Used %s without initialization!\n", spi);
        Simulator_Log_Location(LOG_ERROR, msg);
        free(msg);
        Fault_BitMap |= Fault_CRC;
        EnterFaultState();
    }
}

/**
 * @brief   Sets the LTC6811's SPI port clock to either the fast speed for
 *          communicating with the LTC6811 or the slow speed for clocking
 *          the I2C commnication with the mux
 * @param   speed  either SPI_SLOW or SPI_FAST
 * @return  None
 */
void BSP_SPI_SetClock(spi_speed_t speed) {
    char* msg;
    if(!initialized) {
        asprintf(&msg, "Used SPI LTC6811 without initialization!\n");
        Simulator_Log_Location(LOG_ERROR, msg);
        free(msg);
        Fault_BitMap |= Fault_CRC;
        EnterFaultState();
    }
}
