#include "common.h"
#include "config.h"
#include "LTC6811.h"
#include "BSP_UART.h"

cell_asic minions[NUM_MINIONS];

int main() {

    BSP_UART_Init();    // Initialize printf

    printf("Testing LTC6811 functions.\r\n");

    LTC6811_Init(minions);

    LTC6811_wrcfg(NUM_MINIONS, minions);

    LTC6811_rdcfg(NUM_MINIONS, minions);

    LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);

    LTC6811_rdcv(0, NUM_MINIONS, minions);

    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("Minion %d:\r\n", i);
        printf("\tVoltages:\r\n");

        for(int j = 0; j < 12; j++) {
            printf("\t%d: %dmV\r\n", j, minions[i].cells.c_codes[j]/10);
        }
    }

    while(1) {

    }
}
