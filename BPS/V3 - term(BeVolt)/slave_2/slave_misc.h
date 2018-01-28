#ifndef SLAVE_MISC_H
#define SLAVE_MISC_H

void misc_initialize(void);
unsigned char misc_readID(void);
unsigned char misc_isCalUnlocked(void);
unsigned char misc_isDebug(void);
unsigned char misc_isSetupUnlocked(void);
void misc_setLEDA(unsigned char status);
void misc_setLEDB(unsigned char status);
void misc_setLEDC(unsigned char status);
void misc_setLEDread(unsigned char status);
void misc_setLEDfault(unsigned char status);
void misc_setLEDbalance(unsigned char status);




#endif
