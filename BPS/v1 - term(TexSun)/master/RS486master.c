/********************************************
/RS485master.c
/Implements the RS485 bus master
/uses UART1 and TIMER2
/TXenable (active high) on RD0
/RXenable (active low) on RD1
********************************************/

#include <p30F6012A.h>

#define RS485_START_TOKEN 0x01
#define RS485_END_TOKEN 0xFE
#define RS485_PADDING 0xFF
#define RS485_TIMEOUT 25000 //10ms timeout
#define RS485_SLAVE_TIMEOUT 65000 //~25ms timeout

unsigned char RS485_outData[255];					//data to slaves
unsigned char RS485_inData[255];					//data from slaves
unsigned char RS485_command;						//master's command
unsigned char RS485_slave;							//slave we're talking to
unsigned char RS485_response;						//response from slaves
unsigned char RS485_outChecksum;					//checksum to send 
unsigned char RS485_inChecksum;						//checksum from slaves
unsigned char RS485_outLength;						//DATA length on commands
unsigned char RS485_inLength;						//DATA length on response DATA
unsigned char RS485_inDataPointer;					//array element pointer when receiver DATA from slvaes
unsigned char RS485_inStep;							//which section of packet we are receiving
unsigned char RS485_outStep;						//which section of packet we are sending
unsigned char RS485_outDataPointer;					//array element pointer when sending DATA to slaves
unsigned char RS485_goodPacket;						//slave sent a good packet
unsigned char RS485_receive;						//flag telling it to wait for a new packet to come in or just transmit 1 = works normally, 0 = just transmit (testing)
unsigned char RS485_status;							//0 = idle
													//1 = transmitting
													//2 = waiting on reception ("slave is thinking")
													//3 = receiving
													//4 = got good response
													//5 = inter-byte receive timeout
													//6 = corrupt packet received 
													//7 = timed out waiting for slave response						

void RS485_receiverReset(void);
void RS485_timerTest(void);
void RS485_transmitterReset(void);
void RS485_sendPacket(unsigned char waitResp);
void RS485_initialize(void);
unsigned char RS485_sendRetry(unsigned char numRetries);
unsigned char RS485_ready(void);
void RS485_test(void);
unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB);
unsigned char RS485_splitMSB(unsigned int input);
unsigned char RS485_splitLSB(unsigned int input);

void RS485_receiverReset(void)
{
	unsigned char junk;
	junk = U1RXREG;
	junk = U1RXREG;
	junk = U1RXREG;
	junk = U1RXREG;
	junk = U1RXREG;				//clear out the receiver buffer
	U1STAbits.OERR = 0;			//clear overflow flag
	RS485_goodPacket = 1;
	RS485_inDataPointer = 0;
	RS485_inStep = 0;
	RS485_inChecksum = 0;
	PORTDbits.RD0 = 0;	//TX off
	PORTDbits.RD1 = 0;	//RX on
	PORTD = PORTD&0xFFFC;
	IEC0bits.U1RXIE = 1;
	//PR2 = RS485_SLAVE_TIMEOUT;
	PR2 = 50000;
	TMR2 = 0;
	T2CONbits.TON = 1;
}

void RS485_transmitterReset(void)
{
	IEC0bits.U1RXIE = 0;
	PORTDbits.RD0 = 1;	//TX ON
		PORTDbits.RD0 = 1;	//TX ON
			PORTDbits.RD0 = 1;	//TX ON
				PORTDbits.RD0 = 1;	//TX ON
	PORTDbits.RD1 = 1;	//RX off
	PORTD = PORTD|0x0003;
	T2CONbits.TON = 0;
}

void RS485_sendPacket(unsigned char waitResp)
{
	RS485_outChecksum = 0;		//initialize checksum
	RS485_outStep = 0;			//start at step 0
	RS485_outDataPointer = 0;	//start at first byte of response data/payload array
	PORTDbits.RD0 = 1;	//TX on
	PORTDbits.RD1 = 1;	//RX off
	PORTD = PORTD|0x0003;
	RS485_receive = waitResp;
	RS485_status = 1;
	U1TXREG = RS485_PADDING;	//put start token in Transmit register
}

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


	IFS0bits.U1TXIF = 0;
	IFS0bits.U1RXIF = 0;
	IEC0bits.U1TXIE = 1;
	IEC0bits.U1RXIE = 1;
	TRISDbits.TRISD0 = 0;
	TRISDbits.TRISD1 = 0;
	PORTDbits.RD0 = 1;	//TX on			//rs485 master should idle with the transmitter enabled - that way the master keeps the bus held at a high so that the bus is not undriven (except during the master's wait for a slave response), preventing the slaves from getting false start bits
	PORTDbits.RD1 = 1;	//RX off
	PORTD = PORTD|0x0003;
}

void RS485_test(void)
{
}

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt(void)
{	//gets called whenever the TXbuffer is empty. so this code basically runs right when the previously sent byte's start bit heads down the wire
	//recently added a new first step. first we send a junk byte as the first byte of the packet. this gives the slave some time to breath and get ready
	//for the next master command if the master was wanting to send commands as fast as it could.
	IFS0bits.U1TXIF = 0;
	if(RS485_outStep == 0)						//just finished from step 0, outputting the start token
	{											//so we output the length
		U1TXREG = RS485_START_TOKEN;				//queue up the response packet length field
		RS485_outStep = 1;						//just finished step 1
		return;									//exit from ISR					
	}
	if(RS485_outStep == 1)						//just finished from step 0, outputting the start token
	{											//so we output the length
		U1TXREG = RS485_outLength;				//queue up the response packet length field
		RS485_outChecksum += RS485_outLength;	//add sent byte to checksum
		RS485_outStep = 2;						//just finished step 1
		return;									//exit from ISR					
	}
	else if (RS485_outStep == 2)				//just finished from step 1, outputting the length
	{											//so we send the master-ID code
		U1TXREG = RS485_slave;					//send out the slave-ID
		RS485_outChecksum += RS485_slave;		//add to checksum	
		RS485_outStep = 3;						//just finished step 2
		return;									//exit from ISR
	}
	else if (RS485_outStep == 3)				//just finished step 2, outputting the master-ID
	{											//now send the status
		U1TXREG = RS485_command;				//put response in serial port transmit waiting hold
		RS485_outChecksum += RS485_command;		//add to the checksum
		if (RS485_outLength == 0)				//check to see if there are any DATA/PAYLOAD byte(s) to send
			RS485_outStep = 5;					//if there aren't any, we just finished "sending" the non-existant data (step4), go to the step after send-DATA
		else	
			RS485_outStep = 4;					//just finished step 3, no goto step 4
		return;									//exit ISR
	}
	else if (RS485_outStep == 4)				//just finished sending the status-response byte, and there's data to send
	{											//now send response-DATA 
		if (RS485_outLength <= (RS485_outDataPointer+1))				//either only one byte for response-DATA, or need to send out last data byte
		{	
			U1TXREG = RS485_outData[RS485_outDataPointer];	//send data byte
			RS485_outChecksum += RS485_outData[RS485_outDataPointer];	//add data byte to the checksum
			RS485_outStep = 5;					//just finished sending the DATA, step 4, so goto step 5, sending checksum				
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
	else if (RS485_outStep == 5)				//just finished from step 4, outputting the DATA (or we skipped it)
	{											//so we send the checksum
		RS485_outChecksum = ~RS485_outChecksum; //complement the checksum
		U1TXREG = RS485_outChecksum;			//send out the checksum
		RS485_outStep = 6;						//just finished step 5
		return;									//exit from ISR
	}
	else if (RS485_outStep == 6)				//just finished from step 5, outputting the checksum
	{											//so we send the end-token
		U1TXREG = RS485_END_TOKEN;				//send out the end toekn
		RS485_outStep = 7;						//just finished step 6
		return;									//exit from ISR
	}
	else if (RS485_outStep == 7)				//just finished from step 1, outputting the end-toen
	{											//so we send a junk byte. this is because the interrupt is called when the TXREG is moved into the TX shift reg and TXreg becomes empty (i.e., when the start-bit of the previously sent byte goes out!)
		U1TXREG = 0;							//so we send another junk-ass byte after the actual last byte such that we can switch back to reveive mode as soon as the junk byte's start-bit hits the uart output pins
		RS485_outStep = 8;						//just finished step 2
		return;									//exit from ISR
	}
											//TXREG is empty, this means that end-token has been sent and that the junk-byte is about to be shifted out, so set to receive mode				

	if(RS485_receive)
	{
		RS485_status = 2;
		RS485_receiverReset();				//reset the receiver, get ready for the slave response (only if we expect a slave response
	}
	else
	{
		RS485_status = 0;
		RS485_transmitterReset();
	}	
	return;									//exit ISR, we're done sending the packet. the ISR shouldn't be called again unless the RS485_sendResponse is called since the UART TX is empty
	
}

//interrupt for when the RS485 receives a byte
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
	
	unsigned char temp;							//holds the last received byte
	//IEC0bits.U1RXIE = 0;	//disable receive interrupt while in interrupt handler
	IFS0bits.U1RXIF = 0;


	if (RS485_inStep == 0)
	{											//this is the first byte received
		temp = U1RXREG;
		RS485_status = 3;
		if (temp == RS485_START_TOKEN)
		{
			TMR2 = 0;							//reset the timer
			PR2 = RS485_TIMEOUT;				//switch the timer over from the (long) slave-thinking timer to the inter-byte timer
			RS485_inStep = 1;					//just finished the first step (start token)
			return;								//exit interrupt handler
		}
		else
		{
			RS485_status = 6;
			RS485_transmitterReset();			//bad packet, just reset everything
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
		return;									//exit ISR
	}
	else if (RS485_inStep == 2)
	{
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;
		if(temp == 0)					//packet is for the master?
		{									//yes it is for me
			RS485_inChecksum += temp;			//add to checksum
		}
		else
		{
			RS485_status = 6;
			RS485_goodPacket = 0;
		}
									//crap packet, no need to worry about checksum from now on....
		RS485_inStep = 3;
		return;
	}
	else if (RS485_inStep == 3)					//if we're on the response code step
	{
		TMR2 = 0;								//reset the timer
		temp = U1RXREG;
		if (RS485_goodPacket)					//only do stuff with the byte if it's a packet we care about
		{
			RS485_response = temp;				//set the command
			RS485_inChecksum += temp;			//add to checksum
		}
		if(RS485_inLength)						//if DATA field length non-zero
			RS485_inStep = 4;					//go to the data packet reception step
		else
			RS485_inStep = 5;					//if no data packet contents, skip that step
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
		return;
	} 
	else if (RS485_inStep == 6)
	{
		T2CONbits.TON = 0;								//turn off the timer
		if( RS485_goodPacket && (U1RXREG == RS485_END_TOKEN))		
		{	//we have a good packet here, process it
			RS485_status = 4;
			RS485_transmitterReset();
			//RS485_processPacket();						//TODO: do the packet command
		}
		else
		{	//we have a bad packet
			RS485_status = 6;
			RS485_transmitterReset();
		}
		return;
	}
	
	//shouldn't get here
	//unless this interrupt triggers while we are processing/sending a packet (shouldn't happen)
	T2CONbits.TON = 0;
	//RS485_receiverReset();
}


void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void)
{
	IFS0bits.T2IF = 0;
	T2CONbits.TON = 0;
	if(RS485_status == 2)	//if we're waiting for the slave to think
		RS485_status = 7;
	if(RS485_status == 3)	//if we're in the process of receiving bytes...
		RS485_status = 5;
	RS485_transmitterReset();
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
	return (unsigned char) input&0x00FF;
}

unsigned char RS485_splitLSB(unsigned int input)
{
	return (unsigned char) input&0x00FF;
}

//returns 1 if ready, 0 if busy (waiting or transmitting)
unsigned char RS485_ready(void)
{
	//TODO: fix this?
	return (RS485_status >= 4 || RS485_status == 0);
	//return RS485_status >= 4;
}

//return 0 for OK, 1 for error
unsigned char RS485_sendRetry(unsigned char numRetries)
{
}


//newline
