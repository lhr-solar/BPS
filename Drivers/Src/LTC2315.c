#include "LTC2315.h"
#include "BSP_SPI.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_PLL.h"
#include <stdio.h>

// offset for current reading
// experimentally determined at startup through calibration
static int32_t offset;

// Helper functions copied from LTC681x.c

static void LTC2315_delay_u(uint16_t micro) {
    uint32_t delay = BSP_PLL_GetSystemClock() / 1000000;
	for(volatile uint32_t i = 0; i < micro; i++) {
		for(volatile uint32_t j = 0; j < delay; j++);
	}
}

/* Pulse CS
 * 2 pulses to go into nap mode,
 * 4 pulses to go into sleep mode.
 */
static void LTC2315_cs_pulse(){
    BSP_SPI_SetStateCS(spi_ltc2315, 0);
    LTC2315_delay_u(16);
    BSP_SPI_SetStateCS(spi_ltc2315, 1);
    LTC2315_delay_u(16);
}

/* Wake isoSPI from sleep
 */
static void LTC2315_wakeup_sleep()
{
    BSP_SPI_SetStateCS(spi_ltc2315, 0);
    LTC2315_delay_u(500); // Guarantees that isoSPI is awake
    BSP_SPI_SetStateCS(spi_ltc2315, 1);
    LTC2315_delay_u(150);
}

/* Initialize communication LTC2315
 */
void LTC2315_Init(bsp_os_t spi_os) {
    BSP_SPI_Init(spi_ltc2315, &spi_os, true);

    LTC2315_wakeup_sleep();
}

/* Enter sleep mode
 */
void LTC2315_Sleep() {
    for (uint8_t i = 0; i < 4; i++) {
        LTC2315_cs_pulse();
    }
}

/* Read value from LTC2315
 * LTC2315 outputs 1 bit of 0 + 12 data bits + 3 bits of 0
 * This function returns just the data bits
 * @return Raw 12-bit value read from the ADC
 */
uint16_t LTC2315_Read() {
    OS_ERR err;
    uint8_t rxdata[2] = {0xff, 0xff};

    LTC2315_wakeup_sleep();

    uint8_t count = 0;
    do {
    OSSchedLock(&err);
    assertOSError(err);

    BSP_SPI_SetStateCS(spi_ltc2315, 0);
    // Note: if this is ever changed to send more than 8bytes, we should make
    // sure BSP_SPI_Read() does not call the scheduler or the OS will error out
    // since the scheduler is locked
    BSP_SPI_Read(spi_ltc2315, rxdata, 2);
    BSP_SPI_SetStateCS(spi_ltc2315, 1);

    OSSchedUnlock(&err);
    assertOSError(err);

    if ((count > MAX_PEC_ERRORS) && (rxdata[0] == 0xff) && (rxdata[1] == 0xff)) {
      // trip BPS
      Fault_BitMap |= Fault_CRC;
      OSSemPost(&Fault_Sem4, OS_OPT_POST_1, &err);
      assertOSError(err);
    }

    ++count;
    } while ((rxdata[0] == 0xff) && (rxdata[1] == 0xff)); // sometimes rxdata is 0xffff. I think this is caused by getting interrupted in the middle of requesting a reading from the ADC

    return (((uint16_t) rxdata[0] << 5) | ((uint16_t) rxdata[1] >> 3)) & 0x0fff;  // pack reading into 12 bits
}

/* Gets value from LTC2315
 * @return current in milliamps
 */
#if 0
int32_t LTC2315_GetCurrent() {
    /**
     * The ADC reads: Gain * (Current * 100uOhm) + Ref
     * INA186A3 chip has Gain = 100, we are using Ref = 1.5V
     * With 12 bits and input range [0.0V, 3.0V),
     * precision is 0.73242mV and 73.242mA
     * At Current = 0A, ADC should read 1.5V => 0x800
     * 
     * Temperature effects are ignored (for now?)
     */
   
    int32_t reading = 0;
    for (int i = 0; i < 10; ++i) {
        reading += (int32_t)LTC2315_Read();
    }

    reading /= 10;

    const int32_t PRECISION_MICRO_AMPS = 73242;
    int32_t milliamps = ((reading - offset) * PRECISION_MICRO_AMPS) / 1000;    // during testing, the gain seems to be inverted
    return milliamps;
}
#endif

// hack for busted board at comp
int32_t LTC2315_GetCurrent() {
    /**
     * The ADC reads: Gain * (Current * 100uOhm) + Ref
     * INA186A3 chip has Gain = 100, we are using Ref = 1.5V
     * With 12 bits and input range [0.0V, 3.0V),
     * precision is 0.73242mV and 73.242mA
     * At Current = 0A, ADC should read 1.5V => 0x800
     * 
     * Temperature effects are ignored (for now?)
     */
   
    int32_t reading = 0;
    for (int i = 0; i < 10; ++i) {
        reading += (int32_t)LTC2315_Read();
    }

    reading /= 10;

    const int32_t PRECISION_MICRO_AMPS = 150000;
    // make sure to hook up the shunt so discharging will give us a lower voltage
    int32_t milliamps = ((offset - reading) * PRECISION_MICRO_AMPS) / 1000;
    return milliamps;
}


/**
 * @brief Calibrate the LTC2315 module. Assumes there is no current flowing
 *        through the contactor
 */
void LTC2315_Calibrate() {
    int32_t reading = 0;
    for (int i = 0; i < 10; ++i) {
        reading += (int32_t)LTC2315_Read();
    }

    reading /= 10;

    offset = reading;
}
