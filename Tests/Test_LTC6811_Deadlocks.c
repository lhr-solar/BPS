#include <stdio.h>
#include <stdint.h>
#include "Voltage.h"
#include "Temperature.h"
#include "LTC681x.h"
#include "config.h"
#include "BSP_SPI.h"
#include "os.h"

OS_TCB LTC6811_Deadlocks_TCB;
CPU_STK LTC6811_Deadlocks_Stk[512];

void LTC6811_Deadlocks(void *p_arg){
    (void)p_arg;

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
        printf("running %dth loop\n", counter);
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
    }
}


int main() {
    OS_ERR err;

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

	OSStart(&err);
}
