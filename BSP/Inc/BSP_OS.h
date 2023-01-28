/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#ifndef BSP_OS_H
#define BSP_OS_H
#include "common.h"

typedef struct {
    callback_t pend;
    callback_t post;
} bsp_os_t;

#endif
