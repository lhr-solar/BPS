#include "common.h"
#include "config.h"
#include "BSP_CAN.h"

 int main(void){
   //BSP_CAN_Init();
   uint8_t message[8] = {0x01, 0x82,0x15,0x36,0x75,0,0,0};
   uint8_t readData[8] = {0,0,0,0,0,0,0,0};
   uint32_t id[1] = {0x101};

   uint8_t length = 5;
   
   BSP_CAN_Write(0x10A, message, length);
}