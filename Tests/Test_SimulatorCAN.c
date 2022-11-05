#include "Simulator.h"
#include "BSP_CAN.h"

/**
 * @brief CAN simulator testing
 *        To test the CAN simulator, either modify a .json file to pass into the sim,
 *        or call BSP_CAN_Write() with the following parameters: [CAN_Read cannot be directly simulated, and must be sourced from a file.]
 *        uint32_t ID   -   ID of the message to "send" over CAN 
 *        uint8_t* data -   array of bytes with max length 8, the "message" itself
 *        uint8_t len   -   length (or amount) of bytes being sent.
 * 
 *        Additionally, please see BSP/Simulator_V2/Data JSON files for examples of how
 *        to write sample JSON test files for the simulator.
 */

int main(int argc, char **argv) {
    Simulator_Init(argv[1]);
    BSP_CAN_Init(NULL, NULL, false);
    BSP_CAN_Write(0, (uint8_t*)"ABCDEFGH", 8);
    BSP_CAN_Write(1, (uint8_t*)"LHRSOLAR", 8);
    BSP_CAN_Write(2, (uint8_t*)"DEADBEEF", 8);
    uint8_t hex[] = {0xAA, 0xBB, 0xCC, 0xDD};
    BSP_CAN_Write(3, hex, 4);
    return 0;
}
