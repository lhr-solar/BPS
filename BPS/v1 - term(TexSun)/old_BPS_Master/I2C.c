//***************************************************
//dsPIC30F6012A I2C Master board Driver Source Code File
//Archer Finley, Cory Loflin
//BPS-UTSVT
//April 7, 2010
//Description: Source code for drivers for the I2C module
//on the dsPIC30F6012A(Master Board)
//Filename: I2C.c

#include <p30Fxxxx.h>

//*********I2C_Init***********
//Description: Initializes master board dsPIC for 
//communication using the I2c communication protocol
//with a baud rate of 400kHz
//Inputs: none
//Outputs: none
void I2C_Init(void) {

//Set I2CCON the I2C Control Register
// <15> I2CEN   = 1   Enables the I2C module and configures the SDA and SCL pins as serial port pins
// <13> I2CSIDL = 0   Continue module operation in Idle mode
// <12> SCLREL  = 1   Release SCL clock (when operating as slave)
//                    0 = Hold SCL clock low (clock stretch)
//                    If STREN = 1:
//                      Bit is R/W (i.e., software may write ‘0’ to initiate stretch and write ‘1’ to release clock)
//                      Hardware clear at beginning of slave transmission.
//                      Hardware clear at end of slave reception.
//                    If STREN = 0:
//                      Bit is R/S (i.e., software may only write ‘1’ to release clock)
//                      Hardware clear at beginning of slave transmission.
// <11> IPMIEN  = DNS (operating as master)
// <10> A10M    = 0   I2CADD is a 7-bit slave address
// <9>  DISSLW  = 0   Slew rate control enabled
// <8>  SMEN    = 0   Disable SMBus input thresholds
// <7>  GCEN    = 0   General call address disabled
// <6>  STREN   = 0   Disable software or receive clock stretching
// <5>  ACKDT   = 1   Send NACK during acknowledge
// I2CCON = 1001 0000 0010 0000
  I2CCON = 0x9020;

//Set I2C baud rate at 400khz
  I2CBRG = 0x00F;
}

//*********I2C_WriteRTC***********
//Description: Writes to the RTC (address 0xA2) using the I2C bus on
//the master board dsPIC
//Inputs: address (unsigned char) of data location to be written
//        data (unsigned char) to be written
//Outputs: 0 if no acknowledgement from RTC
//         1 if acknowledgement received from RTC
unsigned char I2C_WriteRTC(unsigned char address, unsigned char data) {
//Wait for the transfer buffer (I2CTRN) to be empty
  while(I2CSTATbits.TBF);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Send the address byte to select RTC with bit 0 = 0 to indicate write
  I2CTRN = 0xA2;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send address of register to modify
  I2CTRN = address;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send data to register under modifications
  I2CTRN = data;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send stop condition
  I2CCONbits.PEN = 1;
//Wait for stop condition to complete
  while(I2CCONbits.PEN);
  return 1;
}

//*********I2C_ReadRTC***********
//Description: Reads from the RTC (address 0xA3) using the I2C bus on
//the master board dsPIC
//Inputs: address (unsigned char) of data location to be read
//Outputs: 0 if no acknowledgement from RTC
//         data (unsigned char) if data is received from RTC
unsigned char I2C_ReadRTC(unsigned char address) {
  unsigned char data;
//Wait for the transfer buffer (I2CTRN) to be empty
  while(I2CSTATbits.TBF);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Send the address byte to select RTC with bit 0 = 0 to indicate write
  I2CTRN = 0xA2;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send address of register to read
  I2CTRN = address;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Wait for the transfer buffer (I2CTRN) to be empty
  while(I2CSTATbits.TBF);
//Send stop condition
  I2CCONbits.PEN = 1;
  while(I2CCONbits.PEN);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Send the address byte to select RTC with bit 0 = 1 to indicate read
  I2CTRN = 0xA3;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Enable receive mode
  I2CCONbits.RCEN = 1;
//Wait for receive to complete
  while(I2CCONbits.RCEN);
  while(!I2CSTATbits.RBF);
  data = I2CRCV;
//Send slave acknowledge bit
  if(data) {
    I2CCONbits.ACKEN = 1;
    while(I2CCONbits.ACKEN);
//Send stop condition
    I2CCONbits.PEN = 1;
    while(I2CCONbits.PEN);
    return data;
  }
  else
    return 0; 
}

//*********I2C_WriteEEPROM***********
//Description: Writes to the EEPROM (address 0xA6 (block 0) or 0xAE (block 1)) using the I2C bus on
//the master board dsPIC
//Inputs: address (unsigned long) of data location to be written
//        data (unsigned char) to be written
//Outputs: 0 if no acknowledgement from RTC
//         1 if acknowledgement received from RTC
unsigned char I2C_WriteEEPROM(unsigned long address, unsigned char data) {
//Wait for the transfer buffer (I2CTRN) to be empty
  while(I2CSTATbits.TBF);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Determine if the address is in block 0 (0x0000-0xFFFF) or block 1 (0x10000-0x1FFFF)
//and send the address byte to select EEPROM with bit 0 = 0 to indicate write
  if(address&0x10000){
//Block 1
    I2CTRN = 0xAE;
  }
  else{
//Block 0
    I2CTRN = 0xA6;
  }
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send high address byte of register to modify
  I2CTRN = (unsigned char) ((address&0xFF00)>>8);
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send low address byte of register to modify
  I2CTRN = (unsigned char) (address&0xFF);
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send data to register under modifications
  I2CTRN = data;
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send stop condition
  I2CCONbits.PEN = 1;
//Wait for stop condition to complete
  while(I2CCONbits.PEN);

//Initiate Acknowledge Polling to ensure that EEPROM is ready for next operation
//Wait for the transfer buffer (I2CTRN) to be empty
  while(I2CSTATbits.TBF);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Determine if the address is in block 0 (0x0000-0xFFFF) or block 1 (0x10000-0x1FFFF)
//and send the address byte to select EEPROM with bit 0 = 0 to indicate write
  if(address&0x10000){
//Block 1
    I2CTRN = 0xAE;
  }
  else{
//Block 0
    I2CTRN = 0xA6;
  }
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and repeat if no acknowledgement
  while(I2CSTATbits.ACKSTAT) {
    while(I2CSTATbits.TBF);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
    I2CCONbits.SEN = 1;
//Wait for the start condition to complete
    while(I2CCONbits.SEN);
//Determine if the address is in block 0 (0x0000-0xFFFF) or block 1 (0x10000-0x1FFFF)
//and send the address byte to select EEPROM with bit 0 = 0 to indicate write
    if(address&0x10000){
//Block 1
      I2CTRN = 0xAE;
    }
    else{
//Block 0
      I2CTRN = 0xA6;
    }
//Wait for transmit to complete
    while(I2CSTATbits.TRSTAT);
  }  
  return 1;
}

//*********I2C_ReadEEPROMRandom***********
//Description: Reads from the EEPROM (address 0xA7 (block 0) or 0xAF (block 1)) using the I2C bus on
//the master board dsPIC
//Inputs: address (unsigned long) of data location to be read
//Outputs: 0 if no acknowledgement from EEPROM
//         data (unsigned char) if data is received from EEPROM
unsigned char I2C_ReadEEPROMRandom(unsigned long address) {
  unsigned char data;
//Wait for the transfer buffer (I2CTRN) to be empty
  while(I2CSTATbits.TBF);
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Determine if the address is in block 0 (0x0000-0xFFFF) or block 1 (0x10000-0x1FFFF)
//and send the address byte to select EEPROM with bit 0 = 0 to indicate write
  if(address&0x10000){
//Block 1
    I2CTRN = 0xAE;
  }
  else{
//Block 0
    I2CTRN = 0xA6;
  }
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send high address byte of register to modify
  I2CTRN = (unsigned char) ((address&0xFF00)>>8);
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Send low address byte of register to modify
  I2CTRN = (unsigned char) (address&0xFF);
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Set SEN(I2CCON<0>) to 1 to Initiate start condition on SDA and SCL pins
  I2CCONbits.SEN = 1;
//Wait for the start condition to complete
  while(I2CCONbits.SEN);
//Determine if the address is in block 0 (0x0000-0xFFFF) or block 1 (0x10000-0x1FFFF)
//and send the address byte to select EEPROM with bit 0 = 1 to indicate read
  if(address&0x10000){
//Block 1
    I2CTRN = 0xAF;
  }
  else{
//Block 0
    I2CTRN = 0xA7;
  }
//Wait for transmit to complete
  while(I2CSTATbits.TRSTAT);
//Verify slave acknowledgement and exit if no acknowledgement
  if(I2CSTATbits.ACKSTAT)  
    return 0;
//Enable receive mode
  I2CCONbits.RCEN = 1;
//Wait for receive to complete
  while(I2CCONbits.RCEN);
  while(!I2CSTATbits.RBF);
  data = I2CRCV;
//Send slave acknowledge bit
  if(data) {
    I2CCONbits.ACKEN = 1;
    while(I2CCONbits.ACKEN);
//Send stop condition
    I2CCONbits.PEN = 1;
    while(I2CCONbits.PEN);
    return data;
  }
  else
    return 0; 
}