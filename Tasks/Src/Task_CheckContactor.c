/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "os.h"
#include "Contactor.h"
#include "Tasks.h"

/* 
 * Launch this task once the contactor has been closed.
 * This will trip the BPS if the contactor is open.
 * The purpose of this is to trip the BPS if the emergency
 * cutoff switch is pressed.
 */
void Task_CheckContactor(void *p_arg) {
    (void)p_arg;
    OS_ERR err;

    while(1) {
        //delay of 250ms
        OSTimeDly(25, OS_OPT_TIME_DLY, &err);
        assertOSError(err);

        // fault if the contactor is open
        if (BSP_Contactor_GetState(HVHIGH_CONTACTOR) != true) {
            Fault_BitMap |= Fault_ESTOP;
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
			assertOSError(err);
        }
    }
}
