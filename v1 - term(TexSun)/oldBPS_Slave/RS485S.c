//***************************************************
//dsPIC30F6012A RS485 Slave board Driver Source Code File
//Archer Finley
//BPS-UTSVT
//March 30,2010
//Description: Source code for drivers for the RS485 module on the dsPIC30F6012A(Slave Board)
//Filename: RS485S.c

#include <p30Fxxxx.h>

//*********RS485S_Init***********
//Description: Initializes slave board dsPIC for 
//communication with the master board dsPIC using
//UART and RS-485 communication protocol
//Inputs: none
//Outputs: none
void RS485S_Init(void) {

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
  U1BRG = 0;

//Turn on /RE (PORTD<1> located on RS-485 chip.
  TRISD = TRISD&0xFFC;
  LATD = LATD&0xFFC;

//Specify the interrupt priority for the interrupt using the
//U1RXIP<2:0> control bits in the IPC2 (Interrupt Priority Control 2) register
  IPC2bits.U1RXIP = 4;

//Clear the interrupt flag status bit associated with the peripheral in the IFS0 Status register
  IFS0bits.U1RXIF = 0;

//Set the U1RXIE bit in the IEC0 (Interrupt Enable Control 0) register
  IEC0bits.U1RXIE = 1;
}

void RS485S_Send(char data) {
  while(!U1STAbits.TRMT);
  U1TXREG = data;
  while(!U1STAbits.TRMT);
}

// <9> UTXBF: Transmit Buffer Full Status bit (Read Only)
//            1 = Transmit buffer is full
//            0 = Transmit buffer is not full, at least one more data word can be written
// <8> TRMT:  Transmit Shift Register is Empty bit (Read Only)
//            1 = Transmit shift register is empty and transmit buffer is empty (the last transmission has completed)
//            0 = Transmit shift register is not empty, a transmission is in progress or queued in the transmit buffer
// <4> RIDLE: Receiver Idle bit (Read Only)
//            1 = Receiver is Idle
//            0 = Data is being received
// <3> PERR:  Parity Error Status bit (Read Only)
//            1 = Parity error has been detected for the current character
//            0 = Parity error has not been detected
// <2> FERR:  Framing Error Status bit (Read Only)
//            1 = Framing Error has been detected for the current character
//            0 = Framing Error has not been detected
// <1> OERR:  Receive Buffer Overrun Error Status bit (Read/Clear Only)
//            1 = Receive buffer has overflowed
//            0 = Receive buffer has not overflowed
// <0> URXDA: Receive Buffer Data Available bit (Read Only)
//            1 = Receive buffer has data, at least one more character can be read
//            0 = Receive buffer is empty


