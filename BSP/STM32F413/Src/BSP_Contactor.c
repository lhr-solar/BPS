/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Contactor.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// Contactor 1
#define C1_PORT		GPIOB
#define C1_PERIPH 	RCC_AHB1Periph_GPIOB
// Contactor 2
#define C2_PORT		GPIOA
#define C2_PERIPH 	RCC_AHB1Periph_GPIOA
// Contactor 3
#define C3_PORT		GPIOC
#define C3_PERIPH 	RCC_AHB1Periph_GPIOC

/**
 * Used pins:
 * A:
 * 		PA2, PA3 - ADC and UART
 * 		PA8 - I2C SCL
 * 		PA4 thru PA7 - Lights
 * 		PA15 - SPI CS
 * B:
 * 		PB3 thru PB5 - SPI
 * 		PB8, PB9 - CAN
 * 		PB10 - UART
 * 		PB12 - Lights
 * 		PB14, PB15 - Fans
 * C:
 * 		PC5 - also UART
 * 		PC6, PC7 - Fans
 * 		PC9 - I2C SDA
 * 		PC0 thru PC4 - Lights
 * 		PC10 thru PC12 - more SPI
 * D:	
 * 		PD2 - SPI CS
 * 		
 * 
 * 		Available Pins for contactors:
 * 		PB0-1 used for C1
 * 		PC0-1 can use for C3
 * 		PA4-5 can use for C2
 */

/**
 * @brief   Initializes the GPIO pins that interfaces with the Contactors.
 *          Two GPIO pins are initialized. One as an output and one as an input.
 *          The output pin controls the state and the input pin views what state the contactor is through the
 *          Aux pins.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Init(void) {
    GPIO_InitTypeDef GPIO_C1Init;
	GPIO_InitTypeDef GPIO_C2Init;
	GPIO_InitTypeDef GPIO_C3Init;
	
	// Initialize clock
	RCC_AHB1PeriphClockCmd(C1_PERIPH, ENABLE);
	RCC_AHB1PeriphClockCmd(C2_PERIPH, ENABLE);
	RCC_AHB1PeriphClockCmd(C3_PERIPH, ENABLE);
	
	// Initialize Port B for Contactor 1
	// PB0 - output
	// PB1 - input
	GPIO_C1Init.GPIO_Pin = GPIO_Pin_0;
	GPIO_C1Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_C1Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_C1Init.GPIO_OType = GPIO_OType_PP;
	GPIO_C1Init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(C1_PORT, &GPIO_C1Init);
	GPIO_C1Init.GPIO_Pin = GPIO_Pin_1;
	GPIO_C1Init.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(C1_PORT, &GPIO_C1Init);

	// Initialize Port A for Contactor 2
	// PA4 - output
	// PA5 - input
	GPIO_C2Init.GPIO_Pin = GPIO_Pin_4;
	GPIO_C2Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_C2Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_C2Init.GPIO_OType = GPIO_OType_PP;
	GPIO_C2Init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(C2_PORT, &GPIO_C2Init);
	GPIO_C2Init.GPIO_Pin = GPIO_Pin_5;
	GPIO_C2Init.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(C2_PORT, &GPIO_C2Init);

	// Initialize Port C for Contactor 3
	// PC0 - output
	// PC1 - input
	GPIO_C3Init.GPIO_Pin = GPIO_Pin_0;
	GPIO_C3Init.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_C3Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_C3Init.GPIO_OType = GPIO_OType_PP;
	GPIO_C3Init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(C3_PORT, &GPIO_C3Init);
	GPIO_C3Init.GPIO_Pin = GPIO_Pin_1;
	GPIO_C3Init.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(C3_PORT, &GPIO_C3Init);
}

/**
 * @brief   Closes all Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(void) {
	// set output pins HIGH
	GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_SET);
	GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_SET);
}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(void) {
    // set output pins LOW
	GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_RESET);
	GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_RESET);
	GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_RESET);
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(void) {
    // The AUX pin we have connected to is negative logic
    if((GPIOB->IDR & GPIO_Pin_1) >> 1) {
		return 0;
	} else {
		return 1;
	}
}