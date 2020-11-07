#include "Tasks.h"
#include "os.h"
#include "Tasks.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Current.h"

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    // SafetyCheck_Sem4 must only be signaled once per parameter at system boot up.
    // These flags indicate was signaled for that parameter
    bool voltageHasBeenChecked = false;
    bool openWireHasBeenChecked = false;
    bool temperatureHasBeenChecked = false;

    while(1) {
        // BLOCKING =====================
        // Update Voltage Measurements
        Voltage_UpdateMeasurements();
        
        // Check if voltage is NOT safe:
        SafetyStatus voltageStatus = Voltage_CheckStatus();
        if(voltageStatus != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
            assertOSError(err);
        
        } else if((voltageStatus == SAFE) && (!voltageHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            assertOSError(err);

            voltageHasBeenChecked = true;
        }

        // BLOCKING =====================
        // Check if open wire is NOT safe:
        SafetyStatus wireStatus = Voltage_OpenWire();
        
        if(wireStatus != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
            assertOSError(err);

        } else if((wireStatus == SAFE) && (!openWireHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            assertOSError(err);

            openWireHasBeenChecked = true;
        }

        // BLOCKING =====================
        // Update Temperature Measurements
        Temperature_UpdateAllMeasurements();
        
        // Check if temperature is NOT safe:
        SafetyStatus temperatureStatus = Temperature_CheckStatus(Current_IsCharging());
        if(temperatureStatus != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
            assertOSError(err);

        } else if((temperatureStatus == SAFE) && (!temperatureHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            assertOSError(err);

            temperatureHasBeenChecked = true;
        }

        // Control Fans depending on temperature

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);
    }
}
