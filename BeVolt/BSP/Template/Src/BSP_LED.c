#include "BSP_LED.h"

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_LEDs_Init(void) {
    // TODO: Initialize all gpio output pins for each LED specified in the Led enum in the header file
    //      Use Positive Logic
}

/**
 * @brief   Toggles a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_Toggle(Led signal) {
    // TODO: Toggle the state for each pin
    switch(signal){
		case ERROR:
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
 * @brief   Turns on a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_On(Led signal) {
    // TODO: Set the state to highs for each pin
    switch(signal){
		case ERROR:
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
 * @brief   Turns off a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_Off(Led signal) {
    // TODO: Clear the state for each pin
    switch(signal){
		case ERROR:
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
