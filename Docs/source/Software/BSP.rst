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

Lights BSP: Manthan Upadhyaya
=================================

Purpose
    This code is used to control the LED lights that are on the master board and are used for
    debugging. These lights mean the following:

    RUN - The BPS has power and is running.

    UVOLT - The BPS is reading an undervoltage condition.
    
    OVOLT - The BPS is reading an overvoltage condition.
    
    OTEMP - The BPS is reading an overtemperature condition.
    
    OCURR - The BPS is reading an overcurrent condition.
    
    WDOG - The BPS watchdog has tripped.
    
    CAN - The BPS is sending a CAN message.
    
    EXTRA - Extra usage
    
    WIRE - The BPS sees an open wire between the battery modules.
    
    STROBE - A fault condition has tripped the BPS.

Usage
    In order to use it, call the init function to initialize all the GPIO's. Then you can call any
    of the functions in BSP_Lights.h whenever needed.

Additional Considerations
    Although all of the lights are meant for debugging, the fault led light is connected directly
    to the strobe light. This means if the fault led turns on, so will the strobe light on the car.
    It doesn't mean that the fault led will turn on if the strobe light turns on.