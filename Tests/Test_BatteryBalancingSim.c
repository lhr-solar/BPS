#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "LTC6811.h"
#include "LTC681x.h"

cell_asic minions[NUM_MINIONS];

int main(void){
    LTC6811_Init(minions);
    minions[0].config.tx_data[4] &= 0x0F;
    LTC6811_wrcfg(NUM_MINIONS, minions);
   exit(0); 
}