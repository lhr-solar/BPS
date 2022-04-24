/* Copyright (c) 2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "os.h"
#include "BSP_WDTimer.h"
#include "BSP_Lights.h"

void Task_PetWDog(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    BSP_WDTimer_Init();
    BSP_WDTimer_Start();
    BSP_Lights_Init();
    

    while (1){
        //take WDog Mutex
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

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
        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);

        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
        assertOSError(err);
    }
}
