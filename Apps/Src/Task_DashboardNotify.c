#include "os.h"
#include "CANbus.h"
#include "common.h"
#include "Tasks.h"

void Task_DashboardNotify(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CANData_t DashboardData;
    DashboardData.f = 0;
    DashboardData.h = 0;
    DashboardData.w = 0;
    CANPayload_t DashboardPayload;
    CANMSG_t DashboardMSG;
    DashboardMSG.id = MOTOR_DISABLE;
    while(1){
        // Create Motor Enable CAN Msg
        if(AdminOverride) DashboardData.b = 1;//motor disabled if true
        else DashboardData.b = 0; //motor enable if false
        DashboardPayload.data = DashboardData;
        DashboardMSG.payload = DashboardPayload;
        // Push message to CAN Q
        OSQPost(&CANBus_MsgQ, &DashboardMSG, sizeof(DashboardMSG), OS_OPT_POST_FIFO, &err);
        assertOSError(err);
        OSTimeDly(50, OS_OPT_TIME_DLY, &err);
    }
}
