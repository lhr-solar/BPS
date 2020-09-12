/** tests.c
 * File containing all tests that were
 * previously in main.c
 * Temporary location until we create 
 * a Makefile that allows for the project
 * to be compiled with various test mains
 */

//****************************************************************************************
// The following code is for testing individual pieces of code.
//****************************************************************************************
// If you want to test an individual module, change the #define NAME to what you want to
// to use. This is used mainly to save memory for both the microcontroller and Keil.
// Keil has a limit of how much memory you can compile.
// E.g. If you want to run a LTC6811 test, change "#define CHANGE_THIS_TO_TEST_NAME" to the
//		following:
//		#define LTC6811_TEST
#define NO_TEST

#ifdef Systick_TEST

int main(){
		__enable_irq();
		while(1){
		DelayMs(1000);
		Contactor_On();
		DelayMs(1000);
		Contactor_Off();
	}
}

#elif defined LED_TEST

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

#elif defined LTC6811_TEST

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

#elif defined VOLTAGE_TEST

#include "UART.h"

int main(){
	UART3_Init();
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
		if(Voltage_CheckStatus() != SAFE){
			break;
		}

		Contactor_On();
		LED_Toggle(RUN);
	}

	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		printf("Battery module %d voltage is %d \r\n", i, Voltage_GetModuleMillivoltage(i));
	}

	faultCondition();
}

#elif defined CURRENT_TEST

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
		printf("Is the battery safe? %d\n\r", Current_CheckStatus(false));
		printf("Is the battery charging? %d\n\r", Current_IsCharging());
		printf("High: %d\n\r", Current_GetHighPrecReading());
		printf("Low: %d\n\r", Current_GetLowPrecReading());

		for(int i = 0; i < 10000000; ++i);
	}
}

#elif defined TEMPERATURE_TEST

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
	while(1) {
		Temperature_UpdateSingleChannel(sensorIndex);
		for (int board = 0; board < NUM_MINIONS; board++) {
			printf("Board %d Sensor %d : %d", board, sensorIndex, Temperature_GetSingleTempSensor(board, sensorIndex));
		}
	}
}

void individualSensorDumpTest(void) {
	while (1) {
		Temperature_UpdateAllMeasurements();
		for (int board = 0; board < NUM_MINIONS; board++) {
			for (int sensor = 0; sensor < MAX_TEMP_SENSORS_PER_MINION_BOARD; sensor++) {
				printf("Board %d, Sensor %d: %d Celsius\r\n", board, sensor, Temperature_GetSingleTempSensor(board, sensor));
				for(int delay = 0; delay < 800000; delay++){}
			}
		}
	}
}

void batteryModuleTemperatureTest (void) {
	while (1) {
		Temperature_UpdateAllMeasurements();
		for (int board = 0; board < NUM_MINIONS; board++) {
		    for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD/2; j++) {
						int moduleNum =  board * MAX_TEMP_SENSORS_PER_MINION_BOARD/2 + j;
		        printf("Board %d Battery Module %d Temp: %d Celsius\r\n", board,  moduleNum, Temperature_GetModuleTemperature(moduleNum));
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
		Temperature_UpdateAllMeasurements();
		if (Temperature_CheckStatus(isCharging) == ERROR) {
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

#elif defined CONTACTOR_TEST

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

#elif defined WATCHDOG_TEST

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

#elif defined SPI_TEST

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

#elif defined UART_TEST

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

#elif defined I2C_TEST

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

#elif defined GYRO_TEST

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

#elif defined ADC_TEST

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

#elif defined SOC_TEST

// *****************************************************************************************
#include "Charge.h"
#include <stdio.h>
#include <UART.h>
void ChargingChargeTest(void);
void DischargingChargeTest(void);

extern uint32_t fixedPoint_Charge;
extern float float_Charge;

int main(){
	UART3_Init(9600);
	Charge_Init();
	//ChargingChargeTest();
	//DischargingChargeTest();
}

void ChargingChargeTest(void) {
	char str[50];
	sprintf(str,"Starting Charge Charging Test..");
	UART3_Write(str, strlen(str));

	fixedPoint_Charge = 0;
	float_Charge = 0;
	while(1){
		Charge_Calculate(500); 									// Charging with 500 mA, should take a while
		sprintf(str,"Charge: %.2f%%\r\n",float_Charge);
		UART3_Write(str, strlen(str));
	}
}

void DischargingChargeTest(void) {
	char str[50];
	sprintf(str,"Starting Charge Discharging Test..");
	UART3_Write(str, strlen(str));

	fixedPoint_Charge = 10000;
	float_Charge = 100.00;
	while(1){
		Charge_Calculate(-500); 									// Consuming 500 mA, should take a while
		sprintf(str,"Charge: %.2f%%\r\n",float_Charge);
		UART3_Write(str, strlen(str));
	}
}

/** Tests
 * 	TODO: Need to test SetAccumulator, GetPercent and Calibrate on faults
 */

#elif defined Current_CheckStatus_Test
#include "uart.h"
extern int32_t LowPrecisionCurrent;

int main() {
	int32_t LowPrecisionTestValue = -60;
	int8_t LoopNum = 20;
	UART3_Init();
	printf("0 means SAFE, 1 means DANGER\n\r\n\r");
	while(LoopNum>0){
		LowPrecisionTestValue+=10;
		LoopNum-=1;
		//Current_UpdateMeasurements();
		LowPrecisionCurrent = LowPrecisionTestValue;
		printf("TestCurrent = %d \n\r", LowPrecisionCurrent);
		printf("Override Off yields %d \n\r", Current_CheckStatus(false));
		printf("Override Set yields %d \n\r", Current_CheckStatus(true));
		printf("\n\r");
}
}

#elif defined EEPROM_WRITE_TEST

//******************************************************************************************
#include "UART.h"

int main(){
	//initialize stuff
	UART3_Init();
	__disable_irq();
	EEPROM_Init();
	__enable_irq();
	EEPROM_Reset();
	printf("initialized\n");

	EEPROM_Tester();		//write test codes
	printf("done");
	while(1){
		printf("done\n\r");
	};		//get stuck in loop

}

#elif defined EEPROM_READ_TEST

#include "UART.h"

int main(){
	UART3_Init();

	printf("starting\n\r");
	__disable_irq();
	EEPROM_Init();
	__enable_irq();
	EEPROM_Reset();
	printf("initialized\n\r");
	EEPROM_Tester();
	printf("written\n\r");
	EEPROM_SerialPrintData();
	printf("done\n\r");
	while(1){};
}

#elif defined EEPROM_RESET

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

#elif defined UART_INTERRUPT2
    /* Includes ---------------------------------------------------*/
    //#include "stm32f2xx.h"
	#include "UART.h"
		
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
			
#elif defined UART_INTERRUPT
#include "UART.h"
#include "FIFO.h"
int main(void){
	UART3_Init();
	Fifo uartFifo;
	fifoInit(&uartFifo);
	__enable_irq();
	char buffer[fifo_size];
	while(1){
		UART3_CheckAndEcho(&uartFifo);
		if (UART3_HasCommand(&uartFifo)){
			UART3_GetCommand(&uartFifo, buffer);
			printf("\n\r%s\n\r", buffer);//service command
		}
	}
}

#elif defined OPEN_WIRE_TEST

//******************************************************************************************
#include "Voltage.h"
#include <stdio.h>
#include "UART.h"
int main(){
	UART3_Init(115200);
	Voltage_Init(Minions);
	/*
	//printf("%x", Voltage_Get
  
  
  ());
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

#elif defined FULL_TEST

#include "UART.h"
//tests the BPS functions
int main(void) {
	__disable_irq();		// Disable all interrupts until initialization is done
	initialize();			// Initialize codes/pins
	preliminaryCheck();		// Wait until all boards are powered on
	__enable_irq();			// Enable interrupts

	//WDTimer_Start();
	UART3_Init();

	bool override = false;		// This will be changed by user via CLI	
	printf("initialized\n\r");
	// First update the measurements.
	Voltage_UpdateMeasurements();
	SafetyStatus *voltageModulesInDanger;
	voltageModulesInDanger = Voltage_GetModulesInDanger();
	printf("Voltage modules in danger: ");
	for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++) {
		if (voltageModulesInDanger[i] != SAFE){
			printf("%d ", i);
		}
	}
	printf("\n\r");
	
	for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){
		uint16_t voltage = Voltage_GetModuleMillivoltage(i);
		//print fixed point millivoltage of module
		printf("module %d millivolts: %d.%d%d%d%d%dV\n\r", i, voltage/100000, (Voltage_GetModuleMillivoltage(i)%100000)/10000, (Voltage_GetModuleMillivoltage(i)%1000000)/1000, (Voltage_GetModuleMillivoltage(i)%1000000)/100, (Voltage_GetModuleMillivoltage(i)%10000000)/10, (Voltage_GetModuleMillivoltage(i)%100000000)); 
	}
	
	Current_UpdateMeasurements();
	
	if (Current_CheckStatus(override) == SAFE){
		printf("current is safe\n\r");
	}else{
		printf("current is not safe\n\r");
	}
	
	printf("current high precision: %d\n\r", Current_GetHighPrecReading());
	printf("current low precision: %d\n\r", Current_GetLowPrecReading());
	
	Temperature_UpdateAllMeasurements();
	
	uint8_t *tempModulesInDanger;
	tempModulesInDanger = Temperature_GetModulesInDanger();
	printf("Temperature modules in danger: ");
	for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++) {
		if (tempModulesInDanger[i] == DANGER){
			printf("%d ", i);
		}
	}
	printf("\n\r");
	
	for (uint8_t i = 0; i < NUM_TEMPERATURE_SENSORS; i++){
		uint16_t temperature = Temperature_GetSingleTempSensor(i/2, i%2);
		printf("temperature sensor %d: %d*C\n\r", i, temperature);
	}
		
	SafetyStatus current = Current_CheckStatus();
	SafetyStatus temp = Temperature_CheckStatus(Current_IsCharging());
	SafetyStatus voltage = Voltage_CheckStatus();

	// Check if everything is safe (all return SAFE = 0)
	if((current == SAFE) && (temp == SAFE) && (voltage == SAFE) && !override) {
		Contactor_On();
		printf("everything is safe\n\r");
	}
	else if((current == SAFE) && (temp == SAFE) && (voltage == UNDERVOLTAGE) && override) {
		Contactor_On();
		printf("contactor on (override on)\n\r");
	} else {
		printf("not safe!\n\r");
	}

	Contactor_On();
	for (led signal = FAULT; signal <= EXTRA; signal++){
		LED_On(signal);
	}
	for (volatile int i = 0; i < 1000000; i++){}
	Contactor_Off();
	for (led signal = FAULT; signal <= EXTRA; signal++){
		LED_Off(signal);
	}
	
	EEPROM_Reset();
	EEPROM_Tester();
	EEPROM_SerialPrintData();
	
	// Update necessary
	// CAN_SendMessageStatus()	// Most likely need to put this on a timer if sending too frequently

	WDTimer_Reset();

	// BPS has tripped if this line is reached
	faultCondition();
	while(1){};
	return 0;
}

#elif defined CLI_TEST
int main(){
	initialize();
	__enable_irq();
	Contactor_On();
	char command[fifo_size];
	CLI_Startup();
	while(1) {
		UART3_CheckAndEcho(&CLIFifo);
		if (UART3_HasCommand(&CLIFifo)) {
			UART3_GetCommand(&CLIFifo, command);
			CLI_Handler(command);
		}
	}
}

#elif defined CAN_TEST_2
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
	PLL_Init80MHz();
	uint8_t a = 0;
	uint32_t b = 30000;
	uint16_t d = 30000;
	float c = 80.00;
	CAN1_Init(CAN_Mode_Normal);
	CAN1_Send(TRIP, (CANData_t) a);
	CAN1_Send(ALL_CLEAR, (CANData_t) a);
	CAN1_Send(CONTACTOR_STATE, (CANData_t) a);
	CAN1_Send(CURRENT_DATA, (CANData_t) b);
	CAN1_Send(VOLT_DATA, (CANData_t) d);
	CAN1_Send(TEMP_DATA, (CANData_t) b);
	CAN1_Send(SOC_DATA, (CANData_t) c);
	CAN1_Send(WDOG_TRIGGERED, (CANData_t) a);
	CAN1_Send(CAN_ERROR, (CANData_t) a);
}

#elif defined CAN_TEST

// ************************************
#include "CAN.h"
#include "UART.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Current.h"
#include "Charge.h"

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
			uint32_t *temp = Charge_GetPercent();

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
	}
}

#elif defined CAN_TEST_2

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
	
	CANPayload_t payload;
	payload.idx = 1;
	
	CAN1_Init(CAN_Mode_Normal);
	
	payload.data.b = trip;
	CAN1_Send(TRIP, payload);
	
	payload.data.b = clear;
	CAN1_Send(ALL_CLEAR, payload);
	
	payload.data.b = contact;
	CAN1_Send(CONTACTOR_STATE, payload);
	
	for(uint32_t i = 0; i < 1000; i++){
		for(uint32_t j = 0; j < 1000; j++){
		}
	}
	
	payload.data.f = current;
	CAN1_Send(CURRENT_DATA, payload);
	
	payload.data.f = voltage;
	CAN1_Send(VOLT_DATA, payload);
	
	payload.data.f = temperature;
	CAN1_Send(TEMP_DATA, payload);
	
	for(uint32_t i = 0; i < 1000; i++){
		for(uint32_t j = 0; j < 1000; j++){
		}
	}
	
	payload.data.f = soc;
	CAN1_Send(SOC_DATA, payload);
	
	payload.data.b = 0;
	CAN1_Send(WDOG_TRIGGERED, payload);
	
	payload.data.b = 0;
	CAN1_Send(CAN_ERROR, payload);
	while(1){}
}

#endif