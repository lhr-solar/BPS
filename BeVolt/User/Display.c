/** Display.c
 * Display driver for e-ink display that will be placed on the dashboard. This is independent of the dashboard
 * since the whole electrical system turns off except the BPS during faults
 * @authors Sijin Woo
 * @lastRevised 10/2/2018
 */
 
// Display: Pervasive Displays e-ink 2.66"
// http://www.pervasivedisplays.com/products/266 
// Uses SPI

#include <stdint.h>
#include "stm32f4xx.h"
#include "SPI.h"

/** Display_Init
 * Initializes and configures display module.
 */
void Display_Init(void){
	
}

/** Display_UpdateScreen
 * Updates the display screen to new measurements.
 */
void Display_Update(void){
	
}
