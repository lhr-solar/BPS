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

cell_asic Minions[NUM_MINIONS];

void initialize(void);
void preliminaryCheck(void);
void faultCondition(void);

int mainmain(){
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
	return 0;
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
}

/** faultCondition
 * This block of code will be executed whenever there is a fault.
 * If bps trips, make it spin and impossible to connect the battery to car again
 * until complete reboot is done.
 */
void faultCondition(void){
	Contactor_Off();
	LED_Off(RUN);

	while(1){
		// CAN_SendMessageStatus()
		if(!Current_IsSafe()){
			// Toggle Current fault LED
		}

		if(!Voltage_IsSafe()){
			// Toggle Voltage fault LED
			switch(Voltage_IsSafe()){
				case OVERVOLTAGE:
					LED_On(OVOLT);
					break;

				case UNDERVOLTAGE:
					LED_On(UVOLT);
					break;

				default:
					break;
			}
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
#define UART_INTERRUPT


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

	while(Voltage_Init() != SUCCESS) {
		printf("Communication Failed.\n\r");
	}
	printf("Writing and Reading to Configuration Register Successful. Initialization Complete\n\r");

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
//1.154
#ifdef TEMPERATURE_TEST
#include "UART.h"
#include "Temperature.h"

// Backspace needs to be fixed for scanf
void dumpRawData(void);
void checkModuleTemperatureTest(void);
void checkIndividualSensorTest(void);
void checkDangerTest(void);
void checkMasterTest(void);						// To implement later. Able to check everything with easeee
void individualSensorDumpTest(void);
void moduleTemperatureDumpTest(void);

extern int16_t ModuleTemperatures[NUM_TEMPERATURE_BOARDS][20];
extern cell_asic TemperatureModule[NUM_MINIONS];

#include "UART.h"



int main(){
	UART3_Init(9600);
	printf("I'm alive\n\r");

	Temperature_Init();
//	individualSensorDumpTest();
//	checkIndividualSensorTest();
//	moduleTemperatureDumpTest();
//	checkModuleTemperatureTest();
//	checkDangerTest();
//  dumpRawData();
	while(1){}
}

/***** just testing the temperature sensor w/o battery ****/
void checkIndividualSensorTest(void) {
	printf("Individual Sensor Test\r\n");
	int moduleNum;
	int sensorToTest = 0;
	int loopLen = 1;

	while(1) {
		printf("Enter Board Number: ");
		while(scanf("%d", &moduleNum) == EOF || moduleNum > NUM_TEMPERATURE_BOARDS || moduleNum <= 0) {
			printf("\r\nERROR -- please input valid board number: ");
		}
		printf("\r\nSensor Number (-1 for all): ");
		while(scanf("%d", &sensorToTest) == EOF || sensorToTest > NUM_SENSORS_ON_TEMP_BOARD_1) {
			printf("\r\nERROR -- please input valid sensor number: ");
		}
		printf("\r\n");

		printf("How many times do you want to check? (input -1 if you want infinite)\r\n");
		scanf("%d", &loopLen);
		if (loopLen > 0) {
			for (int loop = 0; loop < loopLen; loop++) {
					Temperature_UpdateMeasurements();
					if (sensorToTest == -1 ) {
						for (int i = 0; i < NUM_SENSORS_ON_TEMP_BOARD_1; i++) {
							printf("Board %d, Sensor %d: %d Celsius\r\n", moduleNum, i+1, ModuleTemperatures[moduleNum-1][i]);
						}
					} else {
						printf("Board %d, Sensor %d: %d Celsius\r\n", moduleNum, sensorToTest, ModuleTemperatures[moduleNum-1][sensorToTest-1]);
					}
				}
			}
		else {
			while (1) {
				Temperature_UpdateMeasurements();
				if (sensorToTest == -1 ) {
					for (int i = 0; i < NUM_SENSORS_ON_TEMP_BOARD_1; i++) {
						printf("Board %d, Sensor %d: %d Celsius\r\n", moduleNum, i+1, ModuleTemperatures[moduleNum-1][i]);
					}
				} else {
					printf("Board %d, Sensor %d: %d Celsius\r\n", moduleNum, sensorToTest, ModuleTemperatures[moduleNum-1][sensorToTest-1]);
				}
			}
		}
	}
}

void individualSensorDumpTest(void) {
	while (1) {
		Temperature_UpdateMeasurements();
		for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
			for (int j = 0; j < NUM_SENSORS_ON_TEMP_BOARD_1; j++) {
				printf("Board %d, Sensor %d: %d Celsius\r\n", i+1, j+1, ModuleTemperatures[i][j]);
				//for(int delay = 0; delay < 800000; delay++){}
			}
		}
	}
}

void checkDangerTest(void) {
	int isCharging;
	printf("Danger Test\r\n");
	printf("Discharging or Charging? (0/1)\r\n");
	scanf("%d", &isCharging);
	while (1) {
		Temperature_UpdateMeasurements();
		if (Temperature_IsSafe(isCharging) == ERROR) {
			printf("SOMETHINGS WRONG! AHHH\r\n");
			printf("----------Dumping Sensor data----------\r\n");
			uint8_t* dangerList = Temperature_GetModulesInDanger();
			for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
				if (dangerList[i] == 1) {
					printf("Board %d is in danger\r\n", i+1);
					for (int j = 0; j < NUM_SENSORS_ON_TEMP_BOARD_1; j++) {
						printf("Board %d Sensor %d : %d Celsius\r\n", i+1, j+1, ModuleTemperatures[i][j]);
					}
				}
			}
			while (1){}
		} else {
			printf("we good.. \r\n");
		}
	}
}
void moduleTemperatureDumpTest (void) {
	while (1) {
		Temperature_UpdateMeasurements();
		for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
			printf("Module %d Temp: %d Celsius\r\n", i+1, Temperature_GetModuleTemperature(i));
			//for(int delay = 0; delay < 800000; delay++){}
		}
		printf("Total Average is %d\r\n", Temperature_GetTotalPackAvgTemperature());
		//for(int delay = 0; delay < 800000; delay++){}
	}
}

void checkModuleTemperatureTest(void) {
	int moduleToCheck;				// index of module to check average temperature
	int32_t loopLen;					// how many times you are checking the temperature
	bool checkAllFlag = false;
	while (1) {
		// User input
		printf("Please input board number to test or 7 to check all: ");
		while(scanf("%d", &moduleToCheck) == EOF || moduleToCheck > NUM_TEMPERATURE_BOARDS || moduleToCheck <= 0) {
			printf("\r\nERROR -- please input valid board number: ");
		}
		if (moduleToCheck != 7) {
			printf("\r\nTesting module %d..\r\n", moduleToCheck);
		}
		else {
			printf("\r\nTesting all modules...\r\n");
			checkAllFlag = true;
		}

		printf("How many times do you want to check? (input -1 if you want infinite)\r\n");
		scanf("%d", &loopLen);

		// Temperature checking
		if (loopLen >= 0) {
			printf("----------START----------\r\n");
			for (int i = 0; i < loopLen; i++) {
				Temperature_UpdateMeasurements();
				if(checkAllFlag) {
					for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
						printf("Module %d Temp: %d Celsius\r\n", i, Temperature_GetModuleTemperature(i));
					}
					printf("Total Average is %d\r\n", Temperature_GetTotalPackAvgTemperature());
				} else {
					printf("Module %d Temp: %d Celsius\r\n", moduleToCheck, Temperature_GetModuleTemperature(moduleToCheck));
				}
			}
			checkAllFlag = false;
			printf("----------DONE----------\r\n");
		} else {
			while (1) {
				Temperature_UpdateMeasurements();
				if(checkAllFlag) {
					for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
						printf("Module %d Temp: %d Celsius\r\n", i, Temperature_GetModuleTemperature(i));
					}
					printf("Total Average is %d\r\n", Temperature_GetTotalPackAvgTemperature());
				} else {
					printf("Module %d Temp: %d Celsius\r\n", moduleToCheck, Temperature_GetModuleTemperature(moduleToCheck));
				}
			}
		}
/*
	int32_t buffer[12];

	Temperature_Init();
	printf("Activated\n\r");

	LTC2983_ReadConversions(buffer, TEMP_CS1, 1);
	while(1){
		int32_t buf[12] = {0};
		LTC2983_StartMeasuringADC(TEMP_CS1);

		LTC2983_ReadConversions(buf, TEMP_CS1, 1);
					printf("\n\r");
		//for(int i = 0; i < 12; i++) {
		int i = 0;
			LTC2983_ConversionValidity((uint8_t)(buf[i] >> 24));

			printf("Channel %d: %f\n\r", i+1, ((float)(buf[i] & 0x007FFFFF) /2097152));
		//}
		for(int i = 0; i < 10000000; i++);
*/
	}
}

void dumpRawData(void){
	UART3_Init(115200);
	LED_Init();

	// delay for UART to USB IC to bootup
	for(int i = 0; i < 1000000; i++);

	while(Voltage_Init() != SUCCESS) {
		printf("Communication Failed.\n\r");
	}
	printf("Writing and Reading to Configuration Register Successful. Initialization Complete\n\r");

	while(1){
		printf("ADC Raw Data: %d\n\r", Temperature_GetRawADC(MD_422HZ_1KHZ));
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
	UART1_Init(115200);
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

#ifdef UART_INTERRUPT2
    /* Includes ---------------------------------------------------*/
    //#include "stm32f2xx.h"
		#include "uart.h"
		
		uint8_t RxData;//data received flag
				
    /* Private function prototypes --------------------------------*/
    void USART1_Config(void);

    /* Private functions ------------------------------------------*/
		
		/**
		 * @brief This function handles USART3 global interrupt request.
     * @param None
     * @retval None
     */
    void USART1_IRQHandler(void){
			if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
     /* Read one byte from the receive data register */
          RxData = USART_ReceiveData(USART1);
      }
    }
		
    /**
     * @brief Main program
     * @param None
     * @retval None
     * */
		#define size 32
     int main(void)
     {
			 char fifo[size];
			 uint8_t head = 0;
			 uint8_t tail = 0;
			 __enable_irq();
			 
      NVIC_InitTypeDef NVIC_InitStructure;
     /****************************************************************
      -Step1-
      Configure the peripheral and enable the interrupt generation
     ****************************************************************/
      /* Configure USART3 with receive interrupt enabled (generated
      when the receive data register is not empty) */
      USART1_Config();
     /****************************************************************
      -Step2-
      Enable peripheral IRQ channel in the NVIC controller
     ****************************************************************/

      /* Enable USART3 IRQ channel in the NVIC controller.
      When the USART3 interrupt is generated (in this example when
      data is received) the USART3_IRQHandler will be served */
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
     /****************************************************************
      -Step3-
      Implement the peripheral interrupt handler (PPP_IRQHandler)
      in stm32f2xx_it.c file.
     ****************************************************************/
      /* Refer to next section "stm32f2xx_it.c" */

      while (1)
      {
				//do something important
				if ((RxData) && (((head + 1) % size) != tail)){
					//insert data
						
					fifo[head] = (char) RxData;
					head = ((head + 1) % size);
					if ((RxData == 0x0d) && ((head + 1) % size) != tail){//if carriage return and space in queue
						fifo[head] = 0x0a;//put line feed in
						head = ((head + 1) % size);
					}
					RxData = 0;
				}
				if (head != tail) {
					printf("%c", fifo[tail]);
					tail = (tail +1) % size;
				}else{
					//printf(".");
				}
      }
     }
	 
     /**
      * @brief Configures the USART3 Peripheral.
      * @param None
      * @retval None
      */
     void USART1_Config(void)
     {
      GPIO_InitTypeDef GPIO_InitStructure;
      USART_InitTypeDef USART_InitStructure;
      /* USART IOs configuration ***********************************/
      /* Enable GPIOA clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      /* Connect PA9 to USART1_Tx */
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART3);
      /* Connect PA10 to USART1_Rx*/
         GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART3);
          /* Configure USART1_Tx and USART3_Rx as alternate function */
          GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
          GPIO_Init(GPIOA, &GPIO_InitStructure);
          /* USART configuration ***************************************/
          /* USART3 configured as follow:
          - BaudRate = 115200 baud
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Hardware flow control disabled (RTS and CTS signals)
          - Receive and transmit enabled
          */
          /* Enable USART1 clock */
          RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

          USART_InitStructure.USART_BaudRate = 115200;
          USART_InitStructure.USART_WordLength = USART_WordLength_8b;
          USART_InitStructure.USART_StopBits = USART_StopBits_1;
          USART_InitStructure.USART_Parity = USART_Parity_No;
          USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
          USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
          USART_Init(USART1, &USART_InitStructure);
          /* Enable USART1 */
          USART_Cmd(USART1, ENABLE);

          /* Enable USART1 Receive interrupt */
          USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
      }
#endif
			
#ifdef UART_INTERRUPT
#include "uart.h"
#include "fifo.h"
int main(void){
	USART1_Config();
	Fifo uartFifo;
	fifoInit(&uartFifo);
	__enable_irq();
	extern uint8_t RxData;
	char buffer[fifo_size];
	while(1){
		checkUARTandEcho(&uartFifo);
		if (hasCommand(&uartFifo)){
			getCommand(&uartFifo, buffer);
			printf("\n\r%s\n\r", buffer);
		}
	}
}
	
			
#endif
