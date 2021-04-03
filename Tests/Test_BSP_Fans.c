#include "common.h"
#include "config.h"
#include <stdio.h>
#include "BSP_Fans.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

int main() {
    BSP_Fans_Init();
    TIM_SetCompare1(TIM8, 4000);
    TIM_SetCompare2(TIM8, 4000);
    TIM_SetCompare3(TIM8, 4000);
    TIM_SetCompare4(TIM8, 4000);
    while(1){
        
    };
}
