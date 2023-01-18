/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "Contactor.h"

/**
 * @brief Initializes all Contactor pins used by the BPS
 * @param none
 * @return none
 */
void Contactor_Init(void) {
	//Contactor Init is called before fan init, so initialize pwm here first. This will setup the contactor outputs
	#ifdef SIMULATION
		Simulator_Log(LOG_INFO, "Contactor Initialized\n");
	#endif
	BSP_PWM_Init();
}

/**
 * @brief   Closes the specified Contactor switch(es) i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   Contactor to turn on
 * @return  None
 */
void Contactor_On(Contactors_t contactorChoice) {
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
void Contactor_Off(Contactors_t contactorChoice) {
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
 * @note	If you pass in All_Contactors, you will get the state as true if any of the contactors are on.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool Contactor_GetState(Contactors_t contactorChoice) {
	bool retval;

	switch (contactorChoice) {
		case ARRAY_CONTACTOR:
			retval = BSP_PWM_Get(ARRAY_CONTACTOR_OUT) ? true : false;
			break;
		case HVHIGH_CONTACTOR:
			retval = BSP_PWM_Get(HVHIGH_CONTACTOR_OUT) ? true : false;
			break;
		case HVLOW_CONTACTOR:
			retval = BSP_PWM_Get(HVLOW_CONTACTOR_OUT) ? true : false;
			break;
		case ALL_CONTACTORS:
		default:
			retval = (BSP_PWM_Get(HVLOW_CONTACTOR_OUT) | 
					BSP_PWM_Get(HVHIGH_CONTACTOR_OUT) |
					BSP_PWM_Get(ARRAY_CONTACTOR_OUT)) ? true : false;
			break;
		}

	return retval;
}
