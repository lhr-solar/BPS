/*******************************************************
/RS485slave.c
/Implements the slave RS485 bus on UART1
/receiver enable (active low) on RD1
/transmitter enable (active high) on RD0
/uses TIMER2 (for the receiver timout)
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>

#define RS485_START_TOKEN 0x01
#define RS485_END_TOKEN 0xFE
#define RS485_TIMEOUT 25000 //10ms timeout

unsigned char RS485_outData[255];					//data to send back to master
unsigned char RS485_inData[255];					//data from master
unsigned char RS485_command;						//master's command
unsigned char RS485_response;						//slave's response code to master
unsigned char RS485_outChecksum;					//checksum to send to master
unsigned char RS485_inChecksum;						//checksum from master
unsigned char RS485_outLength;						//DATA length from master
unsigned char RS485_inLength;						//DATA length to master
unsigned char RS485_inDataPointer;					//array element pointer when receiver DATA from master
unsigned char RS485_inStep;							//which section of packet we are receiving
unsigned char RS485_outStep;						//which section of packet we are sending
unsigned char RS485_outDataPointer;					//array element pointer when sending DATA to master
unsigned char RS485_goodPacket;
unsigned char RS485_myID;
unsigned char RS485_transmitting;					//flag if currently transmitting (such that the receiver can ignore anything)

extern unsigned int ATcur;			//top current
extern unsigned int ATsup;			//top 24V supply
extern unsigned int ATvlt;			//top voltage
extern unsigned int ATtmp;			//top temperature
extern unsigned int A5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
extern unsigned int ABcur;			//bottom current
extern unsigned int ABsup;			//bottom supply
extern unsigned int ABvlt;			//bottom volt
extern unsigned int ABtmp;			//bottom temp
extern unsigned int A15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int A15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int AVsup;			//12-15V supply from the master to power the slaves
extern unsigned int Atemp;			//temperature of the bootstrap slave logic board
extern unsigned char ATstatus;		//status of the top charger LEDs
extern unsigned char ABstatus;		//status of the bottom charger LEDs
//set B
extern unsigned int BTcur;			//top current
extern unsigned int BTsup;			//top 24V supply
extern unsigned int BTvlt;			//top voltage
extern unsigned int BTtmp;			//top temperature
extern unsigned int B5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
extern unsigned int BBcur;			//bottom current
extern unsigned int BBsup;			//bottom supply
extern unsigned int BBvlt;			//bottom volt
extern unsigned int BBtmp;			//bottom temp
extern unsigned int B15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int B15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int BVsup;			//12-15V supply from the master to power the slaves
extern unsigned int Btemp;			//temperature of the bootstrap slave logic board
extern unsigned char BTstatus;		//status of the top charger LEDs
extern unsigned char BBstatus;		//status of the bottom charger LEDs

extern unsigned char setAGood;		
extern unsigned char setBGood;

void RS485_receiverReset(void);
void RS485_processPacket(void);
void RS485_initialize(void);
void RS485_test(void);
unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB);
unsigned char RS485_splitMSB(unsigned int input);
unsigned char RS485_splitLSB(unsigned int input);




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
	U1BRG = 4; //15 isclose to 9600 buad


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

void RS485_sendResponse()
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
	
	unsigned char temp;						//holds the last received byte
	unsigned char t;
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


void RS485_processPacket()
{
	char valid = 0;
	unsigned int temp;
	unsigned char i;
	unsigned char readno;
	char t;
	t++;
	t++;
	if (RS485_command == 1)			//IDENT
	{
		if(RS485_inLength == 0)
		{
			valid = 1;
			RS485_response = 2;	//2 = bootstrap board
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
	else if (RS485_command == 4)		//start acquisition (not on bootstrap board
	{
			valid = 0;
	}
	else if (RS485_command == 5)		//stop acquisition (not on bootstrap board
	{
			valid = 0;
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
	else if (RS485_command == 7)		//read voltages (not on bootstrap board)
	{
		valid = 0;
	}
	else if (RS485_command == 8)		//read temp (not on bootstrap board)
	{
		valid = 0;
	}
	else if (RS485_command == 9)		//set balancing (not on bootstrap board)
	{
		valid = 0;
	}
	else if (RS485_command == 10)		//read health
	{
	}
	else if (RS485_command == 11)		//read bootstrap
	{
		/*
	extern unsigned int ATcur;			//top current
extern unsigned int ATsup;			//top 24V supply
extern unsigned int ATvlt;			//top voltage
extern unsigned int ATtmp;			//top temperature
extern unsigned int A5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
extern unsigned int ABcur;			//bottom current
extern unsigned int ABsup;			//bottom supply
extern unsigned int ABvlt;			//bottom volt
extern unsigned int ABtmp;			//bottom temp
extern unsigned int A15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int A15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int AVsup;			//12-15V supply from the master to power the slaves
extern unsigned int Atemp;			//temperature of the bootstrap slave logic board
extern unsigned char ATstatus;		//status of the top charger LEDs
extern unsigned char ABstatus;		//status of the bottom charger LEDs
*/		if(RS485_inLength == 0)
		{
			valid = 1;
			if(setAGood)
			{
				RS485_response = 0;
				RS485_outLength = 18;
				RS485_outData[0] = RS485_splitMSB(ATvlt);
				RS485_outData[1] = RS485_splitLSB(ATvlt);
				RS485_outData[2] = RS485_splitMSB(ATcur);
				RS485_outData[3] = RS485_splitLSB(ATcur);
				RS485_outData[4] = RS485_splitMSB(ATtmp);
				RS485_outData[5] = RS485_splitLSB(ATtmp);
				RS485_outData[6] = RS485_splitMSB(ATsup);
				RS485_outData[7] = RS485_splitLSB(ATsup);
				RS485_outData[8] = ATstatus;
				RS485_outData[9] = RS485_splitMSB(ABvlt);
				RS485_outData[10] = RS485_splitLSB(ABvlt);
				RS485_outData[11] = RS485_splitMSB(ABcur);
				RS485_outData[12] = RS485_splitLSB(ABcur);
				RS485_outData[13] = RS485_splitMSB(ABtmp);
				RS485_outData[14] = RS485_splitLSB(ABtmp);
				RS485_outData[15] = RS485_splitMSB(ABsup);
				RS485_outData[16] = RS485_splitLSB(ABsup);
				RS485_outData[17] = ABstatus;
			}
			else if (setBGood)
			{
				RS485_response = 0;
				RS485_outLength = 18;
				RS485_outData[0] = RS485_splitMSB(BTvlt);
				RS485_outData[1] = RS485_splitLSB(BTvlt);
				RS485_outData[2] = RS485_splitMSB(BTcur);
				RS485_outData[3] = RS485_splitLSB(BTcur);
				RS485_outData[4] = RS485_splitMSB(BTtmp);
				RS485_outData[5] = RS485_splitLSB(BTtmp);
				RS485_outData[6] = RS485_splitMSB(BTsup);
				RS485_outData[7] = RS485_splitLSB(BTsup);
				RS485_outData[8] = BTstatus;
				RS485_outData[9] = RS485_splitMSB(BBvlt);
				RS485_outData[10] = RS485_splitLSB(BBvlt);
				RS485_outData[11] = RS485_splitMSB(BBcur);
				RS485_outData[12] = RS485_splitLSB(BBcur);
				RS485_outData[13] = RS485_splitMSB(BBtmp);
				RS485_outData[14] = RS485_splitLSB(BBtmp);
				RS485_outData[15] = RS485_splitMSB(BBsup);
				RS485_outData[16] = RS485_splitLSB(BBsup);
				RS485_outData[17] = BBstatus;
			}
			else		//fault, no good data
			{
				RS485_response = 1;
				RS485_outLength = 0;
			}
		}
	}
	else if (RS485_command == 100)		//blinkLEDs
	{
		misc_setLEDread(1);
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

void RS485_receiverReset()
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
	if(!LATDbits.LATD0)
		RS485_receiverReset();
}

unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB)
{
	unsigned int out = 0;
	out += MSB;
	out = out << 8;
	out = out & 0xFF00;
	out += LSB;
	return out;
}

unsigned char RS485_splitMSB(unsigned int input)
{
	input = input & 0xFF00;
	input = input >> 8;
	return input&0x00FF;
}

unsigned char RS485_splitLSB(unsigned int input)
{
	return input&0x00FF;
}


void RS485_test()
{
	RS485_response = 0x01;
	RS485_outLength = 10;
	RS485_outData[0] = 'U';
	RS485_outData[1] = 'T';
	RS485_outData[2] = 'S';
	RS485_outData[3] = 'V';
	RS485_outData[4] = 'T';
	RS485_outData[5] = ' ';
	RS485_outData[6] = 'B';
	RS485_outData[7] = 'P';
	RS485_outData[8] = 'S';
	RS485_outData[9] = '!';
	RS485_sendResponse();
}

//newline, damnit!
