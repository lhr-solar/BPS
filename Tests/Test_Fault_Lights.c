/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
/* This tests the lights logic that is in the EnterFaultState Function. Since the function has a while loop, at the end, it is redefined
here to only test the lights functionality.
*/
typedef enum {
    Fault_UVOLT   = 0x001,
    Fault_OVOLT   = 0x002,
    Fault_OTEMP   = 0x004,
    Fault_OCURR   = 0x008,
    Fault_OW      = 0x010,
    Fault_HANDLER = 0x020,
    Fault_OS      = 0x040,
    Fault_WDOG    = 0x080,
    Fault_CRC     = 0x100,
    Fault_ESTOP   = 0x200,
    Fault_Max
}Fault_Set;
extern Fault_Set Fault_BitMap;
struct FaultToOut_t {
    Light light;
    char* string;
};
static const struct FaultToOut_t FaultDict[Fault_Max] = {
    [Fault_UVOLT]       = {.light = UVOLT, .string = "UVOLT"}, //do not have enough led's so some are reused
    [Fault_OVOLT]       = {.light = OVOLT, .string = "OVOLT"},
    [Fault_OTEMP]       = {.light = OTEMP, .string = "OTEMP"},
    [Fault_OCURR]       = {.light = OCURR, .string = "OCURR"},
    [Fault_OW]          = {.light = WIRE, .string = "OW"},
    [Fault_HANDLER]     = {.light = EXTRA, .string = "HANDLER"},
    [Fault_OS]          = {.light = EXTRA, .string = "OS"},
    [Fault_WDOG]        = {.light = WDOG, .string = "WDOG"},
    [Fault_CRC]         = {.light = CAN, .string = "CRC"},
    [Fault_ESTOP]       = {.light = WIRE, .string = "ESTOP"}
};
void FakeEnterFaultState() {
    BSP_Lights_Init();
    BSP_Light_Off(RUN); //turn off run light
    BSP_Light_On(FAULT);
    for (uint16_t i = 1; i < Fault_Max; i <<= 1){
        if (Fault_BitMap & i) {
            #ifdef SIMULATION
                sprintf(err, "$$$ Entered fault in state {%d} - %s\n", stateCount - 1, FaultDict[i].string);
                Simulator_Log_Location(LOG_INFO, err);
            #endif
            BSP_Light_On(FaultDict[i].light); //allow multiple lights to be turned on
        }
    }
}
int main() {
    for (Fault_BitMap = 1; Fault_BitMap < Fault_Max; Fault_BitMap <<= 1){
        FakeEnterFaultState(); // Test each light individually
        BSP_PLL_DelayMs(500);
    }
}
