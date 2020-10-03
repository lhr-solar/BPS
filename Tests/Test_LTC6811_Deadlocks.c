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
        //for(int i = 0; i < 4; i++) {
        OSSemPend(&SafetyCheck_Sem4,
                    0,
                    OS_OPT_PEND_BLOCKING,
                    &ts,
                    &err);
        //}

        printf("running %dth loop, thread 1\n", counter);
        printf("running Voltage_UpdateMeasurements...\n");
        Voltage_UpdateMeasurements();
        printf("running Voltage_CheckStatus...\n");
        Voltage_CheckStatus();
        printf("running Voltage_GetModulesInDanger...\n");
        Voltage_GetModulesInDanger();
        printf("running Voltage_OpenWireSummary...\n");
        Voltage_OpenWireSummary();
        printf("running Volatge_OpenWire...\n");
        Voltage_OpenWire();
        printf("running Volatge_GetOpenWire...\n");
        Voltage_GetOpenWire();
        printf("running Volatge_GetModuleMillivoltage...\n");
        Voltage_GetModuleMillivoltage(arg);
        printf("running Volatge_GetTotalPackVoltage...\n");
        Voltage_GetTotalPackVoltage();
        printf("running Temperature_ChannelConfig...\n");
        Temperature_ChannelConfig(arg);
        printf("running milliVoltToCelsius...\n");
        milliVoltToCelsius((float) arg);
        printf("running Temperature_UpdateSingleChannel...\n");
        Temperature_UpdateSingleChannel(arg);
        printf("running Temperature_UpdateAllMeasurements...\n");
        Temperature_UpdateAllMeasurements();
        printf("running Temperature_CheckStatus...\n");
        Temperature_CheckStatus(arg);
        printf("running Temperature_SetChargeState...\n");
        Temperature_SetChargeState(arg);
        printf("running Temperature_GetModulesInDanger...\n");
        Temperature_GetModulesInDanger();
        printf("running Temperature_GetSingleTempSensor...\n");
        Temperature_GetSingleTempSensor(arg, arg);
        printf("running Temperature_GetModuleTemperature...\n");
        Temperature_GetModuleTemperature(arg);
        printf("running Temperature_GetTotalPackAvgTemperature...\n");
        Temperature_GetTotalPackAvgTemperature();
        printf("running Temperature_SampleADC...\n");
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
        printf("running Voltage_UpdateMeasurements...\n");
        Voltage_UpdateMeasurements();
        printf("running Voltage_CheckStatus...\n");
        Voltage_CheckStatus();
        printf("running Voltage_GetModulesInDanger...\n");
        Voltage_GetModulesInDanger();
        printf("running Voltage_OpenWireSummary...\n");
        Voltage_OpenWireSummary();
        printf("running Volatge_OpenWire...\n");
        Voltage_OpenWire();
        printf("running Volatge_GetOpenWire...\n");
        Voltage_GetOpenWire();
        printf("running Volatge_GetModuleMillivoltage...\n");
        Voltage_GetModuleMillivoltage(arg);
        printf("running Volatge_GetTotalPackVoltage...\n");
        Voltage_GetTotalPackVoltage();
        printf("running Temperature_ChannelConfig...\n");
        Temperature_ChannelConfig(arg);
        printf("running milliVoltToCelsius...\n");
        milliVoltToCelsius((float) arg);
        printf("running Temperature_UpdateSingleChannel...\n");
        Temperature_UpdateSingleChannel(arg);
        printf("running Temperature_UpdateAllMeasurements...\n");
        Temperature_UpdateAllMeasurements();
        printf("running Temperature_CheckStatus...\n");
        Temperature_CheckStatus(arg);
        printf("running Temperature_SetChargeState...\n");
        Temperature_SetChargeState(arg);
        printf("running Temperature_GetModulesInDanger...\n");
        Temperature_GetModulesInDanger();
        printf("running Temperature_GetSingleTempSensor...\n");
        Temperature_GetSingleTempSensor(arg, arg);
        printf("running Temperature_GetModuleTemperature...\n");
        Temperature_GetModuleTemperature(arg);
        printf("running Temperature_GetTotalPackAvgTemperature...\n");
        Temperature_GetTotalPackAvgTemperature();
        printf("running Temperature_SampleADC...\n");
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


int main() {
    OS_ERR err;
    OSInit(&err);

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0,
                &err);

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
				2,			// Priority
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
