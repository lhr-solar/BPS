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
	xSemaphoreTake(WDog_Mutex, (TickType_t)portMAX_DELAY); 

        WDog_BitMap |= WD_BALANCING;

	xSemaphoreGive(WDog_Mutex);
        RTOS_BPS_DelayMs(100);
    }
}
