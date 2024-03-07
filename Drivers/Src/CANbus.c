/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "CANbus.h"
#include "BSP_CAN.h"
#include "RTOS_BPS.h"
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
static volatile bool initialized = false;	// used to check for double initialization -- volatile so we don't accidentally optimize it out

/**
 * @brief   Releases hold of the mailbox semaphore.
 * @note	Do not call directly.
 */
static void CANbus_Release(void) {
    RTOS_BPS_SemPost(&CANbus_MailSem4, OS_OPT_POST_1);
}

/**
 * @brief	Increments the receive semaphore.
 * @note	Do not call directly.
 */
static void CANbus_CountIncoming(void) {
    RTOS_BPS_SemPost(&CANbus_ReceiveSem4, OS_OPT_POST_1);
}

/**
 * @brief   Initializes the CAN system. If reinitializing for fault state, must
 * deinitialize first.
 * @param   loopback	: if we should use loopback mode (for testing)	
 * @param 	faultState  : determines whether to implement Rx and Tx interrupts
 * @param   txIDFilter    : array of IDs to accept messages from. Pass NULL for no filtering.
 * @param   txIDFilterLen : length of txIDFilter array. Max 28 * 4 (28 filter banks * 4 IDs per bank)
 * @return  None
 */
void CANbus_Init(bool loopback, bool faultState, uint16_t *txIDFilter, uint8_t txIDFilterLen) {
    if(!faultState && !initialized){
        initialized = true;
        RTOS_BPS_MutexCreate(&CANbus_TxMutex, "CAN TX Lock");
        RTOS_BPS_MutexCreate(&CANbus_RxMutex, "CAN RX Lock");
        RTOS_BPS_SemCreate(&CANbus_MailSem4, "CAN Mailbox Semaphore", 3); // # of mailboxes
        RTOS_BPS_SemCreate(&CANbus_ReceiveSem4, "CAN Queue Counter Semaphore", 0);
        // Initialize and pass interrupt hooks
        BSP_CAN_Init(CANbus_CountIncoming, CANbus_Release, faultState, loopback, txIDFilter, txIDFilterLen);
    }else if (faultState){
        BSP_CAN_Init(NULL, NULL, faultState, loopback, NULL, 0);
    }
}

/**
 * @brief  name says it all
 */
bool CANbus_IsInitialized() {
    return initialized;
}

/**
 * @brief   Deinitializes the CAN system
 * @return  None
 */
void CANbus_DeInit() {
    BSP_CAN_DeInit();
}

// Static method, call CANbus_Send or CANbus_BlockAndSend instead
static ErrorStatus CANbus_SendMsg(CANID_t id, CANPayload_t payload) {
    //set parameters for the CAN message being sent: datalength, idx_used
    uint8_t txdata[8];
    bool idx_used = CanMetadataLUT[id].idx_used;
    uint8_t payload_size = CanMetadataLUT[id].len;
    uint8_t data_length = payload_size + (idx_used ? 1 : 0);
    void* canData = (void*) &payload.data;

    //Copies over payload data into CAN txdata 
    if (idx_used) {
        txdata[0] = payload.idx;
        memcpy(&txdata[1], canData, payload_size);
    } else {
        memcpy(txdata, canData, payload_size);
    }

    // The mutex is require to access the CAN bus.
    // This is because the software is responsible for
    // choosing the mailbox to put the message into,
    // leaving a possible race condition if not protected.
    RTOS_BPS_MutexPend(&CANbus_TxMutex, OS_OPT_PEND_BLOCKING);

    // Write the data to the bus
    ErrorStatus retVal = BSP_CAN_Write(id, txdata, data_length);

    RTOS_BPS_MutexPost(&CANbus_TxMutex, OS_OPT_POST_1);

    return retVal;
}

ErrorStatus CANbus_SendMsg_FaultState(CANID_t id, CANPayload_t payload) {
    //set parameters for the CAN message being sent: datalength, idx_used
    uint8_t txdata[8];
    bool idx_used = CanMetadataLUT[id].idx_used;
    uint8_t payload_size = CanMetadataLUT[id].len;
    uint8_t data_length = payload_size + (idx_used ? 1 : 0);
    void* canData = (void*) &payload.data;
    
    //Copies over payload data into CAN txdata 
    if (idx_used) {
        txdata[0] = payload.idx;
        memcpy(&txdata[1], canData, payload_size);
    } else {
        memcpy(txdata, canData, payload_size);
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
ErrorStatus CANbus_BlockAndSend(CANID_t id, CANPayload_t payload) {
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
ErrorStatus CANbus_Send(CANID_t id, CANPayload_t payload) {	
    // Check to see if a mailbox is available
    RTOS_BPS_SemPend(&CANbus_MailSem4, OS_OPT_PEND_NON_BLOCKING);
    // Send the message
    ErrorStatus result = CANbus_SendMsg(id, payload);
    if (result == ERROR) {
        CANbus_Release();
    }
    return result;
}

static ErrorStatus CANbus_GetMsg(CANID_t *id, CANPayload_t *payload) {
    uint32_t id_int;

    // Mutex is required for BSP access
    RTOS_BPS_MutexPend(&CANbus_RxMutex, OS_OPT_PEND_BLOCKING);
    uint8_t retVal = BSP_CAN_Read(&id_int, payload->data.bytes);
    RTOS_BPS_MutexPost(&CANbus_RxMutex, OS_OPT_POST_1);
    // note: data received is not valid if retVal is ERROR
    if (retVal == ERROR) return ERROR;

    // fill out payload according to CAN ID
    *id = id_int;

    bool idx_used = CanMetadataLUT[id_int].idx_used;
    uint8_t payload_size = CanMetadataLUT[id_int].len;
    if (idx_used) {
        payload->idx = payload->data.bytes[0];
        memcpy(&payload->data.bytes[1], &payload->data.bytes[0], payload_size);
    }

    return retVal;
}

/**
 * @brief   Receives data from the CAN bus. This is a non-blocking operation.
 * @param   id : pointer to id variable
 * @param   payload : pointer to payload buffer
 * @return  ERROR if there was no message, SUCCESS otherwise.
 */
ErrorStatus CANbus_Receive(CANID_t *id, CANPayload_t *payload) {
    // Check to see if a mailbox is available
    BPS_OS_SEM_CTR cnt = RTOS_BPS_SemPend(&CANbus_ReceiveSem4, OS_OPT_PEND_NON_BLOCKING);
    if (cnt == BPS_OS_SEM_WOULD_BLOCK) {
        return ERROR;
    }
    // Send the message
    return CANbus_GetMsg(id, payload);
}

/**
 * @brief   Waits for data to arrive.
 * @param   id : pointer to id variable
 * @param   payload : pointer to payload buffer
 * @return  ERROR if there was an error, SUCCESS otherwise.
 */
ErrorStatus CANbus_WaitToReceive(CANID_t *id, CANPayload_t *payload) {
    // Pend for a mailbox (blocking)
    RTOS_BPS_SemPend(&CANbus_ReceiveSem4, OS_OPT_PEND_BLOCKING);
    return CANbus_GetMsg(id, payload);
}
