/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include "common.h"
#include "stm32f4xx.h"
#include "config.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "Contactor.h"
#include "EEPROM.h"
#include "WDTimer.h"
#include "SoC.h"
#include "LED.h"
#include "SysTick.h"

cell_asic Minions[NUM_MINIONS];

void initialize(void);
void preliminaryCheck(void);
void faultCondition(void);

int realmainmain(){
	__disable_irq();		// Disable all interrupts until initialization is done
	initialize();			// Initialize codes/pins
	preliminaryCheck();		// Wait until all boards are powered on
	__enable_irq();			// Enable interrupts

	WDTimer_Start();

	bool override = false;		// This will be changed by user via CLI	
	while(1){
		// First update the measurements.
		Voltage_UpdateMeasurements();
		Current_UpdateMeasurements();
		Temperature_UpdateMeasurements();
		
		SafetyStatus current = Current_IsSafe();
		SafetyStatus temp = Temperature_IsSafe(Current_IsCharging());
		SafetyStatus voltage = Voltage_IsSafe();

		// Check if everything is safe (all return SAFE = 0)
		if((current == SAFE) && (temp == SAFE) && (voltage == SAFE) && !override) {
			Contactor_On();
		}
		else if((current == SAFE) && (temp == SAFE) && (voltage == UNDERVOLTAGE) && override) {
			Contactor_On();
			continue;
		} else {
			break;
		}

		// Update necessary
		// CAN_SendMessageStatus()	// Most likely need to put this on a timer if sending too frequently

		WDTimer_Reset();
	}

	// BPS has tripped if this line is reached
	faultCondition();
	return 0;
}

/**
 * Initialize system.
 *	1. Initialize device drivers.
 *		- This includes communication protocols, GPIO pins, timers
 *	2. Set the current, voltage, and temperature limits.
 *		- Give wrappers (Voltage, Current, Temperature) the limits
 */
void initialize(void){
	LED_Init();
	Contactor_Init();
	Contactor_Off();
	WDTimer_Init();
	EEPROM_Init();

	Current_Init();
	Voltage_Init(Minions);
	Temperature_Init(Minions);
}

/** preliminaryCheck
 * Before starting any data monitoring, check if all the boards are powered. If we start the data
 * collection before everything is powered on, then the system will immediately fault and not turn on
 * even though everything is safe.
 */
void preliminaryCheck(void){
	// Check if Watch dog timer was triggered previously
	if (WDTimer_DidSystemReset() == DANGER) {
		LED_On(FAULT);
		LED_On(WDOG);
		while(1);		// Spin
	}
}

/** faultCondition
 * This block of code will be executed whenever there is a fault.
 * If bps trips, make it spin and impossible to connect the battery to car again
 * until complete reboot is done.
 */
void faultCondition(void){
	Contactor_Off();
	LED_Off(RUN);
  LED_On(FAULT);
  
	uint8_t error = 0;

	if(!Current_IsSafe()){
		error |= FAULT_HIGH_CURRENT;
		LED_On(OCURR);
	}

	if(!Voltage_IsSafe()){
		// Toggle Voltage fault LED
		switch(Voltage_IsSafe()){
			case OVERVOLTAGE:
				error |= FAULT_HIGH_VOLT;
				LED_On(OVOLT);
				break;
				
			case UNDERVOLTAGE:
				error |= FAULT_LOW_VOLT;
				LED_On(UVOLT);
				break;

			default:
				error |= FAULT_VOLT_MISC;
				LED_On(OVOLT);
				LED_On(UVOLT);
				break;
		}
	}

	if(!Temperature_IsSafe(Current_IsCharging())){
		error |= FAULT_HIGH_TEMP;
		LED_On(OCURR);
	}
	
	// Log all the errors that we have
	for(int i = 1; i < 0x00FF; i <<= 1) {
		if(error & i) EEPROM_LogError(i);
	}
	
	// Log all the relevant data for each error
	for(int i = 1; i < 0x00FF; i <<= 1) {
		if((error & i) == 0) continue;
		
		SafetyStatus *voltage_modules;
		uint8_t *temp_modules;
		uint16_t curr;
		switch(i) {
		// Temperature fault handling
		case FAULT_HIGH_TEMP:
			temp_modules = Temperature_GetModulesInDanger();
			for(int j = 0; j < NUM_BATTERY_MODULES; ++j)
				if(temp_modules[j]) EEPROM_LogData(FAULT_HIGH_TEMP, j);
			break;
		
		// Voltage fault handling
		case FAULT_HIGH_VOLT:
		case FAULT_LOW_VOLT:
		case FAULT_VOLT_MISC:
			voltage_modules = Voltage_GetModulesInDanger();
			for(int j = 0; j < NUM_BATTERY_MODULES; ++j)
				if(voltage_modules[j]) EEPROM_LogData(i, j);
			break;
		
		// Current fault handling
		case FAULT_HIGH_CURRENT:
			curr = Current_GetLowPrecReading();
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & curr);
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & (curr >> 8));
			curr = Current_GetHighPrecReading();
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & curr);
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & (curr >> 8));
			break;
		}
	}
	
	while(1) {
		WDTimer_Reset();	// Even though faulted, WDTimer needs to be updated or else system will reset
					// causing WDOG error. WDTimer can't be stopped after it starts.
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
#define CAN_TEST_2


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
	UART3_Init(115200);
	LED_Init();
	Contactor_Init();

	// delay for UART to USB IC to bootup
	for(int i = 0; i < 1000000; i++);

	while(Voltage_Init(Minions) != SUCCESS) {
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
		Voltage_UpdateMeasurements();
		if(Voltage_IsSafe() != SAFE){
			break;
		}

		Contactor_On();
		LED_Toggle(RUN);
	}

	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		printf("Battery module %d voltage is %d \r\n", i, Voltage_GetModuleVoltage(i));
	}

	faultCondition();
}
#endif

#ifdef CURRENT_TEST
#include "UART.h"
#include "ADC.h"
int main(){
	UART3_Init(9600);
	Current_Init();	// Initialize the driver

	// Loop over the tests
	while(true) {
		Current_UpdateMeasurements();	// Get the most recent readings

		printf("\n\r==============================\n\rCurrent Test:\n\r");
		printf("ADC High: %d\n\r", ADC_ReadHigh());
		printf("ADC Low: %d\n\r", ADC_ReadLow());
		printf("Is the battery safe? %d\n\r", Current_IsSafe());
		printf("Is the battery charging? %d\n\r", Current_IsCharging());
		printf("High: %d\n\r", Current_GetHighPrecReading());
		printf("Low: %d\n\r", Current_GetLowPrecReading());

		for(int i = 0; i < 10000000; ++i);
	}
}
#endif

#ifdef TEMPERATURE_TEST
#include "UART.h"
#include "Temperature.h"

void singleSensorTest(void);												// Prints out a single sensor 
void individualSensorDumpTest(void);                 // Prints out each individual sensor temperature on all boards
void batteryModuleTemperatureTest(void);      			// Prints out every battery modules temperature average with their 2 sensors
void checkDangerTest(void);													// checks for danger

extern int16_t ModuleTemperatures[NUM_MINIONS][MAX_TEMP_SENSORS_PER_MINION_BOARD];

int main(){
	UART3_Init(9600);
	printf("I'm alive\n\r");
	while(Temperature_Init(Minions) != SUCCESS) {
		printf("Communication Failed.\n\r");
	}
	printf("Writing and Reading to Configuration Register Successful. Initialization Complete\n\r");

//  singleSensorTest();
//	individualSensorDumpTest();
//	moduleTemperatureDumpTest();
//	checkDangerTest();
	while(1){}
}

void singleSensorTest(void) {
	int sensorIndex = 0; // <-- replace this with which sensor you want to test
	Temperature_ChannelConfig(sensorIndex);
	while(1) {
		Temperature_GetRawADC(MD_422HZ_1KHZ);
		for (int i = 0; i < NUM_MINIONS; i++) {
			int temp = milliVoltToCelsius(Minions[i].aux.a_codes[0]*0.1);
			printf("Board %d Sensor %d : %d", i, sensorIndex, temp);
		}
	}
}

void individualSensorDumpTest(void) {
	while (1) {
		Temperature_UpdateMeasurements();
		for (int i = 0; i < NUM_MINIONS; i++) {
			for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
				printf("Board %d, Sensor %d: %d Celsius\r\n", i, j, ModuleTemperatures[i][j]);
				for(int delay = 0; delay < 800000; delay++){}
			}
		}
	}
}

void batteryModuleTemperatureTest (void) {
	while (1) {
		Temperature_UpdateMeasurements();
		for (int i = 0; i < NUM_MINIONS; i++) {
		    for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD/2; j++) {
			int moduleNum =  i * MAX_TEMP_SENSORS_PER_MINION_BOARD/2 + j;
		        printf("Board %d Battery Module %d Temp: %d Celsius\r\n", i,  moduleNum, Temperature_GetModuleTemperature(moduleNum));
		        for(int delay = 0; delay < 800000; delay++){}
		    }
		}
		printf("Total Average is %d\r\n", Temperature_GetTotalPackAvgTemperature());
		for(int delay = 0; delay < 800000; delay++){}
	}
}

void checkDangerTest(void) {
	int isCharging = 1;  // 1 if pack is charging, 0 if discharging
	printf("Danger Test\r\n");
	while (1) {
		Temperature_UpdateMeasurements();
		if (Temperature_IsSafe(isCharging) == ERROR) {
			printf("SOMETHINGS WRONG! AHHH\r\n");
			printf("----------Dumping Sensor data----------\r\n");
			uint8_t* dangerList = Temperature_GetModulesInDanger();
			for (int i = 0; i < NUM_MINIONS; i++) {
				if (dangerList[i] == 1) {
					printf("Board %d is in danger\r\n", i);
					for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
						printf("Board %d Sensor %d : %d Celsius\r\n", i, j, ModuleTemperatures[i][j]);
					}
				}
			}
			while (1){}
		} else {
			printf("we good.. \r\n");
		}
	}
}
#endif

#ifdef CONTACTOR_TEST
int main(){
	Contactor_Init();
	static uint32_t contactor_status = -1;
	Contactor_Off();
	for(int32_t i = 0; i < 1000000; i++);	// delay
	Contactor_On();
	contactor_status = Contactor_Flag();
	for(int32_t i = 0; i < 5000000; i++);	// delay
	Contactor_Off();
	contactor_status = Contactor_Flag();
	//while(1){
		//Contactor_On();
	//}
}
#endif

#ifdef WATCHDOG_TEST
int main(){
	WDTimer_Init();
	LED_Init();

	if(WDTimer_DidSystemReset() != SAFE){
		LED_On(WDOG);
		while(1);
	}

	WDTimer_Start();

	// reset WDTimer 10 times. With this counter, the watchdog timer should not reset the system shortly after it starts.
	for(int32_t i = 0; i < 10; i++){
		for(int32_t j = 0; j < 100000; j++);	// Delay
		LED_Toggle(RUN);
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
int main(){

	ADC_InitHilo();

	volatile int result = 0;
	volatile int delay = 0;

	while(1){
		result = ADC_ReadHigh();	// PA2
		delay = 0;
		result = ADC_ReadLow(); // PA3
		delay = 0;

		// Delay
		for(int i = 0; i < 1000; ++i);
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

#ifdef EEPROM_WRITE_TEST
//******************************************************************************************
#include "UART.h"

int main(){
	//initialize stuff
	UART3_Init(115200);
	__disable_irq();
	EEPROM_Init();
	__enable_irq();
	printf("initialized\n");

	EEPROM_Tester();		//write test codes
	printf("done");
	while(1){
		printf("done\n\r");
	};		//get stuck in loop

}

#endif

#ifdef EEPROM_READ_TEST
#include "UART.h"

int main(){
	UART1_Init(115200);

	printf("starting\n\r");
	__disable_irq();
	EEPROM_Init();
	__enable_irq();
	printf("initialized\n\r");
	EEPROM_Tester();
	printf("written\n\r");
	EEPROM_SerialPrintData();
	printf("done\n\r");
	while(1){};
}

#endif

#ifdef EEPROM_RESET
#include "UART.h"

int main() {
	UART1_Init(115200);

	printf("Starting reset\n\r");
	__disable_irq();
	EEPROM_Init();
	__enable_irq();
	printf("Initialized\n\r");
	//EEPROM_Load();
	//printf("Loaded\n\r");
	EEPROM_Reset();
	printf("EEPROM has been reset\n\r");\
	while(1);
}

#endif

#ifdef OPEN_WIRE_TEST
//******************************************************************************************
#include "Voltage.h"
#include <stdio.h>
#include "UART.h"
int main(){
	UART3_Init(115200);
	Voltage_Init(Minions);
	/*
	//printf("%x", Voltage_GetOpenWire());
	//printf("\n\r");
	static uint32_t open_wires = 0;
	open_wires = Voltage_GetOpenWire();
	if(Voltage_OpenWire() == DANGER){
		printf("return = DANGER\n\r");
		Voltage_OpenWireSummary();
		printf("\n\r");
	}
	else if(Voltage_OpenWire() == SAFE){
		printf("return = SAFE\n\r");
	}
	*/
	static uint16_t voltage = 0;
	Voltage_UpdateMeasurements();
	for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
		voltage = Voltage_GetModuleMillivoltage(i);
	}
}
#endif

#ifdef CAN_TEST
// ************************************
#include "CAN.h"
#include "UART.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Current.h"
#include "SoC.h"

// Define this if listening to yourself
//#define CAN_SELF_TEST

int CAN_send(uint32_t id)
{
	uint8_t length;
	uint8_t data[32];

	switch(id)
	{
		case CAN_ID_BPS_TRIP:
			length = 1;
			// b1 1-trip, 0-not
			data[0] = 0;
		break;

		case CAN_ID_BPS_ALL_CLEAR:
			length = 1;
			// b1 1-clear, 0-not
			// Check if everything is safe (all return SAFE = 0)
			if((Current_IsSafe() == SAFE) && (Temperature_IsSafe(Current_IsCharging()) == SAFE) && (Voltage_IsSafe() == SAFE))
				data[0] = 1;
			else 
				data[0] = 0;
		break;

		case CAN_ID_BPS_CONTACTOR: // contactor state
			length = 1;
			if (Contactor_Flag() == ENABLE)
				data[0] = 1;
			else
				data[0] = 0;
		break;

		case CAN_ID_CURRENT_DATA:
			length = 4;
			uint32_t* temp = Current_GetHighPrecReading();


			for (int i = 0; i < 4; i++)
			{
				data[i] = *temp;
				*temp = *temp>>8;
			}
		break;

		case CAN_ID_TOTAL_VOLTAGE_DATA:
			length = 2;
			// TODO : FIX THIS
		break;

		case CAN_ID_AVG_TEMPERATURE_DATA:
			length = 4;
			// TODO : FIX THIS
		break;

		case CAN_ID_SOC_DATA:
			length = 4;
			uint32_t *temp = SoC_GetPercent();

			for (int i = 0; i < 4; i++)
			{
				data[i] = *temp;
				*temp = *temp>>8;
			}
		break;

		case CAN_ID_WDOG_TRIGGERED: // only sent if happens
			length = 1;
			// 1-trip, 0-not
			data[0] = 1;
		break;

		case CAN_ID_ERROR: // only sent if happens
			length = 1;
			// 1-error, 0-not
			data[0] = 1;
		break;
	}

	int box;
	// Transmit the data
	do 
	{
		int box = CAN1_Write(id, &d2, 1);
	} 
	while (box == CAN_TxStatus_NoMailBox);
	
	int status;
	// Wait for the data to transmit
	do 
	{
		status = CAN_TransmitStatus(CAN1, box);
	} 
	while( status == CAN_TxStatus_Pending);
	
	if(status != CAN_TxStatus_Ok) 
		while(1);

	return 1;
}

int main() {
	// Start UART comms
	UART3_Init(115200);

	// Start CAN comms
	#ifdef CAN_SELF_TEST
	CAN1_Init(CAN_Mode_LoopBack);
	#else
	CAN1_Init(CAN_Mode_Normal);
	#endif

	// Data to transmit
	uint8_t data[8] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
	uint32_t id = CAN_ID_BPS_ALL_CLEAR;
	uint8_t d2 = 0x80;
	uint8_t RxData[8];

	while(1)
	{
		int box;
		// Transmit the data
		do 
		{
			int box = CAN1_Write(id, &d2, 1);
		} 
		while (box == CAN_TxStatus_NoMailBox);
		
		int status;
		// Wait for the data to transmit
		do 
		{
			status = CAN_TransmitStatus(CAN1, box);
		} 
		while( status == CAN_TxStatus_Pending);
		
		if(status != CAN_TxStatus_Ok) 
			while(1);

		// #ifdef CAN_SELF_TEST
		// Read all the data
		while(1) 
		{
			while(CAN1_Read(RxData) == false);

			printf("CAN RxMessage: ");
			for(int i = 0; i < 8; i++)
			{
				printf("%d", RxData[i]);
			}
			printf("\n");
		}
		// #endif
	}
}

#endif

#ifdef CAN_TEST_2
#include "CAN.h"
/*message:
typedef enum {
	TRIP = 0x02,
	ALL_CLEAR = 0x101,
	CONTACTOR_STATE = 0x102,
	CURRENT_DATA = 0x103,
	VOLT_DATA = 0x104,
	TEMP_DATA = 0x105,
	SOC_DATA = 0x106,
	WDOG_TRIGGERED = 0x107,
	CAN_ERROR = 0x108,
} CANMessage_t;

data: 
typedef union {
	uint8_t b;
	uint16_t h;
	uint32_t w;
	float f;
} CANData_t;

data is either a float, uint32_t, or uint8_t
*/

int main(void){
	uint8_t trip = 0;
	uint8_t contact = 1;
	uint8_t clear = 1;
	float voltage = 3.400;
	float temperature = 25.00;
	float current = 50.00;
	float soc = 80.00;
	
	CANData_t data;
	data.idx = 1;
	
	CAN1_Init(CAN_Mode_Normal);
	
	data.b = trip;
	CAN1_Send(TRIP, (CANData_t) data);
	
	data.b = clear;
	CAN1_Send(ALL_CLEAR, (CANData_t) data);
	
	data.b = contact;
	CAN1_Send(CONTACTOR_STATE, (CANData_t) data);
	
	for(uint32_t i = 0; i < 1000; i++){
		for(uint32_t j = 0; j < 1000; j++){
		}
	}
	
	data.f = current;
	CAN1_Send(CURRENT_DATA, data);
	
	data.f = voltage;
	CAN1_Send(VOLT_DATA, data);
	
	data.f = temperature;
	CAN1_Send(TEMP_DATA, data);
	
	for(uint32_t i = 0; i < 1000; i++){
		for(uint32_t j = 0; j < 1000; j++){
		}
	}
	
	data.f = soc;
	CAN1_Send(SOC_DATA, data);
	
	data.b = 0;
	CAN1_Send(WDOG_TRIGGERED, data);
	
	data.b = 0;
	CAN1_Send(CAN_ERROR, data);
	while(1){}
}
#endif
