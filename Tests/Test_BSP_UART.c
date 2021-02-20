#include "common.h"
#include "config.h"
#include "BSP_UART.h"
#include "BSP_PLL.h"
#include "stm32f4xx.h"

void foo(void){
    return;
}

int main() {
    BSP_PLL_Init();
    BSP_UART_Init(foo, foo, UART_USB);

    
    printf("Hello\n\r");
    printf("%ld\n\r", BSP_PLL_GetSystemClock());
    printf("%lx\n\r", RCC->CFGR);
    while(1) {
        //printf("Hello\n\r");
        //for (volatile int i = 0; i < 100; i++);
        //printf("%ld\n\r", BSP_PLL_GetSystemClock());
    }
}
