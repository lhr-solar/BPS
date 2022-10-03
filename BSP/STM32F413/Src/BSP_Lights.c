/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Lights.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// Just some lookup for the values
static const uint32_t LIGHT_VALS[LIGHTS_MAX] = {
	GPIO_Pin_4,	// RUN, 
	GPIO_Pin_7, // UVOLT, 
	GPIO_Pin_6,	// OVOLT, 
	GPIO_Pin_5,	// OTEMP, 
	GPIO_Pin_4,	// OCURR, 
	GPIO_Pin_2,	// WDOG, 
	GPIO_Pin_1,	// CAN, 
	GPIO_Pin_0,	// EXTRA, 
	GPIO_Pin_3,	// WIRE,
    GPIO_Pin_12 // STROBE
};

static GPIO_TypeDef * const LIGHT_PORTS[LIGHTS_MAX] = {
    GPIOC,  // RUN, 
    GPIOA,  // UVOLT, 
    GPIOA,  // OVOLT, 
    GPIOA,  // OTEMP, 
    GPIOA,  // OCURR, 
    GPIOC,  // WDOG, 
    GPIOC,  // CAN, 
    GPIOC,  // EXTRA, 
    GPIOC,  // WIRE,
    GPIOB,  // STROBE
};

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4; 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
    LIGHT_PORTS[signal]->ODR ^= LIGHT_VALS[signal];
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
    LIGHT_PORTS[signal]->ODR |= LIGHT_VALS[signal];
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
    LIGHT_PORTS[signal]->ODR &= ~LIGHT_VALS[signal];
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of the LED/Light (ON/OFF)
 */
State BSP_Light_GetState(Light signal) {
    return (LIGHT_PORTS[signal]->ODR & LIGHT_VALS[signal]) ? ON : OFF;
}
