/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "PID.h"
#include "Tasks.h"

#define Proportion 1000     
#define Integral 1000
#define Derivative 1000

static int32_t area = 0;
int32_t err;
int32_t rate;
//returns fan speed based on proportion value
int32_t PID_output(int32_t avg_temp, int32_t desired_temp) {
    err = desired_temp - avg_temp;
    area = area + err;
    
    return Proportion*(err);
}



