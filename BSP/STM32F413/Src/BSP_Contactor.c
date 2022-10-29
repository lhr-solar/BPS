/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Contactor.h"
#include "stm32f4xx.h"

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

/**
 * @brief Initializes GPIO ports per Contactor requirements
 * 
 * @param gpioPort Specific port to initialize on
 * @param gpioStruct Specific struct to initialize
 * @param pinOutput pin number for output
 * @param pinInput pin number for input
 * @param mode pin mode
 * @param speed speed
 * @param otype 
 * @param pupd 
 */
static void Setup(GPIO_TypeDef* port, GPIO_InitTypeDef* gpioStruct, uint32_t pinOutput, uint32_t pinInput, GPIOSpeed_TypeDef speed, 
               GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd)
{
    // first output pin, then input pin.
    // other configs stay the same across init calls
    gpioStruct->GPIO_Pin = pinOutput;
    gpioStruct->GPIO_Mode = GPIO_Mode_OUT;
    gpioStruct->GPIO_Speed = speed;
    gpioStruct->GPIO_PuPd = pupd;
    gpioStruct->GPIO_OType = otype;
    GPIO_Init(port, gpioStruct);
    gpioStruct->GPIO_Pin = pinInput;
    gpioStruct->GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(port, gpioStruct);
}

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
	Setup(C1_PORT, &GPIO_C1Init, 
			GPIO_Pin_0, GPIO_Pin_1, GPIO_Speed_50MHz,
			GPIO_OType_PP, GPIO_PuPd_UP);

	// Initialize Port A for Contactor 2
	// PA4 - output
	// PA5 - input
	Setup(C2_PORT, &GPIO_C2Init, 
			GPIO_Pin_4, GPIO_Pin_5, GPIO_Speed_50MHz,
			GPIO_OType_PP, GPIO_PuPd_UP);

	// Initialize Port C for Contactor 3
	// PC0 - output
	// PC1 - input
	Setup(C3_PORT, &GPIO_C3Init, 
			GPIO_Pin_0, GPIO_Pin_1, GPIO_Speed_50MHz,
			GPIO_OType_PP, GPIO_PuPd_UP);
}

/**
 * @brief   Closes all Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(CONT_CHOICE contactorChoice) {
	// set output pins HIGH
	if (contactorChoice & ARRAY_CONTACTOR)
		GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_SET);
	if (contactorChoice & HVHIGH_CONTACTOR)
		GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_SET);
	if (contactorChoice & HVLOW_CONTACTOR)
		GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_SET);
}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(CONT_CHOICE contactorChoice) {
    // set output pins LOW
	if (contactorChoice & ARRAY_CONTACTOR) {
		GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_RESET);
	}
	if (contactorChoice & HVHIGH_CONTACTOR) {
		GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_RESET);
	}
	if (contactorChoice & HVLOW_CONTACTOR) {
		GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_RESET);
	}
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(CONT_CHOICE contactorChoice) {
	if (contactorChoice & ARRAY_CONTACTOR) {
		return ((C1_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1;
	}
	else if (contactorChoice & HVHIGH_CONTACTOR) {
		return ((C2_PORT->IDR & GPIO_Pin_5) >> 5) ? 0 : 1;
	}
	else {
		return ((C3_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1;
	}
}