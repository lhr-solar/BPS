#include "os.h"
#include "Tasks.h"

void Task_Init(void *p_arg) {

    OS_ERR err;

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0,
                &err);

    // ASSERT err

    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);

    // ASSERT err

    OSMutexCreate(&MinionsASIC_Mutex,
                "Minions ASIC Mutex",
                &err);

    // ASSERT err

    // TODO: Initialize threads! Look at main.c for reference
}
