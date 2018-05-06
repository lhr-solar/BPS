/*******************************************************************************
 * Sensors.cpp
 *
 * Sensor Reading function declarations
 * Fall 2017 MPPT using the Nucleo-L432KC (Pholociraptor)
 * 
 * Author: Samantha Guu
 * Team: Samuel Chiu, Madeline Drake, Kyle Grier, Rafael Ibanez, Chase Lansdale,
 *       Brooks Loper
 * Created on: November 12, 2017
 * Revised on: November 14, 2017
 ******************************************************************************/
// ****************************** ReadInCurrent ********************************
// This function reads in the input current
// Inputs: none
// Outputs: input current (float)
// Errors: none 
// *****************************************************************************
float ReadInCurrent(void);

// ****************************** ReadOutCurrent *******************************
// This function reads in the output current
// Inputs: none
// Outputs: output current (float)
// Errors: none 
// *****************************************************************************
float ReadOutCurrent(void);

// ****************************** ReadInVoltage ********************************
// This function reads in the input voltage
// Inputs: none
// Outputs: input voltage (float)
// Errors: none 
// *****************************************************************************
float ReadInVoltage(void);

// ****************************** ReadOutVoltage ********************************
// This function reads in the output voltage
// Inputs: none
// Outputs: output voltage (float)
// Errors: none 
// *****************************************************************************
float ReadOutVoltage(void);