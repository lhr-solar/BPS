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
 * @param current reading from hall effect sensors. Fixed point of 0.001 (1.500 Amps = 1500)
 * not a constant samping. Add on however much from the previous
 */
void Charge_Calculate(int32_t milliamps){ 
	
	uint32_t counter = BSP_Timer_GetTicksElapsed();
	
	float timeElapsed = (0xFFFF - counter) / 60;
	timeElapsed /= BSP_Timer_GetRunFreq();  // Time in seconds
    timeElapsed /= 60;  			        // time in minutes
    timeElapsed /= 60;                      // time in hours
	
	float fAmps = ((float) milliamps) * 0.001;
	
	float accumulatedAmpHrs = timeElapsed * fAmps;
	
	/* Update Charge, units of 0.01% */
	charge += (uint32_t) (CHARGE_RESOLUTION_SCALE * (100 * (accumulatedAmpHrs / MAX_CHARGE_AMP_HRS)));
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

