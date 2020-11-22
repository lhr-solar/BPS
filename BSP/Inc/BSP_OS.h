#ifndef __BSP_OS_H
#define __BSP_OS_H

typedef struct {
    void (*pend)(void);
    void (*post)(void);
} bsp_os_t;

#endif
