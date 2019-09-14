/** Display.c
 * Display driver for e-ink display that will be placed on the dashboard. This is independent of the dashboard
 * since the whole electrical system turns off except the BPS during faults
 * @authors Sijin Woo, Manolo Alvarez
 * @lastRevised 2/23/2019
 */
 
// Display: Pervasive Displays e-ink 2.66"
// http://www.pervasivedisplays.com/products/266 
// Uses SPI

#include <stdint.h>
#include "app.h"
#include "stm32f4xx_hal.h"
#include "imagedata.h"
#include "epdpaint.h"
#include "fonts.h"

#define DISPLAY_WIDTH		((uint32_t)296)		// E-Ink Display resolution width
#define DISPLAY_HEIGHT		((uint32_t)152)		// E-Ink Display resolution height
#define BACKGROUND_WIDTH	((uint32_t)296)		// background image image
#define BACKGROUND_HEIGHT	((uint32_t)152)		// background image height
#define STOPSIGN_WIDTH		((uint32_t)55)		// Stop Sign image width
#define STOPSIGN_HEIGHT		((uint32_t)56)		// Stop Sign image height

typedef struct State{

	uint16_t voltage;
	uint16_t current;
	uint16_t stateofCharge;
	uint16_t temperature;
	uint16_t status;
	
} State;

void Display_Init(Paint *Display_Background);
void Display_Voltage(State);
void Display_Current(State);
void Display_SOC(State);
void Display_Temperature(State);
void Display_Status(State);

void App_Handler(void){

	State s;
	Paint *background;
	Paint *stopSign;

	s.current = 7;
	s.stateofCharge = 7;
	s.voltage = 7;
	s.temperature = 7;

	//Display_Init(Display_Background);	
	Paint_Init(background, Display_Background, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
	Paint_Init(stopSign, Display_StopSign, STOPSIGN_WIDTH, STOPSIGN_HEIGHT);

	Paint_DrawCharAt(background, 10, 62, (char) s.current, &Font8, 0); // Draw amount of current in mA. On top of background image. start at pixel 10x62, character to draw, font8, not inverted
	//Paint_DrawCharAt(background, 20, 136, (char) s.status, Font8, 0) // Draw amount of current in mA. On top of background image. start at pixel 10x62, character to draw, font8, not inverted
	Paint_DrawCharAt(background, 167, 62, (char) s.stateofCharge, &Font8, 0); // Draw amount of current in mA. On top of background image. start at pixel 10x62, character to draw, font8, not inverted
	Paint_DrawCharAt(background, 180, 106, (char) s.voltage, &Font8, 0); // Draw amount of current in mA. On top of background image. start at pixel 10x62, character to draw, font8, not inverted
	Paint_DrawCharAt(background, 183, 146, (char) s.temperature, &Font8, 0); // Draw amount of current in mA. On top of background image. start at pixel 10x62, character to draw, font8, not inverted

}