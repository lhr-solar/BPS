/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "os.h"
#include "Tasks.h"
#include "BSP_Contactor.h"
#include "BSP_Lights.h"
#include "Voltage.h"
#include "BSP_Fans.h"
#include "Temperature.h"
#include "Amps.h"
#include "EEPROM.h"
#include "BSP_WDTimer.h"
#include "CLI.h"
#include "CANbus.h"
#include "BSP_UART.h"
#include "config.h"

/*
 * Note: do not call this directly if it can be helped.
 * Instead, call an RTOS function to unblock the mutex
 * that the Fault Task is pending on.
 */
void EnterFaultState() {
    // Turn Contactor Off
    BSP_Contactor_Init();
    BSP_Contactor_Off();
    //Set Fans to full speed
    BSP_Fans_Init();
    BSP_Fans_SetAll(TOPSPEED);
    // Turn Strobe Light On
    // Turn LEDs On and logs Error into EEPROM
    BSP_Lights_Init();
    BSP_Light_Off(RUN); //turn off run light
    BSP_Light_On(FAULT);

    EEPROM_Init();

    if (BSP_WDTimer_DidSystemReset()) {
        Fault_BitMap = Fault_WDOG;
    }
    switch (Fault_BitMap){
        case Fault_UVOLT:
            BSP_Light_On(UVOLT);
            EEPROM_LogError(FAULT_LOW_VOLT);
            for(int i = 0; i < NUM_BATTERY_MODULES; i++) EEPROM_LogData(FAULT_LOW_VOLT, Voltage_GetModuleMillivoltage(i));
            break;
        case Fault_OVOLT:
            BSP_Light_On(OVOLT);
            EEPROM_LogError(FAULT_HIGH_VOLT);
            for(int i = 0; i < NUM_BATTERY_MODULES; i++) EEPROM_LogData(FAULT_HIGH_VOLT, Voltage_GetModuleMillivoltage(i));
            break;
        case Fault_OTEMP:
            BSP_Light_On(OTEMP);
            EEPROM_LogError(FAULT_HIGH_TEMP);
            uint8_t* tempArray = Temperature_GetModulesInDanger();
            for(int i = 0; i < NUM_TEMPERATURE_SENSORS; i++) EEPROM_LogData(FAULT_HIGH_TEMP, tempArray[i]);
            break;
        case Fault_OCURR:
            BSP_Light_On(OCURR);
            EEPROM_LogError(FAULT_HIGH_CURRENT);
            EEPROM_LogData(FAULT_HIGH_CURRENT, Amps_GetReading());
            break;
        case Fault_OW:
            BSP_Light_On(WIRE);
            EEPROM_LogError(FAULT_OPEN_WIRE);
            EEPROM_LogData(FAULT_OPEN_WIRE, Voltage_GetOpenWire());
            break;
        case Fault_HANDLER:
            EEPROM_LogError(FAULT_HANDLER);
            break;
        case Fault_OS:
            EEPROM_LogError(FAULT_OS_ERROR);
            break;
        case Fault_WDOG:
            BSP_Light_On(WDOG);
            EEPROM_LogError(FAULT_WATCHDOG);
            break;
    }

    // avoid infinite recursive faults, since our CAN Driver relies on the OS to work
    if (Fault_BitMap != Fault_OS) {
        CANData_t data;
        data.b = 1;
        CANPayload_t Message;
        Message.data = data;
        // Push Trip message to CAN Q
        CANbus_BlockAndSend(TRIP, Message);
        // Push Contactor State message to CAN Q
        data.b = 0;
        CANbus_BlockAndSend(CONTACTOR_STATE, Message);
    }

#ifdef DEBUGMODE
    char command[COMMAND_SIZE];
#endif
    while(1) {
#ifdef DEBUGMODE
        if (BSP_UART_ReadLine(command)) CLI_Handler(command); // CLI
#endif
        BSP_WDTimer_Reset(); // WDOG Reset
    }
}

void Task_FaultState(void *p_arg) {
    (void)p_arg;
    OS_ERR err;
    CPU_TS ts;

    // BLOCKING =====================
    // Wait until a FAULT is signaled by another task.
    OSSemPend(&Fault_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    
    EnterFaultState();
}

// Rebind all the possible fault handlers to the fault state
void NMI_Handler()        { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void HardFault_Handler()  { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void MemManage_Handler()  { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void BusFault_Handler()   { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void UsageFault_Handler() { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
