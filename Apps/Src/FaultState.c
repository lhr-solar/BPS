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
#ifndef SIMULATION
#include "stm32f4xx_can.h"
#include "BSP_PLL.h"
#else
#include "Simulator.h"
extern uint8_t stateCount;
#endif

void delay_u(uint16_t micro);

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
    
    if ((Fault_BitMap & (Fault_OS | Fault_WDOG)) == 0) {
        //Deinitialize CAN registers
        CAN_DeInit(CAN1);
        //Reinit CAN in fault state
        CANbus_Init(false, true);
    }
    

#ifdef DEBUGMODE
    char command[COMMAND_SIZE];
#endif
    BSP_WDTimer_Init(); //This is in case we did not pass check contactor and watchdog timer was not initialized
    BSP_WDTimer_Start(); 
    while(1) {
        //Send Trip Message
        CANPayload_t payload;
        payload.idx = 0;
        payload.data.w = 1;
        CANbus_BlockAndSend_FaultState(TRIP, payload);
        delay_u(20000);

        // contactor message
        payload.data.w = 2;
        CANbus_BlockAndSend_FaultState(CONTACTOR_STATE, payload);
        delay_u(20000);
        
        //Send Current Readings
        payload.idx = 0;
        payload.data.w = Amps_GetReading();
        CANbus_BlockAndSend_FaultState(CURRENT_DATA, payload);
        delay_u(20000);
        

        //Send Voltage Readings
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            int voltage = Voltage_GetModuleMillivoltage(i);
            payload.idx = i;
            payload.data.w = voltage;
            CANbus_BlockAndSend_FaultState(VOLT_DATA, payload);
            delay_u(20000);
        }

        delay_u(20000);
        CANbus_BlockAndSend_FaultState(CURRENT_DATA, payload);
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            payload.idx = i;
            payload.data.w = Temperature_GetModuleTemperature(i);
            CANbus_BlockAndSend_FaultState(TEMP_DATA, payload);
            delay_u(20000);
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
