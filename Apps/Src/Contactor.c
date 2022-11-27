/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "Contactor.h"

/**
 * @brief Initializes all Contactor pins used by the BPS
 * @param none
 * @return none
 */
void BSP_Contactor_Init(void) {
	//Contactor Init is called before fan init, so initialize pwm here first. This will setup the contactor outputs
	BSP_PWM_Init();

	//setup the input pin
    GPIO_InitTypeDef GPIO_C1Init;
	GPIO_C1Init.GPIO_Pin = GPIO_Pin_1; //input pin is gpio B1
    GPIO_C1Init.GPIO_Mode = GPIO_Mode_IN;
    GPIO_C1Init.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_C1Init.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_C1Init.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(C1_PORT, &GPIO_C1Init);
}

/**
 * @brief   Closes the specified Contactor switch(es) i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   Contactor to turn on
 * @return  None
 */
void BSP_Contactor_On(Contactors_t contactorChoice) {
	// set output pins to start outputing with a duty cycle set by PWM_ON_TIME in the header file
	switch (contactorChoice)
	{
	case ARRAY_CONTACTOR:
		BSP_PWM_Set(ARRAY_CONTACTOR_OUT, PWM_ON_TIME);
		break;
	case HVHIGH_CONTACTOR:
		BSP_PWM_Set(HVHIGH_CONTACTOR_OUT, PWM_ON_TIME);
		break;
	case HVLOW_CONTACTOR:
		BSP_PWM_Set(HVLOW_CONTACTOR_OUT, PWM_ON_TIME);
		break;
	case ALL_CONTACTORS: //c1 and cfan are the only two possible contactors, so we operate on both
		BSP_PWM_Set(CFAN, PWM_ON_TIME);
		BSP_PWM_Set(C1, PWM_ON_TIME);
		break;
	default:
		break;
	}
}

/**
 * @brief   Opens the specified Contactor switch(es) i.e. turns off the whole electrical system.
 * @param   Contactor to turn off
 * @return  None
 */
void BSP_Contactor_Off(Contactors_t contactorChoice) {
    // set output pin to a duty cycle of 0
	switch (contactorChoice)
		{
		case ARRAY_CONTACTOR:
			BSP_PWM_Set(ARRAY_CONTACTOR_OUT, 0);
			break;
		case HVHIGH_CONTACTOR:
			BSP_PWM_Set(HVHIGH_CONTACTOR_OUT, 0);
			break;
		case HVLOW_CONTACTOR:
			BSP_PWM_Set(HVLOW_CONTACTOR_OUT, 0);
			break;
		case ALL_CONTACTORS:
			BSP_PWM_Set(CFAN, 0);
			BSP_PWM_Set(C1, 0);
			break;
		default:
			break;
		}
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @note	You cannot get the state of ALL_CONTACTORS. As such, if that param is passed, it will return the state of the array contactor.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(Contactors_t contactorChoice) {
	bool contactorReturnValue = ((C1_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1; //read the one and only input pin

	/* this is future support for multiple contactors, but we only have one pin right now
	bool contactorReturnValue = false;
	if (contactorChoice == ARRAY_CONTACTOR) {
		contactorReturnValue = ((C1_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1;
	}
	else if (contactorChoice == HVHIGH_CONTACTOR) {
		contactorReturnValue = ((C2_PORT->IDR & GPIO_Pin_5) >> 5) ? 0 : 1;
	}
	else if (contactorChoice == HVLOW_CONTACTOR) {
		contactorReturnValue = ((C3_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1;
	}
	else if (contactorChoice == ALL_CONTACTORS) {
		// return if ANY of the contactors are off.
		contactorReturnValue = (((C1_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1)
			&& (((C2_PORT->IDR & GPIO_Pin_5) >> 5) ? 0 : 1)
			&& (((C3_PORT->IDR & GPIO_Pin_1) >> 1) ? 0 : 1);
	}

	*/
	return contactorReturnValue;
}
