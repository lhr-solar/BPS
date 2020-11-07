/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __SIMULATOR_CONF_H
#define __SIMULATOR_CONF_H

// CSV File Location and Helper Functions
#define CSV_FILE_DIR            "BSP/Simulator/DataGeneration/Data/"
#define GET_CSV_PATH(f)         (CSV_FILE_DIR f)

// CSV File Names
#define ADC_CSV_FILE            "ADC.csv"
#define CAN_CSV_FILE            "CAN.csv"
#define CONTACTOR_CSV_FILE      "Contactor.csv"
#define I2C_CSV_FILE            "I2C.csv"
#define LIGHTS_CSV_FILE         "Lights.csv"
#define PLL_CSV_FILE            "PLL.csv"
#define SPI_R_CSV_FILE          "SPIR.csv"
#define SPI_W_CSV_FILE          "SPIW.csv"
#define TIMER_CSV_FILE          "Timer.csv"
#define WDTIMER_CSV_FILE        "WDTimer.csv"
#define FANS_CSV_FILE           "Fans.csv"

#endif
