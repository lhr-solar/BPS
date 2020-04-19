#include "BSP_Lights.h"

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
    // TODO: Initialize all gpio output pins for each LED specified in the Led enum in the header file
    //      Use Positive Logic
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_Light_Toggle(Led signal) {
    // TODO: Toggle the state for each pin
    switch(signal){
		case FAULT:
			// ERRORpin ^= ERRORpin;
			break;
			
		case RUN:
			// RUNpin ^= RUNpin;
			break;
			
		case OCURR:
			// OCURRpin ^= OCURRpin;
			break;
			
		case OTEMP:
			// OTEMPpin ^= OTEMPpin;
			break;
			
		case OVOLT:
			// OVOLTpin ^= OVOLTpin;
			break;
			
		case UVOLT:
			// UVOLTpin ^= UVOLTpin;
			break;
			
		case WDOG:
			// WDOGpin ^= WDOGpin;
			break;
			
		case CAN:
			// CANpin ^= CANpin;
			break;
			
		case EXTRA:
			// EXTRApin ^= EXTRApin;
			break;

        default:
            break;
	}
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_Light_On(Led signal) {
    // TODO: Set the state to highs for each pin
    switch(signal){
		case FAULT:
			// ERRORpin |= ERRORpin;
			break;
			
		case RUN:
			// RUNpin |= RUNpin;
			break;
			
		case OCURR:
			// OCURRpin |= OCURRpin;
			break;
			
		case OTEMP:
			// OTEMPpin |= OTEMPpin;
			break;
			
		case OVOLT:
			// OVOLTpin |= OVOLTpin;
			break;
			
		case UVOLT:
			// UVOLTpin |= UVOLTpin;
			break;
			
		case WDOG:
			// WDOGpin |= WDOGpin;
			break;
			
		case CAN:
			// CANpin |= CANpin;
			break;
			
		case EXTRA:
			// EXTRApin |= EXTRApin;
			break;

        default:
            break;
	}
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_Light_Off(Led signal) {
    // TODO: Clear the state for each pin
    switch(signal){
		case FAULT:
			// ERRORpin &= ~ERRORpin;
			break;
			
		case RUN:
			// RUNpin &= ~RUNpin;
			break;
			
		case OCURR:
			// OCURRpin &= ~OCURRpin;
			break;
			
		case OTEMP:
			// OTEMPpin &= ~OTEMPpin;
			break;
			
		case OVOLT:
			// OVOLTpin &= ~OVOLTpin;
			break;
			
		case UVOLT:
			// UVOLTpin &= ~UVOLTpin;
			break;
			
		case WDOG:
			// WDOGpin &= ~WDOGpin;
			break;
			
		case CAN:
			// CANpin &= ~CANpin;
			break;
			
		case EXTRA:
			// EXTRApin &= ~EXTRApin;
			break;

        default:
            break;
	}
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
State BSP_Light_GetState(Led signal) {
    // TODO: return the state of the output pin corresponding to the led/light
    switch(signal){
		case FAULT:
			return OFF;
			
		case RUN:
			return OFF;
			
		case OCURR:
			return OFF;
			
		case OTEMP:
			return OFF;
			
		case OVOLT:
			return OFF;
			
		case UVOLT:
			return OFF;
			
		case WDOG:
			return OFF;
			
		case CAN:
			return OFF;
			
		case EXTRA:
			return OFF;

        default:
            return OFF;
	}
}
