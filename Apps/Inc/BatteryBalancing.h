/** BatteryBalancing.h
 *  Functions for battery balancing.
 */
#ifndef BATTERY_BALANCING_H
#define BATTERY_BALANCING_H


#include "LTC6811.h"
#include "Voltage.h"

/**
 * @brief   Loops through all 31 modules, sets discharge bits for any module if its voltage is 
 *          too high, and clears discharge bits for any modules with voltages that are too low
 * @param   Minions array of the ICs that the modules are connected to
 * @return  None
 */
void BattBalancing_Balance(cell_asic Minions[]);

#endif