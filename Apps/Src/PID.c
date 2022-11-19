/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "Temperature.h"
#include "Tasks.h"

//TODO: tune PID with actual pack and fans, and then change values below to appropiate value
#define PROPORTION -1234
#define INTEGRAL 4321
#define DERIVATIVE -9876
#define MAX_FAN_SPEED 4000
#define DIVISOR 25000

static int32_t ErrorSum = 0;
static int32_t Error;
static int32_t Rate;
static int32_t PreviousError = 0;

/**
 * @brief Gives fan speed based on Average temperature of pack and past error values
 * @param InputTemp - current temperature
 * @param DesiredTemp - desired temperature
 * @return FanSpeed: 0-4000 PWM
 */
int32_t PID_Output(int32_t InputTemp, int32_t DesiredTemp) {
    Error = DesiredTemp - InputTemp;
    ErrorSum = ErrorSum + Error;

    if (PreviousError == 0) {PreviousError = Error;} //init previous val first time

    Rate = Error - PreviousError;
    PreviousError = Error;     //updates previous err value

    if (((PROPORTION*(Error) + INTEGRAL*(ErrorSum) + DERIVATIVE*(Rate))/DIVISOR) > MAX_FAN_SPEED) {
        return MAX_FAN_SPEED;
    }
    if (((PROPORTION*(Error) + INTEGRAL*(ErrorSum) + DERIVATIVE*(Rate))/DIVISOR) <= 0) {
        return 0;
    }
    return (PROPORTION*(Error) + INTEGRAL*(ErrorSum) + DERIVATIVE*(Rate))/DIVISOR;
}
