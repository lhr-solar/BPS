/* Copyright (c) 2022 UT Longhorn Racing Solar */
#include "os.h"
#include "Tasks.h"
#include <stdlib.h>
#include "BatteryBalancing.h"

//declared in Tasks.c
extern cell_asic Minions[NUM_MINIONS];

void Task_BatteryBalance(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1){
        Balancing_Balance(Minions);
        //signal watchdog
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        WDog_BitMap |= WD_BALANCING;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);


        //delay of 100ms
        RTOS_BPS_DelayMs(100);
        assertOSError(err);
    }
}
