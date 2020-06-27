#ifndef SPI_H
#define SPI_H

void SPI_initialize(void);
void SPI_chipSelectOn(void);
void SPI_chipSelectOff(void);
void SPI_receiveArray(char *pt, unsigned int length);
unsigned char SPI_sendReceive(unsigned char data);

#endif
