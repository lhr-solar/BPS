#include "LTC6811.h"

void testDischarge(cell_asic ic[]) {
	LTC681x_rdcfg(4, ic);  
	ic[0].config.tx_data[4] |= 0xFF;
	ic[0].config.tx_data[5] |= 0x0F;
	LTC681x_wrcfg(4,ic);
	LTC681x_rdcfg(4,ic); 
}

void testStopDischarge(cell_asic ic[]) {
	LTC681x_rdcfg(4, ic); 
	ic[0].config.tx_data[4] &= 0x00;
	ic[0].config.tx_data[5] &= 0xF0;
	LTC681x_wrcfg(4,ic);
	LTC681x_rdcfg(4,ic); 
}

void testModulesDischarge(cell_asic ic[]) {
	bool dcc[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; 
	LTC6811_set_cfgr_dis(1, ic, dcc);
}#include "LTC6811.h"

void testDischarge(cell_asic ic[]) {
    LTC681x_rdcfg(4, ic);  
    ic[0].config.tx_data[4] |= 0xFF;
    ic[0].config.tx_data[5] |= 0x0F;
    LTC681x_wrcfg(4,ic);
    LTC681x_rdcfg(4,ic); 
}

void testStopDischarge(cell_asic ic[]) {
    LTC681x_rdcfg(4, ic); 
    ic[0].config.tx_data[4] &= 0x00;
    ic[0].config.tx_data[5] &= 0xF0;
    LTC681x_wrcfg(4,ic);
    LTC681x_rdcfg(4,ic); 
}

void testModulesDischarge(cell_asic ic[]) {
    bool dcc[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; 
    LTC6811_set_cfgr_dis(1, ic, dcc);
}