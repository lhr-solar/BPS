/** main.c
 * Program for UTSVT BeVolt's Battery Protection System SOC
 */
#include "SoC.h"
#include "BSP_Timer.h"

#define MAX_CHARGE 1000*1000																								// In amp-hours (Ah), for now it is a dummy value
uint32_t fixedPoint_SoC;																										// % of how much charge is left in battery with .01 resolution
float float_SoC;																														// float vers of SoC

/** Soc_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 * Info found on reference sheet stm32f413 page 535
 */
void SoC_Init(void){ 
	BSP_Timer_Init();
	
	// Grab from EEPROM what is the current SoC
	EEPROM_ReadMultipleBytes(EEPROM_SOC_PTR_LOC, 4, (uint8_t*)&fixedPoint_SoC);
	float_SoC = fixedPoint_SoC * .01;

    BSP_Timer_Start();
}

/** SoC_Calculate
 * Calculates new SoC depending on the current reading
 * @param current reading from hall effect sensors. Fixed point of 0.0001 (1.500 Amps = 15000)
 * not a constant samping. Add on however much from the previous
 */
void SoC_Calculate(int16_t amps){ 
	
	uint32_t counter = BSP_Timer_GetTicksElapsed();
	
	float timeElapsed = (0xFFFF - counter)/60;
	timeElapsed /= 60;
	timeElapsed /= BSP_Timer_GetRunFreq();  			// time in hours
	
	float float_amps = amps * .0001;					// Fixed point to floating point
	
	float AhCollected = timeElapsed * float_amps;
	
	/* Update SoC */
	float_SoC = float_SoC + (AhCollected * 100)/MAX_CHARGE;
	fixedPoint_SoC = float_SoC * 100;
}

/** SoC_Calibrate
 * Calibrates the SoC. Whenever the BPS trips, the SoC should recalibrate. If an undervoltage
 * fault occurs, the SoC calibrates to 0% and whenever there is an overvoltage, the SoC calibrates
 * to 100%.
 * @param voltage fault type. 0 if under voltage, 1 if over voltage
 */
void SoC_Calibrate(int8_t faultType){
	if (faultType == UNDERVOLTAGE) {
		fixedPoint_SoC = 0;
		float_SoC = 0;
	}
	else if (faultType == OVERVOLTAGE) {
		fixedPoint_SoC = 10000;
		float_SoC = 100.00;
	}
}

/** SoC_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.01 (45.55% = 4555)
 */
uint32_t SoC_GetPercent(void){
	return fixedPoint_SoC;
}

/** SoC_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator new value. Resolution = 0.0001.
 */
void SoC_SetAccum(int32_t accum){
	fixedPoint_SoC = accum/10; 
	float_SoC = accum/10000;
}

