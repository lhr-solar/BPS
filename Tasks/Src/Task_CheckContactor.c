/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "RTOS_BPS.h"
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

    while(1) {
        //delay of 250ms
        RTOS_BPS_DelayMs(250);

        // fault if the contactor is open
        if (Contactor_GetState(HV_CONTACTOR_OUT != true)) {
            Fault_BitMap |= Fault_ESTOP;
            RTOS_BPS_SemPost(&Fault_Sem4, OS_OPT_POST_1);
        }
    }
}
