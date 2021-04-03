#include "common.h"
#include "config.h"
#include <stdio.h>
#include "BSP_Fans.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

int main() {
    BSP_Fans_Init();
    TIM8 -> CCR1 = 4000;
    TIM8 -> CCR2 = 4000;
    TIM8 -> CCR3 = 4000;
    TIM8 -> CCR4 = 4000;
    while(1);
}
