/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Strobelight.h"
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
    Simulator_log("Initialized Strobelight\n", 24);
}

/**
 * @brief   Turns on Strobe (pulse once per second)
 * @param   None
 * @return  None
 */
void BSP_Strobe_On(void) {
    if (initialized) {
        state = true;
        Simulator_log("Strobelight enabled\n", 20);
    } else {
        Simulator_log("Hard Fault: Set strobelight before initialization!\n", 51);
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
        Simulator_log("Strobelight disabled\n",21);
    }
    else {
        Simulator_log("Hard Fault: Set strobelight before initialization!\n", 51);
        exit(-1);
    }
}