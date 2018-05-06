/*******************************************************************************
 * constants.h
 *
 * Static constants for the entire program
 * Fall 2017 MPPT using the Nucleo-L432KC (Pholociraptor)
 * 
 * Author: Samantha Guu
 * Team: Samuel Chiu, Madeline Drake, Kyle Grier, Rafael Ibanez, Chase Lansdale,
 *       Brooks Loper
 * Created on: November 12, 2017
 * Revised on: November 14, 2017
 ******************************************************************************/
 
#ifndef __constants_h__
#define __constants_h__


// general program settings
static const bool useTime = false;          // turn on if want to time 1 sample
static const bool staticPerturb = true;     // true = constant perturb amt
                                            // false = calculated perturb amt

// main.cpp
static const double PRINT_PERIOD = 2;       // how fast we are printing info (secs)
static const double SAMPLE_PERIOD = 0.05;   // how fast we are doing P&O (secs)
//     for increment-on-switch-press subprogram
static int STATIC_INCREMENTOR = 50;         // added/subtracted from pulseWidth

// Sensors.cpp
static const double V_IN_MULT = 54;
static const double V_OUT_MULT = 56;
static const double I_IN_MULT = 5;
static const double I_OUT_MULT = 5;
static const double HALL_IN_NO_CURRENT = 2.5;
static const double HALL_OUT_NO_CURRENT = 2.5;
static const double AIN_MULT = 3.3;

// PandO.cpp
static const double PWM_PERIOD_us = 40;     // 25 kHz switching
static const double PWM_PULSEWIDTH_us = 10; // initial duty cycle setting
                                            // currently:
                                            //  PWM_PULSEWIDTH_us/PWM_PERIOD_us
                                            //  10/40 -> 25%
static const int NUM_SAMPLES = 10;
static const float SCALING_FACTOR = 50;     // scales how much we perturb by
                                            // (this is for variable P&O)

static float voltageIn = 0;
static float currentIn = 0;
static float voltageOut = 0;
static float currentOut = 0;

static float algorithm = 2;
static float lastAlgorithm = 2;

static bool changedFlag = false;

#endif