/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_Lights.h"

void delay(void) {
    // Random delay loop
    for (volatile int i = 0; i < 100000; i++);
}

int pattern1[10 * 2] = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
int pattern2[10 * 2] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 0};
int *patterns[] = {pattern1, pattern2};

int lights_lut[10] = {FAULT, RUN, UVOLT, OVOLT, OTEMP, OCURR, WIRE, WDOG, CAN, EXTRA};

void light_map(int *ptr) {
    for (int i = 0; i < 10; i++) {
        ptr[i] ? BSP_Light_On(lights_lut[i]) : BSP_Light_Off(lights_lut[i]);
    }
}

int main() {

    BSP_Lights_Init();
    for (int i = 0; i < (sizeof(patterns) / sizeof(*patterns)); i++) {
        memcpy(patterns[i] + 10, patterns[i], 10 * sizeof(int));
    }

    int off[10] = {0};
    light_map(off);

    for (int i = 0; ; i = (i + 1) % (sizeof(patterns) / sizeof(*patterns))) {
        for (int j = 0; j < 100; j++) {
            int light_idx = j % 10;
            light_map(&patterns[i][light_idx]);
            delay();
        }
    }
}
