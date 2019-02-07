//***************************************************
//dsPIC30F6012A RS485 Master board Driver Source Code File
//Archer Finley
//BPS-UTSVT
//March 30,2010
//Description: Source code for drivers for the RS485 module on the dsPIC30F6012A(Master Board)
//Filename: RS485M.c

#include <p30Fxxxx.h>




//*********RS485M_Init***********
//Description: Initializes master board dsPIC for 
//communication with the slave board dsPIC using
//UART and RS-485 communication protocol
//Inputs: none
//Outputs: none
void RS485M_Init(void) {

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
// <7:6> URXISEL = 00 Interrupt flag bit is set when Receive Buffer is full (i.e., has 4 data characters)
// <5>   ADDEN   = 0  Address Detect mode disabled. If 9-bit mode is not selected, this control bit has no effect.
// U1STA = 1000 0100 0000 0000
  U1STA = 0x8400;

//Set U1BRG (UART1 Baud Rate Register) options
// U1BRG = (Fcy/(16*BaudRate)) - 1 = 0 for 156250 kbaud
  U1BRG = 7;

//Turn on TE(PORTD<0>) and /RE(PORTD<1> located on RS-485 chip.
  TRISD = TRISD&0xFFC;
  LATD = LATD|0x003;
  //LATD = LATD&0xFFD;
}

void RS485M_Send(unsigned char data) {

  while(!U1STAbits.TRMT);
  U1TXREG = data;
  while(!U1STAbits.TRMT);
}

unsigned char RS485M_Receive(void){
  int counter = 0;
//Wait for receive
  while(!U1STAbits.RIDLE);
  while( (!U1STAbits.URXDA) && ( counter<0xFF8 ) ) {counter++;}
  if(counter == 0xFF8){
    return 0xFF;
  }
  else {
    while(!U1STAbits.URXDA);
    return U1RXREG;
  }
}

unsigned char RS485M_SendReceive(unsigned char data) {
  int buffer = 0;
  while(!U1STAbits.TRMT);
  U1TXREG = data;
  while(!U1STAbits.TRMT);
  for(;buffer<0xD0;buffer++){}
  buffer = 0;
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Wait for receive
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  for(buffer=0;buffer<0xD0;buffer++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;  
  return U1RXREG;
}

void RS485M_ReceiveVoltages(unsigned char data, unsigned int *VoltBuffer, unsigned char length) {
  unsigned int buffer = 0;
/*  unsigned char slave_status = 0,
       volt12to5_status = 0,
       volt4to1temp12to9_status = 0,
       temp8to1_status = 0;*/
  unsigned char i = 0;
//Send PEC1 = 'S'
  while(!U1STAbits.TRMT);
  U1TXREG = 0x53;
//Send PEC2 = 'T'
  while(!U1STAbits.TRMT);
  U1TXREG = 0x54;
//Send Address/Command byte
  while(!U1STAbits.TRMT);
  U1TXREG = data;
//Send PEC3 = 'P'
  while(!U1STAbits.TRMT);
  U1TXREG = 0x50;
  while(!U1STAbits.TRMT);
  for(;buffer<0xD0;buffer++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Receive Slave Status
/*  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  slave_status = U1RXREG;
//Receive Error bits
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  volt12to5_status = U1RXREG;
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  volt4to1temp12to9_status = U1RXREG;
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  //temp8to1_status = U1RXREG;*/
//Wait for receive of all voltage data
  for(i=0;i<length;i++) {
    *VoltBuffer = (RS485M_Receive())<<8;
    *VoltBuffer = *VoltBuffer|(((int)RS485M_Receive())&0xFF);
    VoltBuffer++;
    if( (i==3) || (i==7) || (i==11) ) {
      U1STAbits.OERR = 0;
    }
  }  
  for(buffer=0;buffer<0xD0;buffer++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;  
}
/*
void RS485M_ReceiveSlaveStatus(char slave_address, char SlaveStatus[][]){
  char i = 0;
    for(;i<3;i++) {
      while(!U1STAbits.RIDLE);
      while(!U1STAbits.URXDA);
      SlaveStatus[slave_address][i] = U1RXREG
    }
}*/
  
void RS485M_ReceiveTemps(unsigned char data, signed int *TempBuffer, unsigned char length) {
 /* unsigned char slave_status = 0,
       volt12to5_status = 0,
       volt4to1temp12to9_status = 0,
       temp8to1_status = 0;*/
  unsigned int counter = 0, i = 0;
//Send PEC1 = 'S'
  while(!U1STAbits.TRMT);
  U1TXREG = 0x53;
//Send PEC2 = 'T'
  while(!U1STAbits.TRMT);
  U1TXREG = 0x54;
//Send Address/Command byte
  while(!U1STAbits.TRMT);
  U1TXREG = data;
//Send PEC3 = 'P'
  while(!U1STAbits.TRMT);
  U1TXREG = 0x50;
  while(!U1STAbits.TRMT);
  for(counter = 0;counter<0xD0;counter++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Receive Slave Status
/*  while(!U1STAbits.RIDLE);
//  while(!U1STAbits.URXDA);
  slave_status = U1RXREG;
//Receive Error bits
  while(!U1STAbits.RIDLE);
//  while(!U1STAbits.URXDA);
  volt12to5_status = U1RXREG;
  while(!U1STAbits.RIDLE);
//  while(!U1STAbits.URXDA);
  volt4to1temp12to9_status = U1RXREG;
  while(!U1STAbits.RIDLE);
//  while(!U1STAbits.URXDA);
  //temp8to1_status = U1RXREG;*/
//Wait for receive of all voltage data
  for(;i<length;i++) {
    *TempBuffer = (RS485M_Receive())<<8;
    *TempBuffer = *TempBuffer|(((int)RS485M_Receive())&0xFF);
    TempBuffer++;
  }  
  for(counter = 0;counter<0xD0;counter++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;  
}  

unsigned char RS485M_DetectSlaves(unsigned char StatusCode[16][4]){
  unsigned int i=0, j=0;
  unsigned int buffer = 0;
  unsigned char commandbyte = 0;
  unsigned char number_of_slaves = 0;
  for(;i<16;i++) {
    commandbyte = i;
    commandbyte = commandbyte<<4;
    commandbyte = commandbyte&0xF0;
    commandbyte = commandbyte+1;
//Send PEC1 = 'S'
    while(!U1STAbits.TRMT);
    U1TXREG = 0x53;
//Send PEC2 = 'T'
    while(!U1STAbits.TRMT);
    U1TXREG = 0x54;
    while(!U1STAbits.TRMT);
    U1TXREG = commandbyte;
//Send PEC3 = 'P'
    while(!U1STAbits.TRMT);
    U1TXREG = 0x50;
    while(!U1STAbits.TRMT);
    for(buffer=0;buffer<0xD0;buffer++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
    LATD &= 0xFFC;
    for(j=0;j<4;j++) {
      StatusCode[i][j] = RS485M_Receive();
    }
    if(StatusCode[i][0] == 0xC0){
      number_of_slaves++;
	}
    for(buffer=0;buffer<0xFF8;buffer++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
    LATD |= 0x003;  
  }
  return number_of_slaves;
}		

unsigned char RS485M_SetOV(unsigned char address, unsigned int overvoltage) {
  unsigned char counter = 0,
       commandbyte = 0,
       c_voltage = 0;
  overvoltage = (overvoltage*2)/48;
  c_voltage = overvoltage;
  address &= 0x0F;
  commandbyte = address<<4;
  commandbyte += 4;
  while(!U1STAbits.TRMT);
  U1TXREG = commandbyte;
  while(!U1STAbits.TRMT);
  U1TXREG = c_voltage;
  while(!U1STAbits.TRMT);
  for(counter=0;counter<0xD0;counter++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Wait for receive of Slave Status
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  for(counter=0;counter<0xD0;counter++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;
  return U1RXREG;
}

unsigned char RS485M_SetUV(unsigned char address, unsigned int undervoltage) {
  unsigned char counter = 0,
       commandbyte = 0,
       c_voltage = 0;
  undervoltage = (undervoltage*2)/48;
  c_voltage = undervoltage;
  address &= 0x0F;
  commandbyte = address<<4;
  commandbyte += 5;
  while(!U1STAbits.TRMT);
  U1TXREG = commandbyte;
  while(!U1STAbits.TRMT);
  U1TXREG = c_voltage;
  while(!U1STAbits.TRMT);
  for(counter=0;counter<0xD0;counter++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Wait for receive of Slave Status
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  for(counter=0;counter<0xD0;counter++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;
  return U1RXREG;
}

unsigned char RS485M_SetOT(unsigned char address, unsigned int overtemp) {
  unsigned char counter = 0,
       commandbyte = 0,
       MSB = (overtemp>>8)&0xFF,
       LSB = overtemp&0xFF;
  
  address &= 0x0F;
  commandbyte = address<<4;
  commandbyte += 6;
  RS485M_Send(commandbyte);
  RS485M_Send(MSB);
  RS485M_Send(LSB);
  for(;counter<0xD0;counter++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Wait for receive of Slave Status
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  for(counter=0;counter<0xD0;counter++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;
  return U1RXREG;
}

unsigned char RS485M_SetUT(unsigned char address, unsigned int undertemp) { 
  unsigned char counter = 0,
       commandbyte = 0,
       MSB = (undertemp>>8)&0xFF,
       LSB = undertemp&0xFF;
  
  address &= 0x0F;
  commandbyte = address<<4;
  commandbyte += 7;
  RS485M_Send(commandbyte);
  RS485M_Send(MSB);
  RS485M_Send(LSB);
  for(;counter<0xD0;counter++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Wait for receive of Slave Status
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  for(counter=0;counter<0xD0;counter++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;
  return U1RXREG;
}

unsigned char RS485M_SetNumberOfModules(unsigned char address, unsigned char amount) {
  unsigned char counter = 0,
       commandbyte = 0;
  address &= 0x0F;
  commandbyte = address<<4;
  commandbyte += 8;
  RS485M_Send(commandbyte);
  RS485M_Send(amount);
    for(;counter<0xD0;counter++){}
//Turn off TE(PORTD<0>) and turn on /RE(PORTD<1>)
  LATD &= 0xFFC;
//Wait for receive of Slave Status
  while(!U1STAbits.RIDLE);
  while(!U1STAbits.URXDA);
  for(counter=0;counter<0xD0;counter++){}
//Turn on TE(PORTD<0>) and turn off /RE(PORTD<1>)
  LATD |= 0x003;
  return U1RXREG;
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

