/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include <string.h>
#include "Tasks.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Fans.h"
#include "CANbus.h"
#include "Amps.h"
#include "CAN_Queue.h"

//declared in Tasks.c
extern cell_asic Minions[NUM_MINIONS];

static uint32_t voltage_totals[NUM_BATTERY_MODULES] = {0};

static uint32_t task_cycle_counter = 0;

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg; 

    Fans_Init();
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
        task_cycle_counter++;       // used for output and sampling decimation / averaging

        // BLOCKING =====================
        // Update Voltage Measurements
        Voltage_UpdateMeasurements();
        
        // Check if voltage is NOT safe:
        SafetyStatus voltageStatus = Voltage_CheckStatus();
        if(voltageStatus != SAFE) {
            if (voltageStatus == UNDERVOLTAGE) Fault_BitMap = Fault_UVOLT;
            if (voltageStatus == OVERVOLTAGE) Fault_BitMap = Fault_OVOLT;
            EnterFaultState(); 
        } else if((voltageStatus == SAFE) && (!voltageHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
            voltageHasBeenChecked = true;
        }
        
        //Send measurements to CAN queue
        bool charge_enable = true;
        CanMsg.id = VOLTAGE_DATA_ARRAY;
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            
            uint16_t voltage = Voltage_GetModuleMillivoltage(i);
            voltage_totals[i] += voltage;

            if (voltage > CHARGE_DISABLE_VOLTAGE){
                charge_enable = false;
            }

            // Send voltages over CAN every ODR_VOLTAGE_AVERAGING iterations
            if (task_cycle_counter % ODR_VOLTAGE_AVERAGING == 0) {
                CanPayload.idx = i;
                CanData.w = (int)(voltage_totals[i] / ODR_VOLTAGE_AVERAGING);
                CanPayload.data = CanData;
                CanMsg.payload = CanPayload;
                CAN_TransmitQueue_Post(CanMsg);

                voltage_totals[i] = 0;
            }
        }
        
        // BLOCKING =====================
        // Check if open wire is NOT safe:
    
        // TODO: get OpenWire to not take forever and actually run this
        // SafetyStatus wireStatus = Voltage_OpenWire();
        SafetyStatus wireStatus = SAFE;
        
        if(wireStatus != SAFE) {
            Fault_BitMap = Fault_OW;
            EnterFaultState();
        } else if((wireStatus == SAFE) && (!openWireHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1); 
            openWireHasBeenChecked = true;
        }

        // BLOCKING =====================
        // Update Temperature Measurements (every ODR_TEMPERATURE_DECIMATION iterations)
        if (task_cycle_counter % ODR_TEMPERATURE_DECIMATION == 0) {
            Temperature_UpdateAllMeasurements();
        }

        // Check if temperature is NOT safe: for all modules
        SafetyStatus temperatureStatus = Temperature_CheckStatus(Amps_IsCharging());
        if(temperatureStatus != SAFE) {
            Fault_BitMap = Fault_OTEMP;
            EnterFaultState();
        } else if((temperatureStatus == SAFE) && (!temperatureHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
            temperatureHasBeenChecked = true;
        } 
        //PID loop - sets fan speed based on avg temperature and desired temperature
        //overrides PID loop if above PID_MAX_TEMPERATURE or if it's FAULT
        if (temperatureStatus == SAFE) {
            Fans_SetAll(Temperature_PID_Output(Temperature_GetTotalPackAvgTemperature(), PID_DESIRED_TEMPERATURE));
        }

        //Check if car should be allowed to charge or not
        for (uint8_t sensor = 0; sensor < NUM_TEMPERATURE_SENSORS; sensor++) {
            uint32_t temp = Temperature_GetSingleTempSensor(sensor);
            if (temp > MAX_CHARGE_TEMPERATURE_LIMIT && temp < MAX_DISCHARGE_TEMPERATURE_LIMIT){
                //suggest that the battery should not be charged
                charge_enable = false;
            }
        }
        if (!charge_enable){
            CanMsg.id = CHARGING_ENABLED;
            CanPayload.idx = 0;
            CanData.b = 0;
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            CAN_TransmitQueue_Post(CanMsg);
        }
        else {
            //if the temperature gets low enough, suggest battery can be charged
            CanMsg.id = CHARGING_ENABLED;
            CanPayload.idx = 0;
            CanData.b = 1;
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            CAN_TransmitQueue_Post(CanMsg);
        }
        //Send measurements to CAN queue
        CanMsg.id = TEMPERATURE_DATA_ARRAY;
        for (uint8_t sensor = 0; sensor < NUM_TEMPERATURE_SENSORS; sensor++) {
            CanPayload.idx = sensor;
            CanData.w = Temperature_GetSingleTempSensor(sensor);
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            CAN_TransmitQueue_Post(CanMsg);
        }

        Fans_SetAll(TOPSPEED);

        //signal watchdog
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        WDog_BitMap |= WD_VOLT_TEMP; //Set watchdog bits for task

        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE); 
        
        //delay of 20ms
        RTOS_BPS_DelayMs(20);
    }
}
