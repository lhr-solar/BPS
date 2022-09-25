/* Copyright (c) 2022 UT Longhorn Racing Solar */
/** PID.h
 * File contains PID loop function
 */

#ifndef PID_H
#define PID_H

#include "common.h"

/**
 * @brief Gives fan speed based on Average temperature of pack and past error values
 * @param InputTemp - current temperature
 * @param DesiredTemp - desired temperature
 * @return FanSpeed: 0-4000 PWM
 */
int32_t PID_Output(int32_t InputTemp, int32_t DesiredTemp);

#endif