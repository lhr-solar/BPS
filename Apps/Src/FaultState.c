/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "Contactor.h"
#include "BSP_Lights.h"
#include "Voltage.h"
#include "Fans.h"
#include "Temperature.h"
#include "Amps.h"
#include "EEPROM.h"
#include "BSP_WDTimer.h"
#include "CLI.h"
#include "CANbus.h"
#include "BSP_UART.h"
#include "config.h"
#include "BSP_PLL.h"
#ifdef SIMULATION
#include "Simulator.h"
extern uint8_t stateCount;
#endif

#define MESSAGE_BUFFER 20000

/*
 * Note: do not call this directly if it can be helped.
 * Instead, call an RTOS function to unblock the mutex
 * that the Fault Task is pending on.
 */
void EnterFaultState() {

#ifndef SIMULATION
    __disable_irq();
#else
    OS_ERR oserr;
    OSSchedLock(&oserr);
    assertOSError(oserr);
#endif

    // Turn Contactor Off
    Contactor_Init();
    Contactor_Off(HVHIGH_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);
    Contactor_Off(HVLOW_CONTACTOR);
    //Set Fans to full speed
    Fans_Init();
    Fans_SetAll(TOPSPEED);
    // Turn Strobe Light On
    // Turn LEDs On and logs Error into EEPROM
    BSP_Lights_Init();
    BSP_Light_Off(RUN); //turn off run light
    BSP_Light_On(FAULT);

    EEPROM_Init();

    if (BSP_WDTimer_DidSystemReset()) {
        Fault_BitMap = Fault_WDOG;
    }

    // TODO: fix this so it works if there are multiple faults
    #ifdef SIMULATION
    char err[100] = {0};
    #endif
    switch (Fault_BitMap){
        case Fault_UVOLT:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - UNDERVOLT\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(UVOLT);
            break;
        case Fault_OVOLT:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - OVERVOLT\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(OVOLT);
            break;
        case Fault_OTEMP:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - OVERTEMP\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(OTEMP);
            break;
        case Fault_OCURR:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - OVERCURRENT\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(OCURR);
            break;
        case Fault_OW:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - WIRE\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(WIRE);  
            break;
        case Fault_HANDLER:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - FAULT HANDLER\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif    
            break;
        case Fault_OS:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - OS ERROR\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            break;
        case Fault_WDOG:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - WATCHDOG\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(WDOG);
            break;
        case Fault_CRC: //Cannot get Fault_CRC in Simulation
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - CRC\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(EXTRA);
            break;
        case Fault_ESTOP:
        #ifdef SIMULATION
            sprintf(err, "$$$ Entered fault in state {%d} - ELECTRICAL STOP\n", stateCount - 1);
            Simulator_Log_Location(LOG_INFO, err);
        #endif
            BSP_Light_On(WIRE); //This is normally for Open Wire, but is used for ESTOP since we do not check Open Wire as of 1/9/2023
            break;
    }

    EEPROM_LogError(Fault_BitMap);

    // TODO: create an interrupt-independent CAN interface, so we can use CAN from within a fault state
    // avoid infinite recursive faults, since our CAN Driver relies on the OS to work
    // also don't call CAN if the watchdog tripped, since CAN won't be initialized

    //Deinitialize CAN registers
    CANbus_DeInit();
    //Reinit CAN in fault state
    CANbus_Init(false, true);
    

#ifdef DEBUGMODE
    char command[COMMAND_SIZE];
#endif
    BSP_WDTimer_Init(); //This is in case we did not pass check contactor and watchdog timer was not initialized
    BSP_WDTimer_Start(); 
    while(1) {
        //Send Trip Readings
        CANPayload_t payload;
        payload.data.w = 1;
        CANbus_SendMsg_FaultState(TRIP, payload);
        BSP_PLL_DelayU(MESSAGE_BUFFER);

        //Send Contactor Readings
        payload.data.b = 0;
        CANbus_SendMsg_FaultState(CONTACTOR_STATE, payload);
        BSP_PLL_DelayU(MESSAGE_BUFFER);
        
        //Send Current Readings
        payload.data.w = Amps_GetReading();
        CANbus_SendMsg_FaultState(CURRENT_DATA, payload);
        BSP_PLL_DelayU(MESSAGE_BUFFER);
        

        //Send Voltage Readings
        for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            payload.idx = i;
            payload.data.w = Voltage_GetModuleMillivoltage(i);
            CANbus_SendMsg_FaultState(VOLT_DATA, payload);
            BSP_PLL_DelayU(MESSAGE_BUFFER);
        }

        //Send Temperature Readings
        for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module temperature data
            payload.idx = i;
            payload.data.w = Temperature_GetModuleTemperature(i);
            CANbus_SendMsg_FaultState(TEMP_DATA, payload);
            BSP_PLL_DelayU(MESSAGE_BUFFER);
        }

#ifdef DEBUGMODE
        if (BSP_UART_ReadLine(command)) CLI_Handler(command); // CLI
#endif
        BSP_WDTimer_Reset(); // WDOG Reset
#ifdef SIMULATION
        Simulator_Log(LOG_INFO, "Completed fault state\n");
        Simulator_Shutdown(0);
#endif
    }
}

// Rebind all the possible fault handlers to the fault state
void NMI_Handler()        { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void HardFault_Handler()  { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void MemManage_Handler()  { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void BusFault_Handler()   { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
void UsageFault_Handler() { Fault_BitMap = Fault_HANDLER; EnterFaultState(); }
