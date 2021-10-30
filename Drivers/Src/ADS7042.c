#include "ADS7042.h"
#include "BSP_SPI.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_PLL.h"

// Helper functions copied from LTC681x.c

static void cs_set(uint8_t state){
	BSP_SPI_SetStateCS(spi_ads7042, state);
}

static void ADS7042_delay_u(uint16_t micro)
{
  uint32_t delay = BSP_PLL_GetSystemClock() / 1000000;
	for(uint32_t i = 0; i < micro; i++)
	{
		for(uint32_t j = 0; j < delay; j++);
	}
}

//Generic wakeup command to wake the LTC6813 from sleep
static void ADS7042_wakeup_sleep(uint8_t total_ic)
{
  for (int i =0; i<total_ic; i++)
  {
    cs_set(0);
    ADS7042_delay_u(500); // Guarantees the LTC6813 will be in standby
    cs_set(1);
    ADS7042_delay_u(150);
  }
}

/* Initialize communication ADS7042
 * and run startup offset calibration
 */
void ADS7042_Init(bsp_os_t spi_os) {
    BSP_SPI_Init(spi_ads7042, &spi_os);

    // 2-byte read on startup for offset calibration
    ADS7042_Read();
}

/* Read value from ADS7042
 * ADS7042 outputs 2 bits of 0 + 12 data bits + 2 bits of 0
 * This function returns just the data bits
 * @param data pointer to data buffer
 * @return SUCCESS or ERROR
 */
uint16_t ADS7042_Read() {
    uint8_t rxdata[2];

    ADS7042_wakeup_sleep(1);

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
    const int32_t NUMERATOR = 7324;
    const int32_t DENOMINATOR = 100;
    int32_t milliamps = -1 * ((reading - OFFSET) * NUMERATOR) / DENOMINATOR;    // during testing, the gain seems to be inverted
    return milliamps;
}