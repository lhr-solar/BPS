/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"
#include "LTC6811.h"
#include "BSP_OS.h"
#include "BSP_PLL.h"
#include "BSP_Lights.h"

void doNothing(void){return;}

int main() {
    BSP_PLL_Init();
    BSP_Lights_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);    // Initialize UART to use printf

    bsp_os_t spi_os;
    spi_os.pend = doNothing;
    spi_os.post = doNothing;

    BSP_SPI_Init(spi_ltc6811, &spi_os);

    

    uint8_t data[4+NUM_MINIONS*8] = {0x00, 0x01, 0x3D, 0x6E};   // 0x00, 0x02, 0x2B, 0x0A
    BSP_Light_On(FAULT);

    BSP_SPI_Write(spi_ltc6811, data, 4);
    
    
    BSP_SPI_Read(spi_ltc6811, data, 4+NUM_MINIONS*8);

    for(int i = 0; i < NUM_MINIONS*8; i++) {
        printf("%d,", data[i]);
    }

    while(1) {

    }
}
