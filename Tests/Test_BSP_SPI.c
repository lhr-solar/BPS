#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"

int main() {

    BSP_UART_Init();    // Initialize UART to use printf

    BSP_SPI_Init();

    //uint8_t data[4+NUM_MINIONS*8] = {0x00, 0x01, 0x00, 0x02};
    uint8_t data[5] = {'a','b','c','d', 'e'};
    BSP_SPI_Write(data, 5);
    uint8_t address[20];
    BSP_SPI_Read(address, 10);
    printf("%s\n", address);
}
