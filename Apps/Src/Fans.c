/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/* Fans.c
Controls speed of fans which cool down the battery pack.
Uses Pins PC6,7 and PB14,15
*/

#include "Fans.h"
#include "Contactor.h"

/**
 * @brief   Initialize all the GPIO pins connected to each Fan
 * @param   None
 * @return  None
 */
void Fans_Init(void){
    #ifdef SIMULATION
        Simulator_Log(LOG_INFO, "Fans Initialized\n");
    #endif
    BSP_PWM_Init();
}

/**
 * @brief This function will change the speed of the fans
 * @param fan Number of fan to change speed (1-4)
 * @param speed Speed of Fan(0-TOPSPEED)
 * @return ErrorStatus will return 1 if successful, 0 if error occurred
 */
ErrorStatus Fans_Set(uint8_t fan, uint8_t speed){
    //don't mess with the contactor
    if (fan == CFAN) return ERROR;
    //Range of pulse is 0-4000
    //First check to make sure that change is within range of values
    //Load new value into Compare and Capture Register
    return (speed < 0 || speed > TOPSPEED) ? BSP_PWM_Set(fan, TOPSPEED * DIVIDER) : BSP_PWM_Set(fan, speed * DIVIDER); //if not in range, leave function with error code
}

/**
 * @brief   Reads the duty cycle for a specified fan
 * @param   fan Gets the duty cycle for this fan
 * @return  int representation of the fan speed from 0-8, -1 if an error occurred
 */
int Fans_GetSpeed(uint8_t fan){
    if (fan == CFAN) return -1;
    int fanspeed = BSP_PWM_Get(fan);
    return fanspeed == -1 ? -1 : (fanspeed / DIVIDER);
}

/**
 * @brief   Sets fan duty cycle for all fans
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 * @return  ErrorStatus
 */
ErrorStatus Fans_SetAll(uint8_t speed) {
    ErrorStatus result = SUCCESS;
    for (uint8_t i = 0; i < 4; i++){
        if (i == CFAN) continue;
        ErrorStatus e = Fans_Set(i, speed);
        if (e != SUCCESS) result = e;
    }
    return result;
}
