/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "config.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "Contactor.h"
#include "EEPROM.h"
#include "WDTimer.h"
#include "SoC.h"

void initialize(void);
void preliminaryCheck(void);
void faultCondition(void);

int mainmain(){}
int real_main(){
	__disable_irq();			// Disable all interrupts until initialization is done
	initialize();					// Initialize codes/pins
	preliminaryCheck();		// Wait until all boards are powered on
	__enable_irq();				// Enable interrupts
	
	while(1){
		// First update the measurements.
		Voltage_UpdateMeasurements();
		Current_UpdateMeasurements();
		Temperature_UpdateMeasurements();
		
		// Check if everything is safe
		if(Current_IsSafe() && Temperature_IsSafe(Current_IsCharging()) && Voltage_IsSafe()){
			Contactor_On();
		}else{
			break;
		}
		
		// Update necessary
		// CAN_SendMessageStatus()	// Most likely need to put this on a timer if sending too frequently
		
	}
	
	// BPS has tripped if this line is reached
	faultCondition();
}

/**
 * Initialize system.
 *	1. Initialize device drivers.
 *			- This includes communication protocols, GPIO pins, timers
 *	2. Get EEPROM data that holds all fault conditions.
 *			- By regulations, we are not allowed to be able to set the voltage, current, temperature
 *				limits while the car is moving. To make sure this doesn't happen, the EEPROM needs to be
 *				written and cannot be modified (locked) once programmed.
 *	3. Set the current, voltage, and temperature limits.
 *			- Give wrappers (Voltage, Current, Temperature) the limits
 */
void initialize(void){
	Contactor_Init();
	Contactor_Off();
	EEPROM_Init();
	
	Current_Init();
	Voltage_Init();
	Temperature_Init();
}

/** preliminaryCheck
 * Before starting any data monitoring, check if all the boards are powered. If we start the data
 * collection before everything is powered on, then the system will immediately fault and not turn on
 * even though everything is safe.
 */
void preliminaryCheck(void){
	// Check if Watch dog timer was triggered previously
}

/** faultCondition
 * This block of code will be executed whenever there is a fault.
 * If bps trips, make it spin and impossible to connect the battery to car again
 * until complete reboot is done.
 */
void faultCondition(void){
	Contactor_Off();
	while(1){
		// CAN_SendMessageStatus()
		if(!Current_IsSafe()){
			// Toggle Current fault LED
		}
		
		if(!Voltage_IsSafe()){
			// Toggle Voltage fault LED
		}
		
		if(!Temperature_IsSafe(Current_IsCharging())){
			// Toggle Temperature fault LED
		}
	}
}


//****************************************************************************************
// The following code is for testing individual pieces of code.
//****************************************************************************************
// If you want to test an individual module, change the #define NAME to what you want to
// to use. This is used mainly to save memory for both the microcontroller and Keil.
// Keil has a limit of how much memory you can compile.
// E.g. If you want to run a LTC6811 test, change "#define CHANGE_THIS_TO_TEST_NAME" to the
//		following:
//		#define LTC6811_TEST
#define OPEN_WIRE_TEST

#ifdef LED_TEST
#include "LED.h"
void LEDdelay(int time){
	for(int i = 0; i < time; i++);
}

int main(){
	LED_Init();
	while(1){
		LED_Toggle(FAULT);
		LEDdelay(100000);
		LED_Toggle(RUN);
		LEDdelay(100000);
		LED_Toggle(UVOLT);
		LEDdelay(100000);
		LED_Toggle(OVOLT);
		LEDdelay(100000);
		LED_Toggle(OTEMP);
		LEDdelay(100000);
		LED_Toggle(OCURR);
		LEDdelay(100000);
		LED_Toggle(WDOG);
		LEDdelay(100000);
		LED_Toggle(CAN);
		LEDdelay(100000);
		LED_Toggle(EXTRA);
		LEDdelay(100000);
	}
}
#endif

#ifdef LTC6811_TEST
// LTC6811 Test
#include "SPI.h"
#include "LTC6811.h"
#include "UART.h"
#include <string.h>
void printCells(cell_asic *mods);
void print_config(cell_asic *bms_ic);
int main(){
	// Local var
	int8_t error = 0;
	
	// Initialize LTC
	__disable_irq();
	cell_asic battMod[NUM_VOLTAGE_BOARDS];
	UART3_Init(9600);
	printf("Testing LTC6811:\n\r");
	LTC6811_Init(battMod);
	LTC6811_wrcfg(NUM_VOLTAGE_BOARDS, battMod);
	print_config(battMod);
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
	error = LTC6811_rdcfg(NUM_VOLTAGE_BOARDS, battMod);
  LTC6811_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
	__enable_irq();
	while(1){
		printf("Waiting...\n\r");
		wakeup_sleep(NUM_VOLTAGE_BOARDS);
		LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
    LTC6811_pollAdc();
    error = LTC6811_rdcv(0, NUM_VOLTAGE_BOARDS, battMod); // Set to read back all cell voltage registers
		printCells(battMod);
		for(uint32_t i = 0; i < 10000000; i++);
	}
}

void printCells(cell_asic *mods){
	for (int current_ic = 0 ; current_ic < NUM_VOLTAGE_BOARDS; current_ic++){
		for (int i=0; i<mods[0].ic_reg.cell_channels; i++){
      printf("C%d", i + 1);
      printf(":");
      printf("%4f", mods[current_ic].cells.c_codes[i]*0.0001);
      printf("\n\r");
    }
		printf("\n\r");
  }
}

void print_config(cell_asic *bms_ic)
{
  int cfg_pec;

  printf("Written Configuration: ");
  for (int current_ic = 0; current_ic<NUM_VOLTAGE_BOARDS; current_ic++)
  {
    printf(" IC ");
    printf("%d", current_ic+1);
    printf(": ");
    printf("0x");
    printf("%x", bms_ic[current_ic].config.tx_data[0]);
    printf(", 0x");
    printf("%x", bms_ic[current_ic].config.tx_data[1]);
    printf(", 0x");
    printf("%x", bms_ic[current_ic].config.tx_data[2]);
    printf(", 0x%x", bms_ic[current_ic].config.tx_data[3]);
    printf(", 0x%x", bms_ic[current_ic].config.tx_data[4]);
    printf(", 0x%x", bms_ic[current_ic].config.tx_data[5]);
    cfg_pec = pec15_calc(6,&bms_ic[current_ic].config.tx_data[0]);
		printf(", Calculated PEC: 0x%x",(uint8_t)(cfg_pec>>8));
    printf("%d", (uint8_t)(cfg_pec>>8));
    printf(", 0x%x", (uint8_t)cfg_pec);
    printf("\n\r");
  }
}
#endif

#ifdef VOLTAGE_TEST
#include "UART.h"

int main(){
	UART1_Init(115200);
	//printf("Are you alive?");
	while(Voltage_Init()) {
		printf("Communication Failed.\n\r");
	}
	printf("Writing and Reading to Configuration Register Successful. Initialization Complete\n\r");
	
	Voltage_UpdateMeasurements();
	printf("Successfully Updated Voltages.\n\r");
	printf("\n\rVoltage Test:\n\r");
	printf("Is it safe? %d\n\r\n\r", Voltage_IsSafe());
	printf("Voltages of all modules:\n\r");
	for(int32_t i = 0; i < NUM_BATTERY_MODULES; i++){
		printf("%d : %f\n\r", i, (float)(Voltage_GetModuleVoltage(i)*0.0001));  // Place decimal point.
	}
	while(1){
//		for(int32_t i = 0; i < NUM_BATTERY_MODULES; i++){
//			printf("%d : %d\n\r", i, Voltage_GetModuleVoltage(i));
//		}
	}
}
#endif

#ifdef CURRENT_TEST
int main(){
	UART3_Init(9600);
	Current_Init();
	Current_UpdateMeasurements();
	printf("\n\rCurrent Test:\n\r");
	printf("Is it safe? %d\n\r", Current_IsSafe());
	printf("Is the battery charging? %d\n\r\n\r", Current_IsCharging());
	printf("Low Precision: %d\n\r", Current_GetLowPrecReading());
	printf("High Precision: %d\n\r", Current_GetHighPrecReading());
	while(1){
	
	}
}
#endif

#ifdef TEMPERATURE_TEST
#include "UART.h"
#include "LTC2983.h"
#include "SPI.h"
#include "stm32f4xx.h"




int main(){
	UART3_Init(9600);
	printf("I'm alive\n\r");
	int32_t buffer[20];
	
	Temperature_Init();

	LTC2983_ReadConversions(buffer, BOARD_CS1, 20);
	while(1){
		int32_t buf[20] = {0};
		LTC2983_StartMeasuringADC(BOARD_CS1);
		LTC2983_ReadConversions(buf, BOARD_CS1, 20);
	}
}
#endif

#ifdef CONTACTOR_TEST
int main(){
	Contactor_Init();
	Contactor_Off();
	//for(int32_t i = 0; i < 1000000; i++);	// delay
	//Contactor_On();
	//for(int32_t i = 0; i < 5000000; i++);	// delay
	//Contactor_Off();
	while(1){
		//Contactor_On();
	}
}
#endif

#ifdef WATCHDOG_TEST
int main(){
	WDTimer_Init();
	WDTimer_Start();
	
	// reset WDTimer 10 times. With this counter, the watchdog timer should not reset the system shortly after it starts.
	for(int32_t i = 0; i < 10; i++){
		for(int32_t j = 0; j < 100000; j++);	// Delay
		WDTimer_Reset();
	}
	while(1){
		
	}
}
#endif

#ifdef SPI_TEST
//****************************************************************************************
// SPI Test
#include "SPI.h"
#include "UART.h"
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
#endif

#ifdef UART_TEST
//****************************************************************************************
// Debug UART Test
#include "UART.h"
int main(){
	UART3_Init(115200);
	while(1){
		printf("Die world\r\n");
		for(uint32_t i = 0; i < 100000; i++);
	}
}
#endif

#ifdef I2C_TEST
//****************************************************************************************
// I2C test
#include "I2C.h"
int main(){
	uint8_t randomData[5] = {0x04, 0x02, 0x42, 0x24, 0x44};
	I2C3_Init();
	I2C3_WriteMultiple(0xA0, 0xCC, randomData);
	while(1){
	
	}
}
#endif

#ifdef GYRO_TEST
//****************************************************************************************
// Gyro test
int gyroTestmain(){
	//FXAS21002CQR1_Init();
	UART3_Init(9600);
	while(1){
		//uint16_t *vals = FXAS21002CQR1_Measure();
		// print or something
	}
}
#endif

#ifdef ADC_TEST
//****************************************************************************************
#include "ADC.h"
#include <stdio.h>
#include <UART.h>
int ADCmain(){
	char str[50];
	UART3_Init(9600);
	ADC_InitHilo();
	while(1){
		//sprintf(str,"%d\n",ADC_ChooseHiLo(ADC_ReadHigh(),ADC_ReadLow()));
		sprintf(str,"%d\r\n",ADC_Conversion(ADC_ReadLow()));
		UART3_Write(str,strlen(str));
	}		
}
#endif

#ifdef SOC_TEST
// *****************************************************************************************
#include "SOC.h"
#include <stdio.h>
#include <UART.h>
void ChargingSoCTest(void);
void DischargingSoCTest(void);

extern uint32_t fixedPoint_SoC;
extern float float_SoC;

int main(){
	UART3_Init(9600);
	SoC_Init();
	//ChargingSoCTest();
	//DischargingSoCTest();
}

void ChargingSoCTest(void) {
	char str[50];
	sprintf(str,"Starting SoC Charging Test..");
	UART3_Write(str, strlen(str));
	
	fixedPoint_SoC = 0;
	float_SoC = 0;
	while(1){
		SoC_Calculate(500); 									// Charging with 500 mA, should take a while
		sprintf(str,"SoC: %.2f%%\r\n",float_SoC);
		UART3_Write(str, strlen(str));
	}
}

void DischargingSoCTest(void) {
	char str[50];
	sprintf(str,"Starting SoC Discharging Test..");
	UART3_Write(str, strlen(str));
	
	fixedPoint_SoC = 10000;
	float_SoC = 100.00;
	while(1){
		SoC_Calculate(-500); 									// Consuming 500 mA, should take a while
		sprintf(str,"SoC: %.2f%%\r\n",float_SoC);
		UART3_Write(str, strlen(str));
	}
}

/** Tests
 * 	TODO: Need to test SetAccumulator, GetPercent and Calibrate on faults
 */
#endif

#ifdef OPEN_WIRE_TEST
//******************************************************************************************
#include "Voltage.h"
#include <stdio.h>
#include "UART.h"
int main(){
	UART3_Init(115200);
	Voltage_Init();
	printf("%x", Voltage_OpenWireSummary());
	printf("\n\r");
	if(Voltage_OpenWire() == DANGER) {
		printf("return = DANGER\n\r");
	}
	else if(Voltage_OpenWire() == SAFE){
		printf("return = SAFE\n\r");
	}
}
#endif
