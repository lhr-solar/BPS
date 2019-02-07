/*******************************************************
/RS485slave.c
/Implements the slave RS485 bus on UART1
/receiver enable (active low) on RD1
/transmitter enable (active high) on RD0
/uses TIMER2 (for the receiver timout)
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>
#include "RS485slave.h"
#include "slave_misc.h"
#include "Sregisters.h"
#include "slave_globals.h"

#define RS485_START_TOKEN 0x01
#define RS485_END_TOKEN 0xFE
#define RS485_TIMEOUT 25000 						//10ms timeout

//Global globals
unsigned char RS485_outData[255];					//data to send back to master
unsigned char RS485_inData[255];					//data from master
unsigned char RS485_command;						//master's command
unsigned char RS485_response;						//slave's response code to master
unsigned char RS485_outLength;						//DATA length from master
unsigned char RS485_inLength;						//DATA length to master

//local globals
static unsigned char RS485_outChecksum;				//checksum to send to master
static unsigned char RS485_inChecksum;				//checksum from master
static unsigned char RS485_inDataPointer;			//array element pointer when receiver DATA from master
static unsigned char RS485_inStep;					//which section of packet we are receiving
static unsigned char RS485_outStep;					//which section of packet we are sending
static unsigned char RS485_outDataPointer;			//array element pointer when sending DATA to master
static unsigned char RS485_goodPacket;
static unsigned char RS485_myID;
static unsigned char RS485_transmitting;			//flag if currently transmitting (such that the receiver can ignore anything)

//local/private functions
static void RS485_receiverReset(void);
static void RS485_processPacket(void);
static unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB);
static unsigned char RS485_splitMSB(unsigned int input);
static unsigned char RS485_splitLSB(unsigned int input);

//init RS485
void RS485_initialize()
{
//set up the inter-byte receive timeout timer (Timer2)
	T2CONbits.TGATE = 0;	//don't gate it
	T2CONbits.TCKPS0 = 0;	//1:1 (no) prescaller
	T2CONbits.TCKPS1 = 0;
	T2CONbits.TCS = 0;		//use Fcy clock
	PR2 = RS485_TIMEOUT;	//load the timer period register
	IFS0bits.T2IF = 0;		//clear interrupt flag
	IEC0bits.T2IE = 1;		//enable interrupt

//Set U1MODE (UART1 Mode Register) options 
// <15>  UARTEN = 1  UART Enable bit
// <13>  USIDL  = 0  Continue operation in Idle mode
// <10>  ALTIO  = 0  UART communicates using UxTX and UxRX I/O pins
// <7>   WAKE   = 1  Wake-up on Start bit Detect During Sleep Mode enabled
// <6>   LPBACK = 0  Loopback mode is disabled
// <5>   ABAUD  = 1  Auto Baud Enable bit, Input to Capture module from UxRX pin
// <2:1> PDSEL  = 00 8-bit data, no parity
// <0>   STSEL  = 1  2 Stop bits
// U1MODE = 1000 0000 1010 0001
	U1MODE = 0x80A1;
	U1MODEbits.ABAUD = 0;

//Set U1STA (UART1 Status and Control Register) options
// <15>  UTXISEL = 1  Interrupt when a character is transferred to the Transmit Shift 
//                    register and as result, the transmit buffer becomes empty
// <11>  UTXBRK  = 0  U1TX pin operates normally
// <10>  UTXEN   = 1  UART transmitter enabled, UxTX pin controlled by UART (if UARTEN = 1)
// <7:6> URXISEL = 00 Interrupt flag bit is set when Receive Buffer receives 1 character(1 byte of data)
// <5>   ADDEN   = 0  Address Detect mode disabled. If 9-bit mode is not selected, this control bit has no effect.
// U1STA = 1000 0100 0000 0000
	U1STA = 0x8400;

//Set U1BRG (UART1 Baud Rate Register) options
// U1BRG = (Fcy/(16*BaudRate)) - 1 = 0 for 625 kbaud

	U1BRG = 4; //15 is close to 9600 buad


	RS485_myID = misc_readID();
	if(misc_readID() == 0)
		RS485_myID = reg_read(1,3);		//if the DIP switches are set to ID0 (master's code), it uses the pre-programmed ID, and not the DIP switch value
	IFS0bits.U1TXIF = 0;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1TXIE = 1;
	IEC0bits.U1RXIE = 1;
	TRISDbits.TRISD0 = 0;
	TRISDbits.TRISD1 = 0;
	RS485_receiverReset();
}

static void RS485_sendResponse()
{
	RS485_transmitting = 1;
	IEC0bits.U1RXIE = 0;
	RS485_outChecksum = 0;		//initialize checksum
	RS485_outStep = 0;			//start at step 0
	RS485_outDataPointer = 0;	//start at first byte of response data/payload array
	LATDbits.LATD1 = 1;			//turn off receiver
	LATDbits.LATD0 = 1;			//turn on transmitter		
	U1TXREG = RS485_START_TOKEN;	//put start token in Transmit register
}

//
void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt(void)
{	//gets called whenever the TXbuffer is empty. so this code basically runs right when the previously sent byte's start bit heads down the wire
	IFS0bits.U1TXIF = 0;
	if(RS485_outStep == 0)						//just finished from step 0, outputting the start token
	{											//so we output the length
		U1TXREG = RS485_outLength;				//queue up the response packet length field
		RS485_outChecksum += RS485_outLength;	//add sent byte to checksum
		RS485_outStep = 1;						//just finished step 1
		return;									//exit from ISR					
	}
	else if (RS485_outStep == 1)				//just finished from step 1, outputting the length
	{											//so we send the master-ID code
		U1TXREG = 0;							//send out the master-ID
												//no point in adding 0 to the checksum....
		RS485_outStep = 2;						//just finished step 2
		return;									//exit from ISR
	}
	else if (RS485_outStep == 2)				//just finished step 2, outputting the master-ID
	{											//now send the status
		U1TXREG = RS485_response;				//put response in serial port transmit waiting hold
		RS485_outChecksum += RS485_response;	//add to the checksum
		if (RS485_outLength == 0)				//check to see if there are any DATA/PAYLOAD byte(s) to send
			RS485_outStep = 4;					//if there aren't any, we just finished "sending" the non-existant data (step4), go to the step after send-DATA
		else	
			RS485_outStep = 3;					//just finished step 3, no goto step 4
		return;									//exit ISR
	}
	else if (RS485_outStep == 3)				//just finished sending the status-response byte, and there's data to send
	{											//now send response-DATA 
		if (RS485_outLength <= (RS485_outDataPointer+1))				//either only one byte for response-DATA, or need to send out last data byte
		{	
			U1TXREG = RS485_outData[RS485_outDataPointer];	//send data byte
			RS485_outChecksum += RS485_outData[RS485_outDataPointer];	//add data byte to the checksum
			RS485_outStep = 4;					//just finished sending the DATA, step 4, so goto step 5, sending checksum				
			return;								//exit ISR								
		}
		else									//we have at least one more byte to send after this
		{
			U1TXREG = RS485_outData[RS485_outDataPointer];	//send data byte
			RS485_outChecksum += RS485_outData[RS485_outDataPointer];	//add data byte to the checksum
			RS485_outDataPointer += 1;			//increment to the next data byte (to be sent next cycle)
			return;								//exit ISR
		}
	}
	else if (RS485_outStep == 4)				//just finished from step 4, outputting the DATA (or we skipped it)
	{											//so we send the checksum
		RS485_outChecksum = ~RS485_outChecksum; //complement the checksum
		U1TXREG = RS485_outChecksum;			//send out the checksum
		RS485_outStep = 5;						//just finished step 5
		return;									//exit from ISR
	}
	else if (RS485_outStep == 5)				//just finished from step 5, outputting the checksum
	{											//so we send the end-token
		U1TXREG = RS485_END_TOKEN;				//send out the end toekn
		RS485_outStep = 6;						//just finished step 6
		return;									//exit from ISR
	}
	else if (RS485_outStep == 6)				//just finished from step 1, outputting the end-toen
	{											//so we send a junk byte. this is because the interrupt is called when the TXREG is moved into the TX shift reg and TXreg becomes empty (i.e., when the start-bit of the previously sent byte goes out!)
		U1TXREG = 0;							//so we send another junk-ass byte after the actual last byte such that we can switch back to reveive mode as soon as the junk byte's start-bit hits the uart output pins
		RS485_outStep = 7;						//just finished step 2
		return;									//exit from ISR
	}
											//TXREG is empty, this means that end-token has been sent and that the junk-byte is about to be shifted out, so set to receive mode				
	RS485_receiverReset();					//reset the receiver, get ready for next command
	return;									//exit ISR, we're done sending the packet. the ISR shouldn't be called again unless the RS485_sendResponse is called since the UART TX is empty
	
}

//interrupt for when the RS485 receives a byte
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
	
	unsigned char temp;							//holds the last received byte
		IEC0bits.U1RXIE = 0;
	IFS0bits.U1RXIF = 0;
	//if(RS485_transmitting)
		//return;
	if(LATDbits.LATD0)
	{
		T2CONbits.TON = 0;
		RS485_inStep = 0;
			IEC0bits.U1RXIE = 1;
		return;
	}	
	if (RS485_inStep == 0)
	{											//this is the first byte received
		temp = U1RXREG;
		if (temp == RS485_START_TOKEN)
		{
			TMR2 = 0;							//reset the timer
			T2CONbits.TON = 1;						//turn on the timer.
			RS485_inStep = 1;					//just finished the first step (start token)
			IEC0bits.U1RXIE = 1;
			return;								//exit interrupt handler
		}
		else
		{
			RS485_receiverReset();				//bad packet, just reset everything
			return;								//exit interrupt handler
		}
	}
	else if (RS485_inStep == 1)
	{											//this is the second (length) byte
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;							//read out of the receiver  buffer (can only do once, as its a FIFO?, hence the temp variable)
		RS485_inLength = temp;					//set packet length
		RS485_inChecksum += temp;				//add to the checksum
		RS485_inStep = 2;						//just finished the second step (length)
		IEC0bits.U1RXIE = 1;
		return;									//exit ISR
	}
	else if (RS485_inStep == 2)
	{
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;
		if(temp == RS485_myID)					//packet is for me?
		{										//yes it is for me
			RS485_inChecksum += temp;			//add to checksum
		}
		else									//for someone else
		{										//this is a bad/junk packet from this board's POV
			RS485_goodPacket = 0;				//this is a bad packet, but we'll read all of it (not putting it in the registers), such that we don't look for a new start token until after this one's thru
		}										//crap packet, no need to worry about checksum from now on....
		RS485_inStep = 3;
		IEC0bits.U1RXIE = 1;
		return;
	}
	else if (RS485_inStep == 3)					//if we're on the command step
	{
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;
		if (RS485_goodPacket)					//only do stuff with the byte if it's a packet we care about
		{
			RS485_command = temp;				//set the command
			RS485_inChecksum += temp;			//add to checksum
		}
		if(RS485_inLength)						//if DATA field length non-zero
			RS485_inStep = 4;					//go to the data packet reception step
		else
			RS485_inStep = 5;					//if no data packet contents, skip that step
		IEC0bits.U1RXIE = 1;
		return;	
	}
	else if (RS485_inStep == 4)
	{
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;
		if (RS485_goodPacket)
		{
			RS485_inChecksum += temp;
			RS485_inData[RS485_inDataPointer] = temp;
		}
		RS485_inDataPointer += 1;
		if(RS485_inDataPointer >= RS485_inLength)	//did we get the last data byte?
			RS485_inStep = 5;						//then go to the next step
		IEC0bits.U1RXIE = 1;
		return;										//othewise just come back here and grab the next data byte
	}
	else if (RS485_inStep == 5)
	{
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;
		RS485_inChecksum = ~RS485_inChecksum;		//bitwise invert the checksum 
		if(temp != RS485_inChecksum)				//if the checksums don't match
			RS485_goodPacket = 0;					//then the packet is no good
		RS485_inStep = 6;	
		IEC0bits.U1RXIE = 1;	
		return;
	} 
	else if (RS485_inStep == 6)
	{
		T2CONbits.TON = 0;								//turn off the timer
		if( RS485_goodPacket && (U1RXREG == RS485_END_TOKEN))		
		{	//we have a good packet here, process it
			RS485_processPacket();						//do the packet command
		}
		else
		{	//we have a bad packet
			RS485_receiverReset();
		}
		IEC0bits.U1RXIE = 1;
		return;
	}
	
	//shouldn't get here
	//unless this interrupt triggers while we are processing/sending a packet (shouldn't happen)
	T2CONbits.TON = 0;
	RS485_receiverReset();
}


static void RS485_processPacket()
{
	char valid = 0;
	unsigned int temp;
	unsigned char i;
	unsigned char readno;
	if (RS485_command == 1)			//IDENT
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			RS485_response = 1;
			RS485_outLength = 0;
		}
	}
	else if (RS485_command == 2)		//set register
	{
		if(RS485_inLength == 4)
		{
			valid = 1;
			RS485_response = reg_write(RS485_inData[0], RS485_inData[1],RS485_combineBytes(RS485_inData[2],RS485_inData[3]));
			RS485_outLength = 0;
		}
	}
	else if (RS485_command == 3)		//read register
	{
		if(RS485_inLength == 2)
		{
			valid = 1;
			RS485_response = reg_invalid(RS485_inData[0], RS485_inData[1]);
			RS485_outLength = 2;
			if(RS485_response == 0)
			{
				temp = reg_read(RS485_inData[0], RS485_inData[1]);
				RS485_outData[0] = RS485_splitMSB(temp);
				RS485_outData[1] = RS485_splitLSB(temp);
			}
			else
			{
				RS485_outData[0] = 0;
				RS485_outData[1] = 0;
			}
		}
	}
	else if (RS485_command == 4)		//start acquisition
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			acquisition_enable = 1; //will start to acquire values for temperature and voltage
			RS485_response = 0;
			RS485_outLength = 0;
		}
	}
	else if (RS485_command == 5)		//stop acquisition
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			acquisition_enable = 0; //will start to acquire values for temperature and voltage
			RS485_response = 0;
			RS485_outLength = 0;
		}
	}
	else if (RS485_command == 6)		//reboot
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			RS485_outLength = 0;
			asm("reset");				//reset the uC
		}
	}
	else if (RS485_command == 7)		//read voltages
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			if(arrayAgood)
			{
				readno = reg_read(1,1);			//number of voltages to read
				RS485_outLength = 2*readno;
				RS485_response = 0;
				for(i = 0; i < readno; i++)
				{
					RS485_outData[2*i]   = RS485_splitMSB(voltArrayA[i]);
					RS485_outData[2*i+1] = RS485_splitLSB(voltArrayA[i]);
				}
			}
			else if(arrayBgood)
			{
				readno = reg_read(1,1);
				RS485_outLength = 2*readno;
				RS485_response = 0;
				for(i = 0; i < readno; i++)
				{
					RS485_outData[2*i]   = RS485_splitMSB(voltArrayB[i]);
					RS485_outData[2*i+1] = RS485_splitLSB(voltArrayB[i]);
				}
			}
			else
			{
				//TODO: send propper fault codes on LTC error, etc..
				RS485_outLength = 0;
				RS485_response = 1;			//no data is available error
			}	
		}
	}
	else if (RS485_command == 8)		//read temp
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			if(arrayAgood)
			{
				readno = reg_read(1,2);					//number of temperature sensors
				RS485_outLength = 2*readno;
				RS485_response = 0;
				for(i = 0; i < readno; i++)
				{
					RS485_outData[2*i]   = RS485_splitMSB(tempArrayA[i]);
					RS485_outData[2*i+1] = RS485_splitLSB(tempArrayA[i]);
				}
			}
			else if(arrayBgood)
			{
				readno = reg_read(1,2);
				RS485_outLength = 2*readno;
				RS485_response = 0;
				//for(i = 0; i < readno; i++)
				for(i = 0; i < readno; i++)
				{
					RS485_outData[2*i]   = RS485_splitMSB(tempArrayB[i]);
					RS485_outData[2*i+1] = RS485_splitLSB(tempArrayB[i]);
				}
			}
			else
			{
				RS485_outLength = 0;
				RS485_response = 1;			//no data is available error
			}	
		}
	}
	else if (RS485_command == 9)		//set balancing
	{
		//#TODO: program me!
		valid = 1;
	}
	else if (RS485_command == 10)		//read health
	{
	}
	else if (RS485_command == 100)		//blinkLEDs
	{
		misc_setLEDread(1);
		valid = 1;
		RS485_response = 1;
		RS485_outLength = 0;
	}
	else if (RS485_command == 150)		//blinkLEDs (A = on)
	{
		misc_setLEDA(1);
		valid = 1;
		RS485_response = 1;
		RS485_outLength = 0;
	}
	else if (RS485_command == 160)		//blinkLEDs (A = off)
	{
		misc_setLEDA(0);
		valid = 1;
		RS485_response = 1;
		RS485_outLength = 0;
	}
	else								//unrecognized command
	{
	}
	if(valid)
		RS485_sendResponse();
	else
		RS485_receiverReset();
}

static void RS485_receiverReset()
{
	unsigned char junk;
	RS485_inChecksum = 0;					//reset the checksum acumulator
	RS485_inDataPointer = 0;				//reset the DATA pointer
	RS485_inStep = 0;						//reset the step of the decoder
	RS485_goodPacket = 1;					//assume the incomming packet is good unless it isn't
	junk = U1RXREG;
	junk = U1RXREG;
	junk = U1RXREG;
	junk = U1RXREG;		//clear out the 4-deep receive FIFO
	junk = U1RXREG;
	U1STAbits.OERR = 0; 	//clear overflow flag
	LATDbits.LATD1 = 0;						//turn on receiver
	LATDbits.LATD0 = 0;						//turn off transmitter
	T2CONbits.TON = 0;							//turn off the timer
	RS485_transmitting = 0;
	IEC0bits.U1RXIE = 1;
}

//timer 2 interrupt. this one fires for a timeout when receiving 
//there's only a timout between bytes
void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0;
	T2CONbits.TON = 0;
//	if(!LATDbits.LATD0)
	if(!RS485_transmitting)
		RS485_receiverReset();
}

static unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB)
{
	unsigned int out = 0;
	out += MSB;
	out = out << 8;
	out = out & 0xFF00;
	out += LSB;
	return out;
}

static unsigned char RS485_splitMSB(unsigned int input)
{
	input = input & 0xFF00;
	input = input >> 8;
	return input&0x00FF;
}

static unsigned char RS485_splitLSB(unsigned int input)
{
	return input&0x00FF;
}

//newline, damnit!
