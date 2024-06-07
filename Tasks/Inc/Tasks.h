/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
#ifndef TASKS_H
#define TASKS_H

#include "RTOS_BPS.h"

#define TASK_INIT_PRIO                      0
#define TASK_PETWDOG_PRIO                   1
#define TASK_CHECK_CONTACTOR_PRIO           2
#define TASK_CANBUS_PRODUCER_PRIO           3
#define TASK_CANBUS_CONSUMER_PRIO           4
#define TASK_VOLT_TEMP_MONITOR_PRIO         5
#define TASK_AMPERES_MONITOR_PRIO           6
#define TASK_BATTERY_BALANCE_PRIO           7
#define TASK_LOG_INFO_PRIO                  8 
#define TASK_CLI_PRIO                       9  
#define TASK_PRINT_PRIO                     9
#define TASK_IDLE_PRIO                      10

#define DEFAULT_STACK_SIZE                  256
#define WATERMARK_STACK_LIMIT               DEFAULT_STACK_SIZE/2

#define TASK_PETWDOG_STACK_SIZE             DEFAULT_STACK_SIZE
#define TASK_VOLT_TEMP_MONITOR_STACK_SIZE   512
#define TASK_AMPERES_MONITOR_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_LOG_INFO_STACK_SIZE            DEFAULT_STACK_SIZE
#define TASK_CANBUS_CONSUMER_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_CANBUS_PRODUCER_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_CHECK_CONTACTOR_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_BATTERY_BALANCE_STACK_SIZE     512
#define TASK_CLI_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_BLE_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_PRINT_STACK_SIZE               DEFAULT_STACK_SIZE
#define TASK_IDLE_STACK_SIZE                DEFAULT_STACK_SIZE
#define TASK_INIT_STACK_SIZE                DEFAULT_STACK_SIZE

/**
 * enum to keep track of which tasks correspond to each bit in WDog_BitMap
 */
typedef enum {
    WD_NONE = 0x0,
    WD_VOLT_TEMP = 0x1,
    WD_AMPERES = 0x2,
    WD_BALANCING = 0x4,
} WDOGBits_e;

void Task_Init(void *p_arg);

void Task_PetWDog(void *p_arg);

void Task_VoltTempMonitor(void *p_arg);

void Task_AmperesMonitor(void *p_arg);

void Task_LogInfo(void *p_arg);

void Task_CANBusConsumer(void *p_arg);

void Task_CANBusProducer(void *p_arg);

void Task_CheckContactor(void *p_arg);

void Task_BatteryBalance(void *p_arg);

void Task_CLI(void *p_arg);

void Task_BLE(void *p_arg);

void Task_Print(void *p_arg);

void Task_Idle(void *p_arg);

void Task_Init(void *p_arg);

void EnterFaultState();


/**
 * Used to assert if there has been an error in one of the OS functions
 * Kills the car if there is an OS error
 **/
void assertOSError(BPS_OS_ERR err);

/**
 * Used to Enter Fault State in case OS is not working
 **/

void EnterFaultState(void);

/*
 * Thread Control Blocks that contain information about each thread
 */
extern OS_TCB FaultState_TCB;
extern OS_TCB PetWDog_TCB;
extern OS_TCB VoltTempMonitor_TCB;
extern OS_TCB AmperesMonitor_TCB;
extern OS_TCB LogInfo_TCB;
extern OS_TCB CANBusConsumer_TCB;
extern OS_TCB CANBusProducer_TCB;
extern OS_TCB CheckContactor_TCB;
extern OS_TCB BatteryBalance_TCB;
extern OS_TCB CLI_TCB;
extern OS_TCB BLE_TCB;
extern OS_TCB Print_TCB;
extern OS_TCB Idle_TCB;
extern OS_TCB Init_TCB;
/**
 * Stacks for each thread
 */
extern CPU_STK PetWDog_Stk[TASK_PETWDOG_STACK_SIZE];
extern CPU_STK VoltTempMonitor_Stk[TASK_VOLT_TEMP_MONITOR_STACK_SIZE];
extern CPU_STK AmperesMonitor_Stk[TASK_AMPERES_MONITOR_STACK_SIZE];
extern CPU_STK LogInfo_Stk[TASK_LOG_INFO_STACK_SIZE];
extern CPU_STK CANBusConsumer_Stk[TASK_CANBUS_CONSUMER_STACK_SIZE];
extern CPU_STK CANBusProducer_Stk[TASK_CANBUS_PRODUCER_STACK_SIZE];
extern CPU_STK CheckContactor_Stk[TASK_CHECK_CONTACTOR_STACK_SIZE];
extern CPU_STK BatteryBalance_Stk[TASK_BATTERY_BALANCE_STACK_SIZE];
extern CPU_STK CLI_Stk[TASK_CLI_STACK_SIZE];
extern CPU_STK BLE_Stk[TASK_BLE_STACK_SIZE];
extern CPU_STK Print_Stk[TASK_PRINT_STACK_SIZE];
extern CPU_STK Idle_Stk[TASK_IDLE_STACK_SIZE];
extern CPU_STK Init_Stk[TASK_INIT_STACK_SIZE];

/**
 *  Semaphores, Mutexes, & Bitmaps that are used by multiple threads
 */
extern OS_SEM SafetyCheck_Sem4;
extern OS_MUTEX WDog_Mutex;
extern OS_MUTEX MinionsASIC_Mutex;
extern WDOGBits_e WDog_BitMap;
extern uint8_t Fault_Flag;

typedef enum {
    Fault_UVOLT   = 0x001,
    Fault_OVOLT   = 0x002,
    Fault_OTEMP   = 0x004,
    Fault_OCURR   = 0x008,
    Fault_OW      = 0x010,
    Fault_HANDLER = 0x020,
    Fault_OS      = 0x040,
    Fault_WDOG    = 0x080,
    Fault_CRC     = 0x100,
    Fault_ESTOP   = 0x200,
    FAULT_MAX
}Fault_Set;

extern Fault_Set Fault_BitMap;

#endif
