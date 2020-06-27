/*******************************************************
/misc.c (master)
/implementes the misc hardware stuff that didn't fit into the other drive files
/functions to read the DIP switches and toggle the LEDs
/also controls the contactor and the maser's simple digital IO
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>


unsigned char misc_isSetupUnlocked(void);
unsigned char misc_isCalUnlocked(void);
unsigned char misc_isDebug(void);
unsigned char misc_isThreshUnlocked(void);
void misc_setLEDError(unsigned char status);
void misc_setLEDConfig(unsigned char status);
void misc_setLEDBalance(unsigned char status);
void misc_setLEDDebug(unsigned char status);
void misc_setLEDFault(unsigned char status);
void misc_setOutTrip(unsigned char status);
void misc_setOutWarn(unsigned char status);
void misc_setOutBPSON(unsigned char status);
unsigned char misc_isIgnition(void);
unsigned char misc_ContactorCheck(void);
unsigned char misc_DRV102CMD(void);
unsigned char misc_DRV102Flag(void);
void misc_setContactor(unsigned char status);
void misc_setWDT(unsigned char status);
void misc_initialize(void);

/*********************************************
//1F schematic is wrong on some of these....
/=====outputs=====
/RG0	BalanceLED		1 = on  //F1
/RG1	DebugLED		1 = on	//F0
/RG3	ConfigLED		1 = on  //G1 = configUnlocked
/RG2	ERROR LED		1 = on  //G0
/RD11	TRIP out		1 = on
/RD10	WARN out		1 = on
/RD9	BPSON			1 = on	(contactor out put signal, not actual contactor control line)
/RG15	contactor ctrl	1 = on
/RG14	watchdogFeed	1 = on
/RG6	FAULT LED		1 = on
/=====inputs=====
/RD7	setup DIP 		1 = on (unlocked) //RD4
/RD6 	CAL DIP			1 = on (unlocked)
/RD5 	DEBUG DIP		1 = on (unlocked)
/RD4 	Thresh DIP		1 = on (unlocked) //RD7
/RD8	Ignition 		1 = ignition on
/RG7	contactorCheck	1 = contactor NOT closed (0 = contacts closed)		//INVERTED FUNCTION
/RG8	DRV102command	1 = contactor commanded on
/RG13	FLAG			1 = DRV happy, 0 = DRV fault						//INVERTED FUNCTION
**********************************************/


void misc_initialize(void)
{
	TRISGbits.TRISG0 = 0;
	TRISGbits.TRISG1 = 0;
	TRISGbits.TRISG6 = 0;
	TRISGbits.TRISG14 = 0;
	TRISGbits.TRISG15 = 0;
	TRISDbits.TRISD9 = 0;
	TRISDbits.TRISD10 = 0;
	TRISDbits.TRISD11 = 0;
	TRISFbits.TRISF0 = 0;
	TRISFbits.TRISF1 = 0;
	TRISDbits.TRISD4 = 1;
	TRISDbits.TRISD5 = 1;
	TRISDbits.TRISD6 = 1;
	TRISDbits.TRISD7 = 1;
	TRISDbits.TRISD8 = 1;
	TRISGbits.TRISG7 = 1;
	TRISGbits.TRISG8 = 1;
	TRISGbits.TRISG13 = 1;
}

unsigned char misc_isSetupUnlocked(void)
{
	return PORTDbits.RD4;
}

unsigned char misc_isCalUnlocked(void)
{
	return PORTDbits.RD6;
}

unsigned char misc_isDebug(void)
{
	return PORTDbits.RD5;
}

unsigned char misc_isThreshUnlocked(void)
{
	return PORTDbits.RD7;
}

void misc_setLEDError(unsigned char status)
{
	if(status)
		PORTGbits.RG0 = 1;
	else
		PORTGbits.RG0 = 0;
}

void misc_setLEDConfig(unsigned char status)
{
	if(status)
		PORTGbits.RG1 = 1;
	else
		PORTGbits.RG1 = 0;
}

void misc_setLEDBalance(unsigned char status)
{
	if(status)
		PORTFbits.RF1 = 1;
	else
		PORTFbits.RF1 = 0;
}

void misc_setLEDDebug(unsigned char status)
{
	if(status)
		PORTFbits.RF0 = 1;
	else
		PORTFbits.RF0 = 0;
}

void misc_setLEDFault(unsigned char status)
{
	if(status)
		PORTGbits.RG6 = 1;
	else
		PORTGbits.RG6 = 0;
}

void misc_setOutTrip(unsigned char status)
{
	if(status)
		PORTDbits.RD11 = 1;
	else
		PORTDbits.RD11 = 0;
}

void misc_setOutWarn(unsigned char status)
{
	if(status)
		PORTDbits.RD10 = 1;
	else
		PORTDbits.RD10 = 0;
}

void misc_setOutBPSON(unsigned char status)
{
	if(status)
		PORTDbits.RD9 = 1;
	else
		PORTDbits.RD9 = 0;
}

unsigned char misc_isIgnition(void)
{
	return PORTDbits.RD8;
}

unsigned char misc_ContactorCheck(void)
{
	if(PORTGbits.RG7)
		return 0;
	else
		return 1;
}

unsigned char misc_DRV102CMD(void)
{
	return PORTGbits.RG8;
}

unsigned char misc_DRV102Flag(void)
{
	if(PORTGbits.RG13)
		return 0;
	else
		return 1;
}

void misc_setContactor(unsigned char status)
{
	if(status)
		PORTGbits.RG15 = 1;
	else
		PORTGbits.RG15 = 0;
}

void misc_setWDT(unsigned char status)
{
	if(status)
		PORTGbits.RG14 = 1;
	else
		PORTGbits.RG14 = 0;
}




//newline

