/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "Voltage.h"
#include "BSP_UART.h"
#include <unistd.h>
#include "CLI.h"

cell_asic minions[NUM_MINIONS];


int main() {
    uint32_t fifo_size = 128;
	char command[fifo_size];
    CLI_Startup();
    CLI_Init(minions);
    
    while(1) {
        // BLOCKING =====================
        // Wait for command
		if(BSP_UART_ReadLine(command, UART_USB) > 0){
			CLI_Handler(command);	//handle command
		}
    }
}
