#include "common.h"
#include "config.h"
#include "BSP_Lights.h"

void delay(void) {
    // Random delay loop
    for(int i = 0; i < 10000000; i++);
}

int main() {

    BSP_Lights_Init();

    BSP_Light_Off(FAULT);
    BSP_Light_Off(RUN);
    BSP_Light_Off(UVOLT);
    BSP_Light_Off(OVOLT);
    BSP_Light_Off(OTEMP);
    BSP_Light_Off(OCURR);
    BSP_Light_Off(WDOG);
    BSP_Light_Off(CAN);
    BSP_Light_Off(EXTRA);

    while(1) {
        BSP_Light_Toggle(FAULT);
        BSP_Light_Toggle(RUN);
        BSP_Light_Toggle(UVOLT);
        BSP_Light_Toggle(OVOLT);
        BSP_Light_Toggle(OTEMP);
        BSP_Light_Toggle(OCURR);
        BSP_Light_Toggle(WDOG);
        BSP_Light_Toggle(CAN);
        BSP_Light_Toggle(EXTRA);
        delay();
    }
}
