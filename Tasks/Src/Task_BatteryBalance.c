/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include <stdlib.h>
#include "BatteryBalancing.h"

//declared in Tasks.c
extern cell_asic Minions[NUM_MINIONS];

void Task_BatteryBalance(void *p_arg) {
    (void)p_arg;

    while(1){
        // Gets the start time of the task
        BPS_OS_ERR err;
        CPU_TS time = OSTimeGet(&err);
        Balancing_Balance(Minions);
        //signal watchdog
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        WDog_BitMap |= WD_BALANCING;

        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE);
        
        // Gets the time it takes to run as task
        time-=OSTimeGet(&err);
        RTOS_BPS_DelayMs(100);
    }
}
