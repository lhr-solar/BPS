#include "common.h"
#include "config.h"
#include "BSP_SPI1.h"
#include "BSP_UART.h"

int main() {

    BSP_UART_Init();    // Initialize UART to use printf

    BSP_SPI1_Init();

    uint8_t data[4+NUM_MINIONS*8] = {0x00, 0x01, 0x00, 0x02};

    BSP_SPI1_Write(data, 4);
    BSP_SPI1_Read(data, 4+NUM_MINIONS*8);

    while(1) {
        
    }
}
