/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "LTC6811.h"
#include "Voltage.h"
#include "BatteryBalancing.h"
#include "config.h"
#include "Tasks.h"

static void Balancing_ClearDischargeBit(int Cell, uint8_t total_ic, cell_asic *ic);
static void Balancing_GetICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber);
static void Balancing_SetDischargeBit(uint8_t module, cell_asic ic[]);

/**
 * @brief   Finds lowest voltage amongst the 31 modules. Meant to be called only in Balancing_Balance()
 * @param   None
 * @return  the  minimum voltage
 */
static uint16_t GetMinimumVoltage(void) {
	uint16_t minVoltage = 0;
	minVoltage = Voltage_GetModuleMillivoltage(0);
	for (uint8_t i = 1; i < NUM_BATTERY_MODULES; i++){
		if(Voltage_GetModuleMillivoltage(i) < minVoltage) {
			minVoltage = Voltage_GetModuleMillivoltage(i);//Find minimum voltage
		}
	}
	return minVoltage;
}

/**
 * @brief   Loops through all 31 modules, sets discharge bits for any module if its voltage is too high, and clears discharge bits for any modules with voltages that are too low
 * @param   Minions array of the ICs that the modules are connected to
 * @return  None
 */	
void Balancing_Balance(cell_asic Minions[]){ 
	uint16_t lowest = GetMinimumVoltage(); //get lowest voltage 
	for (uint8_t k = 0; k < NUM_BATTERY_MODULES; k++) {
		uint16_t voltage = Voltage_GetModuleMillivoltage(k);	
		if (voltage > lowest + BALANCING_TOLERANCE_START) {	
			Balancing_SetDischargeBit(k, Minions);	
		}
		//Clear discharge bit of module if it reaches minimum
		else if(voltage < lowest + BALANCING_TOLERANCE_STOP) {
			uint8_t ICIndex;
			uint8_t MNumber;
			Balancing_GetICNumber(k, &ICIndex, &MNumber);	
			Balancing_ClearDischargeBit(MNumber, NUM_MINIONS, &Minions[ICIndex]);	
		}
	}
}

/**
 * @brief   Clears the discharge bit of the desired module
 * @param   Cell cell that will stop discharging
 * @param   total_ic total number of ICs in the system
 * @param   ic array of ICs in the system
 * @return  None
 */
static void Balancing_ClearDischargeBit(int Cell, uint8_t total_ic, cell_asic *ic){
// No simulator support for battery balancing (yet)
#ifndef SIMULATION
	xSemaphoreTake(MinionsASIC_Mutex, (TickType_t)portMAX_DELAY); 
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
	xSemaphoreGive(MinionsASIC_Mutex);
#endif
}

/**
 * @brief   Recieves module number out of 31 and stores the module number and its IC number in two buffers
 * @param   i module number out of NUM_BATTERY_MODULES
 * @param   ICNumber buffer for IC number
 * @param   ModuleNumber buffer for module number
 * @return  None
 */
static void Balancing_GetICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber) {
	//TODO: Find a way to generalize getting the IC number and Module number per board
	//TODO: Find a way to generalize getting module number given IC number and Module number per board (used in Temperature.c)
	*ICNumber = i / MAX_VOLT_SENSORS_PER_MINION_BOARD; //IC number is between 0 and NUM_MINIONS
	*ModuleNumber = i % MAX_VOLT_SENSORS_PER_MINION_BOARD; //Module Number is between 0 and MAX_VOLT_SENSORS_PER_MINION_BOARD
}

/**
 * @brief   Sets the discharge bit of the desired module
 * @param   module cell that will discharge
 * @param   ic array of ICs in the system
 * @return  None
 */
static void Balancing_SetDischargeBit(uint8_t module, cell_asic ic[]) { 
// No simulator support for battery balancing (yet)
#ifndef SIMULATION
	uint8_t ICNumber = 0; 
	uint8_t ModuleNumber = 0;
	Balancing_GetICNumber(module, &ICNumber, &ModuleNumber);//Get IC and ModuleInIC number
	
	xSemaphoreTake(MinionsASIC_Mutex, (TickType_t)portMAX_DELAY); 
	
	LTC6811_rdcfg_safe(NUM_MINIONS, ic);
	LTC6811_set_discharge(ModuleNumber, NUM_MINIONS, &ic[ICNumber]); //Set discharge bit
	LTC6811_wrcfg(NUM_MINIONS, ic);	
	LTC6811_rdcfg_safe(NUM_MINIONS, ic);
	
	xSemaphoreGive(MinionsASIC_Mutex);
#endif

}
