/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** main.c
 * Program for UTSVT BeVolt's Battery Protection System State of Charge
 */
#include "Charge.h"
#include "BSP_Timer.h"
#include "VoltageToSoC.h"
#include "Voltage.h"
#include "config.h"

#define CHARGE_RESOLUTION_SCALE (int64_t)1e8   // scale to 0.000001%
#define MAX_CHARGE_uAh          (OCV_SOC_TOTAL_CAP_uAh * MODULE_CELLS_IN_PARALLEL)

static int64_t total_charge_uah = 0;
static uint32_t charge_micropercent = 0;        // units of 0.000001% -- e.g. (45,550,000 = 45.55%)

// helpful constants
static const int64_t MICROSECONDS_PER_SECOND = (int64_t)1e6;
static const int64_t SECONDS_PER_HOUR = 3600;


static uint32_t cell_mv_to_charge_uah(uint32_t mv) {
    if (mv < OCV_SOC_LUT_MIN_MV) {
        return 0;
    } else if (mv >= OCV_SOC_LUT_MAX_MV) {
        return lg21700m50t_ocv_soc_lut_uAh[OCV_SOC_LUT_SIZE - 1];
    }
    
    // linear interpolate an approx charge
    uint32_t idx_lo = (mv - OCV_SOC_LUT_MIN_MV) / OCV_SOC_LUT_STRIDE;
    uint32_t idx_hi = idx_lo + 1;
    uint32_t weight_hi = mv % OCV_SOC_LUT_STRIDE;    // min_mv is multiple of lut_stride
    uint32_t weight_lo = OCV_SOC_LUT_STRIDE - weight_hi;

    return (lg21700m50t_ocv_soc_lut_uAh[idx_lo] * weight_lo + 
            lg21700m50t_ocv_soc_lut_uAh[idx_hi] * weight_hi) / (OCV_SOC_LUT_STRIDE);
}

/** Charge_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm. Voltage_UpdateMeasurements() must be called before this.
 * Info found on reference sheet stm32f413 page 535
 */
void Charge_Init(void) {
    uint32_t avg_module_mv = Voltage_GetTotalPackVoltage() / NUM_BATTERY_MODULES;
    total_charge_uah = ((int64_t)cell_mv_to_charge_uah(avg_module_mv)) * MODULE_CELLS_IN_PARALLEL;

    BSP_Timer_Start_TickCounter();
}

/** Charge_Calculate
 * Calculates new Charge depending on the current reading
 * @param milliamps reading from current sensors. Fixed point of 0.001 (1.500 Amps = 1500)
 * not a constant sampling. Add on however much from the previous
 */
void Charge_Calculate(int32_t milliamps){ 
    /* Update Charge, units of 0.000001% . 100,000,000 is charge at 100% */
    
    int64_t micro_sec = (int64_t)BSP_Timer_TicksToMicros(BSP_Timer_GetTicksElapsed());
    static int64_t prev_ma = 0;

    // perform a trapezoidal integration
    total_charge_uah -= (micro_sec * 1000 * ((int64_t)milliamps + prev_ma))
                      / (2 * MICROSECONDS_PER_SECOND * SECONDS_PER_HOUR);
    prev_ma = milliamps;

    // update percent
    charge_micropercent = (uint32_t)((total_charge_uah * CHARGE_RESOLUTION_SCALE) / MAX_CHARGE_uAh);
}

/** Charge_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.000001% (45,550,000 = 45.55%)
 */
uint32_t Charge_GetPercent(void){
    return charge_micropercent;
}

