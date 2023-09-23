/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include <stdio.h>
#include <stdint.h>
#include "Voltage.h"
#include "Temperature.h"
#include "LTC681x.h"
#include "config.h"
#include "BSP_SPI.h"
#include "FreeRTOS.h"
#include "Tasks.h"

StaticTask_t LTC6811_Deadlocks_TCB;
CPU_STK LTC6811_Deadlocks_Stk[512];

StaticTask_t LTC6811_Deadlocks2_TCB;
CPU_STK LTC6811_Deadlocks2_Stk[512];

int counter1 = 0;
int counter2 = 0;


void LTC6811_Deadlocks(void *p_arg){
    (void)p_arg;

    VoltageSafety_t system;
    cell_asic boards[NUM_MINIONS];
    printf("initializing...\n");
    bsp_os_t spi_os;
    SemaphoreHandle_t semaphore;
	semaphore = xSemaphoreCreateBinary();
	xSemaphoreTake(semaphore, (TickType_t)portMAX_DELAY);
	xSemaphoreGive(semaphore);

    BSP_SPI_Init(spi_ltc6811, &spi_os, 0);
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
	xSemaphoreTake(SafetyCheck_Sem4, (TickType_t)portMAX_DELAY);
        //}

        printf("running %dth loop, thread 1\n", counter);
        printf("running Voltage_UpdateMeasurements...\n");
        Voltage_UpdateMeasurements();
        printf("running Voltage_CheckStatus...\n");
        Voltage_CheckStatus();
        printf("running Voltage_GetModulesInDanger...\n");
        Voltage_GetModulesInDanger(&system);
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
    
    VoltageSafety_t system;
    cell_asic boards[NUM_MINIONS];
    printf("initializing...\n");
    bsp_os_t spi_os;
    SemaphoreHandle_t semaphore;
	semaphore = xSemaphoreCreateBinary();
	xSemaphoreTake(semaphore, (TickType_t)portMAX_DELAY);
	xSemaphoreGive(semaphore);

    BSP_SPI_Init(spi_ltc6811, &spi_os, 0);
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
	xSemaphoreGive(SafetyCheck_Sem4);

        printf("running %dth loop, thread 2\n", counter);
        printf("running Voltage_UpdateMeasurements...\n");
        Voltage_UpdateMeasurements();
        printf("running Voltage_CheckStatus...\n");
        Voltage_CheckStatus();
        printf("running Voltage_GetModulesInDanger...\n");
        Voltage_GetModulesInDanger(&system);
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

	SafetyCheck_Sem4 = xSemaphoreCreateBinary();

    xTaskCreateStatic(LTC6811_Deadlocks,
		"LTC6811 Deadlocks Test",
		512,
		(void *)0,
		1,
		LTC6811_Deadlocks_Stk,
		&LTC6811_Deadlocks_TCB);

    xTaskCreateStatic(LTC6811_Deadlocks2,
		"LTC6811 Deadlocks Test",
		512,
		(void *)0,
		2,
		LTC6811_Deadlocks2_Stk,
		&LTC6811_Deadlocks2_TCB);

	OSStart(&err);
}
