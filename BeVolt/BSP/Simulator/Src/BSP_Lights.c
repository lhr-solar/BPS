#include "BSP_Lights.h"
#include <stdint.h>
#include <stdlib.h>

static const char* file = "BSP/Simulator/DataGeneration/Data/Lights.csv";
int LEDReg;
/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
	//doesn't need to be written
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
	FILE *fp;
	fp = fopen(file, "w+"); //open file for reading and writing
	char csv[9]; //initialize 9 character array
	fgets(csv, 9, fp); //get values and store them in array
	LEDReg = atoi(csv); //convert values to integers
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
	fprintf(fp, "%d", LEDReg);; //Write number to file
	fclose(fp); //close file
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
	FILE *fp;
	fp = fopen(file, "w+"); //open file for reading and writing
	char csv[9]; //initialize 9 character array
	fgets(csv, 9, fp); //get values and store them in array
	LEDReg = atoi(csv); //convert values to integers
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
	fprintf(fp, "%d", LEDReg);; //Write number to file
	fclose(fp); //close file
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
	FILE *fp;
	fp = fopen(file, "w+"); //open file for reading and writing
	char csv[9]; //initialize 9 character array
	fgets(csv, 9, fp); //get values and store them in array
	LEDReg = atoi(csv); //convert values to integers
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
	fprintf(fp, "%d", LEDReg);; //Write number to file
	fclose(fp); //close file
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of signal
 */
State BSP_Light_GetState(Light signal) {
	int returnVal;
	FILE *fp;
	fp = fopen(file, "r"); //open file for reading
	char csv[9]; //initialize 9 character array
	fgets(csv, 9, fp); //get values and store them in array
	LEDReg = atoi(csv); //convert values to integers
    switch(signal){
		case FAULT:
			returnVal = (LEDReg & ~0x0100) >> 8;
			break;
		case RUN:
			returnVal = (LEDReg & ~0x0080) >> 7;
			break;
		case OCURR:
			returnVal = (LEDReg & ~0x0040) >> 6;
			break;
		case OTEMP:
			returnVal = (LEDReg & ~0x0020) >> 5;
			break;
		case OVOLT:
			returnVal = (LEDReg & ~0x0010) >> 4;
			break;
		case UVOLT:
			returnVal = (LEDReg & ~0x0008) >> 3;
			break;
		case WDOG:
			returnVal = (LEDReg & ~0x0004) >> 2;
			break;
		case CAN:
			returnVal = (LEDReg & ~0x0002) >> 1;
			break;
		case EXTRA:
			returnVal = (LEDReg & ~0x0001);
			break;
        default:
            returnVal = OFF;
	}
	fclose(fp); //close file
	return returnVal;
}
