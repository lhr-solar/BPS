/* Copyright (c) 2022 UT Longhorn Racing Solar */

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
    Simulator_log("Lights initialized\n");
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
	if (signal < 0 || signal >= LIGHTS_MAX) {
		Simulator_log("Error: light out of bounds\n");
	}

	if (initialized) {
		states[signal] ^= states[signal];
		char str[64];
		sprintf(str, "set light %s to %d\n", lightsNames[signal], states[signal]);
	} else {
		Simulator_log("Hard Fault: used lights before initialization\n");
		exit(-1);
	}
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
	if (signal < 0 || signal >= LIGHTS_MAX) {
		Simulator_log("Error: light out of bounds\n");
	}

	if (initialized) {
		states[signal] = 1;
		char str[64];
		sprintf(str, "set light %s to %d\n", lightsNames[signal], states[signal]);
	} else {
		Simulator_log("Hard Fault: used lights before initialization\n");
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
		Simulator_log("Error: light out of bounds\n");
	}

	if (initialized) {
		states[signal] ^= 0;
		char str[64];
		sprintf(str, "set light %s to %d\n", lightsNames[signal], states[signal]);
	} else {
		Simulator_log("Hard Fault: used lights before initialization\n");
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
		Simulator_log("Error: light out of bounds\n");
	}

	if (initialized) {
		return states[signal];
	} else {
		Simulator_log("Hard Fault: used lights before initialization\n");
		exit(-1);
	}
}
