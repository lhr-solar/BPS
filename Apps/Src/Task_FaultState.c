#include "os.h"
#include "Tasks.h"
#include "BSP_Contactor.h"
#include "BSP_Lights.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Current.h"
#include "EEPROM.h"
#include "BSP_WDTimer.h"
#include "CLI.h"
#include "CANbus.h"
#include "BSP_UART.h"
#include "config.h"
//Manthan Upadhyaya wuz here: 10/2020

//Manthan Upadhyaya wuz here: 10/2020

//Manthan Upadhyaya wuz here: 10/2020

void Task_FaultState(void *p_arg) {
    (void)p_arg;
    OS_ERR err;
    CPU_TS ts;

    // BLOCKING =====================
    // Wait until a FAULT is signaled by another task.
    OSSemPend(&Fault_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    OSSemPost(&Fault_Sem4, OS_OPT_PEND_BLOCKING, &err);
    // Turn Contactor Off
    BSP_Contactor_Off();
    //Set Fans to full speed
    BSP_Fans_Set(1, 8);
    BSP_Fans_Set(2, 8);
    BSP_Fans_Set(3, 8);
    BSP_Fans_Set(4, 8);
    // Turn Strobe Light On
    // Turn LEDs On and logs Error into EEPROM
    BSP_Light_Off(RUN); //turn off run light
    BSP_Light_On(FAULT);
    if (Voltage_CheckStatus() == OVERVOLTAGE){
        BSP_Light_On(OVOLT);
        EEPROM_LogError(FAULT_HIGH_VOLT);
    }
    if (Voltage_CheckStatus() == UNDERVOLTAGE){
        BSP_Light_On(UVOLT);
        EEPROM_LogError(FAULT_LOW_VOLT);
    }
    if (Temperature_CheckStatus(Current_IsCharging()) == DANGER){
        BSP_Light_On(OTEMP);
        EEPROM_LogError(FAULT_HIGH_TEMP);
    }
    if (Current_CheckStatus(false) == DANGER){
        BSP_Light_On(OCURR);
        EEPROM_LogError(FAULT_HIGH_CURRENT);
    }
    if (BSP_WDTimer_DidSystemReset()){
        BSP_Light_On(WDOG);
        EEPROM_LogError(FAULT_WATCHDOG);
    }
    CANData_t data;
    data.b = 1;
    CANPayload_t Message;
    Message.data = data;
    // Push Trip message to CAN Q
    CANbus_Send(TRIP, Message);
    // Push Contactor State message to CAN Q
    data.b = 0;
    CANbus_Send(CONTACTOR_STATE, Message);
    char command[COMMAND_SIZE];
    while(1) {
        #ifdef DEBUGMODE
        if (BSP_UART_ReadLine(command)) CLI_Handler(command); // CLI
        #endif
        BSP_WDTimer_Reset(); // WDOG Reset
    }
}
