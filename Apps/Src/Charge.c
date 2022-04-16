/* Copyright (c) 2020 UT Longhorn Racing Solar */

/** main.c
 * Program for UTSVT BeVolt's Battery Protection System State of Charge
 */
#include "Charge.h"
#include "BSP_Timer.h"

#define CHARGE_RESOLUTION_SCALE 1000000     // What we need to multiply 100% by before storing. Ensure it is >= 10,000 to avoid depleted charge calculation issues
#define MAX_CHARGE_MILLI_AMP_HRS 41300    // In miliamp-hours (mAh), calculated from 12950(mah)*14 bats in parallel
static int32_t charge;  // % of charge left with 0.000001% resolution

/** Charge_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 * Info found on reference sheet stm32f413 page 535
 */
void Charge_Init(void){ 
	BSP_Timer_Init();
	
	// Grab from EEPROM what is the current Charge
	charge = EEPROM_GetCharge();

    BSP_Timer_Start();
}

/** Charge_Calculate
 * Calculates new Charge depending on the current reading
 * @param milliamps reading from current sensors. Fixed point of 0.001 (1.500 Amps = 1500)
 * not a constant samping. Add on however much from the previous
 */
void Charge_Calculate(int32_t milliamps){ 
	/* Update Charge, units of 0.000001% . 100,000,000 is charge at 100% */
	
	int64_t micro_sec = (int64_t)BSP_Timer_GetMicrosElapsed();
	int64_t millis = (int64_t)milliamps;
	                                                                   // Psuedo code that represents what this math does. In the coded math, order of operations is very important to avoid overflow 
	charge -= (int32_t) (micro_sec * millis                            // microseconds / 1,000,000 / 3600 = hours_elapsed
						* ((100 * CHARGE_RESOLUTION_SCALE) / 1000000)  // (milliamp * hours_elapsed) / max_amp_hour = percent_of_charge_elapsed
						/ 3600 / MAX_CHARGE_MILLI_AMP_HRS);            // percent_charge_elapsed * 100 * charge resolution = a much larger number which represents the charge depleted
}

/** Charge_Calibrate
 * Calibrates the Charge. Whenever the BPS trips, the Charge should recalibrate. If an undervoltage
 * fault occurs, the Charge calibrates to 0% and whenever there is an overvoltage, the Charge calibrates
 * to 100%.
 * @param voltage fault type. 0 if under voltage, 1 if over voltage
 */
void Charge_Calibrate(int8_t faultType){
	if (faultType == UNDERVOLTAGE) {
        charge = 0;
    } else {
        charge = CHARGE_RESOLUTION_SCALE * 100;
    }
}

/** Charge_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.000001% (45,550,000 = 45.55%)
 */
uint32_t Charge_GetPercent(void){
	return charge;
}

/** Charge_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator value in percent of total charge,
 *                    with a resolution = 0.000001% (100,000,000 = 100%)
 */
void Charge_SetAccum(int32_t accum){
	charge = accum;
}

