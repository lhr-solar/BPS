/*******************************************************************************
 * PandO.h
 *
 * P&O Algorithm function declarations
 * Fall 2017 MPPT using the Nucleo-L432KC (Pholociraptor)
 * 
 * Author: Samantha Guu
 * Team: Samuel Chiu, Madeline Drake, Kyle Grier, Rafael Ibanez, Chase Lansdale,
 *       Brooks Loper
 * Created on: November 12, 2017
 * Revised on: November 14, 2017
 ******************************************************************************/
#ifndef PANDO_H
#define PANDO_H

#include "constants.h"

// *********************** PandO_Init ******************************************
// This function initializes anything needed the for P&O algo
// Inputs: timingOn (bool)
// Outputs: none
// Errors: none
// *****************************************************************************
void PandO_Init();

// ************************** perturb_and_observe ******************************
// This function directs the flow of the Perturb and Observe algorithm
// Inputs: none (reads in 4 sensor values in subroutines)
// Outputs: none (changes global variable pulseWidth)
// Errors: none (pulseWidth error checks in pulseWidthErrorHandling subroutine)
// *****************************************************************************
void perturb_and_observe(void);

// ******************************* printInfo ***********************************
// This function prints various info to a computer terminal
// Inputs: none
// Outputs: none (prints to the terminal)
// Errors: none
// *****************************************************************************

#endif