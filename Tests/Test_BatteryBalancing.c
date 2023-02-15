/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "LTC6811.h"

void testDischarge(cell_asic ic[]) {
    LTC681x_rdcfg(4, ic);  
    ic[0].config.tx_data[4] |= 0xFF;
    ic[0].config.tx_data[5] |= 0x0F;
    LTC681x_wrcfg(NUM_MINIONS,ic);
    LTC681x_rdcfg(NUM_MINIONS,ic); 
}

void testStopDischarge(cell_asic ic[]) {
    LTC681x_rdcfg(4, ic); 
    ic[0].config.tx_data[4] &= 0x00;
    ic[0].config.tx_data[5] &= 0xF0;
    LTC681x_wrcfg(NUM_MINIONS,ic);
    LTC681x_rdcfg(NUM_MINIONS,ic); 
}

void testModulesDischarge(cell_asic ic[]) {
    bool dcc[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; 
    LTC6811_set_cfgr_dis(1, ic, dcc);
}

int main(void){
    cell_asic Minions[NUM_MINIONS];
    LTC6811_Init(Minions);

    testDischarge(Minions);
    for(volatile int i = 0; i < 5000000; i++);
    testStopDischarge(Minions);
    for(volatile int i = 0; i < 5000000; i++);
    testModulesDischarge(Minions);
    for(volatile int i = 0; i < 5000000; i++);
}