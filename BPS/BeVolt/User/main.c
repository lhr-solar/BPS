/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 * @authors Sijin Woo, Chase Block
 * @lastRevised 10/2/2018
 */

#include <stdint.h>
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
//#include "Contactor.h"
#include "EEPROM.h"
//#include "CANlib.h"
#include "WDTimer.h"
//#include "Gyro.h"
#include "stm32f4xx.h"


void initialize();

int Mainmain(){
	initialize();
	WDTimer_Start();
	while(1){
		if(Voltage_IsSafe() && Current_IsSafe() && Temperature_IsSafe()){
			Voltage_UpdateMeasurements();
			Current_UpdateMeasurements();
			Temperature_UpdateMeasurements();
		}
		
		WDTimer_Reset();
	}
}

void initialize(void){
	EEPROM_Init();
	WDTimer_Init();
	//TODO EEPROM
	//VoltSlave.setLimits();
	//AmpSlave.setLimits();
	//CelsiusSlave.setLimits();
}





//*************************************************************************************
// The following code is for testing individual pieces of code.
//*************************************************************************************

// LTC6811 Test
#include "SPI.h"
#include "LTC6811.h"
#include "UART.h"
#include "Definition.h"
#include <string.h>
void printCells(cell_asic *mods);
int main(){
	// Local var
	int8_t error = 0;
	
	// Initialize LTC
	__disable_irq();
	cell_asic battMod[NUM_VOLTAGE_BOARDS];
	UART3_Init(9600);
	LTC6811_Init(battMod);
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
  LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	__enable_irq();
	while(1){
		UART3_Write("Waiting...\n\r", 12);
		wakeup_sleep(NUM_VOLTAGE_BOARDS);
    error = LTC6811_rdcv(0, NUM_VOLTAGE_BOARDS, battMod); // Set to read back all cell voltage registers
		printCells(battMod);
		for(uint32_t i = 0; i < 1000000; i++);
	}
}

void printCells(cell_asic *mods){
	/*
	for (int current_ic = 0 ; current_ic < NUM_VOLTAGE_BOARDS; current_ic++){
		for (int i=0; i<mods[0].ic_reg.cell_channels; i++){
      UART3_Write(" C", 2);
      Serial.print(i+1,DEC);
      Serial.print(":");
      Serial.print(bms_ic[current_ic].cells.c_codes[i]*0.0001,4);
      Serial.print(",");
    }
		Serial.println();
  }
  Serial.println();
	*/
}

// SPI Test
int SPITestmain(){
	__disable_irq();
	UART3_Init(9600);
	SPI_Init8();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO portB clock
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;								// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIOB->ODR |= GPIO_Pin_6;
	__enable_irq();
	
	char str[30] = "Testing\n\r";
	UART3_Write(str, strlen(str));
	GPIOB->ODR &= ~GPIO_Pin_6;
	SPI_Write8((uint8_t *)str, strlen(str));
	GPIOB->ODR |= GPIO_Pin_6;
	while(1){
		
	}
}

// Debug UART Test
int UARTTestmain(){
	char str[] = "Hello there.";
	UART3_Init(9600);
	while(1){
		UART3_Write(str, strlen(str));
		for(uint32_t i = 0; i < 100000; i++);
	}
}

// Gyro test
int gyroTestmain(){
	//FXAS21002CQR1_Init();
	UART3_Init(9600);
	while(1){
		//uint16_t *vals = FXAS21002CQR1_Measure();
		// print or something
	}
}
