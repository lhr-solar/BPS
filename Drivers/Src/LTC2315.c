#include "LTC2315.h"
#include "BSP_SPI.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_PLL.h"
#include <stdio.h>

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
    BSP_SPI_Init(spi_ltc2315, &spi_os);

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
 * LTC2315 outputs 2 bits of 0 + 12 data bits + 2 bits of 0
 * This function returns just the data bits
 * @param data pointer to data buffer
 * @return SUCCESS or ERROR
 */
uint16_t LTC2315_Read() {
    uint8_t rxdata[2];

    LTC2315_wakeup_sleep();

    BSP_SPI_SetStateCS(spi_ltc2315, 0);
    BSP_SPI_Read(spi_ltc2315, rxdata, 2);
    BSP_SPI_SetStateCS(spi_ltc2315, 1);
    // uint16_t reading = (((uint16_t) rxdata[0] << 5) | ((uint16_t) rxdata[1] >> 3)) & 0x0fff;  // pack reading into 12 bits
    // printf("raw ADC reading: %x\r\n", reading);

    return (((uint16_t) rxdata[0] << 5) | ((uint16_t) rxdata[1] >> 3)) & 0x0fff;  // pack reading into 12 bits
}

/*
static void sort(int32_t *arr, uint8_t length) {
    // insertion sort
    for (uint8_t i = 0; i < length; ++i) {
        int32_t min = arr[i];
	uint8_t minIdx = i;
        for (int32_t j = i; j < length; ++j) {
            if (arr[min] > arr[j]) {
                min = arr[j];
		minIdx = j;
	    }
	}
	arr[minIdx] = arr[i];
	arr[i]      = min;
    }
}
*/

/* Gets value from LTC2315
 * @return current in milliamps
 */
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

    // assume first reading is at 0 milliamps for calibration
    // should be a safe assumption, since contactor will be open
    
    static bool calibrated = false;
    static int32_t offset;
    /*
    static int32_t readings[20];

    // take 20 readings
    for (int i = 0; i < 20; ++i) {
        readings[i] = (int32_t)LTC2315_Read();
    }

    // find averaage of middle 10 readings
    int32_t reading = 0;
    sort(readings, 20);
    for (int i = 5; i < 15; ++i) {
        reading += readings[i];
    }
    */

    int32_t reading = 0;
    for (int i = 0; i < 10; ++i) {
        reading += (int32_t)LTC2315_Read();
    }

    reading /= 10;

    if (!calibrated) {
        calibrated = true;
        offset = reading;
    }
    // const int32_t OFFSET = 0x0800;
    const int32_t PRECISION_MICRO_AMPS = 73242;
    int32_t milliamps = -1 * ((reading - offset) * PRECISION_MICRO_AMPS) / 1000;    // during testing, the gain seems to be inverted
    return milliamps;
    // return reading;
}
