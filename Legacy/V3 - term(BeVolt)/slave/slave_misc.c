/*******************************************************
/slave_misc.c
/implementes the misc hardware stuff that didn't fit into the other drive files
/basically just functions to read the DIP switches and toggle the LEDs
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>
#include "slave_misc.h"


void misc_initialize(void)
{
	//I could have done this is fewer operations by binary OR'in TRISD, but whatever....
	//set inputs
	TRISDbits.TRISD2 = 1;
	TRISDbits.TRISD3 = 1;
	TRISDbits.TRISD4 = 1;
	TRISDbits.TRISD5 = 1;
	TRISDbits.TRISD6 = 1;
	TRISDbits.TRISD7 = 1;
	TRISFbits.TRISF0 = 1;
	//set outputs
	TRISGbits.TRISG0 = 0;
	TRISGbits.TRISG1 = 0;
	TRISFbits.TRISF1 = 0;
	TRISGbits.TRISG14 = 0;
	TRISGbits.TRISG13 = 0;
	TRISGbits.TRISG12 = 0;	
}

/***********************************************
/misc_readID()
/returns a # from 0-15, based on the setting of the dip switches
/ 1	RD2
/ 2	RD3
/ 4	RD4
/ 8	RD5
***********************************************/
unsigned char misc_readID(void)
{
	unsigned char ID;
	ID = 0;
	ID += PORTDbits.RD2;
	ID += 2*PORTDbits.RD3;
	ID += 4*PORTDbits.RD4;
	ID += 8*PORTDbits.RD5;
	return ID;
}

/***********************************************
/misc_isCalUnlocked()
/returns a # from 0-15, based on the setting of the dip switches
/updates to the GAIN/OFFSET analog calibration registers is not allowed when switch is OFF
/RD6
/when switch is ON, pin is at 5V, and returns 1
/when switch is OFF, pin is at 0V, and returns 0
***********************************************/
unsigned char misc_isCalUnlocked(void)
{
	return PORTDbits.RD6;
}

/***********************************************
/misc_isDebug()
/returns a # from 0-15, based on the setting of the dip switches
/special debug routines can be run when turned on 
/RF0
/when switch is ON, pin is at 5V, and returns 1
/when switch is OFF, pin is at 0V, and returns 0
***********************************************/
unsigned char misc_isDebug(void)
{
	return PORTFbits.RF0;
}

/***********************************************
/misc_isSetupUnlocked()
/returns a # from 0-15, based on the setting of the dip switches
/if the switch is ON, the configuration registers can be changed
/RD7
/when switch is ON, pin is at 5V, and returns 1
/when switch is OFF, pin is at 0V, and returns 0
***********************************************/
unsigned char misc_isSetupUnlocked(void)
{
	return PORTDbits.RD7;
}

/***********************************************
/misc_setLEDA()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG0
***********************************************/
void misc_setLEDA(unsigned char status)
{
	if(status)
		PORTGbits.RG0 = 1;
	else
		PORTGbits.RG0 = 0;
}

/***********************************************
/misc_setLEDB()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG1
***********************************************/
void misc_setLEDB(unsigned char status)
{
	if(status)
		PORTGbits.RG1 = 1;
	else
		PORTGbits.RG1 = 0;
}

/***********************************************
/misc_setLEDC()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RF1
***********************************************/
void misc_setLEDC(unsigned char status)
{
	if(status)
		PORTFbits.RF1 = 1;
	else
		PORTFbits.RF1 = 0;
}

/***********************************************
/misc_setLEDread()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG14
***********************************************/
void misc_setLEDread(unsigned char status)
{
	if(status)
		PORTGbits.RG14 = 1;
	else
		PORTGbits.RG14 = 0;
}

/***********************************************
/misc_setLEDbalance()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG13
***********************************************/
void misc_setLEDbalance(unsigned char status)
{
	if(status)
		PORTGbits.RG13 = 1;
	else
		PORTGbits.RG13 = 0;
}

/***********************************************
/misc_setLEDfault()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG12
***********************************************/
void misc_setLEDfault(unsigned char status)
{
	if(status)
		PORTGbits.RG12 = 1;
	else
		PORTGbits.RG12 = 0;
}

//mind the newline

