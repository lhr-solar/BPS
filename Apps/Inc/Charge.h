/** Charge.c
 * State of Charge algorithm.
 */

#ifndef CHARGE_H
#define CHARGE_H

#include <stdint.h>
#include "config.h"
#include "EEPROM.h"

/** Charge_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 */
void Charge_Init(void);

/** Charge_Calculate
 * Calculates new Charge depending on the current reading
 * @param current reading from hall effect sensors. Fixed point of 0.0001 (1.500 Amps = 15000)
 */
void Charge_Calculate(int32_t milliamps);

/** Charge_Calibrate
 * Calibrates the Charge. Whenever the BPS trips, the Charge should recalibrate. If an undervoltage
 * fault occurs, the Charge calibrates to 0% and whenever there is an overvoltage, the Charge calibrates
 * to 100%.
 * @param voltage fault type. 0 if under voltage, 1 if over voltage
 */
void Charge_Calibrate(int8_t faultType);

/** Charge_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.01 (45.55% = 4555)
 */
uint32_t Charge_GetPercent(void);

/** Charge_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator new value. Resolution = 0.0001.
 */
void Charge_SetAccum(int32_t accum);

#endif
