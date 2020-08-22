#include "SPI.h"
#include "LTC6811.h"
#include "Voltage.h"
#include "BatteryBalancing.h"
#include "config.h"
#include "Current.h"

#define chargingTolerance 0

uint16_t minVoltage = 0;
uint8_t minIndex = 0;

void ReleaseChargeInit(void) {
	if (Current_CheckStatus() == 1){
		return;//If charging, return
	}
	Voltage_UpdateMeasurements();
	minVoltage = Voltage_GetModuleMillivoltage(0);
	for (uint8_t i = 1; i < NUM_BATTERY_MODULES; i++){
		printf("\rChecking %d, found voltage of %d\n\r",i,Voltage_GetModuleMillivoltage(i));
		if(Voltage_GetModuleMillivoltage(i) < minVoltage) {
			minVoltage = Voltage_GetModuleMillivoltage(i);//Find minimum voltage
			minIndex = i;//Find module with minimum voltage
			printf("\rNew min of %d found at %d\n\r", minVoltage, minIndex);
		}
	}
}
	
void ReleaseCharge(cell_asic Minions[]){ 
	bool stillDischarging = false;
	Voltage_UpdateMeasurements();
	for (uint8_t k = 0; k < NUM_BATTERY_MODULES; k++) {
		uint16_t voltage = Voltage_GetModuleMillivoltage(k);//Get voltage of module
		if (voltage > minVoltage + chargingTolerance) {//Check to see if module is greater than min
			setDischarge(k, Minions);//Set discharge bit if module is too high
			printf("\n\rSetting discharge bit #%d",k);
			stillDischarging = true;
		}
		else {//Clear discharge bit of module if it reaches minimum
			uint8_t ICIndex;
			uint8_t MNumber;
			getICNumber(k, &ICIndex, &MNumber);//Get module number and IC Board
			printf("\n\rClearing discharge bit #%d",k);
			ClearDischargeBit(MNumber , 1, &Minions[ICIndex]);//Clear discharge bit
		}
	}
	// make all open circuits
	if (!stillDischarging) {
		clear_discharge(NUM_MINIONS, Minions);
	}
}

void ClearDischargeBit(int Cell, //The cell to be discharged
						   uint8_t total_ic, //Number of ICs in the system
						   cell_asic *ic //A two dimensional array that will store the data
						   )
{
  for(int i=0; i<total_ic; i++)
  {
    if((Cell<9)&& (Cell!=0))
      ic[i].config.tx_data[4] = ic[i].config.tx_data[4] & ~(1<<(Cell-1));
    else if(Cell < 13)
      ic[i].config.tx_data[5] = ic[i].config.tx_data[5] & ~(1<<(Cell-9));
	else
		break;
 }
}

void getICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber) {
	uint8_t total = 0;
	for(int m = 0; m < NUM_MINIONS; m++) {
		total += NUM_MODULES_PER_MINION[m];
		if(i < total) {
			*ICNumber = m;
			*ModuleNumber = i - total;
			return;
		}
	}
}

void setDischarge(uint8_t i, cell_asic ic[]) { //i is module number
	uint8_t ICNumber = 0; 
	uint8_t ModuleNumber = 0;
	getICNumber(i, &ICNumber, &ModuleNumber);//Get IC and ModuleInIC number
	LTC681x_rdcfg(4,ic);
	
	LTC6811_set_discharge(ModuleNumber, 4, &ic[ICNumber]); //Set discharge bit
	LTC681x_wrcfg(4,ic);	
	LTC681x_rdcfg(4,ic);
	printf("\r\n%d",ic[0].config.rx_data[4]);
	printf(" %d\n" ,ic[0].config.rx_data[5]);
}



void testDischarge(cell_asic ic[]) {
	LTC681x_rdcfg(4, ic);  
	printf("0x%x\n\r",ic[0].config.tx_data[4]);
	ic[0].config.tx_data[4] |= 0xFF;
	ic[0].config.tx_data[5] |= 0x0F;
	LTC681x_wrcfg(4,ic);
	LTC681x_rdcfg(4,ic); 
	printf("0x%x\n\r",ic[0].config.tx_data[4]);
}

void testStopDischarge(cell_asic ic[]) {
	LTC681x_rdcfg(1, ic); 
	printf("0x%x\n\r",ic[0].config.tx_data[4]);
	ic[0].config.tx_data[4] &= 0x00;
	ic[0].config.tx_data[5] &= 0xF0;
	LTC681x_wrcfg(1,ic);
	LTC681x_rdcfg(1,ic); 
	printf("0x%x\n\r",ic[0].config.tx_data[4]);
}

void testTheirDischarge(cell_asic ic[]) {
	bool dcc[12] = {1,1,1,1,1,1,1,1,1,1,1,1}; 
	LTC6811_set_cfgr_dis(1, ic, dcc);
}