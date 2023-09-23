/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "CANbus.h"
#include "BSP_CAN.h"
#include "FreeRTOS.h"
#include "Tasks.h"

/* Locking mechanism for the CAN bus.
 * 
 * The mutexes gate access to the BSP layer.
 * 
 * The mail sem4 counts the number of mailboxes
 * in use, so that we can block if we can't get
 * a hold of one.
 * 
 * The receive sem4 counts the number of messages
 * currently in the receiving queue.
 */
static SemaphoreHandle_t CANbus_TxMutex;
static SemaphoreHandle_t CANbus_RxMutex;
static SemaphoreHandle_t	CANbus_MailSem4;
static SemaphoreHandle_t	CANbus_ReceiveSem4;

/**
 * @brief   Releases hold of the mailbox semaphore.
 * @note	Do not call directly.
 */
static void CANbus_Release(void) {
	xSemaphoreGive(CANbus_MailSem4);
}

/**
 * @brief	Increments the receive semaphore.
 * @note	Do not call directly.
 */
static void CANbus_CountIncoming(void) {
	xSemaphoreGive(CANbus_ReceiveSem4);
}

/**
 * @brief   Initializes the CAN system. If reinitializing for fault state, must
 * deinitialize first.
 * @param   loopback	: if we should use loopback mode (for testing)	
 * @param 	faultState  : determines whether to implement Rx and Tx interrupts
 * @return  None
 */
void CANbus_Init(bool loopback, bool faultState) {
	if(!faultState){
		CANbus_TxMutex = xSemaphoreCreateMutex();
		CANbus_RxMutex = xSemaphoreCreateMutex();
		CANbus_MailSem4 = xSemaphoreCreateCounting(255, 3);
		CANbus_ReceiveSem4 = xSemaphoreCreateBinary();
		// Initialize and pass interrupt hooks
    	BSP_CAN_Init(CANbus_CountIncoming, CANbus_Release, faultState, loopback);
	}else{
		BSP_CAN_Init(NULL, NULL, faultState, loopback);
	}

}

/**
 * @brief   Deinitializes the CAN system
 * @return  None
 */
void CANbus_DeInit() {
	BSP_CAN_DeInit();
}

// Static method, call CANbus_Send or CANbus_BlockAndSend instead
static ErrorStatus CANbus_SendMsg(CANId_t id, CANPayload_t payload) {
	uint8_t txdata[8];
	uint8_t data_length = 0;

	// TODO: is it really best to keep the list of
	//		 valid messages to be sending in the driver?
	switch (id) {
		// Handle messages with one byte of data
		case TRIP:
		case ALL_CLEAR:
		case CONTACTOR_STATE:
		case WDOG_TRIGGERED:
		case CAN_ERROR:
		case CHARGE_ENABLE:
			data_length = 1;
			memcpy(txdata, &payload.data.b, sizeof(payload.data.b));
			break;

		// Handle messages with 4 byte data
		case CURRENT_DATA:
		case SOC_DATA:
			data_length = 4;
			memcpy(txdata, &payload.data.w, sizeof(payload.data.w));
			break;

		// Handle messages with idx + 4 byte data
		case VOLT_DATA:
		case TEMP_DATA:
			data_length = 5;
			txdata[0] = payload.idx;
			memcpy(&txdata[1], &payload.data.w, sizeof(payload.data.w));
			break;

		// Handle invalid messages
		default:
			return ERROR;	// Do nothing if invalid
	}

	// The mutex is require to access the CAN bus.
	// This is because the software is responsible for
	// choosing the mailbox to put the message into,
	// leaving a possible race condition if not protected.
	xSemaphoreTake(CANbus_TxMutex, (TickType_t)portMAX_DELAY); 

	// Write the data to the bus
	ErrorStatus retVal = BSP_CAN_Write(id, txdata, data_length);

	xSemaphoreGive(CANbus_TxMutex);

	return retVal;
}

ErrorStatus CANbus_SendMsg_FaultState(CANId_t id, CANPayload_t payload) {
	uint8_t txdata[8];
	uint8_t data_length = 0;

	// TODO: is it really best to keep the list of
	//		 valid messages to be sending in the driver?
	switch (id) {
		// Handle messages with one byte of data
		case TRIP:
		case ALL_CLEAR:
		case CONTACTOR_STATE:
		case WDOG_TRIGGERED:
		case CAN_ERROR:
		case CHARGE_ENABLE:
			data_length = 1;
			memcpy(txdata, &payload.data.b, sizeof(payload.data.b));
			break;

		// Handle messages with 4 byte data
		case CURRENT_DATA:
		case SOC_DATA:
			data_length = 4;
			memcpy(txdata, &payload.data.w, sizeof(payload.data.w));
			break;

		// Handle messages with idx + 4 byte data
		case VOLT_DATA:
		case TEMP_DATA:
			data_length = 5;
			txdata[0] = payload.idx;
			memcpy(&txdata[1], &payload.data.w, sizeof(payload.data.w));
			break;

		// Handle invalid messages
		default:
			return ERROR;	// Do nothing if invalid
	}

	while (BSP_CAN_Write(id, txdata, data_length) == ERROR);
	return SUCCESS;
}	

/**
 * @brief   Transmits data onto the CANbus. If there are no mailboxes available,
 *          this will put the thread to sleep until there are.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if error, SUCCESS otherwise
 */
ErrorStatus CANbus_BlockAndSend(CANId_t id, CANPayload_t payload) {
	// Pend for a mailbox (blocking)
	RTOS_BPS_SemPend(&CANbus_MailSem4,OS_OPT_PEND_BLOCKING);
	ErrorStatus result = CANbus_SendMsg(id, payload);
	if (result == ERROR) {
		CANbus_Release();
	}
	return result;
}

/**
 * @brief   Transmits data onto the CANbus.
 *          This is non-blocking and will fail with an error if
 * 			the CAN mailboxes are fully occupied. Be sure to
 * 			check the return code or call CANbus_BlockAndSend.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if data wasn't sent, otherwise it was sent.
 */
ErrorStatus CANbus_Send(CANId_t id, CANPayload_t payload) {	
	// Check to see if a mailbox is available
	xSemaphoreTake(CANbus_MailSem4, 0);
	// Send the message
	ErrorStatus result = CANbus_SendMsg(id, payload);
	if (result == ERROR) {
		CANbus_Release();
	}
	return result;
}

static ErrorStatus CANbus_GetMsg(CANId_t *id, uint8_t *buffer) {
	// The mutex is require to access the CAN receive queue.
	xSemaphoreTake(CANbus_RxMutex, (TickType_t)portMAX_DELAY); 
	// Write the data to the bus
	uint32_t id_int;
	uint8_t retVal = BSP_CAN_Read(&id_int, buffer);

	*id = id_int;

	xSemaphoreGive(CANbus_RxMutex);

	return retVal;
}

/**
 * @brief   Receives data from the CAN bus. This is a non-blocking operation.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was no message, SUCCESS otherwise.
 */
ErrorStatus CANbus_Receive(CANId_t *id, uint8_t *buffer) {
	// Check to see if a mailbox is available
	xSemaphoreTake(CANbus_ReceiveSem4, 0);
	// Send the message
	return CANbus_GetMsg(id, buffer);
}

/**
 * @brief   Waits for data to arrive.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was an error, SUCCESS otherwise.
 */
ErrorStatus CANbus_WaitToReceive(CANId_t *id, uint8_t *buffer) {
	// Pend for a mailbox (blocking)
	xSemaphoreTake(CANbus_ReceiveSem4, (TickType_t)portMAX_DELAY);
	return CANbus_GetMsg(id, buffer);
}
