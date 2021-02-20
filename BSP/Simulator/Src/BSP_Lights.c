/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_Lights.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>

static const char* file = GET_CSV_PATH(LIGHTS_CSV_FILE);

int LEDReg;
/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
	// Creates file if none exists
	FILE* fp = fopen(file, "w+");

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

	fprintf(fp, "0");

    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

	fclose(fp);
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
	FILE* fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

	char csv[4];
	fgets(csv, 4, fp);
    
    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

    fclose(fp);

	LEDReg = atoi(csv);
    switch(signal){
			
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

    fp = fopen(file, "w");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    fno = fileno(fp);
    flock(fno, LOCK_EX);

	fprintf(fp, "%d", LEDReg);

    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

	fclose(fp);
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
	FILE* fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

	char csv[4];
	fgets(csv, 4, fp);

    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

    fclose(fp);

	LEDReg = atoi(csv);
    switch(signal){
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

    fp = fopen(file, "w");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    fno = fileno(fp);
    flock(fno, LOCK_EX);

	fprintf(fp, "%d", LEDReg);

    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

	fclose(fp);
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
	FILE* fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

	char csv[4];
	fgets(csv, 4, fp);

    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

    fclose(fp);

	LEDReg = atoi(csv);
    switch(signal){
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

    fp = fopen(file, "w");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    fno = fileno(fp);
    flock(fno, LOCK_EX);

	fprintf(fp, "%d", LEDReg);
    
    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

	fclose(fp);
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of signal
 */
State BSP_Light_GetState(Light signal) {
	State returnVal;
	FILE* fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(LIGHTS_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/Lights.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

	char csv[4];
	fgets(csv, 4, fp);
	LEDReg = atoi(csv);
    switch(signal){
		case RUN:
			returnVal = (LEDReg & 0x0080) >> 7;
			break;
		case OCURR:
			returnVal = (LEDReg & 0x0040) >> 6;
			break;
		case OTEMP:
			returnVal = (LEDReg & 0x0020) >> 5;
			break;
		case OVOLT:
			returnVal = (LEDReg & 0x0010) >> 4;
			break;
		case UVOLT:
			returnVal = (LEDReg & 0x0008) >> 3;
			break;
		case WDOG:
			returnVal = (LEDReg & 0x0004) >> 2;
			break;
		case CAN:
			returnVal = (LEDReg & 0x0002) >> 1;
			break;
		case EXTRA:
			returnVal = (LEDReg & 0x0001);
			break;
        default:
            returnVal = OFF;
	}

    // Unlock the lock so the simulator can write to Lights.csv again
    flock(fno, LOCK_UN);

	fclose(fp);

	return returnVal;
}
