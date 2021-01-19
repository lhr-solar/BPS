***********************
Board Support Package
***********************

I2C BSP: Manthan Upadhyaya
=================================

Purpose
    The I2C driver is interrupt driven and is used to communicate with the EEPROM to log data. It 
    requires pins PA8(SCL) and PC9(SDA) for the I2C3.

Usage
    There are multiple definitions of the same functions within this library. They are split into 2 
    categories, one for the RTOS and one for the Bare-Metal version of the BPS. There are ``#define`` that 
    encompass these functions that are set when compiled. The default ``#define`` is "RTOS" but it can be 
    changed to compile the Bare-Metal code if necessary. The ``BSP_I2C.h`` file has more information on how
    to use each function. The same function declarations are used regardless of whether it is RTOS or
    Bare-Metal.

Additional Considerations
    None

UART BSP: Manthan Upadhyaya
==================================

Purpose
    The UART driver is interrupt driven and initializes 2 serial transmission lines(UART 2 and 3). 
    USART 2 is for the Bluetooth module and uses pins PA2 and PA3. USART3 is for the USB and uses pins
    PC5 and PB10.

Usage
    This driver is also split into two sections just like the I2C driver. One is for RTOS and the 
    other is for Bare-Metal. The ``BSP_UART.h file`` has more information on how to use each function.

Additional Considerations
    Since the BPS cannot be changed during the race according to ASC regulations(5.2.E.9), 
    the BLE module must be disabled before the race is started if it is able to change the code. 
    This can be done through software by not initializing the USART2 module or through hardware 
    with a switch to turn off power to the BLE module, or just removing it all together. 

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

Fans BSP: Manthan Upadhyaya
==================================

Purpose
    The Fans driver is used to control the speed of the fans depending on the temperature of the Battery
    Pack. It uses pins PC6, PC7, PB14, PB15 with the alternative function of Pulse-Width Modulation 
    enabled for as many speeds as needed for a total of 4 fans. 

Usage
    It sets the speeds of individual fans and can also return the value of those speeds. The 
    ``BSP_Fans.h`` file has more information on how to use each function.
