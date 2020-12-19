#include "Tasks.h"
#include "os.h"
#include "Voltage.h"
#include "Temperature.h"
#include "BSP_Fans.h"
#include "CANbus.h"
#include "Amps.h"

//declared in Tasks.c
extern cell_asic Minions[NUM_MINIONS];

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    BSP_Fans_Init();
    Voltage_Init(Minions);
    Temperature_Init(Minions);

    // SafetyCheck_Sem4 must only be signaled once per parameter at system boot up.
    // These flags indicate was signaled for that parameter
    bool voltageHasBeenChecked = false;
    bool openWireHasBeenChecked = false;
    bool temperatureHasBeenChecked = false;

    CANData_t CanData;
    CANPayload_t CanPayload;
    CANMSG_t CanMsg;
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
        //Send measurements to CAN queue
        CanMsg.id = VOLT_DATA;
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            CanPayload.idx = i;
            int voltage = Voltage_GetModuleMillivoltage(i);
            CanData.f = (float)voltage/1000; //send data in volts
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            OSQPost(&CANBus_MsgQ, &CanMsg, sizeof(CanMsg), OS_OPT_POST_FIFO, &err);
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
        SafetyStatus temperatureStatus = Temperature_CheckStatus(Amps_IsCharging());
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
        //Send measurements to CAN queue
        CanMsg.id = TEMP_DATA;
        for (uint8_t i = 0; i < NUM_MINIONS; i++){ //send all temperature readings
            for (uint8_t j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++){
                if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j < NUM_TEMPERATURE_SENSORS){
                    CanPayload.idx = i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j;
                    CanData.f = (float)Temperature_GetSingleTempSensor(i, j);
                    CanPayload.data = CanData;
                    CanMsg.payload = CanPayload;
                    OSQPost(&CANBus_MsgQ, &CanMsg, sizeof(CanMsg), OS_OPT_POST_FIFO, &err);
                }
            }
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
