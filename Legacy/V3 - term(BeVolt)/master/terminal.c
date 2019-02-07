//RTC.c
//Fred Engelkemeir, fall 2012
//this code provides a serial text type terminal interface to the BPS
//this is used since the PC-GUI application hasn't been made yet

#include "time.h"
#include "globals.h"
#include "SOC.h"
#include "misc.h"
#include "RTC.h"
#include "terminal.h"
#include "term_util.h"
#include "RS485master.h"
#include "Mregisters.h"
#include "RS422.h"
#include "termregnames.h"

unsigned char term_location;		//where in the menu hirearchy we are in
unsigned int term_sublocation;	
unsigned char term_StillSending;	//true when there is still more stuff to transmit
unsigned char term_help_verse;
unsigned char term_PR_type;
unsigned char term_PR_group;
unsigned char term_PR_reg;
unsigned char term_slave;
unsigned char term_group;
unsigned char term_reg;
unsigned char term_cell;
unsigned char term_inputErr;				//set to true by the input (number) parsing routine, makes the rest of code reset
unsigned int term_readings[128];
int term_value;					//used to set registers
unsigned int term_Tcur;			//top current
unsigned int term_Tsup;			//top 24V supply
unsigned int term_Tvlt;			//top voltage
unsigned int term_Ttmp;			//top temperature
unsigned int term_Bcur;			//bottom current
unsigned int term_Bsup;			//bottom supply
unsigned int term_Bvlt;			//bottom volt
unsigned int term_Btmp;			//bottom temp
unsigned char term_Tstatus;		//status of the top charger LEDs
unsigned char term_Bstatus;		//status of the bottom charger LEDs
unsigned char term_CMD;				//terminal commands that need to be executed from mainline
									//0 = no waiting command
									//1 = read register on slave
									//2 = write register on slave
									//3 = reboot boards
									//4 = read RTC
									//5 = set RTC
									//6 = zeroize battery backed SRAM
									//7 = read out entire battery backed SRAM
									//8 = update the state of charge
unsigned char term_REB_target;		//0 = all boards
									//1-15 = one slave
									//16 = all slaves, not the master
unsigned char term_SRAM[64];		//holds contents of SRAM for reading out by RSRAM
unsigned char term_newSOC;			//holds the new state of charge for SSOC
time term_time;

void term_help(void);
void term_SR(void);
void term_RR(void);
void term_RC(void);
void term_SC(void);
void term_V(void);
void term_T(void);
void term_I(void);
void term_SOC(void);
void term_BAL(void);
void term_ON(void);
void term_OFF(void);
void term_PR(void);
void term_SM(void);
void term_BM(void);
void term_BS(void);
void term_REB(void);
void term_H(void);
void term_SSOC(void);
void term_ZSRAM(void);
void term_RSRAM(void);
void term_RGPIO(void);
void term_RGPIO_printONOFF(unsigned char state);
void term_LCDON(void);
void term_LCDOF(void);
void term_TR(void);
void term_HL(void);
void term_PL(void);

void term_dispatch(void);

//called whenever the user hits the enter key
void term_executeCMD(void)
{
	unsigned char i;
	for(i = 0; i < RS422_termInPointer; i++)	//make input string uppercase
		if((RS422_termInBuff[i] >= 97) && (RS422_termInBuff[i] <= 122))
			RS422_termInBuff[i] -=32;
	if(term_location == 0)
	{
		term_inputErr = 0;
		if((RS422_termInBuff[0] == 'H') && (RS422_termInPointer <= 1))
		{
			term_location = 1;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'S') && (RS422_termInBuff[1] == 'R') && (RS422_termInPointer >= 1))
		{
			term_location = 2;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'R') && (RS422_termInBuff[1] == 'R') && (RS422_termInPointer >= 1))
		{
			term_location = 3;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'R') && (RS422_termInBuff[1] == 'C') && (RS422_termInPointer >= 1))
		{
			term_location = 4;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'S') && (RS422_termInBuff[1] == 'C') && (RS422_termInPointer >= 1))
		{
			term_location = 5;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if(RS422_termInBuff[0] == 'V' && (RS422_termInPointer <= 1))
		{
			term_location = 6;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if(RS422_termInBuff[0] == 'T' && (RS422_termInPointer <= 1))
		{
			term_location = 7;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if(RS422_termInBuff[0] == 'I' && (RS422_termInPointer <= 1))
		{
			term_location = 8;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'B') && (RS422_termInBuff[1] == 'A') && (RS422_termInBuff[2] == 'L') && (RS422_termInPointer >= 3))
		{
			term_location = 9;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'S') && (RS422_termInBuff[1] == 'O') && (RS422_termInBuff[2] == 'C') && (RS422_termInPointer >= 3))
		{
			term_location = 10;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'O') && (RS422_termInBuff[1] == 'N') && (RS422_termInPointer >= 1))
		{
			term_location = 11;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'O') && (RS422_termInBuff[1] == 'F') && (RS422_termInBuff[2] == 'F') && (RS422_termInPointer >= 2))
		{
			term_location = 12;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'P') && (RS422_termInBuff[1] == 'R') && (RS422_termInPointer >= 1))
		{
			term_location = 13;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'S') && (RS422_termInBuff[1] == 'M') && (RS422_termInPointer >= 1))
		{
			term_location = 14;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'B') && (RS422_termInBuff[1] == 'M') && (RS422_termInPointer >= 1))
		{
			term_location = 15;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'B') && (RS422_termInBuff[1] == 'S') && (RS422_termInPointer >= 1))
		{
			term_location = 16;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'R') && (RS422_termInBuff[1] == 'E') && (RS422_termInBuff[2] == 'B') && (RS422_termInPointer >= 2))
		{
			term_location = 17;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'H') && (RS422_termInBuff[1] == 'E') && (RS422_termInBuff[2] == 'L') && (RS422_termInPointer >= 2))
		{
			term_location = 18;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'S') && (RS422_termInBuff[1] == 'S') && (RS422_termInBuff[2] == 'O') && (RS422_termInBuff[3] == 'C') && (RS422_termInPointer >= 3))
		{
			term_location = 19;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'Z') && (RS422_termInBuff[1] == 'S') && (RS422_termInBuff[2] == 'R') && (RS422_termInBuff[3] == 'A') && (RS422_termInBuff[4] == 'M') && (RS422_termInPointer >= 4))
		{
			term_location = 20;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'R') && (RS422_termInBuff[1] == 'S') && (RS422_termInBuff[2] == 'R') && (RS422_termInBuff[3] == 'A') && (RS422_termInBuff[4] == 'M') && (RS422_termInPointer >= 4))
		{
			term_location = 21;
			term_sublocation = 0;
			term_dispatch();
			return;
		}
		else if((RS422_termInBuff[0] == 'R') && (RS422_termInBuff[1] == 'G') && (RS422_termInBuff[2] == 'P') && (RS422_termInBuff[3] == 'I') && (RS422_termInBuff[4] == 'O') && (RS422_termInPointer >= 4))
		{
			term_location = 22;
			term_sublocation = 0;
			term_dispatch();	
			return;
		}
		else if((RS422_termInBuff[0] == 'L') && (RS422_termInBuff[1] == 'C') && (RS422_termInBuff[2] == 'D') && (RS422_termInBuff[3] == 'O') && (RS422_termInBuff[4] == 'N') && (RS422_termInPointer >= 4))
		{
			term_location = 23;
			term_sublocation = 0;
			term_dispatch();	
			return;
		}
		else if((RS422_termInBuff[0] == 'L') && (RS422_termInBuff[1] == 'C') && (RS422_termInBuff[2] == 'D') && (RS422_termInBuff[3] == 'O') && (RS422_termInBuff[4] == 'F') && (RS422_termInPointer >= 4))
		{
			term_location = 24;
			term_sublocation = 0;
			term_dispatch();	
			return;
		}
		else if((RS422_termInBuff[0] == 'T') && (RS422_termInBuff[1] == 'R')  && (RS422_termInPointer >= 1))
		{
			term_location = 25;
			term_sublocation = 0;
			term_dispatch();	
			return;
		}
		else if((RS422_termInBuff[0] == 'H') && (RS422_termInBuff[1] == 'L')  && (RS422_termInPointer >= 1))
		{
			term_location = 26;
			term_sublocation = 0;
			term_dispatch();	
			return;
		}
		else if((RS422_termInBuff[0] == 'P') && (RS422_termInBuff[1] == 'L')  && (RS422_termInPointer >= 1))
		{
			term_location = 27;
			term_sublocation = 0;
			term_dispatch();	
			return;
		}
		else
		{
			RS422_termOutLength = 0;
			term_appendString("\n\rUnrecognized command. RTFM.\n\n\rBPS> ");
			term_StillSending = 0;
			term_location = 0;
			RS422_sendTerm();
			return;
		}
		return;
	}
	term_dispatch();
	return;
}

//called by RS422 interupt handler whenever the terminal has been entered
void term_entered(void)
{
	term_location = 0;
	term_sublocation = 0;
	RS422_termOutLength = 0;
	term_appendString("\n\n\rWelcome to BPS serial Terminal Mode!\n\rpress \"esc\" key to exit back to packet mode\n\rpress \"h\" for a cryptic help menu\n\n\rBPS> ");
	RS422_sendTerm(); 
}

void term_help(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r===Available BPS commands===\n\rSR  - Set Register\n\rRR  - Read Register\n\rRC  - Read RTC\n\rSC  - Set RTC\n\rV   - show cell Voltages\n\rT   - show Temperatures\n\rI   - show current(I) and BPS status\n\rREB - REBoot system\n\r");
		term_sublocation = 1;
		term_StillSending = 1;
	}
	else if(term_sublocation == 1)
	{
		term_appendString("SOC - show State Of Charge\n\rBAL - show BALancing\n\rON  - turn system ON\n\rOFF - turn system OFF\n\rPR - Print list of Registers\n\rSM - Stop Measurement\n\rBM - Begin Measurement\n\rBS - BootStrap\n\rHEL - system HEaLth\n\r");
		term_StillSending = 1;
		term_sublocation = 2;
	}
	else if(term_sublocation == 2)
	{
		term_appendString("SSOC - Set State Of Charge\n\rZSRAM - Zeroize battery-backed SRAM\n\rRSRAM - Read all of battery backed SRAM\n\rRGPIO - Show hardware inputs and outputs\n\rLCDON - Enable LCD update\n\rLCDOFF - temporarily disable LCD update\n\r");
		term_StillSending = 1;
		term_sublocation = 3;
	}
	else if(term_sublocation == 3)
	{
		term_appendString("TR - display the Reason that BPS Tripped\n\rHL - print the Highest and Lowest readings\n\rPL - Print the trip Limits\n\n\rBPS> ");
		term_sublocation = 0;
		term_StillSending = 0;
		term_location = 0;
	}
	RS422_sendTerm(); 
}

//***use these to send message***
//void RS422_sendTerm(void);
//unsigned char RS422_termOutBuff[255];				//holds the output to the user
//unsigned char RS422_termOutLength;
//
/* functons terminal needs
	read register (any one)
	set register (any slave)
	set clock
	show clock
	show volts
	SOC
	BAL
*/

//called by 422TX interrupt handler when finished sending data
void term_sendDone(void)
{
	if(term_StillSending)
	{
		term_dispatch();
	}
}

//go to the appropriate function (for intial entry when called by term-execute CMD, entry of any user data (also called by term-executeCMD), and when returning from sending data (called by term_sendDone)
void term_dispatch(void)
{
	if(term_location == 1)
		term_help();
	else if(term_location == 2)
		term_SR();
	else if(term_location == 3)
		term_RR();
	else if(term_location == 4)
		term_RC();
	else if(term_location == 5)
		term_SC();
	else if(term_location == 6)
		term_V();
	else if(term_location == 7)
		term_T();
	else if(term_location == 8)
		term_I();
	else if(term_location == 9)
		term_BAL();
	else if(term_location == 10)
		term_SOC();
	else if(term_location == 11)
		term_ON();
	else if(term_location == 12)
		term_OFF();
	else if(term_location == 13)
		term_PR();
	else if(term_location == 14)
		term_SM();
	else if(term_location == 15)
		term_BM();
	else if(term_location == 16)
		term_BS();
	else if(term_location == 17)
		term_REB();
	else if(term_location == 18)
		term_H();
	else if(term_location == 19)
		term_SSOC();
	else if(term_location == 20)
		term_ZSRAM();
	else if(term_location == 21)
		term_RSRAM();
	else if(term_location == 22)
		term_RGPIO();
	else if(term_location == 23)
		term_LCDON();
	else if(term_location == 24)
		term_LCDOF();
	else if(term_location == 25)
		term_TR();
	else if(term_location == 26)
		term_HL();
	else if(term_location == 27)
		term_PL();
}

void term_SR(void)
{
	int temp;
	if(term_sublocation == 0)
	{
		RS422_termOutLength = 0;
		term_appendString("\n\rSetting Register\n\renter number of slave(0 = master)\n\n\rslave num> ");
		term_StillSending = 0;
		term_sublocation = 1;
		RS422_sendTerm();
	}
	else if (term_sublocation == 1)
	{
		RS422_termOutLength = 0;
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 15)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_slave = temp;
		if(term_slave)
		{
			//term_appendString("\n\rworking on slave board  ");
			term_appendUint(term_slave);
			term_appendString("\n\renter register group\n\n\rgroup>");
		}
		else
		{
			term_appendString("\n\rworking on the Master\n\renter register group\n\n\r group>");
		}
		term_StillSending = 0;
		term_sublocation = 2;
		RS422_sendTerm();
	}
	else if (term_sublocation == 2)
	{
		RS422_termOutLength = 0;
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 255)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_group = temp;
		term_appendString("\n\renter register\n\rregister>");
		term_StillSending = 0;
		term_sublocation = 3;
		RS422_sendTerm();
	}
	else if (term_sublocation == 3)
	{
		RS422_termOutLength = 0;
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 255)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_reg = temp;
		term_appendString("\n\renter value\n\rvalue>");
		term_StillSending = 0;
		term_sublocation = 4;
		RS422_sendTerm();
	}
	else if (term_sublocation == 4)
	{
		term_value = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		RS422_termOutLength = 0;
		if(term_slave)
		{
			term_CMD = 2;
		}
		else
		{
			temp = reg_write(term_group,term_reg,term_value);
			if(temp == 0)
			{
				term_appendString("\n\rMaster Register ");
				term_appendUint(term_group);
				term_appendChar('.');
				term_appendUint(term_reg);
				temp = reg_read(term_group,term_reg);
				term_appendString(" has the set to ");
				term_appendInt(term_value);
				term_appendString("\n\n\rBPS>");
			}
			else if(temp == 1)
				term_appendString("Error: Invalid Register!\n\n\rBPS>");
			else if(temp == 2)
				term_appendString("Error: Configuration Locked!\n\n\rBPS>");
			else
				term_appendString("Error: unknown error?\n\n\rBPS>");
				
		}
		term_StillSending = 0;
		term_sublocation = 0;
		term_location = 0;
		RS422_sendTerm();
	}
}

void term_RR(void)
{
	int temp;
	if(term_sublocation == 0)
	{
		RS422_termOutLength = 0;
		term_appendString("\n\rReading Register\n\renter number of slave(0 = master)\n\n\rslave num> ");
		term_StillSending = 0;
		term_sublocation = 1;
		RS422_sendTerm();
	}
	else if (term_sublocation == 1)
	{
		RS422_termOutLength = 0;
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 15)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_slave = temp;
		if(term_slave)
		{
			term_appendString("\n\rworking on slave board  ");
			term_appendUint(term_slave);
			term_appendString("\n\r enter register group\n\n\r group>");
		}
		else
		{
			term_appendString("\n\rworking on the Master\n\renter register group\n\n\r group>");
		}
		term_StillSending = 0;
		term_sublocation = 2;
		RS422_sendTerm();
	}
	else if (term_sublocation == 2)
	{
		RS422_termOutLength = 0;
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 255)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_group = temp;
		term_appendString("\n\renter register\n\rregister>");
		term_StillSending = 0;
		term_sublocation = 4;		//skip the value section, since we're reaing not writing
		RS422_sendTerm();
	}
	else if (term_sublocation == 3)	//TODO:delete this
	{
		RS422_termOutLength = 0;
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 255)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_reg = temp;
		term_appendString("\n\renter value\n\rvalue>");
		term_StillSending = 0;
		term_sublocation = 4;
		RS422_sendTerm();
	}
	else if (term_sublocation == 4)
	{
		temp = term_getNum();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		if(temp > 255)
			term_invalidEntry();
		if(term_inputErr)
		{
			term_inputErr = 0;
			return;
		}
		term_reg = temp;
		RS422_termOutLength = 0;
		if(term_slave)
		{
			term_CMD = 1;
		}
		else
		{
			term_appendString("\n\rMaster Register ");
			term_appendUint(term_group);
			term_appendChar('.');
			term_appendUint(term_reg);
			temp = reg_read(term_group,term_reg);
			term_appendString(" has the value ");
			term_appendInt(temp);
			term_appendString("\n\n\rBPS>");
		}
		term_StillSending = 0;
		term_sublocation = 0;
		term_location = 0;
		RS422_sendTerm();
	}
}

void term_RC(void)
{
	if(term_sublocation == 0)
	{
		term_StillSending = 0;
		term_sublocation = 1;
		term_CMD = 4;
	}
	else if(term_sublocation == 1)
	{
		RS422_termOutLength = 0;
		term_appendString("\n\r=====  RTC Clock  =====\n\rDate: ");
		term_appendUint(term_time.month);
		term_appendChar('\\');
		term_appendUint(term_time.day);
		term_appendString("\\20");
		term_appendUintZ(term_time.year,2);
		term_appendString("\n\rTime: ");
		term_appendUint(term_time.hour);
		term_appendChar(':');
		term_appendUintZ(term_time.minute,2);
		term_appendChar(':');
		term_appendUintZ(term_time.seconds,2);
		term_appendString("\n\n\rBPS> ");
		term_sublocation = 0;
		term_location = 0;
		term_StillSending = 0;
		RS422_sendTerm();
	}
}

void term_SC(void)
{
	unsigned int temp;
	if(term_sublocation == 0)
	{
		RS422_termOutLength = 0;
		term_appendString("\n\r=====  Setting BPS Real Time Clock  =====\n\renter Year (2 digits 0-99)> ");
		term_StillSending = 0;
		term_sublocation = 1;
		RS422_sendTerm();
	}
	else if(term_sublocation == 1)
	{
		temp = term_getNum();
		if(term_inputErr || (temp > 99))
		{
			term_invalidEntry();
			term_inputErr = 0;
			return;
		}
		term_time.year = temp;
		RS422_termOutLength = 0;
		term_appendString("\n\renter month(1-12)> ");
		term_StillSending = 0;
		term_sublocation = 2;
		RS422_sendTerm();
	}
	else if(term_sublocation == 2)
	{
		temp = term_getNum();
		if(term_inputErr || (temp > 12) || (temp == 0))
		{
			term_invalidEntry();
			term_inputErr = 0;
			return;
		}
		term_time.month = temp;
		RS422_termOutLength = 0;
		term_appendString("\n\renter day(1-31)> ");
		term_StillSending = 0;
		term_sublocation = 3;
		RS422_sendTerm();
	}
	else if(term_sublocation == 3)
	{
		temp = term_getNum();
		if(term_inputErr || (temp > 31) || (temp == 0))
		{
			term_invalidEntry();
			term_inputErr = 0;
			return;
		}
		term_time.day = temp;
		RS422_termOutLength = 0;
		term_appendString("\n\renter hour(0-23)> ");
		term_StillSending = 0;
		term_sublocation = 4;
		RS422_sendTerm();
	}
	else if(term_sublocation == 4)
	{
		temp = term_getNum();
		if(term_inputErr || (temp > 23))
		{
			term_invalidEntry();
			term_inputErr = 0;
			return;
		}
		term_time.hour = temp;
		RS422_termOutLength = 0;
		term_appendString("\n\renter minutes(0-59)> ");
		term_StillSending = 0;
		term_sublocation = 5;
		RS422_sendTerm();
	}
	else if(term_sublocation == 5)
	{
		temp = term_getNum();
		if(term_inputErr || (temp > 60))
		{
			term_invalidEntry();
			term_inputErr = 0;
			return;
		}
		term_time.minute = temp;
		RS422_termOutLength = 0;
		term_appendString("\n\renter seconds(0-59)> ");
		term_StillSending = 0;
		term_sublocation = 6;
		RS422_sendTerm();
	}
	else if(term_sublocation == 6)
	{
		temp = term_getNum();
		if(term_inputErr || (temp > 60))
		{
			term_invalidEntry();
			term_inputErr = 0;
			return;
		}
		term_time.seconds = temp;
		term_sublocation = 7;
		term_CMD = 5;				//get mainloop code to set the time for us
	}
	else if(term_sublocation == 7)
	{

		RS422_termOutLength = 0;
		term_appendString("\n\rclock updated.\n\n\rBPS> ");
		term_StillSending = 0;
		term_sublocation = 0;
		term_location = 0;
		RS422_sendTerm();
	}
}

void term_V(void)
{
	unsigned char i;
	unsigned int temp;
	temp = reg_read(3,1);
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		if(arrayAgood)
		{
			term_appendString("\n\r===== Cell Voltages =====\n\r");
			term_cell = 0;
			term_sublocation = 1; 
			term_StillSending = 1;
			for(i = 0; i <= temp;i++)
				term_readings[i] = batteryVoltsA[i];
		}
		else if(arrayBgood)
		{
			term_appendString("\n\r===== Cell Voltages =====\n\r");
			term_cell = 0;
			term_sublocation = 1; 
			term_StillSending = 1;
			for(i = 0; i <= temp;i++)
				term_readings[i] = batteryVoltsB[i];
		}
		else
		{
			term_appendString("\n\rERROR: no valid data available.\n\n\r");
			term_StillSending = 0;
			term_location = 0;
			term_sublocation = 0;
		}
	}
	else if(term_sublocation == 1)
	{
		term_appendString("Cell ");
		term_appendUint(term_cell+1);	//make cell list start at 1 and not 0
		term_appendString(": ");
		term_appendVolt(term_readings[term_cell]);
		term_appendString("\n\r");
		term_cell++;
		if(term_cell >= temp)
			term_sublocation = 2;
	}
	else
	{		
		term_appendString("\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
		RS422_sendTerm();
	}
	RS422_sendTerm();
}

void term_T(void)
{
	unsigned char i;
	unsigned int temp;
	temp = reg_read(3,1);
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		if(arrayAgood)
		{
			term_appendString("\n\r===== Cell Temperatures =====\n\r");
			term_cell = 0;
			term_sublocation = 1; 
			term_StillSending = 1;
			for(i = 0; i <= temp;i++)
				term_readings[i] = batteryTempsA[i];
		}
		else if(arrayBgood)
		{
			term_appendString("\n\r===== Cell Temperatures =====\n\r");
			term_cell = 0;
			term_sublocation = 1; 
			term_StillSending = 1;
			for(i = 0; i <= temp;i++)
				term_readings[i] = batteryTempsB[i];
		}
		else
		{
			term_appendString("\n\rERROR: no valid data available.\n\n\r");
			term_StillSending = 0;
			term_location = 0;
			term_sublocation = 0;
		}
	}
	else if(term_sublocation == 1)
	{
		term_appendString("Cell ");
		term_appendUint(term_cell+1);	//make cell list start at 1 and not 0
		term_appendString(": ");
		term_appendTemp(term_readings[term_cell]);
		term_appendString("\n\r");
		term_cell++;
		if(term_cell >= temp)
			term_sublocation = 2;
	}
	else
	{		
		term_appendString("\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
		RS422_sendTerm();
	}
	RS422_sendTerm();
}

void term_I(void)
{
	RS422_termOutLength = 0;
	term_appendString("\n\rBPS system Status: ");
	term_appendUint(BPSstate);
	if(BPSstate == 0)
		term_appendString(" - BPS system is \"off\" and not measuring cells");
	else if(BPSstate == 1)
		term_appendString(" - BPS system is measuring cells and contactor is OFF");
	else if(BPSstate == 2)
		term_appendString(" - BPS system is measuring cells and contactor is ON");
	else if(BPSstate == 3)
		term_appendString(" - BPS is in the tripped state: contactor OFF, BPS measuring cells");
	else if(BPSstate == 4)
		term_appendString(" - BPS has had an internal error: contactor OFF, not measuring cells");
	else if(BPSstate == 5)
		term_appendString(" - BPS has just been commanded on, BPS must measure cells before turning contactor on");
	else
		term_appendString(" - unknown state!");
	term_appendString("\n\rSystem Current (+ = charge, - = discharge): ");
	if(arrayAgood)
		term_appendCurrent(currentA);
	else if(arrayBgood)
		term_appendCurrent(currentB);
	else
		term_appendString("Error: no good data!");
	term_appendString("\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_SOC(void)
{
	RS422_termOutLength = 0;
	term_appendString("\n\r====== State Of Charge ======\n\rSOC Percent:\t\t\t\t");
	term_appendUint(SOC_percent);
	term_appendString("\n\rraw SOC accumulator (Amp-MiliSeconds):\t");
	term_appendULint(SOC_accumulator);
	term_appendString("\n\rSOC initialization state:\t\t");
	term_appendUint(SOC_initState);
	if(SOC_initState == 0)
		term_appendString(" (not initialized yet)");
	else if(SOC_initState == 1)
		term_appendString(" (accumulator loaded from SRAM location 1)");
	else if(SOC_initState == 2)
		term_appendString(" (accumulator loaded from SRAM location 2)");
	else if(SOC_initState == 3)
		term_appendString(" (accumulator loaded from SRAM location 1, both SRAM location good)");
	else if(SOC_initState == 4)
		term_appendString(" (no good data in SRAM, defaults loaded)");
	term_appendString("\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_BAL(void)
{
	RS422_termOutLength = 0;
	term_appendString("\n\rBalancing currently is useless.\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_ON(void)
{
	RS422_termOutLength = 0;
	term_appendString("\n\rAttempting to turn on System (run the I command to see if it worked)\n\n\rBPS> ");
	if(BPSstate != 2)
		BPSstate = 5;
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_OFF(void)
{
	RS422_termOutLength = 0;
	if(BPSstate == 0 || BPSstate == 4)
		BPSstate = 0;
	else
		BPSstate = 1;
	term_appendString("\n\rShutting down the BPS system.\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_PR(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_sublocation = 1;
		term_PR_type = 0;
		term_PR_group = 1;
		term_PR_reg = 1;
		term_StillSending = 1;
		term_appendString("\n\n\r===== Registers on the Master =====\n\r");
		RS422_sendTerm();
		return;
	}
	else if(term_sublocation == 1)
	{
		term_appendRegName(0,term_PR_group,term_PR_reg);
		term_PR_reg++;
		if(term_PR_group == 1 && term_PR_reg > 19)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 2 && term_PR_reg > 16)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 3 && term_PR_reg > 9)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 4 && term_PR_reg > 16)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 5 && term_PR_reg > 24)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 6 && term_PR_reg > 3)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 7 && term_PR_reg > 1)
		{
			term_PR_group = 1;
			term_PR_reg = 1;
			term_sublocation = 2;
		}
		term_appendString("\n\r");
		RS422_sendTerm();
		return;
	}
	else if(term_sublocation == 2)
	{
		term_appendString("\n\n\r===== Registers on the Temperature/Voltage Slave Boards =====\n\r");
		term_sublocation = 3;
		RS422_sendTerm();
		return;
	}
	else if(term_sublocation == 3)
	{
		term_appendRegName(1,term_PR_group,term_PR_reg);
		term_PR_reg++;
		if(term_PR_group == 1 && term_PR_reg > 10)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 2 && term_PR_reg > 12)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 3 && term_PR_reg > 12)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 4 && term_PR_reg > 12)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 5 && term_PR_reg > 12)
		{
			term_PR_group = 2;		//bootstrap board doesn't currently have anything in the CONFIG group (group 1)
			term_PR_reg = 1;
			term_sublocation = 4;
		}
		term_appendString("\n\r");
		RS422_sendTerm();
		return;
	}
	else if(term_sublocation == 4)
	{
		term_appendString("\n\n\r===== Registers on the Bootstap Slave Logic Board =====\n\r");
		term_sublocation = 5;
		RS422_sendTerm();
		return;
	}
	else if (term_sublocation == 5)
	{
		term_appendRegName(2,term_PR_group,term_PR_reg);
		term_PR_reg++;
		if(term_PR_group == 2 && term_PR_reg > 13)
		{
			term_PR_group++;
			term_PR_reg = 0;
		}
		else if(term_PR_group == 3 && term_PR_reg > 13)
		{
			term_location = 0;
			term_sublocation = 0;
			term_StillSending = 0;
		}
		if(term_StillSending)
		{
			term_appendString("\n\r");
			RS422_sendTerm();
		}
		else
		{
			term_appendString("\n\n\n\rBPS> ");
			RS422_sendTerm();
		}
	}
}

void term_SM(void)
{
	RS422_termOutLength = 0;
	if(BPSstate == 1 || BPSstate == 3)
	{
		term_appendString("\n\rMeasurement Stopped.\n\n\rBPS> ");
		BPSstate = 0;
	}
	else
		term_appendString("\n\rDidn't stop measurement since contactor is ON/pending\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_BM(void)
{
	RS422_termOutLength = 0;
	if(BPSstate == 0 || BPSstate == 4)
	{
		term_appendString("\n\rMeasurement Started.\n\n\rBPS> ");
		BPSstate = 1;
	}
	else
		term_appendString("\n\rBPS is already measuring\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

void term_BS(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		if(reg_read(1,7) == 0)
		{
			term_appendString("\n\rERROR: no Bootstrap slave configured on this system.\n\n\rBPS> ");
			term_StillSending = 0;
			term_location = 0;
			term_sublocation = 0;
		}
		else 
		{
			if(BSsetAGood)
			{
				term_appendString("\n\r===== Bootstrap Data for Top BS Module =====\n\r");
				term_sublocation = 1; 
				term_StillSending = 1;
				term_Tcur = ATcur;			//top current
				term_Tsup = ATsup;			//top 24V supply
				term_Tvlt = ATvlt;			//top voltage
				term_Ttmp = ATtmp;			//top temperature
				term_Bcur = ABcur;			//bottom current
				term_Bsup = ABsup;			//bottom supply
				term_Bvlt = ABvlt;			//bottom volt
				term_Btmp = ABtmp;			//bottom temp
				term_Tstatus = ATstatus;		//status of the top charger LEDs
				term_Bstatus = ABstatus;		//status of the bottom charger LEDs
			}
			else if(BSsetBGood)
			{
				term_appendString("\n\r===== Bootstrap Data for Top BS Module =====\n\r");
				term_sublocation = 1; 
				term_StillSending = 1;
				term_Tcur = BTcur;			//top current
				term_Tsup = BTsup;			//top 24V supply
				term_Tvlt = BTvlt;			//top voltage
				term_Ttmp = BTtmp;			//top temperature
				term_Bcur = BBcur;			//bottom current
				term_Bsup = BBsup;			//bottom supply
				term_Bvlt = BBvlt;			//bottom volt
				term_Btmp = BBtmp;			//bottom temp
				term_Tstatus = BTstatus;		//status of the top charger LEDs
				term_Bstatus = BBstatus;		//status of the bottom charger LEDs
			}
			else
			{
				term_appendString("\n\rERROR: no valid Bootstrap data available.\n\n\rBPS> ");
				term_StillSending = 0;
				term_location = 0;
				term_sublocation = 0;
			}
		}
	}
	else if(term_sublocation == 1)
	{
		term_appendString("Top BS module voltage:\t\t");
		term_appendBSVolt(term_Tvlt);
		term_appendString("\n\rTop BS module current:\t\t");
		term_appendCurrent(term_Tcur);
		term_appendString("\n\rTop BS module temperature:\t");
		term_appendTemp(term_Ttmp);
		term_appendString("\n\rTop BS charger input voltage:\t");
		term_appendBSVolt(term_Tsup);
		term_sublocation = 2;
	}
//returns the status of the charger
//0 = charger off (or can't run due to some problem)
//1 = battery absent (charger can't see the battery)
//2 = charge pending (charger is checking battery before starting a fast charge cycle)
//3 = fast charge
//4 = charge complete (top off or trickle charge may be happening)
	else if(term_sublocation == 2)
	{
		term_appendString("\n\rTop BS charger status: \t\t");
		term_appendUint(term_Tstatus);
		if(term_Tstatus == 0)	
			term_appendString(" : charger is off.");
		else if(term_Tstatus == 1)
			term_appendString(" : charger reports battery absent.");
		else if(term_Tstatus == 2)
			term_appendString(" : charger is in charge pending (charger is checking battery before starting a fast charge cycle) mode.");
		else if(term_Tstatus == 3)
			term_appendString(" : charger is doing a fast charge.");
		else if(term_Tstatus == 4)
			term_appendString(" : charger is in charge complete (top off or trickle charge may be happening) mode.");
		else 
			term_appendString(" : charger status is invalid!");
		term_appendString("\n\n\r===== Bootstrap Data for Bottom BS Module =====\n\r");
		term_sublocation = 3;
	}
	else if(term_sublocation == 3)
	{
		term_appendString("Bot BS module voltage:\t\t");
		term_appendBSVolt(term_Bvlt);
		term_appendString("\n\rBot BS module current:\t\t");
		term_appendCurrent(term_Bcur);
		term_appendString("\n\rBot BS module temperature:\t");
		term_appendTemp(term_Btmp);
		term_appendString("\n\rBot BS charger input voltage\t");
		term_appendBSVolt(term_Bsup);
		term_sublocation = 4;
	}
	else if(term_sublocation == 4)
	{
		term_appendString("\n\rBot BS charger status: \t\t");
		term_appendUint(term_Tstatus);
		if(term_Tstatus == 0)	
			term_appendString(" : charger is off.");
		else if(term_Tstatus == 1)
			term_appendString(" : charger reports battery absent.");
		else if(term_Tstatus == 2)
			term_appendString(" : charger is in charge pending (charger is checking battery before starting a fast charge cycle) mode.");
		else if(term_Tstatus == 3)
			term_appendString(" : charger is doing a fast charge.");
		else if(term_Tstatus == 4)
			term_appendString(" : charger is in charge complete (top off or trickle charge may be happening) mode.");
		else 
			term_appendString(" : charger status is invalid!");
		term_appendString("\n\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
	}
	else
	{		
		term_appendString("\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
		RS422_sendTerm();
	}
	RS422_sendTerm();
}

void term_H(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r===== BPS System Health Statistics =====\n\rNumber of Main Loop Iterations:\t\t\t\t");
		term_appendULint(numCycles);
		term_appendString("\n\rNumber of Slave RS485 Errors (temp/voltage read):\t");
		term_appendUint(commErrorsSlave);
		term_appendString("\n\rNumber of Slave RS485 Errors (balance):\t\t\t");
		term_appendUint(commErrorsBalance);
		term_appendString("\n\rNumber of Bootstrap Slave RS485 errors:\t\t\t");
		term_appendUint(commErrorsBootstrap);
		term_StillSending = 1;
		term_sublocation = 1;
	}	
	else
	{
		term_appendString("\n\rmain loop code cycle time:\t\t\t\t");
		term_appendUint(SOC_cycletime/39); 				//actual divisor is 39.0625; 2.5Mhz instruction rate, divide by 64 prescaler
		term_appendString("ms\n\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
	}	
	RS422_sendTerm();
}


void term_invalidEntry(void)
{
	RS422_termOutLength = 0;
	term_appendString("\n\rInvalid Entry, try again from the start.\n\n\rBPS> ");
	term_inputErr = 1;
	term_StillSending = 0;
	term_location = 0;
	term_sublocation = 0;
	RS422_sendTerm();

}

//called from mainline code
void term_serviceWaitingCMD(void)
{
									//0 = no waiting command
									//1 = read register on slave
									//2 = write register on slave
									//3 = reboot boards
									//4 = read RTC
									//5 = set RTC
									//6 = zeroize battery backed SRAM
									//7 = read out entire battery backed SRAM
									//8 = set SoC
	int temp;
	unsigned char i;
	if(!RS422_termMode)
		return;
	if(term_CMD == 0)
		return;
	if(term_CMD == 1)		//read slave register
	{
		RS422_termOutLength = 0;
		while(!RS485_ready());
		RS485_slave = term_slave;
		RS485_command = 3;
		RS485_outData[0] = term_group;
		RS485_outData[1] = term_reg;
		RS485_outLength = 2;
		RS485_sendPacket(1);		//send packet, wait for resp.
		while(!RS485_ready());
		if(RS485_status == 7)	//timeout
		{
			term_appendString("\n\rError: timeout waiting for slave response.");
		}
		else if (RS485_status == 6)	//corrupt packet
		{
			term_appendString("\n\rError: got a corrupt packet.");
		}
		else if (RS485_status == 5) //interbyte timeout
		{
			term_appendString("\n\rError: Timeout between received bytes of a packet.");
		}
		else if (RS485_status == 4) //got good response
		{
			if((RS485_inLength == 2) && (RS485_response == 0))
			{
				temp = RS485_combineBytes(RS485_inData[0],RS485_inData[1]);
				term_appendString("\n\rRegister ");
				term_appendUint(term_group);
				term_appendChar('.');
				term_appendUint(term_reg);
				term_appendString(" on slave board ID ");
				term_appendUint(term_slave);
				term_appendString(" has the value: ");
				term_appendInt(temp);
			}
			else
				term_appendString("\n\rError: slave reported an error.");			
		}
		term_appendString("\n\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
		term_CMD = 0;
		RS422_sendTerm();
	}
	else if(term_CMD == 2)	//write slave register
	{
		RS422_termOutLength = 0;
		while(!RS485_ready());
		RS485_slave = term_slave;
		RS485_command = 2;
		RS485_outData[0] = term_group;
		RS485_outData[1] = term_reg;
		RS485_outData[3] = RS485_splitLSB(term_value);
		RS485_outData[4] = RS485_splitMSB(term_value);
		RS485_outLength = 4;
		RS485_sendPacket(1);		//send packet, wait for resp.
		while(!RS485_ready());
		if(RS485_status == 7)	//timeout
		{
			term_appendString("\n\rError: timeout waiting for slave response.");
		}
		else if (RS485_status == 6)	//corrupt packet
		{
			term_appendString("\n\rError: got a corrupt packet.");
		}
		else if (RS485_status == 5) //interbyte timeout
		{
			term_appendString("\n\rError: Timeout between received bytes of a packet.");
		}
		else if (RS485_status == 4) //got good response
		{
			if((RS485_inLength == 0) && (RS485_response == 0))
			{
				term_appendString("\n\rRegister ");
				term_appendUint(term_group);
				term_appendChar('.');
				term_appendUint(term_reg);
				term_appendString(" on slave board ID ");
				term_appendUint(term_slave);
				term_appendString(" has been set to  ");
				term_appendInt(term_value);
			}
			else if(RS485_response == 1)
				term_appendString("\n\rError: slave says invalid register.");
			else if(RS485_response == 2)
				term_appendString("\n\rError: configuration is locked.");
			else
				term_appendString("\n\rError: slave reported an unspecified error.");			
		}
		term_appendString("\n\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
		term_CMD = 0;
		RS422_sendTerm();
	}
	else if(term_CMD == 3)		//reboot boards
	{
	}
	else if(term_CMD == 4) 	//read the RTC
	{	
		term_time = RTC_getTime();
		term_RC();
		term_CMD = 0;
	}
	else if(term_CMD == 5)		//set the RTC
	{
		RTC_setTime(term_time);
		term_SC();
		term_CMD = 0;
	}
	else if(term_CMD == 6)		//zeroize battery backed SRAM
	{
		for(i = 0; i < 64; i++)
		{
			RTC_writeSRAM(i,0);
		}
		term_ZSRAM();
		term_CMD = 0;
	}
	else if(term_CMD == 7)		//read out entire batterybackled SRAM
	{
		for(i = 0; i < 64; i++)
		{
			term_SRAM[i] = RTC_readSRAM(i);
		}
		term_CMD = 0;
		term_RSRAM();
	}	
	else if(term_CMD == 8)		//update the State of Charge
	{
		//(unsigned long int)SOC_accumulator = (unsigned long int)SOC_percent2acc(term_newSOC);
		SOC_percent2acc(term_newSOC);	//compiler doesn't like returning unsigned long ints, so this function sets the SOC accumulator  itself
		SOC_percent = SOC_getPercent(SOC_accumulator);
		term_CMD = 0;
		term_SSOC();
	}
	
}

void term_REB(void)
{
	if(term_sublocation == 0)
	{
		RS422_termOutLength = 0;
		term_appendString("\n\rReboot which boards?\n\r1-15 for an individual slaves\n\r0 for master board\n\rS for all slave boards\n\rA for all boards\n\n\rreboot target> ");
		term_StillSending = 0;
		term_sublocation = 1;
		RS422_sendTerm();
	}	
	else if(term_sublocation == 1)
	{
		RS422_termOutLength = 0;
		term_sublocation = 1;
		if(RS422_termInBuff[0] == '0')
		{
			term_REB_target = 17;
			term_CMD = 0;
			term_appendString("\n\n\n\rebooting this master board....");
			term_StillSending = 1;
		}
		else if(RS422_termInBuff[0] == 'S')
		{
			term_REB_target = 16;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting all Slave Boards...Please Wait...");
		}
		else if(RS422_termInBuff[0] == 'A')
		{
			term_REB_target = 0;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting entire BPS...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '2')
		{
			term_REB_target = 2;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 2...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '3')
		{
			term_REB_target = 3;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 3...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '4')
		{
			term_REB_target = 4;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 4...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '5')
		{
			term_REB_target = 5;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 5...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '6')
		{
			term_REB_target = 6;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 6...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '7')
		{
			term_REB_target = 7;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 7...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '8')
		{
			term_REB_target = 8;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 8...Please Wait...");
		}
		else if(RS422_termInBuff[0] == '9')
		{
			term_REB_target = 9;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 9...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInPointer == 0))
		{
			term_REB_target = 1;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 1...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInBuff[1] == '0') && (RS422_termInPointer >= 1))
		{
			term_REB_target = 10;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 10...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInBuff[1] == '1') && (RS422_termInPointer >= 1))
		{
			term_REB_target = 11;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 11...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInBuff[1] == '2') && (RS422_termInPointer >= 1))
		{
			term_REB_target = 12;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 12...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInBuff[1] == '3') && (RS422_termInPointer >= 1))
		{
			term_REB_target = 13;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 13...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInBuff[1] == '4') && (RS422_termInPointer >= 1))
		{
			term_REB_target = 14;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 14...Please Wait...");
		}
		else if((RS422_termInBuff[0] == '1') && (RS422_termInBuff[1] == '5') && (RS422_termInPointer >= 1))
		{
			term_REB_target = 15;
			term_CMD = 3;
			term_appendString("\n\n\n\rebooting slave board 15...Please Wait...");
		}
		else
		{
			term_appendString("Invalid Selection\n\n\rBPS> ");
			term_StillSending = 0;
			term_location = 0;
			term_sublocation = 0;
			term_CMD = 0;
		}
		RS422_termOutLength = 0;
		term_appendString("\n\rReboot which boards?\n\r1-15 for an individual slaves\n\r0 for master board\n\rS for all slave boards\n\rA for all boards\n\n\rreboot target> ");
		term_StillSending = 1;
		term_sublocation = 2;
		RS422_sendTerm();
	}
	else
	{
		if(term_REB_target == 17)
			asm("reset");				//reset the uC
		term_StillSending = 0;
		term_sublocation = 0;
		term_location = 0;
	}
	RS422_sendTerm();
}



//set the state of charge accumulator info
void term_SSOC(void)
{
	unsigned int temp;
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r====== Setting the State of Charge ======\n\rEnter new state of charge percentage(0-100)> ");
		term_sublocation = 1;
	}
	else if(term_sublocation == 1)
	{
		temp = term_getNum();
		if(temp > 100)
		{
			term_appendString("\n\rBad entry. try again.\n\n\rBPS> ");
			term_sublocation = 0;
			term_location = 0;
		}
		else
		{
			term_CMD = 8;
			term_newSOC = temp;
			term_sublocation = 2;
		}
	}
	else if(term_sublocation == 2)
	{
		term_appendString("\n\rState of Charge has been updated.\n\n\rBPS> ");
		term_sublocation = 0;
		term_location = 0;
	}
	RS422_sendTerm();
}

void term_ZSRAM(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r====== Zeroize/Erase battery backed SRAM ======\n\n\rARE YOU SURE? SOC data will be deleted.\n\renter \"YES\" to continue: ");
		term_sublocation = 1;
	}
	else if(term_sublocation == 1)
	{
		if((RS422_termInBuff[0] == 'Y') && (RS422_termInBuff[1] == 'E') && (RS422_termInBuff[2] == 'S') && (RS422_termInPointer >= 2))
		{
			term_sublocation = 2;
			term_CMD = 6;
		}
		else
		{
			term_appendString("\n\rOperation canceled.\n\n\rBPS> ");
			term_sublocation = 0;
			term_location = 0;
		}
	}
	else if(term_sublocation == 2)
	{
		term_appendString("\n\rSRAM addresses 0-63 have been set to 0. Be sure to reinitialize the SOC system.\n\n\rBPS> ");
		term_sublocation = 0;
		term_location = 0;
	}
	RS422_sendTerm();
}

void term_RSRAM(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r====== Readout of battery backed SRAM (64 bytes) ======\n\n\r");
		term_sublocation = 1;
		term_StillSending = 0;
		term_CMD = 7;
	}
	else if((term_sublocation > 0)&&(term_sublocation < 65))		//sublocation 1-64 = addresses 0-63
	{
		term_appendString("address ");
		term_appendUintZ(term_sublocation-1,2);
		term_appendString(" has the data:\t");
		term_appendUintZ(term_SRAM[term_sublocation-1],3);
		term_appendString(" (decimal)\t0X");
		term_appendHexByte(term_SRAM[term_sublocation-1]);
		term_appendString("\n\r");
		term_sublocation++;
		term_StillSending = 1;
	}
	else 
	{
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
		term_appendString("\n\n\rBPS> ");
	}
	RS422_sendTerm();
}

//this shows the status of all of the discrete inputs and outputs (i.e., the stuff in misc.c)
void term_RGPIO(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
   		term_appendString("\n\r====== List of DIPswitch settings (master board) ======\n\r");
		term_appendString("System Setup Parameters:\t");
		term_RGPIO_printONOFF(misc_isSetupUnlocked() + 2);
		term_appendString("\n\rCalibration Parameters:\t\t");
		term_RGPIO_printONOFF(misc_isCalUnlocked() + 2);
		term_appendString("\n\rBattery Trip Thresholds:\t");
		term_RGPIO_printONOFF(misc_isThreshUnlocked() + 2);
		term_appendString("\n\rBPS DEBUG mode (unsafe!):\t");
		term_RGPIO_printONOFF(misc_isDebug());
		term_sublocation = 1;
		term_StillSending = 1;
	}
	else if(term_sublocation == 1)
	{
		term_appendString("\n\n\r====== Discrete Input Status ======\n\r");
		term_appendString("IGNITION input:\t\t\t");
		term_RGPIO_printONOFF(misc_isIgnition());
		term_appendString("\n\r\"BPS ON\" Output:\t\t");
		term_RGPIO_printONOFF(misc_readbackOutBPSON());
		term_appendString("\n\rWARN output:\t\t\t");
		term_RGPIO_printONOFF(misc_readbackOutWarn());
		term_appendString("\n\rTRIP output:\t\t\t");
		term_RGPIO_printONOFF(misc_readbackOutTrip());
		term_sublocation = 2;
		term_StillSending = 1;
	}
	else if(term_sublocation == 2)
	{
		term_appendString("\n\n\r====== LED Status ======");
		term_appendString("\n\rDebug LED:\t\t\t");
		term_RGPIO_printONOFF(misc_readbackLEDDebug());
		term_appendString("\n\rSetup LED:\t\t\t");
		term_RGPIO_printONOFF(misc_readbackLEDSetup());
		term_appendString("\n\rThreshold LED:\t\t\t");
		term_RGPIO_printONOFF(misc_readbackLEDThresh());
		term_appendString("\n\rCalibration LED:\t\t");
		term_RGPIO_printONOFF(misc_readbackLEDCal());
		term_appendString("\n\rFault LED:\t\t\t");
		term_RGPIO_printONOFF(misc_readbackLEDFault());
		term_sublocation = 3;
		term_StillSending = 1;
	}
	else if(term_sublocation == 3)
	{
		term_appendString("\n\n\r====== Contactor Control System ======");
		term_appendString("\n\rWatchdog Status:\t\t");
		term_RGPIO_printONOFF(misc_WDTOK() + 4);
		term_appendString("\n\rWatchdog Feed Output:\t\t");
		term_RGPIO_printONOFF(misc_readbackWDTfeed() + 8);

		term_appendString("\n\rMAIN Contactor Command:\t\t");
		term_RGPIO_printONOFF(misc_readbackMainContactor());
		term_appendString("\n\rMAIN Contactor Check:\t\t");
		term_RGPIO_printONOFF(misc_MainContactorCheck() + 6);

		term_appendString("\n\rAUX Contactor Command:\t\t");
		term_RGPIO_printONOFF(misc_readbackAuxContactor());

		term_appendString("\n\rPRECHARGE Contactor Command:\t");
		term_RGPIO_printONOFF(misc_readbackPreContactor());
		term_sublocation = 4;
		term_StillSending = 1;
		
	}
	else 
	{		//not quite enough characters in teh buffer to append the prompt in the last group....
		term_appendString("\n\rPRECHARGE Contactor Check:\t");
		term_RGPIO_printONOFF(misc_PreContactorCheck() + 6);
		term_appendString("\n\n\rBPS> ");
		term_StillSending = 0;
		term_location = 0;
		term_sublocation = 0;
	}
	RS422_sendTerm();
}

//helper function for term_RGPIO
//just append the stirng "on or "off" for state = 0 and 1, respectively, "locked" or "change allowed" for state = 2 or 3 (i.e., add 2 to the bool for the dipswitch settings). add 4 for the watchdog timer and add 6 for the contacotr feedback and 8 for high/low
void term_RGPIO_printONOFF(unsigned char state)
{
	if(state == 0)
		term_appendString("OFF");
	else if (state == 1)
		term_appendString("ON");

 	else if (state == 2)
		term_appendString("CONFIG LOCKED");
	else if (state == 3)
		term_appendString("CHANGE ALLOWED");

 	else if (state == 4)
		term_appendString("WDT OK");
	else if (state == 5)
		term_appendString("WDT TIMOUT!");

 	else if (state == 6)
		term_appendString("OPEN");
	else if (state == 7)
		term_appendString("CLOSED");

 	else if (state == 8)
		term_appendString("LOW");
	else if (state == 9)
		term_appendString("HIGH");
}

//these are used to temporarily disable the LCD updates such that it minimizes the glitches of the serial stream.
void term_LCDON(void)
{
	RS422_termOutLength = 0;
	LCD_updateEnabled = 1;
	term_appendString("\n\rLCD has been re-enabled (i.e., back to the default powerup state)\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}
void term_LCDOF(void)
{
	RS422_termOutLength = 0;
	LCD_updateEnabled = 0;
	term_appendString("\n\rLCD has been temporarily disabled\n\n\rBPS> ");
	term_StillSending = 0;
	term_location = 0;
	RS422_sendTerm();
}

//this will print out the reason for the trip
void term_TR(void)
{
	RS422_termOutLength = 0;
	term_StillSending = 0;
	term_location = 0;
	if(BPSstate == 3)
	{
		term_appendString("\n\n\rBPS has tripped due to ");
		if(tripStatus == 1)

		{
			term_appendString("overcurrent (Discharging) at ");
			term_appendCurrent((signed)tripValue);
		}
		else if(tripStatus == 2)
		{
			term_appendString("overcurrent (Charging)");
			term_appendCurrent((signed)tripValue);
		}
		else if(tripStatus == 3)
		{
			term_appendString("UnderTemperature on Cell # ");
			term_appendUint(tripCell);
			term_appendString(" with ");
			term_appendTemp(tripValue);
		}
		else if(tripStatus == 4)
		{
			term_appendString("OverTemperature on Cell # ");
			term_appendUint(tripCell);
			term_appendString(" with ");
			term_appendTemp(tripValue);
		}
		else if(tripStatus == 5)
		{
			term_appendString("UnderVoltage on Cell # ");
			term_appendUint(tripCell);
			term_appendString(" with ");
			term_appendVolt(tripValue);
		}
		else if(tripStatus == 6)
		{
			term_appendString("OverVOltage on Cell # ");
			term_appendUint(tripCell);
			term_appendString(" with ");
			term_appendVolt(tripValue);
		}
		else if (tripStatus == 7)
			term_appendString("External System Error (contactor, supply voltage, etc.)");
		else
			term_appendString("INTERNAL BPS FAILURE");
		term_appendString("\n\n\rBPS> ");
	}
	else if(BPSstate == 4)
		term_appendString("\n\n\rBPS has had an internal error\n\n\rBPS> ");
	else
		term_appendString("\n\n\rBPS is not in the tripped state\n\n\rBPS> ");
	RS422_sendTerm();
}

//shows the highest and lowest cell readings
void term_HL(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r===High and Low Realtime Readings===\n\rHigh Voltage cell:\t");
		term_appendInt(maxVcell);
		term_appendString("\tat\t");
		term_appendVolt(maxVoltage);
		term_appendString("\n\rLow Voltage cell:\t");
		term_appendInt(minVcell);
		term_appendString("\tat\t");
		term_appendVolt(minVoltage);
		term_sublocation = 1;
		term_StillSending = 1;
	}
	else if(term_sublocation == 1)
	{
		term_appendString("\n\rHigh Temperature cell:\t");
		term_appendInt(maxTcell);
		term_appendString("\tat\t");
		term_appendTemp(maxTemp);
		term_appendString("\n\rLow Temperature cell:\t");
		term_appendInt(minTcell);
		term_appendString("\tat\t");
		term_appendTemp(minTemp);
		term_appendString("\n\n\rBPS> ");
		term_StillSending = 0;
		term_sublocation = 0;
		term_location = 0;
	}

	RS422_sendTerm(); 
}

//shows the current BPS trip limits
void term_PL(void)
{
	RS422_termOutLength = 0;
	if(term_sublocation == 0)
	{
		term_appendString("\n\r===Current BPS Trip Limits===\n\rCharging Current:\t\t\t");
		term_appendCurrent((signed)reg_read(4,3));
		term_appendString("\n\rDischarging Current:\t\t\t");
		term_appendCurrent((signed)reg_read(4,4));
		term_appendString("\n\rHigh Voltage:\t\t\t\t");
		term_appendVolt(reg_read(4,1));
		term_appendString("\n\rLow Voltage:\t\t\t\t");
		term_appendVolt(reg_read(4,2));
		term_sublocation = 1;
		term_StillSending = 1;
	}
	else if(term_sublocation == 1)
	{
		term_appendString("\n\rOverTemperature(charging):\t\t");
		term_appendTemp(reg_read(4,5));
		term_appendString("\n\rUnderTemperature(charging):\t\t");
		term_appendTemp(reg_read(4,6));
		term_StillSending = 1;
		term_sublocation = 2;
	}
	else if(term_sublocation == 2)
	{
		term_appendString("\n\rOverTemperature(discharging):\t\t");
		term_appendTemp(reg_read(4,7));
		term_appendString("\n\rUnderTemperature(discharging):\t\t");
		term_appendTemp(reg_read(4,8));
		term_appendString("\n\n\rBPS> ");
		term_StillSending = 0;
		term_sublocation = 0;
		term_location = 0;
	}

	RS422_sendTerm(); 
}

//newline
