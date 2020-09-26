#include <stdio.h>
#include <stdint.h>
#include "Voltage.h"
#include "Temperature.h"
#include "LTC681x.h"
#include "config.h"
#include "BSP_SPI.h"
#include "os.h"

OS_SEM SafetyCheck_Sem4;

OS_TCB LTC6811_Deadlocks_TCB;
CPU_STK LTC6811_Deadlocks_Stk[512];

OS_TCB LTC6811_Deadlocks2_TCB;
CPU_STK LTC6811_Deadlocks2_Stk[512];

int counter1 = 0;
int counter2 = 0;

void LTC6811_Deadlocks(void *p_arg){
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    cell_asic boards[NUM_MINIONS];
    printf("initializing...\n");
    BSP_SPI_Init();
    printf("SPI initialized\n");
    Voltage_Init(boards);
    printf("Voltage initialized\n");
    Temperature_Init(boards);
    printf("Temperature initialized\n");
    Voltage_Init(boards);
    Temperature_Init(boards);
    int counter = 0;
    int arg = 0;
    while (1){
        for(int i = 0; i < 4; i++) {
        OSSemPend(&SafetyCheck_Sem4,
                    0,
                    OS_OPT_PEND_BLOCKING,
                    &ts,
                    &err);
        }

        printf("running %dth loop, thread 1\n", counter);
        Voltage_UpdateMeasurements();
        Voltage_CheckStatus();
        Voltage_GetModulesInDanger();
        Voltage_OpenWireSummary();
        Voltage_OpenWire();
        Voltage_GetOpenWire();
        Voltage_GetModuleMillivoltage(arg);
        Voltage_GetTotalPackVoltage();
        Temperature_ChannelConfig(arg);
        milliVoltToCelsius((float) arg);
        Temperature_UpdateSingleChannel(arg);
        Temperature_UpdateAllMeasurements();
        Temperature_CheckStatus(arg);
        Temperature_SetChargeState(arg);
        Temperature_GetModulesInDanger();
        Temperature_GetSingleTempSensor(arg, arg);
        Temperature_GetModuleTemperature(arg);
        Temperature_GetTotalPackAvgTemperature();
        Temperature_SampleADC(arg);
        arg ^= 0x01;
        counter++;
        counter1++;
        if (counter >= 1000){
            printf("task 1 ran %d times\ntask 2 ran %d times\n", counter1, counter2);
            exit(0);
        }
    }
}

void LTC6811_Deadlocks2(void *p_arg){
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    cell_asic boards[NUM_MINIONS];
    printf("initializing...\n");
    BSP_SPI_Init();
    printf("SPI initialized\n");
    Voltage_Init(boards);
    printf("Voltage initialized\n");
    Temperature_Init(boards);
    printf("Temperature initialized\n");
    Voltage_Init(boards);
    Temperature_Init(boards);
    int counter = 0;
    int arg = 0;
    while (1){
        OSSemPost(&SafetyCheck_Sem4,
                    OS_OPT_POST_ALL,
                    &err);

        printf("running %dth loop, thread 2\n", counter);
        Voltage_UpdateMeasurements();
        Voltage_CheckStatus();
        Voltage_GetModulesInDanger();
        Voltage_OpenWireSummary();
        Voltage_OpenWire();
        Voltage_GetOpenWire();
        Voltage_GetModuleMillivoltage(arg);
        Voltage_GetTotalPackVoltage();
        Temperature_ChannelConfig(arg);
        milliVoltToCelsius((float) arg);
        Temperature_UpdateSingleChannel(arg);
        Temperature_UpdateAllMeasurements();
        Temperature_CheckStatus(arg);
        Temperature_SetChargeState(arg);
        Temperature_GetModulesInDanger();
        Temperature_GetSingleTempSensor(arg, arg);
        Temperature_GetModuleTemperature(arg);
        Temperature_GetTotalPackAvgTemperature();
        Temperature_SampleADC(arg);
        arg ^= 0x01;
        counter++;
        counter2++;
        if (counter >= 1000){
            printf("task 1 ran %d times\ntask 2 ran %d times\n", counter1, counter2);
            exit(0);
        }
    }
}


void TaskSwitcherTask(void *p_arg){
    (void)p_arg;

}


int main() {
    OS_ERR err;
    OSInit(&err);
    OSTaskCreate(&LTC6811_Deadlocks_TCB,				// TCB
				"LTC6811 Deadlocks Test",	// Task Name (String)
				LTC6811_Deadlocks,				// Task function pointer
				(void *)0,				// Task function args
				1,			// Priority
				LTC6811_Deadlocks_Stk,				// Stack
				256,	// Watermark limit for debugging
				512,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

	// ASSERT err

    OSTaskCreate(&LTC6811_Deadlocks2_TCB,				// TCB
				"LTC6811 Deadlocks Test",	// Task Name (String)
				LTC6811_Deadlocks2,				// Task function pointer
				(void *)0,				// Task function args
				1,			// Priority
				LTC6811_Deadlocks2_Stk,				// Stack
				256,	// Watermark limit for debugging
				512,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

    // ASSERT err

	OSStart(&err);
}
