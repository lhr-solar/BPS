#include "common.h"
#include "config.h"
#include "LTC6811.h"
#include "BSP_UART.h"

#define NUM_MINIONS 4

cell_asic minions[NUM_MINIONS];

int main() {

    BSP_UART_Init();    // Initialize printf

    printf("Testing setting discharge bits.\r\n");

    LTC6811_Init(minions);
    
    printf("Setting discharge bits for all modules in the system...\r\n");
    for(int i = 0; i<NUM_MINIONS; i++){
        minions[i].config.tx_data[4] |= 0xFF;
    }


    
    printf("Clearing discharge bits for all modules in the system...\r\n");
    for(int i = 0; i<NUM_MINIONS; i++){
        minions[i].config.tx_data[4] &= 0x00;
    } 
    
    minions[3].config.tx_data[4] |= 0x01;
    LTC6811_wrcfg(NUM_MINIONS, minions);

    LTC6811_rdcfg(NUM_MINIONS, minions);
}