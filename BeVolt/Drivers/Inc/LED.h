/** LED.c
 * LED driver.
 */
#ifndef LED_H
#define LED_H

typedef enum led{FAULT, RUN, UVOLT, OVOLT, OTEMP, OCURR, WDOG, CAN, EXTRA} led;

void LED_Init(void);

void LED_Toggle(led signal);

void LED_On(led signal);

void LED_Off(led signal);

#endif
