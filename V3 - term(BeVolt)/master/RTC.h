#ifndef RTC_H
#define RTC_H

#include "time.h"
void RTC_setTime(time newtime);
time RTC_getTime(void);
time RTC_getTimeStampUp(void);
time RTC_getTimeStampDown(void);
void RTC_writeSRAM(unsigned char address, unsigned char data);
unsigned char RTC_readSRAM(unsigned char address);

#endif
