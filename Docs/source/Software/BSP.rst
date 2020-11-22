***********************
Battery Support Package
***********************

I2C BSP Driver
==============

The I2C driver is interrupt driven and has a special format. There are multiple definitions of the same functions, 
one for the RTOS and one for the Bare-Metal version of the BPS. There are #defines that ecompass
these functions that are set when compiled. The default #define is "RTOS" but it can be changed to
compile the Bare-Metal code if necessary.