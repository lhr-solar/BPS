#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"

int main() {

    BSP_UART_Init();    // Initialize UART to use printf

    BSP_SPI_Init();

    uint8_t data[4+NUM_MINIONS*8] = {0x00, 0x01, 0x3D, 0x6E};   // 0x00, 0x02, 0x2B, 0x0A

    BSP_SPI_Write(data, 4);

    BSP_SPI_Read(data, 4+NUM_MINIONS*8);

    for(int i = 0; i < NUM_MINIONS*8; i++) {
        printf("%d,", data[i]);
    }

    while(1) {
        
    }
}
