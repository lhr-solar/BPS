/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "EEPROM.h"
#include "Charge.h"
#include "Tasks.h"

void Task_LogInfo(void *p_arg) {
    (void)p_arg;

    while (1){
        // Gets the start time of the task
        BPS_OS_ERR err;
        CPU_TS time = OSTimeGet(&err);
        //sends percentage of charge left in the battery pack to EEPROM
        EEPROM_SetCharge(Charge_GetPercent());
        RTOS_BPS_DelaySecs(3);
        
        // Gets the time it takes to run as task
        time-=OSTimeGet(&err);
    }
}
