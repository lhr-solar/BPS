#include "os.h"
#include "Tasks.h"
#include <stdlib.h>

void Task_BatteryBalance(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // Todo: Battery balancing

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

        WDog_BitMap |= WD_BALANCING;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);
    }
}
