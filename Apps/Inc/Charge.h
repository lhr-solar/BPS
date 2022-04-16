/* Copyright (c) 2020 UT Longhorn Racing Solar */

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
 * Info found on reference sheet stm32f413 page 535
 */
void Charge_Init(void);

/** Charge_Calculate
 * Calculates new Charge depending on the current reading
 * @param milliamps reading from current sensors. Fixed point of 0.001 (1.500 Amps = 1500)
 * not a constant samping. Add on however much from the previous
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
 * @return fixed point percentage. Resolution = 0.000001% (45,550,000 = 45.55%)
 */
uint32_t Charge_GetPercent(void);

/** Charge_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator value in percent of total charge,
 *                    with a resolution = 0.000001% (100,000,000 = 100%)
 */
void Charge_SetAccum(int32_t accum);

#endif
