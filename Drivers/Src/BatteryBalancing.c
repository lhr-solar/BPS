
#include "LTC6811.h"
#include "Voltage.h"
#include "BatteryBalancing.h"
#include "config.h"
#include "Current.h"

#define chargingTolerance 0




static uint16_t Balancing_Init(void) {
	uint16_t minVoltage = 0;
	if (Current_CheckStatus(0) == 1){
		return;//If charging, return
	}
	Voltage_UpdateMeasurements();
	minVoltage = Voltage_GetModuleMillivoltage(0);
	for (uint8_t i = 1; i < NUM_BATTERY_MODULES; i++){
		if(Voltage_GetModuleMillivoltage(i) < minVoltage) {
			minVoltage = Voltage_GetModuleMillivoltage(i);//Find minimum voltage
		}
	}
	return minVoltage;
}
	
void Balancing_Balance(cell_asic Minions[]){ 
	uint16_t lowest = Balancing_Init(); //get lowest voltage 
	Voltage_UpdateMeasurements();
	for (uint8_t k = 0; k < NUM_BATTERY_MODULES; k++) {
		uint16_t voltage = Voltage_GetModuleMillivoltage(k);//Get voltage of module
		if (voltage > lowest + chargingTolerance) {//Check to see if module is greater than min
			setDischarge(k, Minions);//Set discharge bit if module is too high
		}
		else {//Clear discharge bit of module if it reaches minimum
			uint8_t ICIndex;
			uint8_t MNumber;
			Balancing_GetICNumber(k, &ICIndex, &MNumber);//Get module number and IC Board
			Balancing_ClearDischargeBit(MNumber , 1, &Minions[ICIndex]);//Clear discharge bit
		}
	}
}

void Balancing_ClearDischargeBit(int Cell, uint8_t total_ic, cell_asic *ic){
	for(int i=0; i<total_ic; i++){
		if((Cell<9)&& (Cell!=0)){
			ic[i].config.tx_data[4] = ic[i].config.tx_data[4] & ~(1<<(Cell-1));
		}
    	else if(Cell < 13){
			ic[i].config.tx_data[5] = ic[i].config.tx_data[5] & ~(1<<(Cell-9));
		}
		else{
			break;
		}
  	}
}

void Balancing_GetICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber) {
	uint8_t total = 0;
	uint8_t NUM_MODULES_PER_MINION[4] = {8,8,8,7};
	for(int m = 0; m < NUM_MINIONS; m++) {
		total += NUM_MODULES_PER_MINION[m];
		if(i < total) {
			*ICNumber = m;
			*ModuleNumber = i - total;
			return;
		}
	}
}

void Balancing_SetDischarge(uint8_t module, cell_asic ic[]) { 
	uint8_t ICNumber = 0; 
	uint8_t ModuleNumber = 0;
	Balancing_GetICNumber(module, &ICNumber, &ModuleNumber);//Get IC and ModuleInIC number
	LTC681x_rdcfg(4,ic);
	
	LTC6811_set_discharge(ModuleNumber, 4, &ic[ICNumber]); //Set discharge bit
	LTC681x_wrcfg(4,ic);	
	LTC681x_rdcfg(4,ic);
}



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

void testTheirDischarge(cell_asic ic[]) {
	bool dcc[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; 
	LTC6811_set_cfgr_dis(1, ic, dcc);
}