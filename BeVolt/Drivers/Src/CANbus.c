#include "CANbus.h"
#include "BSP_CAN.h"

static void floatTo4Bytes(uint8_t f, uint8_t bytes[4]);

/**
 * @brief   Initializes the CAN system
 * @param   None
 * @return  None
 */
void CANbus_Init(void) {
    BSP_CAN_Init();
}

/**
 * @brief   Transmits data onto the CANbus
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  0 if data wasn't sent, otherwise it was sent.
 */
int CANbus_Send(CANId_t id, CANPayload_t payload) {
    uint8_t txdata[5];
	
	switch (id) {
		case TRIP:
			return CAN1_Write(id, &payload.data.b, 1);

		case ALL_CLEAR:
			return CAN1_Write(id, &payload.data.b, 1);

		case CONTACTOR_STATE:
			return CAN1_Write(id, &payload.data.b, 1);

		case CURRENT_DATA:
			floatTo4Bytes(payload.data.f, &txdata[0]);
			return CAN1_Write(id, txdata, 4);

		case VOLT_DATA:
		case TEMP_DATA:
			txdata[0] = payload.idx;
			floatTo4Bytes(payload.data.f, &txdata[1]);
			return CAN1_Write(id, txdata, 5);

		case SOC_DATA:
			floatTo4Bytes(payload.data.f, &txdata[0]);
			return CAN1_Write(id, txdata, 4);

		case WDOG_TRIGGERED:
			return CAN1_Write(id, &payload.data.b, 1);

		case CAN_ERROR:
			return CAN1_Write(id, &payload.data.b, 1);
	}
	return 0;
}


static void floatTo4Bytes(uint8_t val, uint8_t bytes_array[4]) {
	uint8_t temp;
	// Create union of shared memory space
	union {
			float float_variable;
			uint8_t temp_array[4];
	} u;
	// Overite bytes of union with float variable
	u.float_variable = val;
	// Assign bytes to input array
	memcpy(bytes_array, u.temp_array, 4);
	temp = bytes_array[3];
	bytes_array[3] = bytes_array[0];
	bytes_array[0] = temp;
	temp = bytes_array[2];
	bytes_array[2] = bytes_array[1];
	bytes_array[1] = temp;	
}
