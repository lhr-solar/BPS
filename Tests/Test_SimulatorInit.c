#include "Simulator.h"
#include "BSP_CAN.h"

int main(void) {
    Simulator_init();
    BSP_CAN_Init(NULL, NULL, false);
    uint8_t data[8] = "NICECOCK";
    BSP_CAN_Write(69, data, 8);
    return 0;
}