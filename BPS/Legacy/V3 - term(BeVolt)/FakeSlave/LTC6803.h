#ifndef LTC6803_H
#define LTC6803_H


void LTC_readConfig(char *configs);
unsigned char LTC_setConfig(char CFG0, char CFG1, char CFG2, char CFG3, char CFG4, char CFG5);
void LTC_timerInitialize(void);
void LTC_startConversion(void);
void LTC_startOWConversion(void);
void LTC_startTimer(void);
void LTC_waitConversion(void);
char LTC_readVoltage(char *voltages);
void LTC_sortVoltages(char *input, unsigned int *output);
void LTC_scaleVoltages(unsigned int *voltages);
int LTC_OWcompare(unsigned int *voltages, unsigned int *OWvoltages);

#endif
