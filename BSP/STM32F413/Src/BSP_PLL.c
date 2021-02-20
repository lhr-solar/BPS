/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_PLL.h"
#include "stm32f4xx.h"

// PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N 
#define PLL_M 8
#define PLL_N 160

// SYSCLK = PLL_VCO / PLL_P 
#define PLL_P 2

// USB OTG FS, SDIO and RNG Clock = PLL_VCO / PLLQ 
#define PLL_Q 5

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void) {
    RCC_HSEConfig(RCC_HSE_ON);
	
	ErrorStatus status = RCC_WaitForHSEStartUp();

	if(status == SUCCESS){
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
		
	}else{
		while(1);	// Spin for error
	}
}

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void) {
    return SystemCoreClock;    // PLL set to 80MHz
}
