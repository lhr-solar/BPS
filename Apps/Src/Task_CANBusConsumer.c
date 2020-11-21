#include "os.h"
#include "Tasks.h"
#include "BSP_CAN.h"
#include "stm32f4xx.h"


void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    OS_MSG_SIZE msg_size;
    CPU_TS ts;
    CanTxMsg *message;

    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        message = (CanTxMsg *)OSQPend(&CANBus_MsgQ,
                                    0,  //timeout
                                    OS_OPT_PEND_BLOCKING,   //optimization
                                    &msg_size,  
                                    &ts,    //pointer to variable that will recieve timestamp of when the message was recivied.
                                    &err);
        assertOSError(err);
        CAN_Transmit(CAN1, &message);

        
    }
}
