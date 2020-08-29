/** BatteryBalancing.h
 *  functions for battery balancing.
 */
#ifndef BATTERY_BALANCING_H
#define BATTERY_BALANCING_H


#include "LTC6811.h"
#include "Voltage.h"

/**
 * @brief   Finds lowest voltage amongst the 31 modules. Meant to be called only in Balancing_Balance()
 * @param   None
 * @return  the  
 */
static uint16_t Balancing_Init(void);

/**
 * @brief   Loops through all 31 modules, sets discharge bits for any module if its voltage is too high, and clears discharge bits for any modules with voltages that are too low
 * @param   Minions array of the ICs that the modules are connected to
 * @return  None
 */
void Balancing_Balance(cell_asic Minions[]);

/**
 * @brief   Recieves module number out of 31 and stores the module number and its IC number in two buffers
 * @param   i module number out of 31
 * @param   ICNumber buffer for IC number
 * @param   ModuleNumber buffer for module number
 * @return  None
 */
void Balancing_GetICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber);


/**
 * @brief   Loops through all 31 modules, sets discharge bits for any module if its voltage is too high, and clears discharge bits for any modules with voltages that are too low
 * @param   module module number 
 * @param   ic array of the ICs in the system
 * @return  None
 */
void Balancing_SetDischarge(uint8_t module, cell_asic ic[]);

	

/**
 * @brief   Clears the discharge bit of the desired module
 * @param   Cell cell that will stop discharging
 * @param   total_ic total number of ICs in the system
 * @param   ic array of ICs in the system
 * @return  None
 */
void Balancing_ClearDischargeBit(int Cell, uint8_t total_ic, cell_asic ic[]);

void testDischarge(cell_asic ic[]);

void testStopDischarge(cell_asic ic[]);

void testTheirDischarge(cell_asic ic[]);

#endif