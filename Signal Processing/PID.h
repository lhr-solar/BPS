#include "mbed.h"

class PID {
    
    
    
    public:
        PID(int[] constants);
        float generatePIDOutput(float error);
        void setDesiredState(float newDesState);
        
    private:
        float generateP(float error);
        float generateI(float error);
        float generateD(float error);
        float generateError(float currentState);
        float desiredState;
        Timer t;
        bool reset = false;
        float integral;
        float lastError;
        float timeOfLastMeasurement;
        
    
}

#endif
