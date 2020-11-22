#include "os.h"

void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message

        // BLOCKING =====================
        // Transmit to CAN Bus
    }
}
