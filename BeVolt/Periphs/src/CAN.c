/** CAN.c
 * CAN driver for the STM32F413
 */
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;

static void Init_RxMes(CanRxMsg *RxMessage);
static bool RxFlag = false;

void CAN1_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
  
  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CAN1);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 125 KBps (CAN clocked at 30 MHz) 
	 * 1/(prescalar + (prescalar*(BS1+1)) + (prescalar*(BS2+1))) * 30M = CAN Baudrate
	*/
  CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq; 
  CAN_InitStructure.CAN_Prescaler = 60; 
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(CAN1, &CAN_FilterInitStructure);
  
  /* Transmit Structure preparation */
  TxMessage.ExtId = 0x0;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	// Enable Rx interrupts
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
	
	Init_RxMes(&RxMessage);
}

void CAN1_Write(uint32_t id, uint8_t data[8]){
	TxMessage.StdId = id;
	for(int i = 0; i < 8; i++){
		TxMessage.Data[i] = data[i];
	}
	CAN_Transmit(CAN1, &TxMessage);
}

bool CAN1_Read(uint8_t *data){
	if(RxFlag){
		for(int i = 0; i < 8; i++){
			data[i] = RxMessage.Data[i];
		}
		RxFlag = false;
		return 1;
	}
	return 0;
}

static void Init_RxMes(CanRxMsg *RxMessage){
  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
	for(int i = 0; i < 8; i++){
		TxMessage.Data[i] = 0;
	}
}

void CAN1_RX0_IRQHandler(void)
{
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  
  if ((RxMessage.StdId == 0x001)&&(RxMessage.IDE == CAN_ID_STD) && (RxMessage.DLC == 1)){
    // Do stuff
		RxFlag = true;
  }
}
