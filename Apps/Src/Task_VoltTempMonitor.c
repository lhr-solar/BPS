#include "Tasks.h"
#include "os.h"

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // BLOCKING =====================
        // Update Voltage Measurements
        // Check if voltage is NOT safe:

        // BLOCKING =====================
        // Update Open Wire Measurements
        // Check if open wire is NOT safe:

        // BLOCKING =====================
        // Update Temperature Measurements
        // Check if temperature is NOT safe:

        // Control Fans depending on temperature

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);

        //ASSERT err

        WDog_BitMap |= WD_VOLT_TEMP;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);

        //ASSERT err
    }
}
