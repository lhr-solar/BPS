#include "stm32f4xx.h"

#define RCC_PLLM_MASK    ((uint32_t)0x0000003F)
#define RCC_PLLM_POS     0
#define RCC_PLLN_MASK    ((uint32_t)0x00007FC0)
#define RCC_PLLN_POS     6
#define RCC_PLLP_MASK    ((uint32_t)0x00030000)
#define RCC_PLLP_POS     16
#define RCC_PLLQ_MASK    ((uint32_t)0x0F000000)
#define RCC_PLLQ_POS     24
#define RCC_PLLR_MASK    ((uint32_t)0x70000000)
#define RCC_PLLR_POS     28

/** PLL_Init80MHz
 * Sets PLL to be 80MHz. That means each cycle is being executed at 80MHz (System clock)
 * f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
 * f(PLL general clock output) = f(VCO clock) / PLLP
 */
void PLL_Init80MHz(void){
	
	// Enable HSE clock
	RCC->CR |= RCC_CR_HSEON;
	
	// Wait until HSE stabilized
	while((RCC->CR & RCC_CR_HSERDY) == 0);
	
	// Select HSE clock as main clock
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_HSE;
	
	// Disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	
	// Configure to be 80MHz
	// PLLM
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLM_MASK) | ((10 << RCC_PLLM_POS) & RCC_PLLM_MASK);
	
	// PLLN
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLN_MASK) | ((200 << RCC_PLLN_POS) & RCC_PLLN_MASK);
	
	// PLLP
	RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLP_MASK) | ((((2 >> 1) - 1) << RCC_PLLP_POS) & RCC_PLLP_MASK);
	
	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	
	// Wait until PLL stabilized
	while((RCC->CR & RCC_CR_PLLRDY) == 0);
	
	// Enable PLL as main clock
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_SW)) | RCC_CFGR_SW_PLL;
	
	SystemCoreClockUpdate();
}

