***********************
Board Support Package
***********************

Contactor BSP
=============

Purpose
    The contactor BSP interfaces with the GPIO pins connected to the contactor. These pins are used to control the contactor (PB0) and to check 
    the state of the contactor (PB1).

Usage
    The peripheral should be initialized by calling ``BSP_Contactor_Init()`` before any of the other functions are called. See ``BPS/BSP/BSP_Contactor.h`` 
    for details on individual functions.

Additional Considerations
    The contactor is one of the most important safety features in the vehicle, so the contactor should only be turned on (closed) if the BPS is sure that the
    battery pack is in a safe state.

I2C BSP: Manthan Upadhyaya
=================================

The I2C driver is interrupt driven and uses I2C3 for communicating with the EEPROM. 
There are multiple definitions of the same functions within this library. They are split into 2 
categories, one for the RTOS and one for the Bare-Metal version of the BPS. There are #defines that 
encompass these functions that are set when compiled. The default #define is "RTOS" but it can be 
changed to compile the Bare-Metal code if necessary.

UART BSP: Manthan Upadhyaya
==================================

The UART driver is interrupt driven and initializes 2 serial transmission lines(UART 2 and 3). One 
is for the Bluetooth module and the other is for the USB. This driver is also split into two
sections just like the I2C driver. One is for RTOS and the other is for Bare-Metal. 

Fans BSP: Manthan Upadhyaya
==================================

The Fans driver is used to control the speed of the fans depending on the temperature of the Battery
Pack. It allows for as many speeds as needed and can change the speed of individual fans. It supports
four fans and uses PWM to change the speed and hopefully save energy.

PLL BSP: Clark Poon
==================================

Purpose
    The PLL is used to increase the clock cycle of the STM clock. Right now, it is set to change it
    from 16 Mhz to 80 Mhz.

Usage
    Calling the init function automatically sets the clock speed to 80 Mhz. in order to verify
    that it worked, you can call BSP_PLL_GetSystemClock().

Additional Considerations
    Increasing the clock speed does take more power. Although this is minor considering how much the
    entire BPS takes, it is worth noting.
