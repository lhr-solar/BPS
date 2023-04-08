/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_PLL.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "Tasks.h"

// PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N 
#define PLL_M 8
#define PLL_N 160

// SYSCLK = PLL_VCO / PLL_P 
#define PLL_P 2

// USB OTG FS, SDIO and RNG Clock = PLL_VCO / PLLQ 
#define PLL_Q 5

// Number of times to retry enabling the PLL
// Will fault the BPS after this number of failures
#define PLL_ATTEMPTS_MAX 10

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void) {
    RCC_HSEConfig(RCC_HSE_ON);
	
	ErrorStatus status = RCC_WaitForHSEStartUp();
	uint8_t pllAttempts = 0;

	// If PLL fails to engage in time retry PLL_ATTEMPTS_MAX amount of times
	// If still fail then we will fault
	while (status == ERROR) {
		if (pllAttempts >= PLL_ATTEMPTS_MAX) {
			Fault_BitMap |= Fault_CRC; // Could probably use a better fault choice than CRC
			EnterFaultState();
		}
		pllAttempts++;
		status = RCC_WaitForHSEStartUp();
	}

	// Flash 3 wait state, prefetch buffer and cache ON
	FLASH_SetLatency(FLASH_Latency_3);
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_InstructionCacheCmd(ENABLE);
	FLASH_DataCacheCmd(ENABLE);
		
	// HCLK = SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
	// PCLK2 = HCLK/2
	RCC_PCLK2Config(RCC_HCLK_Div2);
		
	// PCLK1 = HCLK/4
	RCC_PCLK1Config(RCC_HCLK_Div4);
		
	// Configure the main PLL clock to 80 MHz
	RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q, 2);	// 2 is arbitrary number
		
	// Enable the main PLL
	RCC_PLLCmd(ENABLE);
		
	// Wait till the main PLL is ready
	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
	// Select the main PLL as system clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
	// Update the system clock variable
	SystemCoreClockUpdate();
	
}

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void) {
    return SystemCoreClock;    // PLL set to 80MHz
}

/**
 * @brief   Delays in param micro seconds
 * @param   # of microseconds to delay
 * @return  None
*/
void BSP_PLL_DelayU(uint32_t micro)
{
    uint32_t delay = BSP_PLL_GetSystemClock() / 1000000;
	for(volatile uint32_t i = 0; i < micro; i++)
	{
		for(volatile uint32_t j = 0; j < delay; j++);
	}
}

/**
 * @brief   Delays in param milli seconds
 * @param   # of milliseconds to delay
 * @return  None
*/
void BSP_PLL_DelayM(uint32_t milli)
{
  uint32_t delay = BSP_PLL_GetSystemClock() / 1000;
	for(volatile uint32_t i = 0; i < milli; i++)
	{
		for(volatile uint32_t j = 0; j < delay; j++);
	}
}
