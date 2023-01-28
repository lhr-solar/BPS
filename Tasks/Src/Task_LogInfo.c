/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "EEPROM.h"
#include "Charge.h"
#include "Tasks.h"

void Task_LogInfo(void *p_arg) {
    (void)p_arg;

    while (1){
        //sends percentage of charge left in the battery pack to EEPROM
        EEPROM_SetCharge(Charge_GetPercent());
        RTOS_BPS_DelaySecs(3);
    }
}
