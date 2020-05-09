#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"

int main() {

    BSP_UART_Init();    // Initialize UART to use printf

    BSP_SPI_Init();

    while(1) {
        
    }
}
