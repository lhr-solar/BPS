/* Copyright (c) 2022 UT Longhorn Racing Solar */
#include "os.h"
#include "EEPROM.h"
#include "Charge.h"
#include "Tasks.h"

void Task_LogInfo(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while (1){
        //sends percentage of charge left in the battery pack to EEPROM
        EEPROM_SetCharge(Charge_GetPercent());

        //1 tick every 10 millisconds, so 300 ticks for a delay of 3000ms --> 3s
        //OS_OPT_TIME_DLY specifies relative delay, so the delay is started from the "current time + dly".
        RTOS_BPS_TimeDelay(300);
        assertOSError(&err);
    }
}
