#include "BSP_I2C.h"
#include "stm32f4xx.h"

#define TIMEOUT_THRESHOLD   1200000 // 15 ms delay threshold (3x the write time)
//Manthan Upadhyaya wuz here: 10/2020
#ifdef RTOS
static OS_Q I2Ctx, I2Crx;
static const OS_MSG_QTY AMT = 1028;
static OS_ERR err;
static CPU_TS time;
static OS_MSG_SIZE one = 1;
#endif

//The variables below are to determine what process the I2C is doing
static bool receiving = false, sending = false, startreceiving = false;
static uint8_t* txData;
static uint16_t devAddresstx, devAddressrx;
static uint16_t regAddresstx, regAddressrx;
static uint8_t* readStore;
static uint32_t txLength, rxLength;

#ifdef BAREMETAL
#define FIFOSIZE 100
typedef struct I2CData{
	uint8_t devAddr;
	uint16_t regAddress;
	uint8_t* data;
	uint32_t length;
};
static struct I2CData I2Crx[FIFOSIZE];
static uint16_t rxFifoHead = 0;
static uint16_t rxFifoTail = 0;
static bool rxFifoReceived = false;
static struct I2CData I2Ctx[FIFOSIZE];
static uint16_t txFifoHead = 0;
static uint16_t txFifoTail = 0;
static bool txFifoReceived = false;
#endif

/**
 * @brief   Initializes the I2C port that interfaces with the EEPROM.
 * @param   None
 * @return  None
 */
void BSP_I2C_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	// Uses PA8 for SCL and PC9 for SDA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	// Initialize PA8 for SCL on I2C3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;     // Watch out for Output Type. I2C is an open-drain comm protocol unlike SPI/UART
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Initialize PC9 for SDA on I2C3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;          // Only need to change pin since the rest of the pin configurations are the same
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Set which alternate function mode we want for these pins.
	// We want to make these pins use I2C3.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);     // PA8
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);     // PC9

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;						// regular mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;             // 50% duty cycle on clock
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		// 7-bit addressing mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                    // Enable ack to confirm if the slave device has received the data or not
	I2C_InitStruct.I2C_ClockSpeed = 100000;                     // Generic speed
	I2C_InitStruct.I2C_OwnAddress1 = 0x50 << 1;                 // Any address will do. I just chose the same address as the EEPROM.
                                                                // Note I left shifted the address. That's because the address is supposed
                                                                // to be 7-bits and the least significant bit indicates the R/W setting
	I2C_Init(I2C3, &I2C_InitStruct);
	I2C_ITConfig(I2C3, I2C_IT_EVT, ENABLE); //Enable interrupts
	I2C_Cmd(I2C3, ENABLE);
	#ifdef RTOS
	OSQCreate(&I2Crx, "I2C RX", AMT, &err);
	OSQCreate(&I2Ctx, "I2C TX", AMT, &err);
	#endif
}
/*****THE FOLLOWING CODE IS FOR THE BARE METAL VERSION OF THE BPS*****/
#ifdef BAREMETAL
void I2C3_EV_IRQHandler(void){
	if (!I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)) {
		I2C_AcknowledgeConfig(I2C3, ENABLE);
		// Since no one is using the I2C bus, take control
		I2C_GenerateSTART(I2C3, ENABLE);
		return;
	}
	// Wait until start edge event occurred
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)){
		// Select device to talk to
		if(txFifoReceived){ //if there is data
			devAddresstx = I2Ctx[txFifoTail].devAddr;
			txLength = I2Ctx[txFifoTail].length;
			regAddresstx = I2Ctx[txFifoTail].regAddress;
			txData = I2Ctx[txFifoTail].data; //read address to get data from
			txFifoTail = (txFifoTail + 1) % FIFOSIZE;
			I2C_Send7bitAddress(I2C3, devAddresstx, I2C_Direction_Transmitter);
			sending = true;
		}
		if (!sending && rxFifoReceived){
			devAddressrx = I2Crx[rxFifoTail].devAddr;
			readStore = I2Crx[rxFifoTail].data; //get address to store in
			rxLength = I2Crx[rxFifoTail].length; //get length of data
			regAddressrx = I2Crx[rxFifoTail].regAddress; //get address to read from
			rxFifoTail = (rxFifoTail + 1) % FIFOSIZE;
			I2C_Send7bitAddress(I2C3, devAddressrx, I2C_Direction_Transmitter);
			receiving = true;
		}
		return;
	}
	if (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && 
		I2C_GetLastEvent(I2C3) == I2C_EVENT_MASTER_MODE_SELECT){
		if(I2C3->SR1 & 0x0400 && !receiving) {
			I2C3->SR1 &= ~0x0400;
			I2C_GenerateSTOP(I2C3, ENABLE);
		}
	}
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT) && 
		(I2C_GetLastEvent(I2C3) != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		// Select device to talk to
		if(sending) I2C_Send7bitAddress(I2C3, devAddresstx, I2C_Direction_Transmitter);
		if(!sending && receiving) I2C_Send7bitAddress(I2C3, devAddressrx, I2C_Direction_Transmitter);
		return;
	}
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		// Send start address (MSB first)
		if(txFifoReceived) I2C_SendData(I2C3, (uint8_t)((regAddresstx & 0xFF00) >> 8));
		if(!sending && rxFifoReceived) I2C_SendData(I2C3, (uint8_t)((regAddressrx & 0xFF00) >> 8));
		return;
	}
	//send rest of start address (LSB)
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING) && 
		I2C_GetLastEvent(I2C3) == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED){
		if (sending) I2C_SendData(I2C3, (uint8_t)(regAddresstx & 0x00FF));
		if (receiving && !sending) I2C_SendData(I2C3, (uint8_t)(regAddressrx & 0x00FF));
		return;
	}
	//if byte has been transmitted
	if(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == SET){
		if (sending){
			I2C_SendData(I2C3, *txData);
			txLength--; //decrement counter
			if (txLength == 0){
				I2C_GenerateSTOP(I2C3, ENABLE);
				sending = false;
			}
			txData++; //go to next element to send
		}
		if (receiving && !sending){
			if (!startreceiving){
				I2C_GenerateSTART(I2C3, ENABLE);
				startreceiving = true;
			}
			if(startreceiving){
				while (rxLength > 0){
					if (rxLength > 1){
						*readStore = I2C_ReceiveData(I2C3);
						readStore++;
						rxLength--;
					}
					else{
						// Disable ack, since this is the last byte
						I2C_AcknowledgeConfig(I2C3, DISABLE);
						*readStore = I2C_ReceiveData(I2C3);
						readStore++;
						rxLength = 0;
					}
					// Generate the stop
					receiving = false;
					startreceiving = false;
					I2C_GenerateSTOP(I2C3, ENABLE);
				}
			}
		}
	}
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param	deviceAddr : the device address to write to
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
	//if the head isn't the same as the tail or it is empty
	if ((txFifoHead != txFifoTail) || txFifoReceived == false){
		I2Ctx[txFifoHead].devAddr = deviceAddr;
		I2Ctx[txFifoHead].regAddress = regAddr;
		I2Ctx[txFifoHead].data = txData;
		I2Ctx[txFifoHead].length = txLen;
		txFifoHead = (txFifoHead + 1)%FIFOSIZE;
		txFifoReceived = true;
		return true;
	}
	return false; //return false if fifo is full
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param	deviceAddr : the device address to read from
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, other if success
 */
uint8_t BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
	//if the head isn't the same as the tail or it is empty
	if ((rxFifoHead != rxFifoTail) || rxFifoReceived == false){
		I2Crx[rxFifoHead].devAddr = deviceAddr;
		I2Crx[rxFifoHead].regAddress = regAddr;
		I2Crx[rxFifoHead].data = rxData;
		I2Crx[rxFifoHead].length = rxLen;
		rxFifoHead = (rxFifoHead + 1)%FIFOSIZE;
		rxFifoReceived = true;
		return true;
	}
	return false; //return false if fifo is full
}
#endif
/********THE FOLLOWING CODE IS FOR THE RTOS VERSION OF THE BPS*********/
#ifdef RTOS
void I2C3_EV_IRQHandler(void){
	asm("CPSID I"); //disable all interrupts
	//all cpu registers are supposed to be saved here
	//but that happens when the ISR is called
	OSIntEnter(); //increments value of nested interrupt counter
	unsigned int *address = 0;
	asm volatile ("STR SP, [%0]\n\t": "=r" ( address)); //Store SP in address
	if (OSIntNestingCtr == 1) OSTCBCurPtr->StkPtr = address;
	if (!I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)) {
		I2C_AcknowledgeConfig(I2C3, ENABLE);
		// Since no one is using the I2C bus, take control
		I2C_GenerateSTART(I2C3, ENABLE);
		OSIntExit();
		asm("CPSIE I"); //enable interrupts
		return;
	}
	// Wait until start edge event occurred
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)){
		// Select device to talk to
		//devAddresstx will be 0 if there is no data in the fifo
		uint16_t* devAddress = OSQPend(&I2Ctx, 1, OS_OPT_PEND_BLOCKING, &one, &time, &err);
		devAddresstx = *devAddress;
		if (devAddresstx){
			uint16_t* regAddress = OSQPend(&I2Ctx, 1, OS_OPT_PEND_BLOCKING, &one, &time, &err);
			regAddresstx = *regAddress;
			uint16_t *lenPtr = OSQPend(&I2Ctx, 0, OS_OPT_PEND_BLOCKING, &one, &time, &err);
			txLength = *lenPtr;
			txData = OSQPend(&I2Ctx, 0, OS_OPT_PEND_BLOCKING, &one, &time, &err);
			I2C_Send7bitAddress(I2C3, devAddresstx, I2C_Direction_Transmitter);
			sending = true;
		}
		if (!sending){
			uint16_t* devAddress = OSQPend(&I2Crx, 1, OS_OPT_PEND_BLOCKING, &one, &time, &err);
			devAddressrx = *devAddress;
			if (devAddressrx){
				uint16_t* regAddress = OSQPend(&I2Crx, 1, OS_OPT_PEND_BLOCKING, &one, &time, &err); //get address to read from
				regAddressrx = *regAddress;
				readStore = OSQPend(&I2Crx, 0, OS_OPT_PEND_BLOCKING, &one, &time, &err); //get address to store in
				uint16_t *lenPtr = OSQPend(&I2Crx, 0, OS_OPT_PEND_BLOCKING, &one, &time, &err); //get length of data
				rxLength = *lenPtr;
				receiving = true;
			}
		}
		OSIntExit();
		asm("CPSIE I"); //enable interrupts
		return;
	}
	if (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && 
		I2C_GetLastEvent(I2C3) == I2C_EVENT_MASTER_MODE_SELECT){
		if(I2C3->SR1 & 0x0400 && !receiving) {
			I2C3->SR1 &= ~0x0400;
			I2C_GenerateSTOP(I2C3, ENABLE);
		}
	}
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT) && 
		(I2C_GetLastEvent(I2C3) != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		// Select device to talk to
		if(sending) I2C_Send7bitAddress(I2C3, devAddresstx, I2C_Direction_Transmitter);
		if(!sending && receiving) I2C_Send7bitAddress(I2C3, devAddressrx, I2C_Direction_Transmitter);
		OSIntExit();
		asm("CPSIE I"); //enable interrupts
		return;
	}
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		// Send start address (MSB first)
		if (sending) I2C_SendData(I2C3, (uint8_t)((regAddresstx & 0xFF00) >> 8));
		if(!sending && receiving) I2C_SendData(I2C3, (uint8_t)((regAddressrx & 0xFF00) >> 8));
		OSIntExit();
		asm("CPSIE I"); //enable interrupts
		return;
	}
	//send rest of start address (LSB)
	if (I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING) && 
		I2C_GetLastEvent(I2C3) == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED){
		if (sending) I2C_SendData(I2C3, (uint8_t)(regAddresstx & 0x00FF));
		if (receiving && !sending) I2C_SendData(I2C3, (uint8_t)(regAddressrx & 0x00FF));
		OSIntExit();
		asm("CPSIE I"); //enable interrupts
		return;
	}
	//if byte has been transmitted
	if(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == SET){
		if (sending){
			I2C_SendData(I2C3, *txData);
			txLength--; //decrement counter
			if (txLength == 0){
				I2C_GenerateSTOP(I2C3, ENABLE);
				sending = false;
			}
			txData++;
		}
		if (receiving && !sending){
			if (!startreceiving){
				I2C_GenerateSTART(I2C3, ENABLE);
				startreceiving = true;
			}
			if(startreceiving){
				while (rxLength > 0){
					if (rxLength > 1){
						*readStore = I2C_ReceiveData(I2C3);
						readStore++;
						rxLength--;
					}
					else{
						// Disable ack, since this is the last byte
						I2C_AcknowledgeConfig(I2C3, DISABLE);
						*readStore = I2C_ReceiveData(I2C3);
						readStore++;
						rxLength = 0;
					}
					// Generate the stop
					receiving = false;
					startreceiving = false;
					I2C_GenerateSTOP(I2C3, ENABLE);
				}
			}
		}
	}
	OSIntExit();
	asm("CPSIE I"); //enable interrupts
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param	deviceAddr : the device address to write to
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
	OSQPost(&I2Ctx, &deviceAddr, 1, OS_OPT_POST_FIFO, &err); //store device address
	OSQPost(&I2Ctx, &regAddr, 1, OS_OPT_POST_FIFO, &err); //store register address
	OSQPost(&I2Ctx, &txLen, 1, OS_OPT_POST_FIFO, &err); //store length of array
	OSQPost(&I2Ctx, txData, txLen, OS_OPT_POST_FIFO, &err); //store address of array
	return SUCCESS;
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param	deviceAddr : the device address to read from
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, other if success
 */
uint8_t BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
	OSQPost(&I2Crx, &deviceAddr, 1, OS_OPT_POST_FIFO, &err); //store device address
	OSQPost(&I2Crx, &regAddr, 1, OS_OPT_POST_FIFO, &err); //store address to read from
	OSQPost(&I2Crx, rxData, 1, OS_OPT_POST_FIFO, &err); //store address to store data in
	OSQPost(&I2Crx, &rxLen, 1, OS_OPT_POST_FIFO, &err);
	// Return Success if all executed properly
	return SUCCESS;
}
#endif