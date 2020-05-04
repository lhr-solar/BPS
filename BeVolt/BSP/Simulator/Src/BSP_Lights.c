#include "BSP_Lights.h"
#include <stdint.h>
#include <stdlib.h>

	const char* Lights = "BSP/Simulator/DataGeneration/Data/Lights.csv";
	uint16_t LEDReg = 0; //global variable containing value of LED status
/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
    //Creates file if it doesn't exist and stores single 16 bit integer acting as register
	//for which LED's are on or off
	//bit 8: bit 7: bit 6: bit 5: bit 4: bit 3: bit 2: bit 1: bit 0
	//FAULT:  RUN : OCURR: OTEMP: OVOLT: UVOLT: WDOG :  CAN : EXTRA
	FILE *LightsFile;
	if (LightsFile = fopen(Lights, "r")) fclose(Lights); //if it exists, don't do anything
	else{
		LightsFile = fopen(Lights, "wb+"); //creates binary file allowing input and ouput
		fwrite(&LEDReg, 2, 1, LightsFile); //Writes 16 bit number to file
		fclose(LightsFile);
	}
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
	FILE *LightsFile;
	LightsFile = fopen(Lights, "wb+"); //open file for reading and writing
	fseek(LightsFile, 0, SEEK_SET); //Go to beginning of file
	fread(&LEDReg, 2, 1, LightsFile); //read data from file
    switch(signal){
		case FAULT:
			LEDReg ^= 0x0100;
			break;
			
		case RUN:
			LEDReg ^= 0x0080;
			break;
			
		case OCURR:
			LEDReg ^= 0x0040;
			break;
			
		case OTEMP:
			LEDReg ^= 0x0020;
			break;
			
		case OVOLT:
			LEDReg ^= 0x0010;
			break;
			
		case UVOLT:
			LEDReg ^= 0x0008;
			break;
			
		case WDOG:
			LEDReg ^= 0x0004;
			break;
			
		case CAN:
			LEDReg ^= 0x0002;
			break;
			
		case EXTRA:
			LEDReg ^= 0x0001;
			break;

        default:
            break;
	}
	fwrite(&LEDReg, 2, 1, LightsFile); //Writes 16 bit number to file
	fclose(LightsFile);
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
	FILE *LightsFile;
	LightsFile = fopen(Lights, "wb+"); //open file for reading and writing
	fseek(LightsFile, 0, SEEK_SET); //Go to beginning of file
	fread(&LEDReg, 2, 1, LightsFile); //read data from file
    switch(signal){
		case FAULT:
			LEDReg |= 0x0100;
			break;
			
		case RUN:
			LEDReg |= 0x0080;
			break;
			
		case OCURR:
			LEDReg |= 0x0040;
			break;
			
		case OTEMP:
			LEDReg |= 0x0020;
			break;
			
		case OVOLT:
			LEDReg |= 0x0010;
			break;
			
		case UVOLT:
			LEDReg |= 0x0008;
			break;
			
		case WDOG:
			LEDReg |= 0x0004;
			break;
			
		case CAN:
			LEDReg |= 0x0002;
			break;
			
		case EXTRA:
			LEDReg |= 0x0001;
			break;

        default:
            break;
	}
	fwrite(&LEDReg, 2, 1, LightsFile); //Writes 16 bit number to file
	fclose(LightsFile);
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
	FILE *LightsFile;
	LightsFile = fopen(Lights, "wb+"); //open file for reading and writing
	fseek(LightsFile, 0, SEEK_SET); //Go to beginning of file
	fread(&LEDReg, 2, 1, LightsFile); //read data from file
    switch(signal){
		case FAULT:
			LEDReg &= ~0x0100;
			break;
			
		case RUN:
			LEDReg &= ~0x0080;
			break;
			
		case OCURR:
			LEDReg &= ~0x0040;
			break;
			
		case OTEMP:
			LEDReg &= ~0x0020;
			break;
			
		case OVOLT:
			LEDReg &= ~0x0010;
			break;
			
		case UVOLT:
			LEDReg &= ~0x0008;
			break;
			
		case WDOG:
			LEDReg &= ~0x0004;
			break;
			
		case CAN:
			LEDReg &= ~0x0002;
			break;
			
		case EXTRA:
			LEDReg &= ~0x0001;
			break;

        default:
            break;
	}
	fwrite(&LEDReg, 2, 1, LightsFile); //Writes 16 bit number to file
	fclose(LightsFile);
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
State BSP_Light_GetState(Light signal) {
	FILE *LightsFile;
	LightsFile = fopen(Lights, "wb+"); //open file for reading and writing
	fseek(LightsFile, 0, SEEK_SET); //Go to beginning of file
	fread(&LEDReg, 2, 1, LightsFile); //read data from file
    switch(signal){
		case FAULT:
			return LEDReg >> 8;
			
		case RUN:
			return LEDReg >> 7;
			
		case OCURR:
			return LEDReg >> 6;
			
		case OTEMP:
			return LEDReg >> 5;
			
		case OVOLT:
			return LEDReg >> 4;
			
		case UVOLT:
			return LEDReg >> 3;
			
		case WDOG:
			return LEDReg >> 2;
			
		case CAN:
			return LEDReg >> 1;
			
		case EXTRA:
			return LEDReg;

        default:
            return OFF;
	}
}
