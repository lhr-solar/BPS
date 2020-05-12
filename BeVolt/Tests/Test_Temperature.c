#include "common.h"
#include "config.h"
#include "Temperature.h"
#include "LTC6811.h"

cell_asic minions[NUM_MINIONS];

int main() {

    Temperature_Init(minions);

    Temperature_UpdateAllMeasurements();

    while(1) {
        
    }
}
