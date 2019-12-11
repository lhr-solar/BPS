/** CAN.h
 * CAN driver for the STM32F413
 */

#ifndef __CAN_H
#define __CAN_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "config.h"

// Generic BPS ID
#define CAN_ID_BPS										(0x01 << 8)
// Specific BPS IDs
#define CAN_ID_BPS_TRIP								0x002
#define CAN_ID_BPS_ALL_CLEAR					(CAN_ID_BPS | 0x01)
#define CAN_ID_BPS_OFF								(CAN_ID_BPS | 0x02)
#define CAN_ID_CURRENT_DATA						(CAN_ID_BPS | 0x03)
#define CAN_ID_TOTAL_VOLTAGE_DATA			(CAN_ID_BPS | 0x04)
#define CAN_ID_AVG_TEMPERATURE_DATA		(CAN_ID_BPS | 0x05)
#define CAN_ID_SOC_DATA								(CAN_ID_BPS | 0x06)
#define CAN_ID_WDOG_TRIGGERED					(CAN_ID_BPS | 0x07)
#define CAN_ID_ERROR									(CAN_ID_BPS | 0x08)

typedef enum {
	TRIP = 0x02,
	ALL_CLEAR = 0x101,
	CONTACTOR_STATE = 0x102,
	CURRENT_DATA = 0x103,
	VOLT_DATA = 0x104,
	TEMP_DATA = 0x105,
	SOC_DATA = 0x106,
	WDOG_TRIGGERED = 0x107,
	CAN_ERROR = 0x108,
} CANMessage_t;

typedef union {
	uint8_t b;
	uint16_t h;
	uint32_t w;
	float f;
} CANData_t;

/** CAN1_Init
 * Initializes CAN bus for pins:
 *		PB8 : RX
 *		PB9 : TX
 * @param CAN_Mode The mode for the CAN bus to operate in
 */
void CAN1_Init(int CAN_Mode);

int CAN1_Write(uint32_t id, uint8_t data[8], uint8_t length);

bool CAN1_Read(uint8_t *data);

//sends a message over CAN
//returns the same thing as CAN1_Write
int CAN1_Send(CANMessage_t message, CANData_t data);

#endif
