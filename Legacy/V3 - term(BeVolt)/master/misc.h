#ifndef MISC_H
#define MISC_H

void misc_initialize(void);

//read from the DIPswitches (for protection EEPROM registers, etc.)
unsigned char misc_isSetupUnlocked(void);
unsigned char misc_isCalUnlocked(void);
unsigned char misc_isDebug(void);
unsigned char misc_isThreshUnlocked(void);

//set the LEDs on the board
void misc_setLEDFault(unsigned char status);
void misc_setLEDConfig(unsigned char status);
void misc_setLEDBalance(unsigned char status);
void misc_setLEDDebug(unsigned char status);
void misc_setLEDFault(unsigned char status);

//discrete outputs for dashboard LEDs, etc.
void misc_setOutTrip(unsigned char status);
void misc_setOutWarn(unsigned char status);
void misc_setOutBPSON(unsigned char status);
unsigned char misc_isIgnition(void);

//contactor interface stuff
unsigned char misc_MainContactorCheck(void);
unsigned char misc_PreContactorCheck(void);
void misc_setMainContactor(unsigned char status);
void misc_setAuxContactor(unsigned char status);
void misc_setPreContactor(unsigned char status);
void misc_setWDT(unsigned char status);
unsigned char misc_WDTOK(void);

//diagnostic readback of outputs
unsigned char misc_readbackLEDDebug(void);
unsigned char misc_readbackLEDSetup(void);
unsigned char misc_readbackLEDThresh(void);
unsigned char misc_readbackLEDCal(void);
unsigned char misc_readbackLEDFault(void);
unsigned char misc_readbackOutTrip(void);
unsigned char misc_readbackOutWarn(void);
unsigned char misc_readbackOutBPSON(void);
unsigned char misc_readbackMainContactor(void);
unsigned char misc_readbackAuxContactor(void);
unsigned char misc_readbackPreContactor(void);
unsigned char misc_readbackWDTfeed(void);

#endif
