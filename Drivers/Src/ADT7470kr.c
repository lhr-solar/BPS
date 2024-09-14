/* Copyright (c) 2018-2024 UT Longhorn Racing Solar */
/** ADT740kr.h
 *  Functions for using the ADT740
 */

#include "BSP_I2C.h"
#include "ADT7470kr.h"

#define AddressLength 8

int8_t ADT740_Init(void)
{
    BSP_I2C_Init();
    // set the ADT740 to monitoring mode
    uint8_t temp;
    if (!ADT7470_ReadRegister(ADT7470_REG_CONFIG1, &temp, AddressLength))
    {
        return READ_ERROR;
    }
    uint8_t monitoringMessage = 0x01 | temp;
    if (!ADT7470_WriteRegister(ADT7470_REG_CONFIG1, &monitoringMessage, AddressLength))
    {
        return WRITE_ERROR;
    }

    uint8_t registerData;
    if (!ADT7470_ReadRegister(ADT7470_DEVICE_ID, &registerData, AddressLength))
    {
        return READ_ERROR;
    }
    if (registerData != ADT7470_DEVICE_ID)
    {
        return INVALID_DEVICE;
    }

    if (!ADT7470_ReadRegister(ADT7470_COMPANY_ID, &registerData, AddressLength))
    {
        return READ_ERROR;
    }
    if (registerData != ADT7470_COMPANY_ID)
    {
        return INVALID_DEVICE;
    }

    if (!ADT7470_ReadRegister(ADT7470_REVISION_ID, &registerData, AddressLength))
    {
        return READ_ERROR;
    }
    if (registerData != ADT7470_REVISION_ID)
    {
        return INVALID_DEVICE;
    }

    tach_rpm = 0;
    return 0;
}

int16_t getFanSpeed(uint8_t fan)
{
    int16_t fan_speed_low;
    int16_t fan_speed_high;
    int32_t fan_speed;

    if (fan == 1)
    {
        if (ADT7470_ReadRegister(ADT7470_REG_TACH1_LOW, &fan_speed_low, AddressLength))
        {

            ADT7470_ReadRegister(ADT7470_REG_TACH1_HIGH, &fan_speed_high, AddressLength);
            fan_speed_high = fan_speed_high << 8;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
            }
        }
        else
        {
            return READ_ERROR;
        }
    }
    else if (fan == 2)
    {
        if (ADT7470_ReadRegister(ADT7470_REG_TACH2_LOW, &fan_speed_low, AddressLength))
        {

            ADT7470_ReadRegister(ADT7470_REG_TACH2_HIGH, &fan_speed_high, AddressLength);
            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
            }
        }
        else
        {
            return READ_ERROR;
        }
    }
    else if (fan == 3)
    {
        if (ADT7470_ReadRegister(ADT7470_REG_TACH3_LOW, &fan_speed_low, AddressLength))
        {

            ADT7470_ReadRegister(ADT7470_REG_TACH3_HIGH, &fan_speed_high, AddressLength);
            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
            }
        }
        else
        {
            return READ_ERROR;
        }
    }
    else if (fan == 4)
    {
        if (ADT7470_ReadRegister(ADT7470_REG_TACH4_LOW, &fan_speed_low, AddressLength))
        {

            ADT7470_ReadRegister(ADT7470_REG_TACH4_HIGH, &fan_speed_high, AddressLength);
            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
            }
        }
        else
        {
            return READ_ERROR;
        }
    }
    return RPM_CONVERT(fan_speed);
}

uint8_t ADT7470_WriteRegister(uint16_t regAddr, uint8_t *txData, uint32_t txLen)
{
    return BSP_I2C_Write(ADT7470_I2C_ADDR, regAddr, txData, txLen);
}

uint8_t ADT7470_ReadRegister(uint16_t regAddr, uint8_t *rxData, uint32_t rxLen)
{
    return BSP_I2C_Read(ADT7470_I2C_ADDR, regAddr, rxData, rxLen);
}