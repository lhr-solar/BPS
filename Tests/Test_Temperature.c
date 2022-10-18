/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "Temperature.h"
#include "Voltage.h"
#include "BSP_UART.h"
#include "Tasks.h"
#include "BSP_PLL.h"
#include "stm32f4xx.h"
#include "BSP_SPI.h"

/******************************************************************************
 * Temperature App Test Plan
 * 
 * 1. The datasheet has a list of mappings from temperatures to millivoltages.
 * I have listed a few of them below. Apply the following voltages to the 
 * temperatures sensor probes, and verify the the relationship holds.
 * 
 * Temperature (Degrees C)      Voltage (mV)
 * 20	                        2365
 * 25	                        2298
 * 30	                        2231
 * 35	                        2163
 * 40	                        2095
 * 45	                        2026
 * 50	                        1958
 * 55	                        1888
 * 60	                        1819
 * 65	                        1749
 * 70	                        1679
 * 75	                        1609
 * 80	                        1539
 * 85	                        1469
 * 
 * 2. Connect the temperature sensors and see if the measurements match the
 * ambient temperature (verify with a themometer or by checking the thermostat).
 * 
 * 3. Put the temperature sensors in the oven? and see if they match the 
 * temperature at 45 degrees C, 60 degrees C, and 75 degrees C
 * 
 *****************************************************************************/

// Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

cell_asic minions[NUM_MINIONS];

void test(void) {
    OS_ERR err;

    BSP_UART_Init(NULL, NULL, UART_USB);    // Initialize printf

    Temperature_Init(minions);

    printf("Testing Temperature functions in loop.\r\n");

    uint8_t isCharging = 0;

    while(1) {
        printf("All good!\r");
        //uint8_t data[] = {0x2C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF};
        //uint16_t pec = pec15_calc(6, data);
        //printf("PEC Value: %X\n\r", pec);
        int status = Temperature_UpdateAllMeasurements();
        printf("%s\r\n", status ? "status :^)" : "status :^(");

        if(Temperature_CheckStatus(isCharging) != SAFE) {
            printf("DANGER!! Temperature levels in danger :(\r\n");
        }
        printf("Printing temperature values.\r\n");
        for(int i = 0; i < NUM_MINIONS; i++) {
            printf("Minion %d:\r\n", i);
            printf("\tTemperature:\r\n");

            for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
                printf("\t%d: %ldmC\r\n", j, Temperature_GetSingleTempSensor(i, j));
            }
        }
        /*uint8_t rxbuf[8];
        BSP_SPI_Read(spi_ltc6811, rxbuf, 8);
        rxbuf[0] = 0;
        */
        RTOS_BPS_DelayTick(400);
    }
}

// Task for this test
void Task1(void *p_arg){
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
	
    test();
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
int main(void) {
    OS_ERR err;
    BSP_PLL_Init();

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);
    assertOSError(err);

    OSStart(&err);
}