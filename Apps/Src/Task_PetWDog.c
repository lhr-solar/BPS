#include "Tasks.h"
#include "os.h"
#include <stdlib.h>

void Task_PetWDog(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    //take WDog Mutex
    OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    assertOSError(err);

    // If WDog_BitMap is all set:
    //      Reset watchdog timer


    //release WDog Mutex
    OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
    assertOSError(err);
}
