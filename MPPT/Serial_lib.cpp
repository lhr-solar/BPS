#include "mbed.h"
#include "Serial_lib.h"

void getMessage()
{
    int index = 0;
    nextChar = 'a';
    while(nextChar != '\n')    
    {
                nextChar = pc.getc();
                if (index < BUFFER_SIZE-1 && nextChar != '\n') 
                {
                    message[index++] = nextChar;
                }
    }
    message[index] = '\0';
}

void interruptFunction()
{
    getMessage();
    
    if(strcmp(message, "PandO") == 0) 
    {
        if(algorithm != 2)
        {
            algorithm = 2;
            changedFlag = true;
        } 
        pc.printf("PandO Set\n");
    }
    else if(strcmp(message, "pause") == 0)
    {
        if(algorithm != 0)
        {
            lastAlgorithm = algorithm;
            algorithm = 0;
            changedFlag = true;
        }
        pc.printf("Paused\n");
    }
    else if(strcmp(message, "resume") == 0)
    {
        if(algorithm == 0)
        {
            algorithm = lastAlgorithm;
            changedFlag = true;
        }
        pc.printf("Resumed\n");
    }
    else if(strcmp(message, "Incremental") == 0)
    {
        if(algorithm != 3)
        {
            algorithm = 3;
            changedFlag = true;            
        }
        pc.printf("Incremental Logic\n");
    }
    else if(strcmp(message, "Fuzzy") == 0)
    {
        if(algorithm != 4)
        {
            algorithm = 4;
            changedFlag = true;
        }
        pc.printf("Fuzzy logic set\n");
    }
    else if(strcmp(message, "Vin") == 0)
    {
        pc.printf("VoltageIn:\t%f\n", voltageIn);
    }
    else if(strcmp(message, "Iin") == 0)
    {
        pc.printf("CurrentIn:\t%f\n", currentIn);
    }
    else if(strcmp(message, "Pin") == 0)
    {
        pc.printf("PowerIn:\t%f\n", voltageIn * currentIn);
    }
    else if(strcmp(message, "Vout") == 0)
    {
        pc.printf("VoltageOut:\t%f\n", voltageOut);
    }
    else if(strcmp(message, "Iout") == 0)
    {
        pc.printf("CurrentOut:\t%f\n", currentOut);
    }
    else if(strcmp(message, "Pout") == 0)
    {
        pc.printf("PowerOut:\t%f\n", voltageOut * currentOut);
    }
    else
    {
        pc.printf("Unknown Command\n");
    }     
}