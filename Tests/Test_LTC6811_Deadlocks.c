#include <stdio.h>
#include <stdint.h>
#include "Voltage.h"
#include "Temperature.h"
#include "LTC681x.h"
#include "config.h"
#include "BSP_SPI.h"

int main(void){
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
