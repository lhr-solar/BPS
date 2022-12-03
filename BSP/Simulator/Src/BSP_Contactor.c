/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * BSP_Contactor.c - Simulates Contactor GPIO pins.
*/

#include "BSP_Contactor.h"
#include "Simulator.h"
#include "Tasks.h"
#include <stdbool.h>

static bool initialized = false;
static bool state = false;

typedef struct {
    bool HVHIGH;
    bool HVLOW;
    bool ARRAY;
} ContactorStates_t;

static ContactorStates_t ContStates;

/*
 * @brief   Initializes the GPIO pins that interfaces with the Contactor.
 *          Two GPIO pins are initialized. One as an output and one as an input.
 *          The output pin controls the state and the input pin views what state the contactor is through the
 *          Aux pins.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Init(void) {
    initialized = true;
    Simulator_Log(LOG_INFO, "Initialized Contactors\n");
}

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(CONT_CHOICE contactor) {
    if (initialized) {
        if (contactor == HVHIGH_CONTACTOR) {
            Simulator_Log(LOG_INFO, "Contactor 'HVHIGH' enabled\n");
            ContStates.HVHIGH = true;
        } else if (contactor == ARRAY_CONTACTOR) {
            Simulator_Log(LOG_INFO, "Contactor 'ARRAY' enabled\n");
            ContStates.HVLOW = true;
        } else if (contactor == HVLOW_CONTACTOR) {
            Simulator_Log(LOG_INFO, "Contactor 'HVLOW' enabled\n");
            ContStates.HVLOW = true;
        } else if (contactor == ALL_CONTACTORS) {
            Simulator_Log(LOG_INFO, "All Contactors enabled\n");
            ContStates.ARRAY = ContStates.HVLOW = ContStates.HVHIGH = true;
        }
    } else {
        Simulator_Log(LOG_ERROR, "Hard Fault: Set contactors before initialization!\n");
        Fault_BitMap = Fault_ESTOP;
        EnterFaultState();
    }
}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(CONT_CHOICE contactor) {
    if (initialized) {
        state = false;
        if (contactor == HVHIGH_CONTACTOR) {
            Simulator_Log(LOG_INFO, "Contactor 'HVHIGH' disabled\n");
            ContStates.HVHIGH = false;
        } else if (contactor == ARRAY_CONTACTOR) {
            Simulator_Log(LOG_INFO, "Contactor 'ARRAY' disabled\n");
            ContStates.ARRAY = false;
        } else if (contactor == HVLOW_CONTACTOR) {
            Simulator_Log(LOG_INFO, "Contactor 'HVLOW' disabled\n");
            ContStates.HVLOW = false;
        } else if (contactor == ALL_CONTACTORS) {
            Simulator_Log(LOG_INFO, "All Contactors disabled\n");
            ContStates.ARRAY = ContStates.HVLOW = ContStates.HVHIGH = false;
        }
    } else {
        Simulator_Log(LOG_ERROR, "Set contactor before initialization!\n");
        Fault_BitMap = Fault_ESTOP;
        EnterFaultState();
    }
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(CONT_CHOICE contactor) {
    if (initialized) {
        if (contactor == HVHIGH_CONTACTOR) {
            return ContStates.HVHIGH;
        } else if (contactor == ARRAY_CONTACTOR) {
            return ContStates.ARRAY;
        } else { 
            // note: this will also return if ALL_CONTACTORS because we can't get every cont state in 1 return
            return ContStates.HVLOW;
        }
    } else {
        Simulator_Log(LOG_ERROR, "Read contactor before initialization!\n");
        Fault_BitMap = Fault_ESTOP;
        EnterFaultState();
        return false; // never reached, but...
    }
}
