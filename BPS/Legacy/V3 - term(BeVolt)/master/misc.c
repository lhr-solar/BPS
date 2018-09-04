/*******************************************************
/misc.c (master)
/implementes the misc hardware stuff that didn't fit into the other drive files
/functions to read the DIP switches and toggle the LEDs
/also controls the contactor and the maser's simple digital IO
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>
#include "misc.h"

/*********************************************
/hardware ver 1.0 and 1.1
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
/RD7	setup DIP 		1 = on (unlocked) 
/RD6 	CAL DIP			1 = on (unlocked)
/RD5 	DEBUG DIP		1 = on (unlocked)
/RD4 	Thresh DIP		1 = on (unlocked) 
/RD8	Ignition 		1 = ignition on
/RG7	contactorCheck	1 = contactor NOT closed (0 = contacts closed)		//INVERTED FUNCTION
/RG8	DRV102command	1 = contactor commanded on
/RG13	FLAG			1 = DRV happy, 0 = DRV fault						//INVERTED FUNCTION
**********************************************/


/*********************************************
/hardware ver3
/=====outputs=====
/(LEDS on board only)
/RG0	ThresholdLED	1 = on  
/RG1	DebugLED		1 = on	
/RG12	SetupLED		1 = on  
/RG13	CalibrateLED	1 = on  
/RG6	FAULT LED		1 = on  
/(discrete outputs)
/RD11	TRIP out		1 = on
/RD10	WARN out		1 = on
/RD9	BPSON			1 = on	(contactor out put signal, not actual contactor control line)
/(contactor control stuff)
/RG15	mainContCMD		1 = on
/RC1	preContCMD		1 = on
/RC2	auxContCMD		1 = on
/RG14	watchdogFeed	1 = on
/=====inputs=====
/RD7	setup DIP 		1 = on (unlocked) 
/RD6 	CAL DIP			1 = on (unlocked)
/RD5 	DEBUG DIP		1 = on (unlocked)
/RD4 	Thresh DIP		1 = on (unlocked) 
/RD8	Ignition 		1 = ignition on
/RG7	MainContCheck	1 = contactor NOT closed (0 = contacts closed)		//INVERTED FUNCTION
/RG9	PreContCheck	1 = contactor NOT closed (0 = contacts closed)		//INVERTED FUNCTION	
/RG8	WDT_FAIL		1 = WDT OK, 0 = WDT time out						//INVERTED FUNCTION
**********************************************/


//just sets direction bits for GPIO pins
void misc_initialize(void)
{	//set outputs
	TRISGbits.TRISG0 = 0;
	TRISGbits.TRISG1 = 0;
	TRISGbits.TRISG6 = 0;
	TRISGbits.TRISG12 = 0;
	TRISGbits.TRISG13 = 0;
	TRISGbits.TRISG14 = 0;
	TRISGbits.TRISG15 = 0;
	TRISDbits.TRISD9 = 0;
	TRISDbits.TRISD10 = 0;
	TRISDbits.TRISD11 = 0;
	TRISCbits.TRISC1 = 0;
	TRISCbits.TRISC2 = 0;

	//set inputs
	TRISDbits.TRISD4 = 1;
	TRISDbits.TRISD5 = 1;
	TRISDbits.TRISD6 = 1;
	TRISDbits.TRISD7 = 1;
	TRISDbits.TRISD8 = 1;
	TRISGbits.TRISG7 = 1;
	TRISGbits.TRISG9 = 1;
	TRISGbits.TRISG8 = 1;
}

unsigned char misc_isSetupUnlocked(void)
{
	return PORTDbits.RD7;
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
	return PORTDbits.RD4;
}

void misc_setLEDDebug(unsigned char status)
{
	if(status)
		PORTGbits.RG1 = 1;
	else
		PORTGbits.RG1 = 0;
}

void misc_setLEDSetup(unsigned char status)
{
	if(status)
		PORTGbits.RG12 = 1;
	else
		PORTGbits.RG12 = 0;
}

void misc_setLEDThresh(unsigned char status)
{
	if(status)
		PORTGbits.RG0 = 1;
	else
		PORTGbits.RG0 = 0;
}

void misc_setLEDCal(unsigned char status)
{
	if(status)
		PORTGbits.RG13 = 1;
	else
		PORTGbits.RG13 = 0;
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
	if(PORTDbits.RD8)
		return 1;
	else
		return 0;
}

//both of these return 1 for contactor shows closed, and 0 for contactor shows as open (or the contactor check contacts are not connected)
unsigned char misc_MainContactorCheck(void)
{
	if(PORTGbits.RG7)
		return 0;
	else
		return 1;
}

unsigned char misc_PreContactorCheck(void)
{
	if(PORTGbits.RG9)
		return 0;
	else
		return 1;
}

//returns 1 for WDT is happy, 0 for a failure
unsigned char misc_WDTOK(void)
{
	if(PORTGbits.RG8)
		return 0;
	else
		return 1;
}

void misc_setMainContactor(unsigned char status)
{
	if(status)
		PORTGbits.RG15 = 1;
	else
		PORTGbits.RG15 = 0;
}

void misc_setAuxContactor(unsigned char status)
{
	if(status)
		PORTCbits.RC2 = 1;
	else
		PORTCbits.RC2 = 0;
}

void misc_setPreContactor(unsigned char status)
{
	if(status)
		PORTCbits.RC1 = 1;
	else
		PORTCbits.RC1 = 0;
}

void misc_setWDT(unsigned char status)
{
	if(status)
		PORTGbits.RG14 = 1;
	else
		PORTGbits.RG14 = 0;
}

//these are used for debugging and checking (esp. in the termianl's RGPIO function)
//it just returns the status of the pin associated with each function
unsigned char misc_readbackLEDDebug(void)
{
	return PORTGbits.RG1;
}

unsigned char misc_readbackLEDSetup(void)
{
	return PORTGbits.RG12;
}

unsigned char misc_readbackLEDThresh(void)
{
	return PORTGbits.RG0;
}

unsigned char misc_readbackLEDCal(void)
{
	return PORTGbits.RG13;
}

unsigned char misc_readbackLEDFault(void)
{
	return PORTGbits.RG6;
}

unsigned char misc_readbackOutTrip(void)
{
	return PORTDbits.RD11;
}

unsigned char misc_readbackOutWarn(void)
{
	return PORTDbits.RD10;
}

unsigned char misc_readbackOutBPSON(void)
{
	return PORTDbits.RD9;
}

unsigned char misc_readbackMainContactor(void)
{
	return PORTGbits.RG15;
}

unsigned char misc_readbackAuxContactor(void)
{
	return PORTCbits.RC2;
}

unsigned char misc_readbackPreContactor(void)
{
	return PORTCbits.RC1;
}

unsigned char misc_readbackWDTfeed(void)
{
	return PORTGbits.RG14;
}


//newline

