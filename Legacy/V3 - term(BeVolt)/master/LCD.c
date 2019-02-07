//drive the LCD serial display
//Fred Engelkemeir
//end of Summer 2014

#include "LCD.h"
#include "globals.h"
#include "LCDutil.h"
#include "RS422.h"
#include "Mregisters.h"

//global globals
unsigned char LCD_buffer[255];
unsigned char LCD_outLength;
unsigned char LCD_outPointer;
unsigned char LCD_TXreqeust;
unsigned char LCD_updateEnabled = 1; 	//used by the terminal to temporarily disable the LCD updates

//local globals
static unsigned char LCD_loopCtr;		//increments each iteration of the main loop (reset to 0 when LCD is updated)
static unsigned char LCD_period;		//how many main loop iterations per LCD update
										//0 = LCD stuff disabled
										//1 = update every main loop iteration
										//2 = update every other main loop iteration
										//...
										//255 = 255 main loop cycles per every LCD update
static unsigned long LCD_updateCtr = 0;	//number of times the LCD has been updated
//local functions
static void LCD_writeMsg(void);
								


static void LCD_writeMsg(void)
{
	LCD_outLength = 0;		//effectively/logically clears the LCD's output buffer
	
	LCD_appendChar(254);	//escape code meaning that next byte is a command
	LCD_appendChar(0x51);		//command to clear the LCD
	
	LCD_appendChar(254);	//escape code meaning that next byte is a command
	LCD_appendChar(0x46);	//move cursor to the first position
	
	LCD_appendPackVolt(totalVoltage);
	LCD_appendString("VOLT ");
	
	//now do the current
	if(arrayAgood)
	{
		LCD_appendPackCurrent(currentA);
	}
	else if (arrayBgood)
	{
		LCD_appendPackCurrent(currentB);
	}
	else
	{
		LCD_appendString("???.?");
	}
	LCD_appendString("AMPS");

	//second line
	LCD_appendChar(254);	//escape code meaning that next byte is a command
	LCD_appendChar(0x45);	//move cursor to the given position
	LCD_appendChar(0x40);	//move cursor to the first position of row 2


	LCD_appendString("LOW:");
	LCD_appendCellVolt(minVoltage);
	LCD_appendString("V HIGH:");
	LCD_appendCellVolt(maxVoltage);
	LCD_appendChar('V');

	//third line
	LCD_appendChar(254);	//escape code meaning that next byte is a command
	LCD_appendChar(0x45);	//move cursor to the given position
	LCD_appendChar(0x14);	//move cursor to the first position of row 3
	LCD_appendString("HiTemp:");
	LCD_appendCellTemp(maxTemp);
	LCD_appendString("C ");
	
	//status
	if(BPSstate == 2)
	{
		if(BPSsubState == 4)
			LCD_appendString("BATT ON");
		else
			LCD_appendString("PRECHRG");
	}
	else if(BPSstate == 3)
		LCD_appendString("TRIPPED");
	else if(BPSstate == 4)
		LCD_appendString("!ERROR!");
	else
		LCD_appendString("BAT OFF");
	
	//fourth line
	LCD_appendChar(254);	//escape code meaning that next byte is a command
	LCD_appendChar(0x45);	//move cursor to the given position
	LCD_appendChar(0x54);	//move cursor to the first position of row 4
	if((BPSstate != 3) && (BPSstate != 4))		//if the BPS isn't in an error or tripped state, show the staee of charge
	{
		LCD_appendString("SOC:");
		LCD_appendSOC(SOC_percent);
		LCD_appendChar('%');
	}
	else if(BPSstate == 3)		//if tripped, show the reason
	{
		if(tripStatus == 1)
		{
			LCD_appendString("overcur(dis):");
			LCD_appendPackCurrent((signed)tripValue);
			LCD_appendChar('A');
		}
		else if(tripStatus == 2)
		{
			LCD_appendString("overcur(chg):");
			LCD_appendPackCurrent((signed)tripValue);
			LCD_appendChar('A');
		}
		else if(tripStatus == 3)
		{
			LCD_appendString("COLD on C");
			LCD_appendUint(tripCell);
			LCD_appendString(" @ ");
			LCD_appendCellTemp(tripValue);
			LCD_appendChar('C');
		}
		else if(tripStatus == 4)
		{
			LCD_appendString("HOT on C");
			LCD_appendUint(tripCell);
			LCD_appendString(" @ ");
			LCD_appendCellTemp(tripValue);
			LCD_appendChar('C');
		}
		else if(tripStatus == 5)
		{
			LCD_appendString("UnderVolt C");
			LCD_appendUint(tripCell);
			LCD_appendString(" @ ");
			LCD_appendCellVolt(tripValue);
			LCD_appendChar('V');
		}
		else if(tripStatus == 6)
		{
			LCD_appendString("OverVolt C");
			LCD_appendUint(tripCell);
			LCD_appendString(" @ ");
			LCD_appendCellVolt(tripValue);
			LCD_appendChar('V');
		}
		else if (tripStatus == 7)
			LCD_appendString("External System Err");
		else
			LCD_appendString("INTERNAL BPS FAILURE");
	}

	//just needs to build the output buffer by calling the functions in LCDutil.c
	//this is called periodically by LCD_update. that function will also initiate the transmission. this function just fills the buffer
/*
	//temp test code
	LCD_appendChar(254);	//escape code meaning that next byte is a command
	LCD_appendChar(0x45);	//move cursor to arbitrary position
	LCD_appendChar(0x40);	//move to first position of second line

	LCD_appendString("LCD loop # ");
	LCD_appendULint(LCD_updateCtr);
	LCD_appendString(" ROM ");
	LCD_appendChar(0xB0);
*/
}

void LCD_initialize(void)
{
	LCD_outLength = 0;
	LCD_TXreqeust = 0;
	LCD_loopCtr = 0;
	LCD_period = reg_read(7,1);		//# of main loop cycles between LCD updates (0 = LCD is disabled)
}

//called each main loop iteration
void LCD_update(void)
{
	if(LCD_period == 0)	//check if we never update LCD...
		return;
	if(!RS422_LCDdone)	//LCD is busy sending out data, don't touch the buffer
		return;
	if(!LCD_updateEnabled)
		return;
	LCD_loopCtr++;
	if(LCD_loopCtr >= LCD_period)
	{
		LCD_loopCtr = 0;
		LCD_updateCtr++;
		LCD_writeMsg();
		RS422_sendLCD();
	}
}



//fucking newline
