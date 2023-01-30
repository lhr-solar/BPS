/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include <stdlib.h>
#include "BatteryBalancing.h"

//declared in Tasks.c
extern cell_asic Minions[NUM_MINIONS];

void Task_BatteryBalance(void *p_arg) {
    (void)p_arg;

    while(1){
        Balancing_Balance(Minions);
        //signal watchdog
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        WDog_BitMap |= WD_BALANCING;

        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE);
        RTOS_BPS_DelayMs(100);
    }
}
