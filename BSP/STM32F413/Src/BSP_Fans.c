/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/* Fans.c
Controls speed of fans which cool down the battery pack.
Uses Pins PC6,7 and PB14,15
*/

#include "BSP_Fans.h"
#include "BSP_Contactor.h"

void BSP_Fans_Init(void){
    BSP_Fans_SetAll(4); //start with all fans half speed
}


/*This function will change the speed of the fans
Inputs: Number of fan to change speed (1-4)
        Speed of Fan(0-8)
*/
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t speed){
    //don't mess with the contactor
    if (fan == CFAN) return ERROR; 
    //Range of pulse is 0-4000
    //First check to make sure that change is within range of values
    //Load new value into Compare and Capture Register
    if  (speed < 0 || speed > TOPSPEED) return ERROR; //if not in range, leave function with error code
    
    return BSP_PWM_Set(fan, speed * DIVIDER);
}

int BSP_Fans_GetSpeed(uint8_t fan){
    if(fan == CFAN) return -1;
    int fanspeed = BSP_PWM_Get(fan);
    if (fanspeed == -1){
        return -1;
    }
    
    return fanspeed / DIVIDER;
}

/**
 * @brief   Sets fan duty cycle for all fans
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 * @return  ErrorStatus
 */
ErrorStatus BSP_Fans_SetAll(uint32_t speed) {
    ErrorStatus result = SUCCESS;
    for (uint8_t i = 1; i <= 4; i++){
        if(i == CFAN) continue;
        ErrorStatus e = BSP_Fans_Set(i, speed);
        if (e != SUCCESS) {
            result = e;
        }
    }
    return result;
}
