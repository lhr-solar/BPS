***********************
Battery Support Package
***********************

I2C Driver: Manthan Upadhyaya
=================================

The I2C driver is interrupt driven and uses I2C3 for communicating with the EEPROM. 
There are multiple definitions of the same functions within this library. They are split into 2 
categories, one for the RTOS and one for the Bare-Metal version of the BPS. There are #defines that 
ecompass these functions that are set when compiled. The default #define is "RTOS" but it can be 
changed to compile the Bare-Metal code if necessary.

UART Driver: Manthan Upadhyaya
==================================

The UART driver is interrupt driven and initializes 2 serial transmission lines(UART 2 and 3). One 
is for the Bluetooth module and the other is for the USB. This driver is also split into two
sections just like the I2C driver. One is for RTOS and the other is for Bare-Metal. 

Fans Driver: Manthan Upadhyaya
==================================

The Fans driver is used to control the speed of the fans depending on the temperature of the Battery
Pack. It allows for as many speeds as needed and can change the speed of individual fans. It supports
four fans and uses PWM to change the speed and hopefully save energy. 