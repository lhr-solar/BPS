#include "os.h"
#include "config.h"
#include "Tasks.h"

void Task_CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    while(OSSemPend(&SafetyCheck_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err) != 4);

    // Turn Contactor On
    
    // Push All Clear message to CAN Q

    // Push Contactor State message to CAN Q
}
