#include "globals.h"
#include "RS422.h"
#include "RS485master.h"	//need this for the MSB/LSB split/combine functions
#include "Mregisters.h"

//this file contains the functions that are used to pack up and send data to the BBone over the RS422
//also has the stuff to figure out what data to send next

unsigned char TX_waiting;	// 0 = nothing is waiting for a chance to send a packet, 1 = something is waiting to send something
unsigned char TX_count;		//number of sending loops (used to send lower-resolution data every few main cycles
unsigned char TX_step;		//which type of message we are about to send (voltage, temp, status, balance...)
unsigned char TX_sendingBatt;	//0 = not sending battery measurement data to BB, 1 = sending battery status packets
unsigned char TX_maxCycle;	//largest cycles per packet value
unsigned char TX_lastState;	//keeps track of what the last state of the send data subsystem is when there's a packet sending request from the main loop code
unsigned char TX_sendingMainline;		//flag is a 1 if we are sending something that the mainline code has requested.


void send_init(void)
{
	TX_step = 0;
	TX_count = 1;
	TX_waiting = 0;
	TX_sendingMainline = 0;
	TX_sendingBatt = 0;
	TX_maxCycle = reg_read(1,2);			//find the max value and put into TX_maxCycle
	if(reg_read(1,3) > TX_maxCycle)
		TX_maxCycle = reg_read(1,3);
	if(reg_read(1,4) > TX_maxCycle)
		TX_maxCycle = reg_read(1,4);
	if(reg_read(1,5) > TX_maxCycle)
		TX_maxCycle = reg_read(1,5);
	if(reg_read(1,6) > TX_maxCycle)
		TX_maxCycle = reg_read(1,6);
	if(reg_read(1,12) > TX_maxCycle)
		TX_maxCycle = reg_read(1,12);
}

//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
//sends the BPS status packet (and the system current)
void send_status(unsigned char whichArray)
{
	RS422_outResponse = 1;
	RS422_outLength = 3;
	RS422_outData[0] = BPSstate;
	if(balancingOn)
		RS422_outData[0] += 0b10000000;
	if(whichArray)
	{
		RS422_outData[1] = RS485_splitMSB(currentB);
		RS422_outData[2] = RS485_splitLSB(currentB);
	}
	else
	{
		RS422_outData[1] = RS485_splitMSB(currentA);
		RS422_outData[2] = RS485_splitLSB(currentA);
	}
	RS422_sendPacket();
}


//sends the State of Charge value
void send_SOC(void)
{
	RS422_outResponse = 11;
	RS422_outLength = 1;
	RS422_outData[0] = SOC_percent;
	if(SOC_initState == 4 || SOC_initState == 0)	//if SoC value was corrupt or SoC has not run yet.
		RS422_outData[0] = 255;						//then send 255 (real vlaues go from 0 to 100)
	RS422_sendPacket();
}

//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
//sends a voltage status packet	
void send_voltages(unsigned char whichArray)
{
	unsigned char numCells;
	unsigned char i;
	numCells = reg_read(3,1);
	RS422_outResponse = 2;
	RS422_outLength = 2*numCells;
	for(i = 0; i < numCells; i++)
	{
		if(whichArray)
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryVoltsB[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryVoltsB[i]);
		}
		else
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryVoltsA[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryVoltsA[i]);
		}
	}
	RS422_sendPacket();	
}
	
//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
//sends a temperature status packet
void send_temps(unsigned char whichArray)
{
	unsigned char numCells;
	unsigned char i;
	numCells = reg_read(3,1);
	RS422_outResponse = 3;
	RS422_outLength = 2*numCells;
	for(i = 0; i < numCells; i++)
	{
		if(whichArray)
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryTempsB[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryTempsB[i]);
		}
		else
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryTempsA[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryTempsA[i]);
		}
	}
	RS422_sendPacket();	
}	

//sends the balancing status
void send_balancing(void)
{
	//TODO: add code!
	RS422_outResponse = 4;
	RS422_sendPacket();	
}

//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B (note that setA and setB good are the ones for bootstrap, not the ones for the main readings!)
//sends a bootstrap update packet
void send_bootstrap(unsigned char whichSet)
{
	//RS485_response = 0;
	if(whichSet)
	{
		RS422_outData[0] = RS485_splitMSB(BTvlt);
		RS422_outData[1] = RS485_splitLSB(BTvlt);
		RS422_outData[2] = RS485_splitMSB(BTcur);
		RS422_outData[3] = RS485_splitLSB(BTcur);
		RS422_outData[4] = RS485_splitMSB(BTtmp);
		RS422_outData[5] = RS485_splitLSB(BTtmp);
		RS422_outData[6] = RS485_splitMSB(BTsup);
		RS422_outData[7] = RS485_splitLSB(BTsup);
		RS422_outData[8] = BTstatus;
		RS422_outData[9] = RS485_splitMSB(BBvlt);
		RS422_outData[10] = RS485_splitLSB(BBvlt);
		RS422_outData[11] = RS485_splitMSB(BBcur);
		RS422_outData[12] = RS485_splitLSB(BBcur);
		RS422_outData[13] = RS485_splitMSB(BBtmp);
		RS422_outData[14] = RS485_splitLSB(BBtmp);
		RS422_outData[15] = RS485_splitMSB(BBsup);
		RS422_outData[16] = RS485_splitLSB(BBsup);
		RS422_outData[17] = BBstatus;
	}
	else
	{
		RS422_outData[0] = RS485_splitMSB(ATvlt);
		RS422_outData[1] = RS485_splitLSB(ATvlt);
		RS422_outData[2] = RS485_splitMSB(ATcur);
		RS422_outData[3] = RS485_splitLSB(ATcur);
		RS422_outData[4] = RS485_splitMSB(ATtmp);
		RS422_outData[5] = RS485_splitLSB(ATtmp);
		RS422_outData[6] = RS485_splitMSB(ATsup);
		RS422_outData[7] = RS485_splitLSB(ATsup);
		RS422_outData[8] = ATstatus;
		RS422_outData[9] = RS485_splitMSB(ABvlt);
		RS422_outData[10] = RS485_splitLSB(ABvlt);
		RS422_outData[11] = RS485_splitMSB(ABcur);
		RS422_outData[12] = RS485_splitLSB(ABcur);
		RS422_outData[13] = RS485_splitMSB(ABtmp);
		RS422_outData[14] = RS485_splitLSB(ABtmp);
		RS422_outData[15] = RS485_splitMSB(ABsup);
		RS422_outData[16] = RS485_splitLSB(ABsup);
		RS422_outData[17] = ABstatus;
	}
	RS422_outLength = 18;
	RS422_outResponse = 5;
	RS422_sendPacket();	
}	

// called by the RS422 TX interrupt handler whenever it has completed sending a packet
// this code will figure out which packet to send net.
void send_next(void)
{
	unsigned char array = 0;
	unsigned char didntsend = 1;
	unsigned char stuckloopctr = 0;
	if (TX_waiting)		//if something from the main loop code is wanting to send a packet, we'l relinquish the bus to it.
	{
		TX_lastState = TX_sendingBatt;
		TX_sendingBatt = 0;
		TX_sendingMainline = 1;
		TX_waiting = 0;
		return;
	}
	if(TX_sendingMainline)	//called by the 422 tx interrupt handler at the end of sending a packet requested from mainloop
	{						//so keep sending packets before the mainloop code jutted in.
		TX_sendingMainline = 0;
		TX_sendingBatt = TX_lastState;
	}
	if((BPSstate == 0) || (BPSstate == 4))	//if BPS is not measuring cells, don't send anything
	{
		TX_sendingBatt = 0;
		return;
	}
	if(arrayAgood)
		array = 0;
	else if(arrayBgood)
		array = 1;
	else					//no good cell readings to send 
	{
		TX_sendingBatt = 0;
		return;	
	}
	while(didntsend && (stuckloopctr < 255))						//loops around until we have something to send!
	{																//stuck loop counter is for making sure we don't get stuck in an infinite loop here
		if((TX_step == 0) && (TX_count >= reg_read(1,2)))
		{
			if(reg_read(1,2))
			{
				didntsend = 0;
				send_status(array);
			}
			else
				TX_step++;
		}
		if((TX_step == 1) && (TX_count >= reg_read(1,3)))
		{
			if(reg_read(1,3))
			{
				didntsend = 0;
				send_voltages(array);
			}
			else
				TX_step++;
		}
		if((TX_step == 2) && (TX_count >= reg_read(1,4)))
		{
			if(reg_read(1,4))
			{
				didntsend = 0;
				send_temps(array);
			}
			else
				TX_step++;
		}
		if((TX_step == 3) && (TX_count >= reg_read(1,5)))
		{
			if(reg_read(1,5))
			{
				didntsend = 0;
				send_balancing();
			}
			else
				TX_step++;
		}
		if((TX_step == 4) && (TX_count >= reg_read(1,6)))
		{
			if(reg_read(1,6))
			{
				if(BSsetAGood)
				{
					didntsend = 0;
					send_bootstrap(0);
				}
				else if(BSsetBGood)
				{
					didntsend = 0;
					send_bootstrap(1);
				}
				else
					TX_step++;
			}
			else
				TX_step++;
		}
		if((TX_step == 5) && (TX_count >= reg_read(1,12)))
		{
			if(reg_read(1,12))
			{
				didntsend = 0;
				send_SOC();
			}
			else
				TX_step++;
		}
		TX_step++;
		if(TX_step >= 6)		//looked at each type of packet to send, so go to the next cycle
		{
			TX_step = 0;
			TX_count++;
			if(TX_count > TX_maxCycle)	//loop back around
				TX_count = 0;
		}
		stuckloopctr++;
	}
	if(didntsend)			//make sure to flag if we didn't send aything
		TX_sendingBatt = 0;
	return; 
}

//this function is used to kick off sending a packet.
void send_start(void)
{
	if(TX_sendingBatt)	//we're already sending battery data
		return;
	if(arrayAgood)
	{
		while(!RS422_ready());
		send_status(0);
		TX_sendingBatt = 1;
		return;
	}
	else if(arrayBgood)
	{
		while(!RS422_ready());
		send_status(1);
		TX_sendingBatt = 1;
		return;
	}
	else
	{
		TX_sendingBatt = 0;
		return;	
	}
}
