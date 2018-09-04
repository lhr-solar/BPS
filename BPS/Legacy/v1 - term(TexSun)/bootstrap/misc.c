/**********************************************
/Misc digital IO for the bootstrap battery slave board
/Summer/fall 2012
/Fred Engelkemeir
/
/
***********************************************/


#include <p30F6012A.h>


unsigned char misc_readID(void);
unsigned char misc_isCalUnlocked(void);
unsigned char misc_isDebug(void);
unsigned char misc_isSetupUnlocked(void);
void misc_setLEDread(unsigned char status);
void misc_setLEDfull(unsigned char status);
void misc_setLEDwarn(unsigned char status);
void misc_setLEDbus(unsigned char status);
void misc_setLEDcharge(unsigned char status);
void misc_setLEDfault(unsigned char status);
void misc_setTopEn(unsigned char status);
void misc_setBotEn(unsigned char status);
unsigned char misc_isTopEn(void);
unsigned char misc_isBotEn(void);
unsigned char misc_isBotLED1(void);
unsigned char misc_isBotLED2(void);
unsigned char misc_isTopLED1(void);
unsigned char misc_isTopLED2(void);
unsigned char misc_isBOK(void);
void misc_initialize(void);



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
	TRISGbits.TRISG6 = 1;
	TRISGbits.TRISG7 = 1;
	TRISGbits.TRISG15 = 1;
	TRISCbits.TRISC1 = 1;
	//set outputs
	TRISGbits.TRISG0 = 0;
	TRISGbits.TRISG1 = 0;
	TRISFbits.TRISF1 = 0;
	TRISGbits.TRISG14 = 0;
	TRISGbits.TRISG13 = 0;
	TRISGbits.TRISG12 = 0;
	TRISCbits.TRISC2 = 0;
	TRISGbits.TRISG8 = 0;
}

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
/misc_setLEDread()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG0
***********************************************/
void misc_setLEDread(unsigned char status)
{
	if(status)
		PORTGbits.RG0 = 1;
	else
		PORTGbits.RG0 = 0;
}

/***********************************************
/misc_setLEDfull()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG1
***********************************************/
void misc_setLEDfull(unsigned char status)
{
	if(status)
		PORTGbits.RG1 = 1;
	else
		PORTGbits.RG1 = 0;
}

/***********************************************
/misc_setLEDwarn()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RF1
***********************************************/
void misc_setLEDwarn(unsigned char status)
{
	if(status)
		PORTFbits.RF1 = 1;
	else
		PORTFbits.RF1 = 0;
}

/***********************************************
/misc_setLEDbus()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG14
***********************************************/
void misc_setLEDbus(unsigned char status)
{
	if(status)
		PORTGbits.RG14 = 1;
	else
		PORTGbits.RG14 = 0;
}

/***********************************************
/misc_setLEDcharge()
/if parameter is a 1, the LED lights up
/if parameter is a 0, the LED is dark
/LED connected (positive logic) to pin
/RG13
***********************************************/
void misc_setLEDcharge(unsigned char status)
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

/***********************************************
/misc_setTopEn()
/if parameter is a 1, the charger is enabled
/if parameter is a 0, the charger is disabled
***********************************************/
void misc_setTopEn(unsigned char status)
{
	if(status)
		PORTCbits.RC2 = 1;
	else
		PORTCbits.RC2 = 0;
}

/***********************************************
/misc_setBotEn()
/if parameter is a 1, charger is enabled
/if parameter is a 0, the charger is off
***********************************************/
void misc_setBotEn(unsigned char status)
{
	if(status)
		PORTGbits.RG8 = 1;
	else
		PORTGbits.RG8 = 0;
}

/***********************************************
//reads back from the enable pins
//returns a 1 if the charger is enabled
//returns a 0 if the charger is disabled
***********************************************/
unsigned char misc_isBotEn(void)
{
	return PORTGbits.RG8;
}

/***********************************************
//reads back from the enable pins
//returns a 1 if the charger is enabled
//returns a 0 if the charger is disabled
***********************************************/
unsigned char misc_isTopEn(void)
{
	return PORTCbits.RC2;
}

/***********************************************
//returns a 1 if the charger LED is on
//returns a 0 if the charger LED is off
***********************************************/
unsigned char misc_isBotLED1(void)
{
	//if(PORTGbits.RG6)
	if(PORTG&0x0040)
		return 0;
	else
		return 1;
}


/***********************************************
//returns a 1 if the charger LED is on
//returns a 0 if the charger LED is off
***********************************************/
unsigned char misc_isBotLED2(void)
{
	//if(PORTGbits.RG7)
	if(PORTG&0x0080)
		return 0;
	else
		return 1;
}

/***********************************************
//returns a 1 if the charger LED is on
//returns a 0 if the charger LED is off
***********************************************/
unsigned char misc_isTopLED1(void)
{
	//if(PORTGbits.RG15)
	if(PORTG&0x8000)
		return 0;
	else
		return 1;
}

/***********************************************
//returns a 1 if the charger LED is on
//returns a 0 if the charger LED is off
***********************************************/
unsigned char misc_isTopLED2(void)
{
	//if(PORTCbits.RC1)
	if(PORTC&0x0002)
		return 0;
	else
		return 1;
}

/***********************************************
//returns a 1 if the BusOK signal is present
//returns a 0 if the BusOK signal isn't there
//only works if the BusOK signal is connected back to the bootstrap power board
***********************************************/
unsigned char misc_isBOK(void)
{
	return PORTGbits.RG9;
}



//mind the newline

