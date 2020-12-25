#include "common.h"
#include "config.h"
#include "BSP_UART.h"

int main() {

    BSP_UART_Init();

    char str[256];

    while(1) {
        int num = BSP_UART_ReadLine(str);
        if(num > 0) {
            printf("%s", str);
        }
    }
}
#include "common.h"
#include "config.h"
#include "BSP_UART.h"

int main() {

    BSP_UART_Init();

    char str[256];

    while(1) {
        int num = BSP_UART_ReadLine(str);
        if(num > 0) {
            printf("%s", str);
        }
    }
}
