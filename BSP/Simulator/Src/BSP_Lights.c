/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * BSP_Lights.c - Simulates LED's on BPS leader board.
*/

#include "BSP_Lights.h"
#include "Simulator.h"

static const char* lightsNames[] = {
	"RUN",
	"UVOLT",
	"OVOLT",
	"OTEMP",
	"OCURR",
	"WDOG",
	"CAN",
	"EXTRA",
	"WIRE",
	"FAULT"
};
static bool initialized = false;
static State states[LIGHTS_MAX];

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
	initialized = true;
    Simulator_Log(LOG_INFO, "Lights initialized\n");
	//Need to initially turn off all lights for simulator to detect wrong end state
	for (uint8_t i = 0; i < LIGHTS_MAX; i++){
		BSP_Light_Off(i);
	}
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
	if (signal < 0 || signal >= LIGHTS_MAX) {
		Simulator_Log_Location(LOG_ERROR, "Light out of bounds\n");
	}

	if (initialized) {
		states[signal] ^= 1;
		char* str;
		asprintf(&str, "Set Light {%s} to {%d}\n", lightsNames[signal], states[signal]);
		Simulator_Log(LOG_OUTPUT, str);
	} else {
		Simulator_Log_Location(LOG_ERROR, "Used lights before initialization\n");
		exit(-1); // Fault state for lights??
	}
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
	if (signal < 0 || signal >= LIGHTS_MAX) {
		Simulator_Log_Location(LOG_ERROR, "Light out of bounds\n");
	}

	if (initialized) {
		states[signal] = 1;
		char* str;
		asprintf(&str, "Set Light {%s} to {%d}\n", lightsNames[signal], states[signal]);
		Simulator_Log(LOG_OUTPUT, str);
	} else {
		Simulator_Log_Location(LOG_ERROR, "Used lights before initialization\n");
		exit(-1);
	}
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
	if (signal < 0 || signal >= LIGHTS_MAX) {
		Simulator_Log_Location(LOG_ERROR, "Light out of bounds\n");
	}

	if (initialized) {
		states[signal] ^= 0;
		char* str;
		asprintf(&str, "Set Light {%s} to {%d}\n", lightsNames[signal], states[signal]);
		Simulator_Log(LOG_OUTPUT, str);
	} else {
		Simulator_Log_Location(LOG_ERROR, "Used lights before initialization\n");
		exit(-1);
	}
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of signal
 */
State BSP_Light_GetState(Light signal) {
	if (signal < 0 || signal >= LIGHTS_MAX) {
		Simulator_Log(LOG_ERROR, "Light out of bounds\n");
	}

	if (initialized) {
		return states[signal];
	} else {
		Simulator_Log_Location(LOG_ERROR, "Used lights before initialization\n");
		exit(-1);
	}
}
