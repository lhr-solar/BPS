/*******************************************************************************
 * Maximum Power Point Tracker Project for EE 464H
 *
 * Perturb and Observe Algorithm (Revised for discrete pulsewidth incrementor)
 * 
 * Author: Juan Cortez, Angus Ranson
 * Team: Angus Ranson, Muhammad Bukhari, Rana Madkour, Josh Frazor, Zach Pavlich
 * Created on: September 14, 2015 at 14:26
 * Revised on: December 5, 2015 at 18:08
 *
 * Edited by: Kyle Grier, Samantha Guu
 * New Team: Samuel Chiu, Madeline Drake, Kyle Grier, Samantha Guu, 
 *           Rafael Ibanez, Chase Lansdale, Cobie Loper
 * New Team Mentor: Dr. Gary Hallock
 * Last Revised: November 14, 2017
 *
 * GitHub Repository: https://github.com/juancortez-ut/mppt
 *
 * Dependent Hardware: 
 *   - Microcontroller: Freescale FRDM-K64F
 *   - CAN_BUS: CAN-BUS Shield v.1.2 by SEEED Studios
 *
 * Dependent Libraries:
 *   - mbed Library: https://developer.mbed.org/users/mbed_official/code/mbed/
 *   - SEEEED_CAN_LIBRARY: /mppt/FRDM-K64F/CAN_BUS/SEEED_CAN/SEEED_CAN_LIBRARY
 *
 * FRDM-K64F Pinout: https://os.mbed.com/platforms/FRDM-K64F/#board-pinout
 *
 * Signal     | FRDM-K64F Pin   | Purpose
 * -----------------------------------------------------------------------
 * PwmOut     | PTC10           | Output PWM Signal to Boost Converter
 * DigitalOut | LED1            | Heartbeat LED
 * AnalogIn   | PTB3            | Input Hall Sensor
 * AnalogIn   | PTB11           | Output Hall Sensor
 * AnalogIn   | PTB2            | Input Voltage
 * AnalogIn   | PTB10           | Output Voltage
 * RawSerial  | USBTX, USBRX    | Serial Communication
 * 
 * Program Description: This program is written for the Freescale FRDM-K64F uC.
 * The description of the P&O algorithm is found below, in the comment section
 *   titled, 'Perturb & Observe Algorithm'.
 *
 *****************************************************************************************************/

/**
*                                   Perturb & Observe Algorithm
*
* The most widely used algorithm is the P&O algorithm. The P&O algorithm perturbs the duty cycle
* which controls the power converter, in this way it takes steps over the p-v characteristic to 
* find the MPPT. This perturbation causes a new operating point with a different output power. In 
* case this output power is larger than the previous output power, this point is set as the new 
* operating point. In case it is lower, the same power point is adjusted to a lower or higher working 
* voltage, depending on the previous step direction. (http://bit.ly/1L73nzE)
*
*
* Pulse Width (PW) is the elapsed time between the rising and falling edges of a single pulse.
* 
* Pulse Repetition Interval (PRI) is the time between sequential pulses.
*
* Pulse Repetition Frequency (PRF) is the reciprocal of PRI. The basic unit of measure for PRF
* is hertz (Hz). 
*
* Duty Cycle describes the "On Time" for a pulsed signal. We can report duty cycle in units of time, 
* but usually as a percentage. To calculate a signals' duty cycle, we need to know the signal's pulse
* width and repetition frequency.
*
**/

#include "FastPWM.h"
#include "stdlib.h"
#include "mbed.h"

#include "PandO.h"
#include "constants.h"
#include "CAN_lib.h"
#include "Serial_lib.h"

//Define Constants
#define true                 1
#define false                0

#define MESSAGE_LENGTH       8
#define READING_COUNT        6

void setup();
//void getMessage();
//void interruptFunction();

// A Ticker is used to call a function at a recurring interval
// void Attach(T *tptr, void(T::*mptr)(void), float t)
// Attach a member function to be called by the Ticker, specifying time in secs
Ticker ticker;
Ticker ticker2;

int main(void)
{
    pc.attach(&interruptFunction, Serial::RxIrq);
    setup();
    while(1)
    {
        if(changedFlag == true)
        {
            setup();
            changedFlag = false;
        }   
    }
}



void setup()
{
    if(algorithm == 1)
    {
        algorithm = lastAlgorithm;
    }
    
    if(algorithm == 0)
    {
        ticker.detach();
    }
    
    if(algorithm == 2)
    {
        PandO_Init();
        ticker.attach(&perturb_and_observe, SAMPLE_PERIOD);
    }
    
    if(algorithm == 3)
    {
        //incrementalInit();
        //ticker.attach(&incremental_conductance, SAMPLE_PERIOD) //Incremental conductance
    }
    
    if(algorithm == 4)
    {
        //fuzzyInit();
        //ticker.attach(&fuzzy_logic_control, SAMPLE_PERIOD)//Fuzzy Logic Control
    }
    
    //add else ifs to add in more algorithms
}
