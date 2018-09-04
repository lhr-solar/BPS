/***********************************************
/I2Cdev.c
/spring 2013 Fred Engelkemeir
/This code provides an interface to reading and writing the RTC and the SRAM
/it uses microchip's I2C drivers (in the I2C2.c file)
***********************************************/


unsigned char I2C2_WriteRTC(unsigned char address, unsigned char data);
unsigned char I2C2_ReadRTC(unsigned char address);
unsigned char I2C2_WriteEEPROM(unsigned long address, unsigned char data);
unsigned char I2C2_ReadEEPROMRandom(unsigned long address);
unsigned char I2C2_WriteRTC_EEPROM(unsigned char address, unsigned char data);
unsigned char I2C2_ReadRTC_EEPROMRandom(unsigned char address);

unsigned char I2C2_WriteRTC(unsigned char address, unsigned char data)
{
	return LDByteWriteI2C(0xDE, address, data);
}

unsigned char I2C2_ReadRTC(unsigned char address)
{
	unsigned char temp;
	LDByteReadI2C(0xDE, address, &temp, 1);
	return temp;
}
unsigned char I2C2_WriteEEPROM(unsigned long address, unsigned char data)
{
	unsigned char chipaddy;
  	if(address&0x10000)
    	chipaddy = 0xAA;
	else
    	chipaddy = 0xA2;
	return HDByteWriteI2C(chipaddy, ((address&0x0000FF00)>>8), address&0x000000FF, data);
}
unsigned char I2C2_ReadEEPROMRandom(unsigned long address)
{
	unsigned char chipaddy;
	unsigned char temp;
  	if(address&0x10000)
    	chipaddy = 0xAA;
	else
    	chipaddy = 0xA2;
	HDByteReadI2C(chipaddy, ((address&0x0000FF00)>>8), address&0x000000FF, &temp, 1);
	return temp;
}

unsigned char I2C2_WriteRTC_EEPROM(unsigned char address, unsigned char data)
{
	return LDByteWriteWI2C(0xAE, address, data);
}

unsigned char I2C2_ReadRTC_EEPROMRandom(unsigned char address)
{
	unsigned char temp;
	LDByteReadI2C(0xAE, address, &temp, 1);
	return temp;
}

//mother fucing newline.
