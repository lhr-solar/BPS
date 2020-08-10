/** BatteryBalancing.h
 *  functions for battery balancing.
 */
#ifndef BATTERY_BALANCING_H
#define BATTERY_BALANCING_H


#include "LTC6811.h"
#include "LTC6811.h"
#include "Voltage.h"

void ReleaseChargeInit(void);
/*
Function wil return index with value of minimum voltage and module with minimum voltage
*/

void ReleaseCharge(cell_asic Minions[]);

void setDischarge(uint8_t i, cell_asic ic[]);
/*Following function was copied from SetDischarge from LTC library
and logical operators were switched to clear bits instead of set
*/
	
void getICNumber(uint8_t i, uint8_t* ICNumber, uint8_t* ModuleNumber);
/*Following function recieves module number out of 31 and 2 pointers
Returns IC number and module number 
*/
 
void ClearDischargeBit(int Cell, uint8_t total_ic, cell_asic ic[]);

void testDischarge(cell_asic ic[]);

void testStopDischarge(cell_asic ic[]);

void testTheirDischarge(cell_asic ic[]);

#endif