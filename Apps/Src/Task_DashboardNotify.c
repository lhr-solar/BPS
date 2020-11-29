#include "os.h"
#include "CANbus.h"
#include "stdio.h"
#include "Task.h"

void Task_DashboardNotify(void* p_arg) {
   (void)p_arg;

    OS_ERR err;
    // Create Motor Enable CAN Msg
    // Push message to CAN Q

    CANbus_Init();

    if (override == true) { //battery needs to be charged, motor needs to be disabled
        CANData_t data = { 1 };
        CANPayload_t payload = { .idx = 0, .data = data };
        CANMSG_t message = { MOTOR_DISABLE, payload };
        OSQPost(&CANBus_MsgQ, &message, sizeof(CANMSG_t), &err);
    }
    else { //battery does not need to be charged, motor can be running
        CANData_t data = { 0 };
        CANPayload_t payload = { .idx = 0, .data = data };
        CANMSG_t message = { MOTOR_DISABLE, payload };
        OSQPost(&CANBus_MsgQ, &message, sizeof(CANMSG_t), &err);
    }

    //1 tick every 10 millisconds, so 50 ticks for a delay of 500ms
    //OS_OPT_TIME_DLY specifies relative delay, so the delay is started from the “current time + dly”.
    OSTimeDly(50, OS_OPT_TIME_DLY, &err);
    assertOSError(err);

}
