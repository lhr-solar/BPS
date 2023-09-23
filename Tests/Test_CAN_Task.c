/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"

/******************************************************************************
 * CAN Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Flash the BPS (see README.md)
 * 3. Connect a logic analyzer to the BPS's CAN port
 * 4. Start recording the BPS's CAN output
 * 5. Reset the BPS by pressing and releasing the reset button
 * 6. Check the logic analyzer recording for the BPS's CAN messages
 * 
 * Expected Result:
 * - Run light should toggle around 10 Hz
 * - BPS should repeatedly send the following CAN messages:
 * 
 * CAN ID = 0x104
 * BPS Voltages (approximate) = [2.55, 2.6, 2.65, 2.7, ... , 3.95, 4.0, 4.05]
 * 
 * CAN ID = 0x10C
 * Charging Enabled = false
 * 
 * CAN ID = 0x105
 * BPS Temperatures (approximate) = [40.5, 41.0, 41.5, ... , 69.5, 70.5, 71.0]
 *****************************************************************************/

// Used by Task1
StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

// Used by Task_Spam
StaticTask_t TaskSpam_TCB;
CPU_STK TaskSpam_Stk[DEFAULT_STACK_SIZE];

// Task to spam CAN messages to test CANBUS Consumer
// heavily influenced by Task_VoltTempMonitor's CAN code
// because that code will send the vast majority of CAN messages
void Task_Spam(void *p_arg){
    CANData_t CanData;
    CANPayload_t CanPayload;
    CANMSG_t CanMsg;

    BSP_Lights_Init();

    while (1) {
        // heartbeat
        BSP_Light_Toggle(RUN);

        //Send fake voltage measurements to CAN queue
        CanMsg.id = VOLT_DATA;
        uint32_t voltage = 2500;
        for (int i = 0; i < NUM_BATTERY_MODULES; i++){ //send all battery module voltage data
            CanPayload.idx = i;
            voltage += 50; // create fake voltages
            CanData.w = voltage; //send data in millivolts
            CanPayload.data = CanData;
            CanMsg.payload = CanPayload;
            CAN_Queue_Post(CanMsg);
        }

        // Send message if car should be allowed to charge or not
        // suggest that the battery should not be charged
        CanMsg.id = CHARGE_ENABLE;
        CanPayload.idx = 0;
        CanData.b = false;
        CanPayload.data = CanData;
        CanMsg.payload = CanPayload;
        CAN_Queue_Post(CanMsg);

        //Send fake temperature measurements to CAN queue
        CanMsg.id = TEMP_DATA;
        CanData.w = 40000;
        for (uint8_t i = 0; i < NUM_MINIONS; i++){ //send all temperature readings
            for (uint8_t j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++){
                if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j < NUM_TEMPERATURE_SENSORS){
                    CanPayload.idx = i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j;
                    CanData.w += 500; // generate fake temperatures
                    CanPayload.data = CanData;
                    CanMsg.payload = CanPayload;
                    CAN_Queue_Post(CanMsg);
                }
            }
        }

        // delay for 50ms (half the time volttemp delays for because other threads will also take CPU during the race)
        RTOS_BPS_DelayTick(5);
    }
}

// Initialization task for this test
void Task1(void *p_arg){
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    OS_ERR err;

    // Spawn a thread to spam CAN messages
    // Has a higher priority than CANBUS Consumer because CAN messages will be sent 
    // by higher priority threads during the race
    xTaskCreateStatic(Task_Spam,
		"TASK_SPAM",
		DEFAULT_STACK_SIZE,
		(void *)0,				// Task function args,
		6,
		TaskSpam_Stk,
		&TaskSpam_TCB);					// return err code}

    // Spawn CANBUS Consumer, PRIO 7
    xTaskCreateStatic(Task_CANBusConsumer,
		"TASK_CANBUS_CONSUMER_PRIO",
		TASK_CANBUS_CONSUMER_STACK_SIZE,
		(void *)true,				// Use loopback mode,
		TASK_CANBUS_CONSUMER_PRIO,
		CANBusConsumer_Stk,
		&CANBusConsumer_TCB);					// return err code
        
        // Initialize CAN queue
        CAN_Queue_Init();
	//delete task
	OSTaskDel(NULL, &err); // Delete task
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
int main(void) {
    OS_ERR err;
    BSP_PLL_Init();

    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);
    assertOSError(err);

    OSStart(&err);
}
