#include "common.h"
#include "config.h"
#include "LTC6811.h"

cell_asic minions[NUM_MINIONS];

int main() {

    LTC6811_Init(minions);

    LTC6811_wrcfg(NUM_MINIONS, minions);

    LTC6811_rdcfg(NUM_MINIONS, minions);

    while(1) {
        
    }
}
