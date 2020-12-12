/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_CAN.h"

int main(void){
   uint8_t message[8] = {0x01, 0x82,0x00,0x36,0x75,0,0,0x96};
   uint8_t readData[8] = {0,0,0,0,0,0,0,0};
   uint32_t id[1];
  
   uint8_t length = 1;
   
   while(1) {
      BSP_CAN_Write(0x10A, message, length);
      BSP_CAN_Read(id, readData);
   }

   exit(0);
}
