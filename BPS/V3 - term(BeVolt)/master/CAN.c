#include "CAN.h"
#include <p30F6012A.h>
#include "globals.h"
#include "RS485master.h" //use this to get those RS485_split and combine MSB/LSB functions
#include "Mregisters.h"

//uses CAN1 module
//1Mbit

static void CAN_setTXID(unsigned char type, unsigned char subcode);
static void CAN_sendNext(void);
static void CAN_assembleData8(unsigned char D0, unsigned char D1,unsigned char D2,unsigned char D3,unsigned char D4,unsigned char D5,unsigned char D6,unsigned char D7);
static void CAN_assembleData4(unsigned char D0, unsigned char D1,unsigned char D2,unsigned char D3);
static void CAN_sendArray(void);
static void CAN_processPacket(void);

unsigned char can_base_address = 0;	//highest 4 bits of the CAN bus address. # from 0 to 16);
unsigned int CAN_buffer[128];
unsigned char CAN_outStep;		//0 = ready (or done)
								//1 = sending status
								//2 = sending volts
								//3 = sending temps
unsigned char CAN_outSubStep;

void CAN_initialize(void)
{
	C1CTRL = 0x0400;		//no can capture, FCAN = 4Fcy = 10Mhz, put into CONFIG mode
	while(C1CTRLbits.OPMODE != 0b100);	//spin until CAN is in config mode
	C1CFG1 = 0x0000;		//prescaler set to 1 (CAN clock = Fosc/2), sync jump  = 1 TQ
	C1CFG2 = 0x0180;		// Phase seg  = 1TQ, SEG1 = 1TQ, SEG2 = 2TQ
	
	can_base_address = reg_read(6,1);
	can_base_address = (can_base_address & 0x0F);	//mask off high bits
	
	C1RXM0SID = 0x1F81;						//look at the first 6 bits, also MIDE = 1 (only match 11 bit frames)
	C1RXF0SID = (can_base_address<<9);		//set the base address (all others are zero) EXIDE = 0 = 11bit
	C1RXF1SID = (can_base_address<<9);		//seems I can't disable the other filter, so i'l just set them the same... (lower number one has priority)

	C1TX0CONbits.TXPRI = 1;	//low priority transmit buffer 0
	C1TX0DLCbits.TXRTR = 0;	//normal (non remote) frame tranmits from TXBUF0

	C1INTEbits.TX0IE = 1;	//enable interrrupt for TX and RX buffer 0
	C1INTEbits.RX0IE = 1;
	IFS1bits.C1IF = 0;		//clear interrupt flag and enable CAN interrupt
	IEC1bits.C1IE = 1;	


	C1CTRLbits.REQOP = 0;	//put into normal mode
	while(C1CTRLbits.OPMODE != 0);	//spin until CAN turns itself on
	CAN_outStep = 0;
}


void __attribute__((__interrupt__, auto_psv)) _C1Interrupt(void)
{
	IFS1bits.C1IF = 0;
	if(C1INTFbits.RX0IF)	//	RXbuf0 interrupt
	{
		CAN_processPacket();
		C1RX0CONbits.RXFUL = 0;	
		C1INTFbits.RX0IF = 0;
	}
	if(C1INTFbits.TX0IF)	//	RTXuf0 interrupt
	{
		C1INTFbits.TX0IF = 0;	//can't just clear the master interrupt, also need to clear teh individual CAN bus flags
		CAN_sendNext();
	}
}

static void CAN_setTXID(unsigned char type, unsigned char subcode)
{
	unsigned int msgID = 0;
	msgID = msgID | (can_base_address<<7);
	type = type & 0x0F;	//mask off high bits
	msgID = msgID | (type<<5);
	subcode = subcode & 0x1F;	//mask off the high bits
	msgID = msgID | subcode;
	C1TX0SID = ((msgID&0x3F)<<2);	//plug in the first 6 bits, no remote frame, std. 11 bit ID
	C1TX0SID |= ((msgID&0x07C0)<<5);
}

void CAN_StartSend(void)
{
	if(CAN_outStep != 0)		//CAN isn't ready
		return;
	CAN_outStep = 1;
	CAN_outSubStep = 0;
	CAN_sendNext();
}

static void CAN_sendNext(void)
{
	unsigned char tempLSB, tempMSB;
	unsigned char sent = 0;
	unsigned char done = 0;
	unsigned char stuckLoopCntr = 0;		//make sure do-while is not an infinite loop
	unsigned char i;
	if(CAN_outStep == 0)
		return;
	do
	{
		if(CAN_outStep == 1)
		{
			if(CAN_outSubStep == 0)		//status frame	
			{
				CAN_setTXID(0,0);
				C1TX0DLCbits.DLC = 4;
				if(arrayAgood)
				{
					tempLSB = RS485_splitLSB(currentA);
					tempMSB = RS485_splitMSB(currentA);
				}
				else if (arrayBgood)
				{
					tempLSB = RS485_splitLSB(currentB);
					tempMSB = RS485_splitMSB(currentB);
				}
				else
				{
					tempLSB = 0;
					tempMSB = 0;
				}
				CAN_assembleData4(BPSstate,SOC_percent,tempLSB, tempMSB);
				sent = 1;
				CAN_outSubStep++;
			}
			else if(CAN_outSubStep == 1)	//high/lows
			{
				if(BPSstate>=1 && BPSstate < 4)		//only send the high/low frame when BPS is measuring.
				{
					CAN_setTXID(0,1);
					//TODO - send out the highLOW data
					//sent = 1;
					sent = 0;		//take out this one when highlow data is implemented (otherwise can will stall out if sent is asserted without anything ebing sent)
				}
				CAN_outStep = 2;
				CAN_outSubStep = 0;
			}	
		}
		else if(CAN_outStep == 2)	//send volts data
		{
			if(CAN_outSubStep<<2 > numCells)	//we send up to 4 readings per frame. if we just sent the final frame, goto next data to send...
			{							
				CAN_outStep = 3;
				CAN_outSubStep = 0;	
				sent = 0;
			}
			if(CAN_outSubStep == 0)		//first one copies in the array
			{
				if(BPSstate>=1 && BPSstate < 4)		//only send volts when BPS is in measure mode
				{
					if(arrayAgood)
					{
						sent = 1;
						for(i = 0; i < numCells; i++)
							CAN_buffer[i] = batteryVoltsA[i];
					}
					else if(arrayBgood)
					{
						sent = 1;
						for(i = 0; i < numCells; i++)
							CAN_buffer[i] = batteryVoltsB[i];
					}
					if(sent)	//don't send anything if there's no good data
					{
						CAN_sendArray();
						CAN_outSubStep++;
					}
					else
						CAN_outStep++;		//goto temps if no volts data
				}
			}
			else	//any volts packet that is not not the first
			{
				CAN_sendArray();
				sent = 1;
				CAN_outSubStep++;
			}
		}
		else if(CAN_outStep == 3)	//send temp data
		{
			if(CAN_outSubStep<<2 > numCells)	//we send up to 4 readings per frame. if we just sent the final frame, goto next data to send...
			{							
				CAN_outStep = 0;		//OK, so we just say that we're idle and break out of the sending data interrupt loop...
				CAN_outSubStep = 0;	
				return;
			}
			if(CAN_outSubStep == 0)		//first one copies in the array
			{
				if(BPSstate>=1 && BPSstate < 4)		//only send temps when BPS is in measure mode
				{
					if(arrayAgood)
					{
						sent = 1;
						for(i = 0; i < numCells; i++)
							CAN_buffer[i] = batteryTempsA[i];
					}
					else if(arrayBgood)
					{
						sent = 1;
						for(i = 0; i < numCells; i++)
							CAN_buffer[i] = batteryTempsB[i];
					}
					if(sent)	//don't send anything if there's no good data
					{
						CAN_sendArray();
						CAN_outSubStep++;
					}
					else
					{
						CAN_outStep = 0;			//just stop this cycle of sending of no volts data
						CAN_outSubStep = 0;	
						return;
					}
				}
			}
			else	//any temps packet that is not not the first
			{
				CAN_sendArray();
				sent = 1;
				CAN_outSubStep++;
			}
		}
		else		//done with this update cycle
		{
			done = 1;
			CAN_outStep = 0;
		}
		stuckLoopCntr++;
	} while (sent == 0 && stuckLoopCntr < 200 && done == 0);		//done is a flag to break out of the loop and NOT transmit a packet
	if(!done)
		C1TX0CONbits.TXREQ = 1;		//request the message be sent	
}

static void CAN_assembleData8(unsigned char D0, unsigned char D1,unsigned char D2,unsigned char D3,unsigned char D4,unsigned char D5,unsigned char D6,unsigned char D7)
{	//transmitt buffers annoying put two bytes in each 16 bit register...
	C1TX0B1 = RS485_combineBytes(D1,D0);
	C1TX0B2 = RS485_combineBytes(D3,D2);
	C1TX0B3 = RS485_combineBytes(D5,D4);
	C1TX0B4 = RS485_combineBytes(D7,D6);
}

static void CAN_assembleData4(unsigned char D0, unsigned char D1,unsigned char D2,unsigned char D3)
{
	C1TX0B1 = RS485_combineBytes(D1,D0);
	C1TX0B2 = RS485_combineBytes(D3,D2);
}

static void CAN_sendArray(void)
{
	if(CAN_outStep*4 >= 128)		//error fuckup frame
	{
		CAN_assembleData8(0,0,0,0,0,0,0,0);
		CAN_setTXID(CAN_outStep, CAN_outSubStep);
		C1TX0DLCbits.DLC = 8;
	}
	else	//here the 16 bit registers actually make it easier
	{
		C1TX0B1 = CAN_buffer[CAN_outStep*4 + 0];
		C1TX0B2 = CAN_buffer[CAN_outStep*4 + 2];
		C1TX0B3 = CAN_buffer[CAN_outStep*4 + 3];
		C1TX0B4 = CAN_buffer[CAN_outStep*4 + 4];
		if(CAN_outStep*4 < numCells)
			C1TX0DLCbits.DLC = 8;
		else
			C1TX0DLCbits.DLC = (numCells-((CAN_outStep-1)*4));
	}
}

static void CAN_processPacket(void)
{
}


//unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB)
//fuckin' newline
