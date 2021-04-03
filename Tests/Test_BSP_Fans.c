#include <stdio.h>
#include "common.h"
#include "config.h"
#include "BSP_Fans.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"

#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"

int main()
{
    BSP_PLL_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    BSP_Fans_Init();
    BSP_Lights_Init();
    /*
    BSP_Fans_Set(1,2);
    BSP_Fans_Set(2,4);
    BSP_Fans_Set(3,6);
    BSP_Fans_Set(4,8);
    */

   /*
    // initialize gpio pins connected to fans as normal outputs
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &gpio);
    //Initialize Pin PB14, PB15
    gpio.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &gpio); 

    // set fans to max
    GPIOB->ODR |= (0x3 << 14);
    GPIOC->ODR |= (0x3 << 6);
    */

   

    while(1){
        BSP_Light_Toggle(FAULT);
        for (volatile int i  = 0; i < 1000000; i++);
    }
}
