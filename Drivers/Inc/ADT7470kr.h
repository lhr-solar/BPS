/* Copyright (c) 2018-2024 UT Longhorn Racing Solar */
/** ADT740kr.h
 *  Functions for using the ADT740
 */

#ifndef ADT740_H
#define ADT740kr_H

#include "common.h"
#include "config.h"
#include "BSP_I2C.h"
/* --------------------------------
    Defines for Constant Variables
   --------------------------------*/

#define ADT7470_I2C_ADDR (0x5C << 1) // Has 3 cases (left justified) for ADDR pin: High = 0x5E, Low = 0x58, Floating = 0x5C (pg. 9)

// Default values to check we picked the right device (pg. 30)
#define ADT7470_DEVICE_ID 0x70   // Device ID for ADT7470 for device verification
#define ADT7470_COMPANY_ID 0x41  // Company ID for ADT7470 for device verification
#define ADT7470_REVISION_ID 0x02 // Revision ID for ADT7470 for device verification

// Tachometer Registers
#define ADT7470_REG_TACH1_LOW 0x2A
#define ADT7470_REG_TACH1_HIGH 0x2B
#define ADT7470_REG_TACH2_LOW 0x2C
#define ADT7470_REG_TACH2_HIGH 0x2D
#define ADT7470_REG_TACH3_LOW 0x2E
#define ADT7470_REG_TACH3_HIGH 0x2F
#define ADT7470_REG_TACH4_LOW 0x30
#define ADT7470_REG_TACH4_HIGH 0x31

// PWM Duty Cycle Registers
#define ADT7470_REG_PWM1_CURRENT_DUTY_CYCLE 0x32
#define ADT7470_REG_PWM2_CURRENT_DUTY_CYCLE 0x33
#define ADT7470_REG_PWM3_CURRENT_DUTY_CYCLE 0x34
#define ADT7470_REG_PWM4_CURRENT_DUTY_CYCLE 0x35

// ADI Test Registers
#define ADT7470_REG_ADI_TEST1 0x37
#define ADT7470_REG_ADI_TEST2 0x3C
#define ADT7470_REG_ADI_TEST3 0x77

// Min and Max values
#define ADT7470_REG_PWM1_MAX_DUTY_CYCLE 0x38
#define ADT7470_REG_PWM2_MAX_DUTY_CYCLE 0x39
#define ADT7470_REG_PWM3_MAX_DUTY_CYCLE 0x3A
#define ADT7470_REG_PWM4_MAX_DUTY_CYCLE 0x3B

#define ADT7470_REG_PWM1_MIN_DUTY_CYCLE 0x6A
#define ADT7470_REG_PWM2_MIN_DUTY_CYCLE 0x6B
#define ADT7470_REG_PWM3_MIN_DUTY_CYCLE 0x6C
#define ADT7470_REG_PWM4_MIN_DUTY_CYCLE 0x6D

// Device Identification Registers
#define ADT7470_REG_DEVICE_ID 0x3D
#define ADT7470_REG_COMPANY_ID 0x3E
#define ADT7470_REG_REVISION_ID 0x3F

// Configuration and Status Registers
#define ADT7470_REG_CONFIG1 0x40
#define ADT7470_REG_INTERRUPT_STATUS1 0x41
#define ADT7470_REG_INTERRUPT_STATUS2 0x42
#define ADT7470_REG_FAN_PULSES_PER_REVOLUTION 0x43

// Tachometer Min/Max Registers
#define ADT7470_REG_TACH1_MIN_LOW 0x58
#define ADT7470_REG_TACH1_MIN_HIGH 0x59
#define ADT7470_REG_TACH2_MIN_LOW 0x5A
#define ADT7470_REG_TACH2_MIN_HIGH 0x5B
#define ADT7470_REG_TACH3_MIN_LOW 0x5C
#define ADT7470_REG_TACH3_MIN_HIGH 0x5D
#define ADT7470_REG_TACH4_MIN_LOW 0x5E
#define ADT7470_REG_TACH4_MIN_HIGH 0x5F
#define ADT7470_REG_TACH1_MAX_LOW 0x60
#define ADT7470_REG_TACH1_MAX_HIGH 0x61
#define ADT7470_REG_TACH2_MAX_LOW 0x62
#define ADT7470_REG_TACH2_MAX_HIGH 0x63
#define ADT7470_REG_TACH3_MAX_LOW 0x64
#define ADT7470_REG_TACH3_MAX_HIGH 0x65
#define ADT7470_REG_TACH4_MAX_LOW 0x66
#define ADT7470_REG_TACH4_MAX_HIGH 0x67

// PWM Configuration Registers
#define ADT7470_REG_PWM1_2_CONFIG 0x68
#define ADT7470_REG_PWM3_4_CONFIG 0x69

// Interrupt Mask and Configuration Registers
#define ADT7470_REG_INTERRUPT_MASK1 0x72
#define ADT7470_REG_INTERRUPT_MASK2 0x73
#define ADT7470_REG_CONFIG2 0x74

// TMP05 Temperature and Zone Selection Registers
#define ADT7470_REG_MAX_TMP05_TEMPERATURE 0x78
#define ADT7470_REG_TMP05_ZONE_SELECT1 0x7C
#define ADT7470_REG_TMP05_ZONE_SELECT2 0x7D

// GPIO Registers
#define ADT7470_REG_GPIO_ENABLE 0x7F
#define ADT7470_REG_GPIO_CONFIG 0x80
#define ADT7470_REG_GPIO_STATUS 0x81

// Error types
#define READ_ERROR 1
#define WRITE_ERROR 2
#define INVALID_DEVICE -1

// RPM to percentage converter
#define RPM_CONVERT(speed) ((ADT7470_REG_FAN_PULSES_PER_REVOLUTION * 60) / speed)

uint32_t tach_rpm;

// Define a new struct called FanController
struct FanController
{
    uint8_t fan_id;
    uint8_t max_speed;
    uint8_t min_speed;
}Fan_Controller, *Fan_Controller_Ptr;

/*
 * ADT740_Init initializes I2C to communicate with ADT740
 */
int8_t ADT740_Init(void);

/*
 * Returns the rpm of the given fan
 */
int8_t getFanSpeed( struct FanController *fan);

/*
 * Set max rpm of the given fan
 */
int8_t setMaxSpeed(struct FanController fan, uint8_t max);

/*
 * Set min rpm of the given fan
 */
int8_t setMinSpeed(struct FanController fan, uint8_t min);

/**
 * @brief Set minimum duty cycle to a fan. Defaults to 50%
 *
 * @param fan Choose a number from 0-3.
 * @param duty Pick a number from 0-100%.
 * @return uint8_t 1 is fail, 0 is pass
 */
uint8_t ADT7470_SetMinDuty(uint8_t fan, uint8_t duty);

/**
 * @brief Disable a tachometer. Enabled on default.
 *
 * @param number Enter a number from 0-3. Check page 36 on datasheet for more info
 * @return uint8_t 1 is fail, 0 is pass
 */
uint8_t ADT7470_DisableTach(uint8_t fan);

/**
 * @brief Enable a tachometer. Enabled on default.
 *
 * @param number Enter a number from 0-3. Check page 36 on datasheet for more info
 * @return uint8_t 1 is fail, 0 is pass
 */
uint8_t ADT7470_EnableTach(uint8_t fan);

/**
 * @brief Set a polling frequency for the tachometer.
 *
 * @param number Enter a number from 0-7. Check page 36 on datasheet for more info
 * @return uint8_t 1 is fail, 0 is pass
 */
uint8_t ADT7470_SetPollingFreq(uint8_t number);

/**
 * @brief Disable getting an interrupt from a fan. Enabled on default.
 *
 * @param number Enter a number from 0-3. Check page 34 on datasheet for more info
 * @return uint8_t 1 is fail, 0 is pass
 */
uint8_t ADT7470_DisableInterrupt(uint8_t number);

/**
 * @brief Enable getting an interrupt from a fan. Enabled on default.
 *
 * @param number Enter a number from 0-3. Check page 34 on datasheet for more info
 * @return uint8_t 1 is fail, 0 is pass
 */
uint8_t ADT7470_EnableInterrupt(uint8_t number);

/*
 * Sets the PWM duty cycle of the given fan
 */
ErrorStatus setFanPWM(uint8_t fan, int32_t pwm);

/**
 * @brief Simplify the I2C Write register function
 *
 * @param regAddr
 * @param txData
 * @param txLen
 * @return uint8_t 0 is fail, 1 is pass
 */
uint8_t ADT7470_WriteRegister(uint16_t regAddr, uint8_t *txData, uint32_t txLen);

/**
 * @brief Simplify the I2C Read register functions
 *
 * @param regAddr
 * @param txData
 * @param txLen
 * @return uint8_t 0 is fail, 1 is pass
 */
uint8_t ADT7470_ReadRegister(uint16_t regAddr, uint8_t *rxData, uint32_t rxLen);

#endif