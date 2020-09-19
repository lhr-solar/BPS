#include "os.h"
#include "Tasks.h"

void Task_FaultState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    // BLOCKING =====================
    // Wait until a FAULT is signaled by another task.
    OSSemPend(&Fault_Sem4,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);

    // Turn Contactor Off

    // Turn Strobe Light On

    // Turn LEDs On

    // Record State of BPS into EEPROM

    // Push Trip message to CAN Q

    // Push Contactor State message to CAN Q

    while(1) {
        // CLI
        // WDOG Reset
    }
}
