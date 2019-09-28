#include <Contactor.h>
#include "stm32f4xx.h"

void StrobeLight_Init(void);
//Initializes variable GPIO_Init at Pin 12
void StrobeLight_On(void);
// Turns strobelight on
void StrobeLight_Off(void);
//turns strobelight off
uint32_t StrobeLight(void);
//returns value of pin 12 as 0 or 1 if its off or on