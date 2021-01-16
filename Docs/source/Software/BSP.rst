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

CAN BSP: Chase Block & Sijin Woo
================================

Purpose
    The purpose of this code is to send and recieve CAN messages. It also contains a queue for 
    messages that are recieved on the CAN bus that has a depth of 10. This queue cannot be accessed
    outside the file.

Usage
    Function usage is declared in the BSP_CAN.h file. When using these functions in the RTOS, pend
    and post must occur before and after the functions are called.

Additional Considerations
    None