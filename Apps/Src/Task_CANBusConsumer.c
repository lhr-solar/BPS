/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "os.h"
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"


void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    OS_MSG_SIZE msg_size;
    CPU_TS ts;
    CANMSG_t *message;

    CANbus_Init();
    
    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        message = (CANMSG_t *)OSQPend(&CANBus_MsgQ,
                                    0,  //timeout
                                    OS_OPT_PEND_BLOCKING,   //optimization
                                    &msg_size,  
                                    &ts,    //pointer to variable that will recieve timestamp of when the message was recivied.
                                    &err);
        assertOSError(err);
        CANbus_BlockAndSend(message->id, message->payload);
    }
}
/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "os.h"
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"


void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    OS_MSG_SIZE msg_size;
    CPU_TS ts;
    CANMSG_t *message;

    CANbus_Init();
    
    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        message = (CANMSG_t *)OSQPend(&CANBus_MsgQ,
                                    0,  //timeout
                                    OS_OPT_PEND_BLOCKING,   //optimization
                                    &msg_size,  
                                    &ts,    //pointer to variable that will recieve timestamp of when the message was recivied.
                                    &err);
        assertOSError(err);
        CANbus_BlockAndSend(message->id, message->payload);
    }
}
