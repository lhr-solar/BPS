#include "CANbus.h"
#include "BSP_CAN.h"
#include "os.h"
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
static OS_MUTEX CANbus_TxMutex;
static OS_MUTEX CANbus_RxMutex;
static OS_SEM	CANbus_MailSem4;
static OS_SEM	CANbus_ReceiveSem4;

/**
 * @brief   Releases hold of the mailbox semaphore.
 * @note	Do not call directly.
 */
static void CANbus_Release(void) {
	OS_ERR err;

	OSSemPost(&CANbus_MailSem4,
			  OS_OPT_POST_1,
			  &err);
	assertOSError(err);
}

/**
 * @brief	Increments the receive semaphore.
 * @note	Do not call directly.
 */
static void CANbus_CountIncoming(void) {
	OS_ERR err;

	OSSemPost(&CANbus_ReceiveSem4,
			  OS_OPT_POST_1,
			  &err);
	assertOSError(err);
}

/**
 * @brief   Initializes the CAN system
 * @param   loopback	: if we should use loopback mode (for testing)	
 * @return  None
 */
void CANbus_Init(bool loopback) {
	OS_ERR err;

	OSMutexCreate(&CANbus_TxMutex,
				  "CAN TX Lock",
				  &err);
	assertOSError(err);

	OSMutexCreate(&CANbus_RxMutex,
				  "CAN RX Lock",
				  &err);
	assertOSError(err);

	OSSemCreate(&CANbus_MailSem4,
                "CAN Mailbox Semaphore",
                3,	// Number of mailboxes
                &err);
	assertOSError(err);

	OSSemCreate(&CANbus_ReceiveSem4,
                "CAN Queue Counter Semaphore",
                0,
                &err);
	assertOSError(err);

	// Initialize and pass interrupt hooks
    BSP_CAN_Init(CANbus_CountIncoming, CANbus_Release, loopback);
}

// Static method, call CANbus_Send or CANbus_BlockAndSend instead
static ErrorStatus CANbus_SendMsg(CANId_t id, CANPayload_t payload) {
	uint8_t txdata[8];
	uint8_t data_length = 0;
	
	OS_ERR err;

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
	RTOS_BPS_MutexPend(&CANbus_TxMutex,
				0,
				OS_OPT_PEND_BLOCKING
				);

	// Write the data to the bus
	ErrorStatus retVal = BSP_CAN_Write(id, txdata, data_length);

	OSMutexPost(&CANbus_TxMutex,
				OS_OPT_POST_1,
				&err);
	assertOSError(err);

	return retVal;
}

/**
 * @brief   Transmits data onto the CANbus. If there are no mailboxes available,
 *          this will put the thread to sleep until there are.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if error, SUCCESS otherwise
 */
ErrorStatus CANbus_BlockAndSend(CANId_t id, CANPayload_t payload) {
	CPU_TS ts;
	OS_ERR err;

	// Pend for a mailbox (blocking)
	OSSemPend(&CANbus_MailSem4,
			  0,
			  OS_OPT_PEND_BLOCKING,
			  &ts,
			  &err);
	assertOSError(err);
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
    CPU_TS ts;
	OS_ERR err;
	
	// Check to see if a mailbox is available
	OSSemPend(&CANbus_MailSem4,
			  0,
			  OS_OPT_PEND_NON_BLOCKING,
			  &ts,
			  &err);
	assertOSError(err);

	// Send the message
	ErrorStatus result = CANbus_SendMsg(id, payload);
	if (result == ERROR) {
		CANbus_Release();
	}
	return result;
}

static ErrorStatus CANbus_GetMsg(CANId_t *id, uint8_t *buffer) {
	OS_ERR err;
	
	// The mutex is require to access the CAN receive queue.
	RTOS_BPS_MutexPend(&CANbus_RxMutex,
				0,
				OS_OPT_PEND_BLOCKING
				);

	// Write the data to the bus
	uint32_t id_int;
	uint8_t retVal = BSP_CAN_Read(&id_int, buffer);

	*id = id_int;

	OSMutexPost(&CANbus_RxMutex,
				OS_OPT_POST_1,
				&err);
	assertOSError(err);

	return retVal;
}

/**
 * @brief   Receives data from the CAN bus. This is a non-blocking operation.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was no message, SUCCESS otherwise.
 */
ErrorStatus CANbus_Receive(CANId_t *id, uint8_t *buffer) {
	CPU_TS ts;
	OS_ERR err;
	
	// Check to see if a mailbox is available
	OSSemPend(&CANbus_ReceiveSem4,
			  0,
			  OS_OPT_PEND_NON_BLOCKING,
			  &ts,
			  &err);
	assertOSError(err);

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
	CPU_TS ts;
	OS_ERR err;

	// Pend for a mailbox (blocking)
	OSSemPend(&CANbus_ReceiveSem4,
			  0,
			  OS_OPT_PEND_BLOCKING,
			  &ts,
			  &err);
	assertOSError(err);

	return CANbus_GetMsg(id, buffer);
}
