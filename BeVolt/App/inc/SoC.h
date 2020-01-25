/** SoC.c
 * State of Charge algorithm.
 */

#ifndef SOC_H
#define SOC_H

#include <stdint.h>
#include "config.h"
#include "stm32f4xx.h"
#include "ADC.h"
#include "EEPROM.h"

/** Soc_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 */
void SoC_Init(void);

/** SoC_Calculate
 * Calculates new SoC depending on the current reading
 * @param current reading from hall effect sensors. Fixed point of 0.0001 (1.500 Amps = 15000)
 */
void SoC_Calculate(int16_t amps);

/** SoC_Calibrate
 * Calibrates the SoC. Whenever the BPS trips, the SoC should recalibrate. If an undervoltage
 * fault occurs, the SoC calibrates to 0% and whenever there is an overvoltage, the SoC calibrates
 * to 100%.
 * @param voltage fault type. 0 if under voltage, 1 if over voltage
 */
void SoC_Calibrate(int8_t faultType);

/** SoC_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.01 (45.55% = 4555)
 */
uint32_t SoC_GetPercent(void);

/** SoC_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator new value. Resolution = 0.0001.
 */
void SoC_SetAccum(int32_t accum);

#endif
