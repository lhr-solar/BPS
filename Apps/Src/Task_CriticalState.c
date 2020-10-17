#include "os.h"
#include "config.h"
#include "BSP_Contactor.h"
#include "CANbus.h"
#include "Tasks.h"

void Task_CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;
    CANData_t Can_Data;
    CANPayload_t Can_MSG; 
    Can_MSG.idx = 0;
    Can_Data.b = 1;
    Can_Data.f = 0;
    Can_Data.h = 0;
    Can_Data.w = 0;
    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    while(OSSemPend(&SafetyCheck_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err) != 4);
    // Turn Contactor On
    BSP_Contactor_On();
    // Push All Clear message to CAN Q
    CANbus_Send(ALL_CLEAR, Can_MSG);
    // Push Contactor State message to CAN Q
    CANbus_Send(CONTACTOR_STATE, Can_MSG);
}
