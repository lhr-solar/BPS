//***************************************************
//dsPIC30F6012A SPI Driver Header File
//Archer Finley, Cory Loflin
//BPS-UTSVT
//February 15, 2010
//Description: Header file for drivers for the SPI2 module on the dsPIC30F6012A
//Filename: SPI.h

//*********SPI_Init***********
//Description: Initializes SPI2 port for communicaiton with LTC6802 voltage board
//Inputs: none
//Outputs: none
void SPI_Init(void);

//*********SPI_SendReceive*********
//Description: Sends 8 bits (data) through SDO2 and receives 8 bits through SDI2
//Inputs: 8 bits (char data) to be sent through SDO2
//Outputs: 8 bits (char data) received from SDI2
char SPI_SendReceive(char data);

//**********SPI_Receive************
//Description: Sends nothing(0x00) through SDO2 and receives
// 8 bits through SDI2
//Inputs: none
//Outputs: 8 bits (char data) received from SDI2
char SPI_Receive(void);

//**********SPI_Send*************
//Description: Sends 8 bits (data) through SDO2
//Inputs: 8 bits (char data) sent through SDO2
//Outputs: Dont Care (char data) received through SDI2
void SPI_Send(char data);

//**********SPI_ReceiveArray**********
//Description: Receives a variable length array set by
// the input parameters.
//Inputs: char pointer to the Array to fill, unsigned 
// short length of input Array
//Outputs: none 
void SPI_ReceiveArray(char *pt, unsigned short length);

//***********SPI_SlaveSelectActiveLow*********
//Description: Sets PORTG<9> to low for SPI CS active low
//Inputs: none
//Outputs: none
void SPI_SlaveSelectActiveLow(void);

//***********SPI_SlaveSelectOffHigh***********
//Description: Sets PORTG<9> to high for SPI CS off high
//Inputs: none
//Outputs: none
void SPI_SlaveSelectOffHigh(void);


