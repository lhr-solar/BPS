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
    bool isfirst_voltage_check = false;
    bool isfirst_openwire_Check = false;
    bool isfirst_temperature_check = false;

    while(1) {
        // BLOCKING =====================
        // Update Voltage Measurements
        Voltage_UpdateMeasurements();

        // Check if voltage is NOT safe:
        SafetyStatus voltage_status = Voltage_CheckStatus();
        if(voltage_status != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
        
        } else if((voltage_status == SAFE) && (!isfirst_voltage_check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_voltage_check = true;
        }

        // BLOCKING =====================
        // Update Open Wire Measurements
        // Check if open wire is NOT safe:
        SafetyStatus wire_status = Voltage_OpenWire();
        if(wire_status != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
        } else if((wire_status == SAFE) && (!isfirst_openwire_Check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_openwire_Check = true;
        }

        // BLOCKING =====================
        // Update Temperature Measurements
        Temperature_UpdateAllMeasurements();

        // Check if temperature is NOT safe:
        SafetyStatus temperature_status = Temperature_CheckStatus(Current_IsCharging());
        if(temperature_status != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
        } else if((temperature_status == SAFE) && (!isfirst_temperature_check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_temperature_check = true;
        }

        // Control Fans depending on temperature

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

        WDog_BitMap |= WD_VOLT_TEMP;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);
    }
}
