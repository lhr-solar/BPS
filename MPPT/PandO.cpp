/*******************************************************************************
 * PandO.cpp
 *
 * P&O Algorithm functions
 * Fall 2017 MPPT using the Nucleo-L432KC (Pholociraptor)
 * 
 * Author: Samantha Guu
 * Team: Samuel Chiu, Madeline Drake, Kyle Grier, Rafael Ibanez, Chase Lansdale,
 *       Brooks Loper
 * Created on: November 12, 2017
 * Revised on: November 14, 2017
 ******************************************************************************/
 
#include "mbed.h"
#include "FastPWM.h"
#include "math.h"

#include "constants.h"
#include "Sensors.h"
#include "PandO.h"

#define true                 1
#define false                0

// tracking
static int trackingOn = true;  //controls whether tracking updates PWM

// timing sections of code
Timer timer;


// calculations
float PvoltageIn = 0;
float PcurrentIn = 0;
float PvoltageOut = 0;
float PcurrentOut = 0;
int pulseWidth = 1500;
double pw;
bool direction = 0;      // if direction is 0 then perturb +, o/w -
int incrementor;         // how much we're perturbing by
float lastPower = 0;     // initialize lastPower to 0
float lastVoltage = 0;   // initialize lastVoltage to 0
float inPower;
float outPower;
double samples_power[NUM_SAMPLES];
double samples_voltage[NUM_SAMPLES];
int sample_idx = 0;

FastPWM mypwm(PA_1);                // PWM on PA_1

DigitalOut messageSignalLED(LED3);  // heartbeat
DigitalOut myBat(PB_4);
DigitalOut myTrack(PB_5);
// *********************** toggleHeartbeat ******************************************
// This function toggles an LED to show that the program is running
// Inputs: none
// Outputs: none
// Errors: none
// *****************************************************************************
void toggleHeartbeat(){
    messageSignalLED = !messageSignalLED; //just using this so I can see when it's sending
}
    
// *********************** PandO_Init ******************************************
// This function initializes anything needed the for P&O algo
// Inputs: none
// Outputs: none
// Errors: none
// *****************************************************************************
void PandO_Init(){
    if(useTime){
        timer.start();
    }
    // set the PWM period, in us (double), keeping the duty cycle the same
    mypwm.period_us(PWM_PERIOD_us); 
    // set the PWM pulsewidth, in us (double), keeping the period the same
    mypwm.pulsewidth_us(PWM_PULSEWIDTH_us);
    pulseWidth = 100*PWM_PULSEWIDTH_us;
    incrementor = STATIC_INCREMENTOR;
    messageSignalLED = 1;
}
    
// *********************** averageSamples **************************************
// This subroutine calculates the average value of an array
// Inputs: pointer to an array of doubles
// Outputs: average of the array (double)
// Errors: none
// *****************************************************************************
double averageSamples(double *samples){
    double avg = 0;
    for(int i=0; i<NUM_SAMPLES; i++){
        avg += samples[i];
    }
    avg = avg / NUM_SAMPLES;
    return avg;
}
    
// *********************** pulseWidthErrorHandling *****************************
// This function restricts the pulseWidth values
// Inputs: Sensor Readings inVoltage, outVoltage
//         (uses pulseWidth global variable)
// Outputs: none (changes pulseWidth global variable)
// Errors: none
// *****************************************************************************
void pulseWidthErrorHandling(float inVoltage, float outVoltage){
    // error handling: duty cycle should not go above 56/80 = 0.7
    if(pulseWidth >= 2800){
        pulseWidth = 2800;
    }
    //output voltage should be kept less than max voltage for battery
    //if(outVoltage >= 120){
    //    pulseWidth -= incrementor;
    //    trackingOn = false;
        //pc.printf("Tracking OFF \r\n Battery FULLY charged \r\n");
    //}//disable tracking if the input is disconnected
    //else if(inVoltage < 5){
    //    trackingOn = false;
        //pc.printf("Tracking OFF \r\n No Input detected \r\n");
    //}
    //else{
    //    trackingOn = true;
        //pc.printf("Tracking ON \r\n Input detected \r\n");
    //}   
    
    //pulsewidth should not be below 0
    if(pulseWidth < 0){
        pulseWidth = 0;
    }
}

// ****************************** calc_perturb_amt *****************************
// This function calculates how much to perturb by (for VARIABLE P&O)
// Inputs: Sensor Readings (inCurrent, outCurrent, inVoltage, outVoltage)
// Outputs: none (changes incrementor global variable)
// Errors: none 
// *****************************************************************************
void calc_perturb_amt(float deltaV, float deltaP){
    
    float logfxn = fabsf(log10(fabsf(deltaP) / fabsf(deltaV)));
    if((int) ((SCALING_FACTOR * logfxn) + 0.5) == 0 ||
        (int) ((SCALING_FACTOR * logfxn) + 0.5) > 100){        // code "snubber"...ish
        logfxn = 1;             // always have an increment that's not 0 or nan
    }
    
    incrementor = (int) ((SCALING_FACTOR * logfxn) + 0.5);
}

// ****************************** perturb **************************************
// This function handles the perturb part of the algorithm
//  - determines direction to perturb in
//  - perturbs by a constant value (incrementor)
// Inputs: Sensor Readings (inCurrent, outCurrent, inVoltage, outVoltage)
// Outputs: none (changes pulseWidth global variable)
// Errors: none (This function does not care about errors - call 
//               pulseWidthErrorHandling right after this function to do checks)
// *****************************************************************************
void perturb(float newVoltage, float newPower){
    if(trackingOn != true){
        return;
    }
    
    // 1) Calculate change in power/voltage

    float deltaPower = newPower - lastPower;
    float deltaVoltage = newVoltage - lastVoltage;

    // 2) Based on change in power/voltage, determine perturb direction
    if(deltaPower < 0){
        if(deltaVoltage < 0){
            direction = 0;
        }else{
            direction = 1;
        }
    }else{
        if(deltaVoltage < 0){
            direction = 1;
        }else{
            direction = 0;
        }
    }
    
    // 3) calculate new perturb amt, unless you want just a steady amt
    if(!staticPerturb){
        calc_perturb_amt(deltaVoltage, deltaPower); // variable perturb
    }   // else static perturb

    // 4) Save new power/voltage for next iteration
    lastPower = newPower;
    lastVoltage = newVoltage;
    
    // 5) Calculate the new pulsewidth based on perturb direction
    if(!staticPerturb){ // variable perturb
        if(direction){
            pulseWidth += incrementor;
        }
        else{
            pulseWidth -= incrementor;
        }
    }else{  // static perturb
        pulseWidth += incrementor;
    }
}

// ******************************* setPWM **************************************
// This function sets the PWM based on previously calculated pulseWidth
// Inputs: none (uses pulseWidth global variable)
// Outputs: none (changes pw global variable)
// Errors: none
// *****************************************************************************
void setPWM(){
    pw = (double)pulseWidth/100;
    // set PWM pulsewidth, specified in usec (double), keeping period same
    mypwm.pulsewidth_us(pw);
}

// ************************** perturb_and_observe ******************************
// This function directs the flow of the Perturb and Observe algorithm
// Inputs: none (reads in 4 sensor values in subroutines)
// Outputs: none (changes global variable pulseWidth)
// Errors: none (pulseWidth error checks in pulseWidthErrorHandling subroutine)
// *****************************************************************************
void perturb_and_observe(void){
    toggleHeartbeat();
    float begin, end, runtime;
    if ((sample_idx == 0) && (useTime)){
        begin = timer.read_us();
    }
    /* Actual reading values */
    currentIn = ReadInCurrent();
    currentOut = ReadOutCurrent();
    voltageIn = ReadInVoltage();
    voltageOut = ReadOutVoltage();
    inPower = voltageIn * currentIn; // Power = Voltage * Current
    outPower = voltageOut * currentOut;
    samples_power[sample_idx] = inPower;
    samples_voltage[sample_idx] = voltageIn;
    sample_idx++;
    if(sample_idx == NUM_SAMPLES){
        sample_idx = 0;
        double newVoltage = averageSamples(samples_voltage);
        double newPower = averageSamples(samples_power);
        perturb(newVoltage, newPower);
        pulseWidthErrorHandling(voltageIn, voltageOut);
        setPWM();
        if(useTime){
            end = timer.read_us();
            runtime = end - begin;
        }
    }
 }

// ******************************* printInfo ***********************************
// This function prints various info to a computer terminal
// Inputs: none
// Outputs: none (prints to the terminal)
// Errors: none
// *****************************************************************************