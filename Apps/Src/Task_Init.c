#include "os.h"
#include "Tasks.h"

void Task_Init(void *p_arg) {

    OS_ERR err;

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0,
                &err);
    assertOSError(err);

    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);
    assertOSError(err);

    OSMutexCreate(&WDog_Mutex,
                "Watchdog Mutex",
                &err);
    assertOSError(err);

    // TODO: Initialize threads! Look at main.c for reference
}
