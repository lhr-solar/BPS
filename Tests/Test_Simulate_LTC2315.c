#include "Simulator.h"
#include "Amps.h"
/**
 * @brief Test for getting current
 * Call Amps_UpdateMeasurements() anytime to get current reading based on JSON file
*/
int main(int argc, char **argv) {
    Simulator_Init(argv[1]);
    Amps_UpdateMeasurements();
    return 0;
}
