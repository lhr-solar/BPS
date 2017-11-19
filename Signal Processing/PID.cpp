#include "PID.h"

PID::PID(float newConstants[],  int newLimits[])
{
    constants = newConstants;
    limits = newLimits;
    reset = true; //triggers an initialization of integral and lastError for us
    timer.start();
}

float PID::calculateError(float currentState) 
{
    return desiredState - currentState;
}
    
float PID::generatePIDOutput(float currentState) 
{
    float error = calculateError(currentState);
    float *PIDterms = generatePIDterms(error);
    float *limitedPIDterms = governPIDterms(PIDterms);
    timeOfLastMeasurment = timer.read_ms();
    lastError = error;
    return limitedPIDterms[0] + limitedPIDterms[1] + limitedPIDterms[2];
}

float *PID::generatePIDterms(float error)
{
    PIDterms[0] = generateProportional(error);
    PIDterms[1] = generateIntegral(error);
    PIDterms[2] = generateDerivative(error);
    return PIDterms;
}

float *PID::governPIDterms(float PIDterms[])
{
    for(int i = 0; i < 3; i++)
    {
        if(PIDterms[i] < limits[i])
        {
            continue;
        }
        else
        {
            PIDterms[i] = (float)limits[i];
        }
    }
    return PIDterms;
}

void PID::setDesiredState(float newDesiredState) 
{
    desiredState = newDesiredState;
    integral = 0;
    reset = true;
}
    
float PID::generateProportional(float error) 
{
    return error * constants[0];    
}
    
float PID::generateIntegral(float error) 
{
    
    int currentTime = timer.read_ms();
    float deltaT= currentTime - timeOfLastMeasurment;
    integral += (deltaT * error) * constants[1];
    return integral;
}

float PID::generateDerivative(float error) 
{
    if(reset) 
    {
        lastError = error;
    }
    
    int currentTime = timer.read_ms();
    float changeInTime = currentTime - timeOfLastMeasurment;
    float derivative = (error - lastError) / changeInTime;   
    return derivative * constants[2];
}