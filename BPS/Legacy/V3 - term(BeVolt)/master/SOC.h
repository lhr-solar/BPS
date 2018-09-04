#ifndef SOC_H
#define SOC_H

void SOC_initialize(void);
void SOC_update(void);
unsigned char SOC_getPercent(unsigned long int acc);
unsigned long int SOC_percent2acc(unsigned char in);

#endif

