/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_WDTimer.h"
#include "BSP_Lights.h"

void Task_PetWDog(void *p_arg) {
    (void)p_arg;

    BSP_WDTimer_Init();
    BSP_WDTimer_Start();    

    while (1){
        //take WDog Mutex
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        // If WDog_BitMap is all set:
        // Reset watchdog timer
        // The three least significant bits of WDog_BitMap will be set if:
        // temp, voltage, current tasks are set --> value = 7
        if ((WDog_BitMap & WD_VOLT_TEMP) && (WDog_BitMap & WD_AMPERES)) {     
            BSP_WDTimer_Reset();
            BSP_Light_Toggle(RUN);
            WDog_BitMap = 0;
        }
        //release WDog Mutex
        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE);
        RTOS_BPS_DelayMs(500);
    }
}
