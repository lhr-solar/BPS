#ifndef I2C_H
#define I2C_H

void I2C_init(void);
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data);
void		 LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length);
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data);
unsigned int HDByteWriteWI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data);
void		 HDByteReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *Data, unsigned char Length);
unsigned int LDByteWriteWI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data);
unsigned int HDByteWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data);

#endif
