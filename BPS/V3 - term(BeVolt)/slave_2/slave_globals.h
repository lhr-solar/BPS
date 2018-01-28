#ifndef SLAVE_GLOBALS_H
#define SLAVE_GLOBALS_H

//main.c
extern unsigned int voltArrayA[12];
extern unsigned int voltArrayB[12];
extern unsigned int tempArrayA[12];
extern unsigned int tempArrayB[12];
extern unsigned char arrayAgood;		//if the voltarrayA and tempArrayA are good/fresh/consistent
extern unsigned char arrayBgood;		//same for B. only one of these will be a 1 at a time (both 0 if not sampled at least once yet
extern unsigned char acquisition_enable;	//1 = system is measuring stuff, 0 = system is not measuring stuff

//RS485slave.c
extern unsigned char RS485_outData[255];					//data to send back to master
extern unsigned char RS485_inData[255];					//data from master
extern unsigned char RS485_command;						//master's command
extern unsigned char RS485_response;						//slave's response code to master
extern unsigned char RS485_outLength;						//DATA length from master
extern unsigned char RS485_inLength;						//DATA length to master

//ADC.h
extern unsigned int ADC_temp[12];
extern unsigned int ADC_Vbus;			//stores the bus voltage
extern unsigned int ADC_V10;			//10V supply for the temp sensors
extern unsigned int ADC_V5;			//5V precision supply for temp circuit

#endif
