/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include <string.h>
#include "Tasks.h"
#include "os.h"
#include "Voltage.h"
#include "Temperature.h"
#include "BSP_Fans.h"
#include "CANbus.h"
#include "Amps.h"
#include "CAN_Queue.h"

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
            if (voltageStatus == UNDERVOLTAGE) Fault_BitMap = Fault_UVOLT;
            if (voltageStatus == OVERVOLTAGE) Fault_BitMap = Fault_OVOLT;
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
            CanData.w = voltage;
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            CAN_Queue_Post(CanMsg);
        }

        // BLOCKING =====================
        // Check if open wire is NOT safe:
	
	// TODO: get OpenWire to not take forever and actually run this
        // SafetyStatus wireStatus = Voltage_OpenWire();
	SafetyStatus wireStatus = SAFE;
        
        if(wireStatus != SAFE) {
            Fault_BitMap = Fault_OW;
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
            Fault_BitMap = Fault_OTEMP;
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
        //Check if car should be allowed to charge or not
        for(uint8_t k = 0; k < NUM_BATTERY_MODULES; k++){
            uint32_t temp = Temperature_GetModuleTemperature(k);
            if(temp > MAX_CHARGE_TEMPERATURE_LIMIT && temp < MAX_DISCHARGE_TEMPERATURE_LIMIT){
                //suggest that the battery should not be charged
                CanMsg.id = CHARGE_ENABLE;
                CanPayload.idx = 0;
                CanData.b = 0;
                CanPayload.data = CanData;
                CanMsg.payload = CanPayload;
                CAN_Queue_Post(CanMsg);
            }
            else {
                //if the temperature gets low enough, suggest battery can be charged
                CanMsg.id = CHARGE_ENABLE;
                CanPayload.idx = 0;
                CanData.b = 1;
                CanPayload.data = CanData;
                CanMsg.payload = CanPayload;
                CAN_Queue_Post(CanMsg);
            }
        }
        //Send measurements to CAN queue
        CanMsg.id = TEMP_DATA;
        for (uint8_t i = 0; i < NUM_MINIONS; i++){ //send all temperature readings
            for (uint8_t j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++){
                if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j < NUM_TEMPERATURE_SENSORS){
                    CanPayload.idx = i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j;
                    CanData.w = (uint32_t)Temperature_GetSingleTempSensor(i, j);
                    CanPayload.data = CanData;
                    CanMsg.payload = CanPayload;
                    CAN_Queue_Post(CanMsg);
                }
            }
        }

        // Control Fans depending on temperature
        // Right now this just sets them to maximum speed
        // Once we get a thermal model of the battery box, we can replace this with someting better
        BSP_Fans_SetAll(TOPSPEED);

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

        WDog_BitMap |= WD_VOLT_TEMP; //Set watchdog bits for task

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);

        //delay of 100ms
        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
        assertOSError(err);
    }
}
