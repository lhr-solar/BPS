#include "ADS7042.h"
#include "BSP_SPI.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_PLL.h"

/* Delay Microseconds
 * @param micro Number of microseconds
 */
static void delay_u(uint16_t micro) {
    uint32_t microsec = BSP_PLL_GetSystemClock() / 1000000;
    for(volatile uint32_t i = 0; i < micro; i++) {
        for(volatile uint32_t j = 0; j < microsec; j++);
    }
}

// Guarantees isoSPI is in ready mode
static void ADS7042_Wakeup_Idle() {
    uint8_t rxdata[2];

    BSP_SPI_SetStateCS(spi_ads7042, 0);
    delay_u(5000);
    BSP_SPI_Read(spi_ads7042, rxdata, 2);
    BSP_SPI_SetStateCS(spi_ads7042, 1);
}

// Wake isoSPI from sleep
// Guarantees isoSPI is in standby
static void ADS7042_Wakeup_Sleep() {
    BSP_SPI_SetStateCS(spi_ads7042, 0);
    delay_u(500);
    BSP_SPI_SetStateCS(spi_ads7042, 1);
    delay_u(150);
}

/* Initialize communication ADS7042
 * and run startup offset calibration
 */
void ADS7042_Init(bsp_os_t spi_os) {
    BSP_SPI_Init(spi_ads7042, &spi_os);

    // Offset calibration is run when CS is held low for >14 cycles
    // Wakeup Sleep should be sufficient long
    ADS7042_Wakeup_Sleep();
}

/* Read value from ADS7042
 * ADS7042 outputs 2 bits of 0 + 12 data bits + 2 bits of 0
 * This function returns just the data bits
 * @param data pointer to data buffer
 * @return SUCCESS or ERROR
 */
uint16_t ADS7042_Read() {
    uint8_t rxdata[2];

    ADS7042_Wakeup_Idle();

    BSP_SPI_SetStateCS(spi_ads7042, 0);
    BSP_SPI_Read(spi_ads7042, rxdata, 2);
    BSP_SPI_SetStateCS(spi_ads7042, 1);

    return (((uint16_t)rxdata[0] & 0x3F) << 6) | (((uint16_t)rxdata[1] >> 2) & 0x3F);
}

/* Gets value from ADS7042
 * @return current in milliamps
 */
int32_t ADS7042_GetCurrent() {
    /**
     * The ADC reads: Gain * (Current * 100uOhm) + Ref
     * INA186A3 chip has Gain = 100, we are using Ref = 1.5V
     * With 12 bits and input range [0.0V, 3.0V (AVDD)],
     * precision is 0.732421875mV and 73.2421875mA
     * At Current = 0A, ADC should read 1.5V => 0x800
     * 
     * Temperature effects are ignored (for now?)
     */
    int32_t reading = (int32_t)ADS7042_Read();
    const int32_t OFFSET = 0x0800;
    const int32_t PRECISION_MICRO_AMPS = 73242;
    int32_t milliamps = -1 * ((reading - OFFSET) * PRECISION_MICRO_AMPS) / 1000;    // during testing, the gain seems to be inverted
    return milliamps;
}