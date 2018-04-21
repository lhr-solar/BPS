#ifndef __CURRENT_H__
#define __CURRENT_H__

class Current{
    public:
    Current(PinName highADC, PinName lowADC);
    int getHighPrec();
    int getLowPrec();
    void setLimits(int limit);
    bool isSafe();
    
    private:
        
    int limit;
    AnalogIn highPrec;
    AnalogIn lowPrec;
};

#endif
