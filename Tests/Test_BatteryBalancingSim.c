/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "LTC6811.h"
#include "BSP_UART.h"

cell_asic minions[NUM_MINIONS];

void foo(){
    return;
}

int main() {
    BSP_UART_Init(foo, foo, UART_USB);    // Initialize printf

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
    
    minions[NUM_MINIONS - 1].config.tx_data[4] |= 0x01;
    LTC6811_wrcfg(NUM_MINIONS, minions);

    LTC6811_rdcfg(NUM_MINIONS, minions);
}
