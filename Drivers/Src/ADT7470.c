/* Copyright (c) 2018-2024 UT Longhorn Racing Solar */
/** ADT740.h
 *  Functions for using the ADT740
 */

#include "BSP_I2C.h"
#include "ADT7470.h"

#define AddressLength 8

// High level funcitons
int8_t ADT740_Init(void) {
    BSP_I2C_Init();
    //set the ADT740 to monitoring mode
    uint8_t temp;
    if(!ADT7470_ReadRegister(ADT7470_REG_CONFIG1, &temp, AddressLength)) {
        return READ_ERROR;
    }
    uint8_t monitoringMessage = 0x01 | temp;
    if(!ADT7470_WriteRegister(ADT7470_REG_CONFIG1, &monitoringMessage, AddressLength)){
        return WRITE_ERROR;
    }

    uint8_t registerData;
    if(!ADT7470_ReadRegister(ADT7470_DEVICE_ID, &registerData, AddressLength)) {
        return READ_ERROR;
    }
    if (registerData != ADT7470_DEVICE_ID)
    {
        return INVALID_DEVICE;
    }
    
    if(!ADT7470_ReadRegister(ADT7470_COMPANY_ID, &registerData, AddressLength)) {
        return READ_ERROR;
    }
    if (registerData != ADT7470_COMPANY_ID)
    {
        return INVALID_DEVICE;
    }
    
    if(!ADT7470_ReadRegister(ADT7470_REVISION_ID, &registerData, AddressLength)) {
        return READ_ERROR;
    }
    if (registerData != ADT7470_REVISION_ID)
    {
        return INVALID_DEVICE;
    }
    


    tach_rpm = 0;
    return 0;
}

uint8_t ADT7470_SetMinDuty(uint8_t fan, uint8_t duty){
    if(fan > 3) {
        return 1; // Too big
    }
    if (ADT7470_ReadRegister(ADT7470_REG_PWM1_MAX_DUTY_CYCLE,))
    {
        /* code */
    }
    
    duty = RPM_CONVERT(duty);
    if(!ADT7470_WriteRegister((ADT7470_REG_PWM1_MIN_DUTY_CYCLE + fan), &duty, AddressLength)) {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_SetMaxDuty(uint8_t fan, uint8_t duty){
    if(fan > 3) {
        return 1; // Too big
    }
    duty = RPM_CONVERT(duty);
    if(!ADT7470_WriteRegister((ADT7470_REG_PWM1_MAX_DUTY_CYCLE + fan), &duty, AddressLength)) {
        return WRITE_ERROR;
    }
    return 0;
}

// IT is suppose to say if the fan is functioning in betweenn the limits of the fan. 
// uint8_t ADT7407Tach_Yeas() {
//     uint16_t one;
//     uint16_t two;
//     if (!((ADT7470_ReadRegister(ADT7470_REG_TACH1_MIN_HIGH, &one, AddressLength)) << 8) + (ADT7470_ReadRegister(ADT7470_REG_TACH1_MIN_LOW, &one, AddressLength))) {
//         return READ_ERROR;
//     }
//     if(!ADT7470_ReadRegister(ADT7470_REG_PWM1_MAX_DUTY_CYCLE, &two, AddressLength)){
//         return READ_ERROR;
//     }
//     if (one < two){
        
//     }
// }

/* Example of ChatGPT
double tachCount = 2604; // Example tachometer count
double pulsesPerRevolution = 2.0; // Default value, can be changed
double timePerRevolution = tachCount * 11.11e-6 * pulsesPerRevolution; // Time for one full revolution in seconds
double rpm = (1 / timePerRevolution) * 60; // Convert revolutions per second to revolutions per minute

// Assuming a hypothetical maximum RPM for the fan
double maxRpm = 2000;
double speedPercentage = (rpm / maxRpm) * 100;

*/

uint8_t ADT7470_DisableInterrupt(uint8_t number) {
    if(number > 3) {
        return 1; // Too big
    }
    uint8_t temp;
    if(!ADT7470_ReadRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength)) {
        return READ_ERROR;
    }
    temp |= 1 << (number + 4);
    if(!ADT7470_WriteRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength)) {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_EnableInterrupt(uint8_t number) {
    if(number > 3) {
        return 1; //Too big
    }
    uint8_t temp;
    if(!ADT7470_ReadRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength)) {
        return READ_ERROR;
    }
    temp |= 0 << (number + 4);
    if(!ADT7470_WriteRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength)) {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_DisableTach(uint8_t number) {
    if(number > 3) {
        return 1; // Too big
    }
    uint8_t temp;
    if(!ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength)) {
        return READ_ERROR;
    }
    temp |= 1 << number;
    if(!ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &temp, AddressLength)) {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_EnableTach(uint8_t number) {
    if(number > 3) {
        return 1; //Too big
    }
    uint8_t temp;
    if(!ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength)) {
        return READ_ERROR;
    }
    temp |= 0 << number;
    if(!ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &temp, AddressLength)) {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_SetPollingFreq(uint8_t number) {
    if(number > 7) {
        return 1;
    }
    uint8_t temp;
    if(!ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength)) {
        return 1;
    }
    temp |= number << 4;
    if(!ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &temp, AddressLength)) {
        return 1;
    }
    return 0;
}

void enterShutdown() {
    uint8_t temp;
    ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength);
    uint8_t shutDownMessage = 0x40 | temp; 
    ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &shutDownMessage, AddressLength);
}

// Low-Level Functions
uint8_t ADT7470_WriteRegister (uint16_t regAddr, uint8_t *txData, uint32_t txLen){
    return BSP_I2C_Write(ADT7470_I2C_ADDR, regAddr, txData, txLen);
}

uint8_t ADT7470_ReadRegister (uint16_t regAddr, uint8_t *rxData, uint32_t rxLen){
    return BSP_I2C_Read(ADT7470_I2C_ADDR, regAddr, rxData, rxLen);
}