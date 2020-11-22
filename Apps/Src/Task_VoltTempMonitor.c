#include "Tasks.h"
#include "os.h"
#include "Tasks.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Current.h"
#include "BSP_Fans.h"
#include "CANbus.h"

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    BSP_Fans_Init();

    // SafetyCheck_Sem4 must only be signaled once per parameter at system boot up.
    // These flags indicate was signaled for that parameter
    bool voltageHasBeenChecked = false;
    bool openWireHasBeenChecked = false;
    bool temperatureHasBeenChecked = false;

    CANData_t CanData;
    CanData.b = 0;
    CanData.h = 0;
    CanData.w = 0;
    CANPayload_t CanPayload;
    CANMSG_t CanMsg;
    while(1) {
        // BLOCKING =====================
        // Update Voltage Measurements
        Voltage_UpdateMeasurements();
        CanMsg.id = VOLT_DATA;
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            CanPayload.idx = i;
            CanData.f = Voltage_GetModuleMillivoltage(i);
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            OSQPost(&CANBus_MsgQ, &CanMsg, sizeof(CanMsg), OS_OPT_POST_FIFO, &err);
        }
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
        CanMsg.id = TEMP_DATA;
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module temp data
            CanPayload.idx = i;
            CanData.f = Temperature_GetModuleTemperature(i);
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            OSQPost(&CANBus_MsgQ, &CanMsg, sizeof(CanMsg), OS_OPT_POST_FIFO, &err);
        }
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
        // Right now this just sets them to maximum speed
        // Once we get a thermal model of the battery box, we can replace this with someting better
        for (uint8_t i = 0; i < 4; i++){
            BSP_Fans_Set(i, TOPSPEED);
        }

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);
    }
}
