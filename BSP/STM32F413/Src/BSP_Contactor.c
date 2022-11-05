/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Contactor.h"
#include "stm32f4xx.h"

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

/**
 * @brief Initializes all Contactor pins used by the BPS
 * @param none
 * @return none
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
 * @brief   Closes the specified Contactor switch(es) i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   Contactor to turn on
 * @return  None
 */
void BSP_Contactor_On(CONT_CHOICE contactorChoice) {
	// set output pins HIGH
	if (contactorChoice == ARRAY_CONTACTOR) {
		GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_SET);
	}
	else if (contactorChoice == HVHIGH_CONTACTOR) {
		GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_SET);
	}
	else if (contactorChoice == HVLOW_CONTACTOR) {
		GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_SET);
	}
	else if (contactorChoice == ALL_CONTACTORS) {
		GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_SET);
	}
}

/**
 * @brief   Opens the specified Contactor switch(es) i.e. turns off the whole electrical system.
 * @param   Contactor to turn off
 * @return  None
 */
void BSP_Contactor_Off(CONT_CHOICE contactorChoice) {
    // set output pins LOW
	if (contactorChoice == ARRAY_CONTACTOR) {
		GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_RESET);
	}
	else if (contactorChoice == HVHIGH_CONTACTOR) {
		GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_RESET);
	}
	else if (contactorChoice == HVLOW_CONTACTOR) {
		GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_RESET);
	}
	else if (contactorChoice == ALL_CONTACTORS) {
		GPIO_WriteBit(C1_PORT, GPIO_Pin_0, Bit_RESET);
		GPIO_WriteBit(C2_PORT, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(C3_PORT, GPIO_Pin_0, Bit_RESET);
	}
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @note	You cannot get the state of ALL_CONTACTORS. As such, if that param is passed, it will return the state of the array contactor.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(CONT_CHOICE contactorChoice) {
	if (contactorChoice == ARRAY_CONTACTOR || contactorChoice == ALL_CONTACTORS) {
		return ((C1_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1;
	}
	else if (contactorChoice == HVHIGH_CONTACTOR) {
		return ((C2_PORT->IDR & GPIO_Pin_5) >> 5) ? 0 : 1;
	}
	else if (contactorChoice == HVLOW_CONTACTOR) {
		return ((C3_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1;
	}
}
