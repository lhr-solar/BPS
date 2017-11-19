#ifndef PID_H
#define PID_H


#include "mbed.h"

class PID {
    
    
    
    public:
        PID(float newConstants[], int newLimits[]);
        float generatePIDOutput(float currentState);
        void setDesiredState(float newDesiredState);
        
    private:
        float *generatePIDterms(float error);
        float *governPIDterms(float PIDterms[]);
        float generateProportional(float error);
        float generateIntegral(float error);
        float generateDerivative(float error);
        float calculateError(float currentState);
        
        float *constants;
        int *limits;
        float *PIDterms;
        
        float desiredState;
        bool reset;
        float integral;
        float lastError;
        
        Timer timer;
        float timeOfLastMeasurment;
        
    
};

#endif