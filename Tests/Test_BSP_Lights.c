/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_Lights.h"

void delay(void) {
    // Random delay loop
    for(volatile int i = 0; i < 50000; i++);
}

int main() {

    BSP_Lights_Init();

    BSP_Light_Off(RUN);
    BSP_Light_Off(UVOLT);
    BSP_Light_Off(OVOLT);
    BSP_Light_Off(OTEMP);
    BSP_Light_Off(OCURR);
    BSP_Light_Off(WDOG);
    BSP_Light_Off(CAN);
    BSP_Light_Off(EXTRA);
    BSP_Light_Off(WIRE);
    BSP_Light_Off(FAULT);

    while(1) {
        BSP_Light_Toggle(FAULT);
        delay();
        BSP_Light_Toggle(RUN);
        delay();
        BSP_Light_Toggle(UVOLT);
        delay();
        BSP_Light_Toggle(OVOLT);
        delay();
        BSP_Light_Toggle(OTEMP);
        delay();
        BSP_Light_Toggle(OCURR);
        delay();
        BSP_Light_Toggle(WIRE);
        delay();
        BSP_Light_Toggle(WDOG);
        delay();
        BSP_Light_Toggle(CAN);
        delay();
        BSP_Light_Toggle(EXTRA);
        delay();
    }
}
