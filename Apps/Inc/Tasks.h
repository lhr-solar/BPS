#ifndef __TASKS_H
#define __TASKS_H

#include "os.h"
#include <stdint.h>

#define TASK_INIT_PRIO                      0
#define TASK_FAULT_STATE_PRIO               1
#define TASK_CRITICAL_STATE_PRIO            2
#define TASK_PETWDOG_PRIO                   3
#define TASK_VOLT_TEMP_MONITOR_PRIO         4
#define TASK_AMPERES_MONITOR_PRIO           5
#define TASK_BATTERY_BALANCE_PRIO           6 //BATTERY
#define TASK_CANBUS_CONSUMER_PRIO           7 //CANBUS
#define TASK_LOG_INFO_PRIO                  8 //LOGINFO
#define TASK_CLI_PRIO                       9  
#define TASK_IDLE_PRIO                      10

#define DEFAULT_STACK_SIZE                  256
#define WATERMARK_STACK_LIMIT               DEFAULT_STACK_SIZE/2

#define TASK_FAULT_STATE_STACK_SIZE         DEFAULT_STACK_SIZE
#define TASK_CRITICAL_STATE_STACK_SIZE      DEFAULT_STACK_SIZE
#define TASK_PETWDOG_STACK_SIZE             DEFAULT_STACK_SIZE
#define TASK_VOLT_TEMP_MONITOR_STACK_SIZE   512
#define TASK_AMPERES_MONITOR_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_LOG_INFO_STACK_SIZE            DEFAULT_STACK_SIZE
#define TASK_CANBUS_CONSUMER_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_BATTERY_BALANCE_STACK_SIZE     512
#define TASK_CLI_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_BLE_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_IDLE_STACK_SIZE                DEFAULT_STACK_SIZE
#define TASK_INIT_STACK_SIZE                DEFAULT_STACK_SIZE


/**
 * enum to keep track of which tasks correspond to each bit in WDog_BitMap
 */
enum WDogBits_e {
    WD_VOLT_TEMP = 0x1,
    WD_AMPERES = 0x2,
    WD_BALANCING = 0x4,
};

void Task_Init(void *p_arg);

void Task_FaultState(void *p_arg);

void Task_CriticalState(void *p_arg);

void Task_PetWDog(void *p_arg);

void Task_VoltTempMonitor(void *p_arg);

void Task_AmperesMonitor(void *p_arg);

void Task_LogInfo(void *p_arg);

void Task_CANBusConsumer(void *p_arg);

void Task_BatteryBalance(void *p_arg);

void Task_CLI(void *p_arg);

void Task_BLE(void *p_arg);

void Task_Idle(void *p_arg);

void Task_Init(void *p_arg);


/**
 * Used to assert if there has been an error in one of the OS functions
 * Kills the car if there is an OS error
 **/
void assertOSError(OS_ERR err);

/*
 * @brief   TCBs
 */
extern OS_TCB FaultState_TCB;
extern OS_TCB CriticalState_TCB;
extern OS_TCB PetWDog_TCB;
extern OS_TCB VoltTempMonitor_TCB;
extern OS_TCB AmperesMonitor_TCB;
extern OS_TCB LogInfo_TCB;
extern OS_TCB CANBusConsumer_TCB;
extern OS_TCB BatteryBalance_TCB;
extern OS_TCB CLI_TCB;
extern OS_TCB BLE_TCB;
extern OS_TCB Idle_TCB;
extern OS_TCB Init_TCB;
/**
 * @brief   Stacks
 */
extern CPU_STK FaultState_Stk[TASK_FAULT_STATE_STACK_SIZE];
extern CPU_STK CriticalState_Stk[TASK_CANBUS_CONSUMER_STACK_SIZE];
extern CPU_STK PetWDog_Stk[TASK_PETWDOG_STACK_SIZE];
extern CPU_STK VoltTempMonitor_Stk[TASK_VOLT_TEMP_MONITOR_STACK_SIZE];
extern CPU_STK AmperesMonitor_Stk[TASK_AMPERES_MONITOR_STACK_SIZE];
extern CPU_STK LogInfo_Stk[TASK_LOG_INFO_STACK_SIZE];
extern CPU_STK CANBusConsumer_Stk[TASK_CANBUS_CONSUMER_STACK_SIZE];
extern CPU_STK BatteryBalance_Stk[TASK_BATTERY_BALANCE_STACK_SIZE];
extern CPU_STK CLI_Stk[TASK_CLI_STACK_SIZE];
extern CPU_STK BLE_Stk[TASK_BLE_STACK_SIZE];
extern CPU_STK Idle_Stk[TASK_IDLE_STACK_SIZE];
extern CPU_STK Init_Stk[TASK_INIT_STACK_SIZE];

/**
 * @brief   Queue for pushing and popping CAN Messages
 */
OS_Q CANBus_MsgQ;

extern OS_SEM SafetyCheck_Sem4;
extern OS_SEM Fault_Sem4;
extern OS_MUTEX WDog_Mutex;
extern uint32_t WDog_BitMap;
extern OS_Q CANBus_MsgQ;

#endif
