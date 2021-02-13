/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_CAN.h"
#include "stm32f4xx.h"
#include "os.h"

#define CAN_MODE        CAN_Mode_Normal

// Queue functions

typedef struct _msg {
    uint32_t id;
    uint8_t data[8];
} msg_t;

#define QUEUE_SIZE 10
typedef struct _queue {
    msg_t msgs[QUEUE_SIZE];
    
    uint8_t head;
    uint8_t tail;
} queue_t;

static queue_t gRxQueue;

// Return ERROR if failure
static ErrorStatus QueuePut(queue_t *target, msg_t *msg) {
    // Check to see if the queue is full
    if((target->head + 1) % QUEUE_SIZE == target->tail) {
        return ERROR;
    }

    // Add the msg to the queue
    target->msgs[target->head] = *msg;
    
    target->head = (target->head + 1) % QUEUE_SIZE;

    return SUCCESS;
}

// Return ERROR if failure
static ErrorStatus QueueGet(queue_t *target, msg_t *msg) {
    // Check if the queue is empty
    if(target->head == target->tail) {
        return ERROR;
    }

    // Get the msg from the queue
    *msg = target->msgs[target->tail];

    target->tail = (target->tail + 1) % QUEUE_SIZE;

    return SUCCESS;
}

// End Queue Functions

// CAN BSP

static CanTxMsg gTxMessage;
static CanRxMsg gRxMessage;

// User parameters for CAN events
static void (*gRxEvent)(void);
static void (*gTxEnd)(void);

/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   rxEvent : the function to execute when recieving a message. NULL for no action.
 * @param   txEnd   : the function to execute after transmitting a message. NULL for no action.
 * @return  None
 */
void BSP_CAN_Init(void (*rxEvent)(void), void (*txEnd)(void)) {
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    // Configure event handles
    gRxEvent  = rxEvent;
    gTxEnd    = txEnd;

    // Configure the queue
    gRxQueue.head = 0;
    gRxQueue.tail = 0;

    /* CAN GPIOs configuration **************************************************/

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF8_CAN1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF8_CAN1); 

    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* CAN configuration ********************************************************/  
    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* CAN register init */
    //CAN_DeInit(CAN1);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_MODE;
    CAN_InitStructure.CAN_SJW  = CAN_SJW_1tq;

    /* CAN Baudrate = 125 KBps
        * 1/(prescalar + (prescalar*(BS1+1)) + (prescalar*(BS2+1))) * Clk = CAN Baudrate
        * The clk is currently set to 80MHz
    */
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; 
    CAN_InitStructure.CAN_Prescaler = 16;
    CAN_Init(CAN1, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(CAN1, &CAN_FilterInitStructure);

    /* Transmit Structure preparation */
    gTxMessage.ExtId = 0x1;
    gTxMessage.RTR = CAN_RTR_DATA;
    gTxMessage.IDE = CAN_ID_STD;
    gTxMessage.DLC = 1;

    /* Receive Structure preparation */
    gRxMessage.StdId = 0x00;
    gRxMessage.ExtId = 0x00;
    gRxMessage.IDE = CAN_ID_STD;
    gRxMessage.DLC = 0;
    gRxMessage.FMI = 0;

    /* Enable FIFO 0 message pending Interrupt */
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    // Enable Rx interrupts
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	

    if(NULL != txEnd) {
        // Enable Tx Interrupts
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0; // TODO: assess both of these priority settings
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}

/**
 * @brief   Transmits the data onto the CAN bus with the specified id
 * @param   id : Message of ID. Also indicates the priority of message. The lower the value, the higher the priority.
 * @param   data : data to be transmitted. The max is 8 bytes.
 * @param   length : num of bytes of data to be transmitted. This must be <= 8 bytes or else the rest of the message is dropped.
 * @return  ERROR if module was unable to transmit the data onto the CAN bus. SUCCESS indicates data was transmitted.
 */
ErrorStatus BSP_CAN_Write(uint32_t id, uint8_t data[8], uint8_t length) {
    
    gTxMessage.StdId = id;
    gTxMessage.DLC = length;
	for(int i = 0; i < length; i++){
        gTxMessage.Data[i] = data[i];
    }
	
    ErrorStatus retVal = (ErrorStatus) (CAN_Transmit(CAN1, &gTxMessage) != 0);

    return retVal;
}

/**
 * @brief   Gets the data that was received from the CAN bus.
 * @note    Non-blocking statement
 * @pre     The data parameter must be at least 8 bytes or hardfault may occur.
 * @param   id : pointer to store id of the message that was received.
 * @param   data : pointer to store data that was received. Must be 8bytes or bigger.
 * @return  ERROR if nothing was received so ignore id and data that was received. SUCCESS indicates data was received and stored.
 */
ErrorStatus BSP_CAN_Read(uint32_t *id, uint8_t *data) {
    // If the queue is empty, return err
    if(gRxQueue.head == gRxQueue.tail) {
        return ERROR;
    }
    
    // Get the message
    msg_t msg;
    QueueGet(&gRxQueue, &msg);

    // Transfer the message to the provided pointers
    for(int i = 0; i < 8; i++){
        data[i] = msg.data[i];
    }
    *id = msg.id;

    return SUCCESS;
}

void CAN1_RX0_IRQHandler(void) {
    #ifdef RTOS
    asm("CPSID I"); //disable all interrupts
	//all cpu registers are supposed to be saved here
	//but that happens when the ISR is called
	OSIntEnter(); //increments value of nested interrupt counter
	unsigned int *address = 0;
	asm volatile ("STR SP, [%0]\n\t": "=r" ( address)); //Store SP in address
	if (OSIntNestingCtr == 1) OSTCBCurPtr->StkPtr = address;
    asm("CPSIE I");   // Re-enable interrupts
    #endif
    // Take any pending messages into a queue
    while(CAN_MessagePending(CAN1, CAN_FIFO0)) {
        CAN_Receive(CAN1, CAN_FIFO0, &gRxMessage);

        msg_t rxMsg;
        rxMsg.id = gRxMessage.StdId;
        memcpy(&rxMsg.data[0], gRxMessage.Data, 8);

        // Place the message in the queue
        if(QueuePut(&gRxQueue, &rxMsg)) {
            // If the queue was not already full...
            // Call the driver-provided function, if it is not null
            if(gRxEvent != NULL) {
                gRxEvent();
            }
        }
    }
    #ifdef RTOS
    OSIntExit();      // Signal to uC/OS
    #endif
}

void CAN1_TX_IRQHandler(void) {
    #ifdef RTOS
    asm("CPSID I"); //disable all interrupts
	//all cpu registers are supposed to be saved here
	//but that happens when the ISR is called
	OSIntEnter(); //increments value of nested interrupt counter
	unsigned int *address = 0;
	asm volatile ("STR SP, [%0]\n\t": "=r" ( address)); //Store SP in address
	if (OSIntNestingCtr == 1) OSTCBCurPtr->StkPtr = address;
    asm("CPSIE I");   // Re-enable interrupts
    #endif
    // Acknowledge 
    CAN_ClearFlag(CAN1, CAN_FLAG_RQCP0 | CAN_FLAG_RQCP1 | CAN_FLAG_RQCP2);

    // Call the function provided
    gTxEnd();
    #ifdef RTOS
    OSIntExit();      // Signal to uC/OS
    #endif
}
