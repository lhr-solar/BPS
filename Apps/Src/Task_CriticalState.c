#include "os.h"
#include "config.h"
#include "BSP_Contactor.h"
#include "CANbus.h"
#include "Tasks.h"

void Task_CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;
    CANData_t CanData;
    CANPayload_t CanMSG; 
    CanMSG.idx = 0;
    CanData.b = 1;
    CanData.f = 0;
    CanData.h = 0;
    CanData.w = 0;
    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    while(OSSemPend(&SafetyCheck_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err) != 4);
    // Turn Contactor On
    BSP_Contactor_On();
    // Push All Clear message to CAN Q
    CANbus_Send(ALL_CLEAR, CanMSG);
    // Push Contactor State message to CAN Q
    CANbus_Send(CONTACTOR_STATE, CanMSG);
    OSTaskDel(NULL, &err); // Delete task
}
