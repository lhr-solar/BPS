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

int8_t getFanSpeed(struct FanController *fan)
{
    uint8_t fan_speed_low;
    uint8_t fan_speed_high;
    uint8_t fan_speed;

    switch (fan->fan_id)
    {

    case 1:

        if (ADT7470_ReadRegister(ADT7470_REG_TACH1_LOW, &fan_speed_low, AddressLength))
        {
            if (!ADT7470_ReadRegister(ADT7470_REG_TACH1_HIGH, &fan_speed_high, AddressLength))
            {
                return READ_ERROR;
                break;
            }

            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
                break;
            }
            break;
        }
        else
        {
            return READ_ERROR;
            break;
        }
        break;

    case 2:

        if (ADT7470_ReadRegister(ADT7470_REG_TACH2_LOW, &fan_speed_low, AddressLength))
        {
            if (!ADT7470_ReadRegister(ADT7470_REG_TACH2_HIGH, &fan_speed_high, AddressLength))
            {
                return READ_ERROR;
                break;
            }

            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
                break;
            }
            break;
        }
        else
        {
            return READ_ERROR;
            break;
        }
        break;

    case 3:

        if (ADT7470_ReadRegister(ADT7470_REG_TACH3_LOW, &fan_speed_low, AddressLength))
        {
            if (!ADT7470_ReadRegister(ADT7470_REG_TACH3_HIGH, &fan_speed_high, AddressLength))
            {
                return READ_ERROR;
                break;
            }

            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
                break;
            }
            break;
        }
        else
        {
            return READ_ERROR;
            break;
        }
        break;
    case 4:

        if (ADT7470_ReadRegister(ADT7470_REG_TACH4_LOW, &fan_speed_low, AddressLength))
        {
            if (!ADT7470_ReadRegister(ADT7470_REG_TACH4_HIGH, &fan_speed_high, AddressLength))
            {
                return READ_ERROR;
                break;
            }

            fan_speed_high = fan_speed_high << 8;
            fan_speed = fan_speed_high + fan_speed_low;
            if (fan_speed == 0xFFFF)
            {
                return READ_ERROR;
                break;
            }
            break;
        }
        else
        {
            return READ_ERROR;
            break;
        }
        break;

    default:
        return READ_ERROR;
        break;
    }
    return RPM_CONVERT(fan_speed);
}

int8_t setMinSpeed(struct FanController fan, uint8_t min)
{
    fan.min_speed = min;
    return 0;
}

int8_t setMaxSpeed(struct FanController fan, uint8_t max)
{
    fan.max_speed = max;
    return 0;
}

uint8_t ADT7470_SetMinDuty(uint8_t fan, uint8_t duty)
{
    uint8_t minVal = duty / 0.39;

    if (fan > 3)
    {
        return 1;
    }

    if (!ADT7470_WriteRegister(ADT7470_REG_PWM1_MIN_DUTY_CYCLE + fan, &minVal, AddressLength))
    {

        return 1;
    }

    return 0;
}

uint8_t ADT7470_DisableTach(uint8_t fan)
{
    if (fan > 3)
    {
        return 1;
    }
    uint8_t temp;
    if (!ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength))
    {
        return READ_ERROR;
    }
    temp |= 1 << fan;
    if (!ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &temp, AddressLength))
    {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_EnableTach(uint8_t fan)
{
    if (fan > 3)
    {
        return 1;
    }
    uint8_t temp;
    if (!ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength))
    {
        return READ_ERROR;
    }
    temp |= 0 << fan;
    if (!ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &temp, AddressLength))
    {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_SetPollingFreq(uint8_t number)
{
    if (number > 7)
    {
        return 1;
    }
    uint8_t temp;
    if (!ADT7470_ReadRegister(ADT7470_REG_CONFIG2, &temp, AddressLength))
    {
        return 1;
    }
    temp |= number << 4;
    if (!ADT7470_WriteRegister(ADT7470_REG_CONFIG2, &temp, AddressLength))
    {
        return 1;
    }
    return 0;
}

uint8_t ADT7470_DisableInterrupt(uint8_t number)
{
    if (number > 3)
    {
        return 1;
    }
    uint8_t temp;
    if (!ADT7470_ReadRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength))
    {
        return READ_ERROR;
    }
    temp |= 1 << (number + 4);
    if (!ADT7470_WriteRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength))
    {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t ADT7470_EnableInterrupt(uint8_t number)
{
    if (number > 3)
    {
        return 1;
    }
    uint8_t temp;
    if (!ADT7470_ReadRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength))
    {
        return READ_ERROR;
    }
    temp |= 0 << (number + 4);
    if (!ADT7470_WriteRegister(ADT7470_REG_INTERRUPT_MASK2, &temp, AddressLength))
    {
        return WRITE_ERROR;
    }
    return 0;
}

uint8_t setFanPWM(uint8_t fan, int32_t pwm)
{
    return 0;
}

uint8_t ADT7470_WriteRegister(uint16_t regAddr, uint8_t *txData, uint32_t txLen)
{
    return BSP_I2C_Write(ADT7470_I2C_ADDR, regAddr, txData, txLen);
}

uint8_t ADT7470_ReadRegister(uint16_t regAddr, uint8_t *rxData, uint32_t rxLen)
{
    return BSP_I2C_Read(ADT7470_I2C_ADDR, regAddr, rxData, rxLen);
}