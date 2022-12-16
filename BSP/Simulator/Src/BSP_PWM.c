 /* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
 /*
    For simulating the PWM outputs of the BPS (contactors and fans)
 */

#include "Simulator.h"
#include "BSP_PWM.h"

static bool initialized = false;
static uint32_t pinSpeeds[] = {0, 0, 0, 0, 0};

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
 * @param   pin Which pin to set the speed. Pins 1-4 are defined to be on the fan board, and pin 5 is the contactor output
 * @param   speed The value to write to the output compare register. The timer counts up from 0 to 4000, so speed must be between 0-4000
 * @return  ErrorStatus will return 1 if successful, 0 if there were problems
 */
ErrorStatus BSP_PWM_Set(uint8_t pin, uint32_t speed){
    //Range of pulse is 0-4000
    //First check to make sure that change is within range of values
    //Load new value into Compare and Capture Register
    char *output;
    if (speed>4000) speed = 4000;

    asprintf(&output, "Setting output pin %d to speed %d", pin, speed);
    Simulator_Log(LOG_INFO, output);

    switch (pin)
    {
    case 1:
        pinSpeeds[0] = speed;
        return SUCCESS;
        break;
    case 2:
        pinSpeeds[1] = speed;
        return SUCCESS;
        break;
    case 3:
        pinSpeeds[2] = speed;
        return SUCCESS;
        break;
    case 4:
        pinSpeeds[3] = speed;
        return SUCCESS;
        break;
    case 5:
        pinSpeeds[4] = speed;
        return SUCCESS;
        break;
    default:
        char *errormessage;
        asprintf(&errormessage, "Failed to set output PWM, pin %d is invalid", pin);
        Simulator_Log(LOG_ERROR, errormessage);
        return ERROR; //invalid fan value
        break;
    }
}

/**
 * @brief   Gets the PWM duty cycle of a pin.
 * @param   pin The pin to get the duty cycle from
 * @return  The value inside the output compare register. You will need to math to convert this to an actual duty cycle. Returns -1 if input pin is invalid
 */
int BSP_PWM_Get(uint8_t pin){
    switch (pin)
    {
    case 1:
        return pinSpeeds[0];
        break;
    case 2:
        return pinSpeeds[1];
        break;
    case 3:
        return pinSpeeds[2];
        break;
    case 4:
        return pinSpeeds[3];
        break;
    case 5:
        return pinSpeeds[4];
        break;
    default:
        Simulator_Log(LOG_WARN, "Attempted to read from nonexistent PWM pin");
        return -1;
    }
}

