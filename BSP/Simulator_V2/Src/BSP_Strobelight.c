/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Strobelight.h"
#include "Simulator.h"
#include <stdint.h>
#include <stdlib.h>

static bool initialized = false;
static bool state = false;
/**
 * @brief   Initializes BSP_strobe 
 * @param   None
 * @return  None
 */
void BSP_Strobe_Init(void) {
	initialized = true;
    Simulator_log("Initialized Strobelight\n");
}

/**
 * @brief   Turns on Strobe (pulse once per second)
 * @param   None
 * @return  None
 */
void BSP_Strobe_On(void) {
    if (initialized) {
        state = true;
        Simulator_log("Strobelight enabled\n");
    } else {
        Simulator_log("Hard Fault: Set strobelight before initialization!\n");
        exit(-1);
    }
}

/**
 * @brief   Turns off Strobe
 * @param   None
 * @return  None
 */
void BSP_Strobe_Off(void) {
    if (initialized) {
        state = false;
        Simulator_log("Strobelight disabled\n");
    }
    else {
        Simulator_log("Hard Fault: Set strobelight before initialization!\n");
        exit(-1);
    }
}