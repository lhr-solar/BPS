#include "PID.h"

float PID::generateError(float currentState) {
    
    float newE = desiredState - currentState;
    return newE;    

    }
    
float PID::generatePIDOutput(float error) {
    
    float i= generateI(error);
    float p= generateP(error);
    float d= generateD(error);
    timeOfLastMeasurment = t.read_ms();
    return i+p+d;
    
    }
    
void PID::setDesiredState(float newDesState) {
    
    desiredState = newDesState;
    integral = 0;
    reset = true; 
    
    }
    
float PID::generateP(float error) {
    
    return error * constants[0];
    
    }
    
float PID::generateI(float error) {
    
    int now= t.read_ms();
    float deltaT= now - timeOfLastMeasurement;
    integral += (deltaT * error) * constants[1];
    return integral;
    }

float PID::generateD(float error) {
    if(reset) {lastError = error;}
    int now= t.read_ms();
    float deltaT= now - timeOfLastMeasurement;
    return ((error - lastError)/deltaT) * constants[2];
    
    }
