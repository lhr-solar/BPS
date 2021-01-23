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

CAN BSP: Chase Block & Sijin Woo
================================

Purpose
    The purpose of this code is to send and recieve CAN messages. 

Usage
    Function usage is declared in the ``BSP_CAN.h file``. When using these functions in the RTOS, pend
    and post must occur before and after the functions are called. The resources used for pend and 
    post can be seen in the CANBus.c driver documentation(insert link here).

Additional Considerations
    It also contains a queue for messages that are recieved on the CAN bus that has a depth of 10. 
    This queue cannot be accessed outside the file. If the queue exceeds it's limit, we will lose
    messages.

Lights BSP: Manthan Upadhyaya
=================================

Purpose
    This code is used to control the LED lights that are on the master board and are used for
    debugging. These lights mean the following:

    ``RUN`` - The BPS has power and is running.

    ``UVOLT`` - The BPS is reading an undervoltage condition.
    
    ``OVOLT`` - The BPS is reading an overvoltage condition.
    
    ``OTEMP`` - The BPS is reading an overtemperature condition.
    
    ``OCURR`` - The BPS is reading an overcurrent condition.
    
    ``WDOG`` - The BPS watchdog has tripped.
    
    ``CAN`` - The BPS is sending a CAN message.
    
    ``EXTRA`` - Extra usage
    
    ``WIRE`` - The BPS sees an open wire between the battery modules.
    
    ``STROBE`` - A fault condition has tripped the BPS.

Usage
    In order to use it, call `BSP_Lights_Init()` to initialize all the GPIO's. Then you can call any
    of the functions in ``BSP_Lights.h`` whenever needed.

Additional Considerations
    Although all of the lights are meant for debugging, the fault LED light is connected directly
    to the strobe light. This means if the fault LED turns on, so will the strobe light on the car.
    It doesn't mean that the fault LED will turn on if the strobe light turns on.

The Fans driver is used to control the speed of the fans depending on the temperature of the Battery
Pack. It allows for as many speeds as needed and can change the speed of individual fans. It supports
four fans and uses PWM to change the speed and hopefully save energy.

PLL BSP: Sijin Woo
==================================

Purpose
    The PLL is used to increase the clock speed of the STM32F413's clock. Right now, it is set to change it
    from 16 MHz to 80 MHz.

Usage
    Calling the init function automatically sets the clock speed to 80 Mhz. in order to verify
    that it worked, you can call BSP_PLL_GetSystemClock().

Additional Considerations
    Increasing the clock speed does take more power. Although this is minor considering how much the
    entire BPS takes, it is worth noting.

SPI BSP: Clark Poon, Sijin Woo, and Sugam Arora
===============================================

Purpose
    SPI is used for communication between the uC and the minion LTC6811s in the system.

Usage
    Our car uses the ``SPI1`` and ``SPI3`` busses. The ``SPI1`` bus is for communication with the LTC6811s
    and the ``SPI3`` bus is for communication with the AS8510 current sensor. When calling the init function, 
    you must specify which port you intend to use (via the ``spi_port_t`` struct). The user can read/write on whichever bus they 
    have chosen. 
    This BSP includes IRQ Handlers for both buses that post the ``MinionsASIC_Mutex``.

    ``SPI_Wait()`` has two different versions - one for the bare-metal BPS code and one for the RTOS code. 
    The BPS code is automatically compiled with ``#define RTOS``. If the user wants to compile the bare-metal
    version of the code, ``#define BAREMETAL`` should be in the code to be compiled.

Additional Considerations
    The ``SPI1`` and ``SPI3`` IRQs are only included in the RTOS version of the code.
