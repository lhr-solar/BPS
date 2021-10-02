/* Copyright (c) 2020 UT Longhorn Racing Solar */

/** main.c
 * Program for UTSVT BeVolt's Battery Protection System State of Charge
 */
#include "Charge.h"
#include "BSP_Timer.h"

#define CHARGE_RESOLUTION_SCALE 100     // What we need to multiply 100% by before storing
#define MAX_CHARGE_AMP_HRS 1000*1000    // In amp-hours (Ah), for now it is a dummy value

static uint32_t charge;  // % of charge left with 0.01% resolution

/** Charge_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 * Info found on reference sheet stm32f413 page 535
 */
void Charge_Init(void){ 
	BSP_Timer_Init();
	
	// Grab from EEPROM what is the current Charge
	EEPROM_ReadMultipleBytes(EEPROM_SOC_PTR_LOC, 4, (uint8_t*)&charge);

    BSP_Timer_Start();
}

/** Charge_Calculate
 * Calculates new Charge depending on the current reading
 * @param current reading from current sensors. Fixed point of 0.001 (1.500 Amps = 1500)
 * not a constant samping. Add on however much from the previous
 */
void Charge_Calculate(int32_t milliamps){ 
	
	//uint32_t counter = BSP_Timer_GetTicksElapsed();
	
	//uint32_t ticksElapsed = 0xFFFF - counter;	// I hate this, but fixing BSP_Timer_GetTicksElapsed() is beyond the scope of this PR. Opened issue #389 for this
	uint32_t ticksElapsed = BSP_Timer_GetTicksElapsed(); // BSP_Timer_GetTicks now counts up from 0, so return value is accurate
	uint32_t clockFrequency = BSP_Timer_GetRunFreq();

	/* Update Charge, units of 0.01% */
	// TODO: I am preserving the existing math to make this PR easier to follow. Fixing it is a problem for issue #390
	charge += (uint32_t) (CHARGE_RESOLUTION_SCALE * 100 * ticksElapsed * milliamps 
							/ clockFrequency / 1000 / 60 / 60 / 60 / MAX_CHARGE_AMP_HRS);
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
 * @return fixed point percentage. Resolution = 0.01 (45.55% = 4555)
 */
uint32_t Charge_GetPercent(void){
	return charge;
}

/** Charge_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator value in percent of total charge,
 *                    with a resolution = 0.01%
 */
void Charge_SetAccum(int32_t accum){
	charge = accum;
}

