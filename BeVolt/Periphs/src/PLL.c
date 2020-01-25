#include "stm32f4xx.h"
#include "LED.h"

// PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N 
#define PLL_M 8
#define PLL_N 160

// SYSCLK = PLL_VCO / PLL_P 
#define PLL_P 2

// USB OTG FS, SDIO and RNG Clock = PLL_VCO / PLLQ 
#define PLL_Q 5


/** PLL_Init80MHz
 * Sets PLL to be 80MHz. That means each cycle is being executed at 80MHz (System clock)
 * f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
 * f(PLL general clock output) = f(VCO clock) / PLLP
 */
void PLL_Init80MHz(void){
	
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
		LED_Init();
		LED_On(EXTRA);
		while(1);	// Spin for error
	}
}
