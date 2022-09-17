/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "PID.h"
#include "Tasks.h"

uint32_t Proportion,Integral,Derivative;


//returns fan speed based on proportion value
int32_t PID_output(int32_t avg_temp, int32_t desired_temp) {
    return Proportion*(avg_temp - desired_temp);
}



