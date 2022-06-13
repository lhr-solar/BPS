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
    BattBalancing_Init();
    
    while(1){
        BattBalancing_Balance(Minions);
        //signal watchdog
        /*OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

        WDog_BitMap |= WD_BALANCING;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);*/


        //delay of 100ms
        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
        assertOSError(err);
    }
}
