#ifndef AS8510_H
#define AS8510_H

// Driver for AS8510 current sensor.

#include "common.h"
#include "config.h"
#include "BSP_OS.h"

/* Initialize communication with the AS8510 to begin measurement of electrical current.
 */
void AS8510_Init(bsp_os_t spi_os);

/* Writes to a register on the AS8510
 * @param addr the address to write to
 * @param data the data to write
 * @param len  the length of the data
 * @return SUCCESS or ERROR
 */
ErrorStatus AS8510_WriteToAddr(uint8_t addr, uint8_t *data, uint8_t len);

/* Reads from a register on the AS8510
 * @param addr the address to read from
 * @param data pointer to the data buffer
 * @param len  the number of bytes to read
 * @return SUCCESS or ERROR
 */
ErrorStatus AS8510_ReadFromAddr(uint8_t addr, uint8_t *data, uint8_t len);

/* Reads from the specified register, but only 1 byte.
 * @param addr the address of the register
 * @return the byte that was read
 */
uint8_t AS8510_ReadRegister(uint8_t addr);

/* Gets the current from the AS8510
 * @return current in milliamps
 */
int32_t AS8510_GetCurrent();

#endif
