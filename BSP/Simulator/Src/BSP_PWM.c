 /* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
 /*
    For simulating the PWM outputs of the BPS (contactors and fans)
 */

#include "Simulator.h"
#include "BSP_PWM.h"

static bool initialized = false;
static uint32_t pinSpeeds[] = {0, 0, 0, 0, 0};
static bool killSwitchLockdown = false;

/**
 * @brief Only used for simulator purposes.
 * 
 * @param val to set kill switch to
 */
void BSP_PWM_SetKillSwitch(bool val) {
    killSwitchLockdown = val;
}

/**
 * @brief   Sets up contactor and fan pin timers for outputting PWM signals
 */

void BSP_PWM_Init(void){
    //initialize all pins pertaining to fans and contactors for PWM here
    initialized = true;
    Simulator_Log(LOG_INFO,"Initialized PWM interface\n");
}

/**
 * @brief   Sets the PWM duty cycle of a specific pin
 * @note
 * @param   pin Which pin to set the speed. Pins 0-3 are defined to be on the fan board, and pin 4 is the contactor output
 * @param   speed The value to write to the output compare register. The timer counts up from 0 to 4000, so speed must be between 0-4000
 * @return  ErrorStatus will return 1 if successful, 0 if there were problems
 */
ErrorStatus BSP_PWM_Set(uint8_t pin, uint32_t speed){
    //Range of pulse is 0-4000
    //First check to make sure that change is within range of values
    //Load new value into Compare and Capture Register

    if(!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used PWM without initialization!\n");
        exit(-1);
    }

    char *output;

    if (speed > 4000){ 
        speed = 4000;
    }
    
    if (killSwitchLockdown == true && pin >= 3) {
        speed = 0;
    }

    char *cont = (pin == 3) ? "{CFAN}" : "{C1}";
    char *status = (speed == 0) ? "{disabled}" : "{enabled}";

    if (pin >= 3){
        asprintf(&output, "Contactor %s %s\n", cont, status);
    }
    else{
        asprintf(&output, "Fan {%d} set to speed {%d}\n", pin, speed);
    }
    Simulator_Log(LOG_OUTPUT, output);
    free(output);
    if (pin > 4) {
        char *errormessage;
        asprintf(&errormessage, "Failed to set output PWM, pin %d is invalid\n", pin);
        Simulator_Log(LOG_ERROR, errormessage);
        free(errormessage);
        return ERROR; //invalid fan value
    }
    pinSpeeds[pin] = speed;
    return SUCCESS;
}

/**
 * @brief   Gets the PWM duty cycle of a pin.
 * @param   pin The pin to get the duty cycle from
 * @return  The value inside the output compare register. You will need to math to convert this to an actual duty cycle. Returns -1 if input pin is invalid
 */
int BSP_PWM_Get(uint8_t pin){
    if(!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used PWM without initialization!\n");
        exit(-1);
    }

    if(pin > 4){
        Simulator_Log(LOG_WARN, "Attempted to read from nonexistent PWM pin\n");
        return -1;
    }

    return pinSpeeds[pin];
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @note	THIS IS ALSO CODE THAT HAS NO HOME. You cannot get the state of ALL_CONTACTORS. As such, if that param is passed, it will return the state of the array contactor.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_Get(uint8_t contactorChoice) {
    return pinSpeeds[4] != 0;
}

