//Bootstrap board: charger.c
//Fred Engelkemeir, Fall 2012
//functions that relate to the contorl of the BQ2004 battery charger modules

//for all functions that take an (unsigned char) charger input, it is 0 for the bottom charger and 1 for the top charger

#include <p30F6012A.h>

unsigned char charger_VsupGood(unsigned char charger);
unsigned char charger_getStatus(unsigned char charger);
unsigned char charger_enable(unsigned char charger);

//returns 1 if charger has power, 0 otherwise
//based on the Vsup reading
unsigned char charger_VsupGood(unsigned char charger)
{
	return 0;
}	

//returns the status of the charger
//0 = charger off (or can't run due to some problem)
//1 = battery absent (charger can't see the battery)
//2 = charge pending (charger is checking battery before starting a fast charge cycle)
//3 = fast charge
//4 = charge complete (top off or trickle charge may be happening)
unsigned char charger_getStatus(unsigned char charger)
{
	unsigned char red;
	unsigned char green;
	if(charger)
	{						//top charger
		red   = misc_isTopLED2();
		green = misc_isTopLED1();
		if(!misc_isTopEn())					//TODO: also check the Vsup!
			return 0;
	}
	else
	{						//bottom charger
		red   = misc_isBotLED2();
		green = misc_isBotLED1();
		if(!misc_isBotEn())					//TODO: also check the Vsup!
			return 0;
	}

	if((green==0)&&(red==0))
		return 1;
	if((green==1)&&(red==1))
		return 2;
	if((green==0)&&(red==1))
		return 3;
	if((green==1)&&(red==0))
		return 4;
}	

//turn on the associated charger
//returns 1 if sucessful, 0 if fault
//checks for propper Vsup at the charger first
unsigned char charger_enable(unsigned char charger)
{
	//TODO: check for propper VSUP! and return 0 if error
	if(charger)
		misc_setTopEn(1);
	else
		misc_setBotEn(1);
	return 1;
}	

//fucking newline

