***********************
Board Support Package
***********************

CAN BSP: Chase Block & Sijin Woo
================================

Purpose
    The purpose of this code is to send and recieve :term:`CAN <CAN Bus>` messages. 

Usage
    Function usage is declared in the ``BSP_CAN.h file``. When using these functions in the :term:`RTOS <RTOS>`,
    pend and post must occur before and after the functions are called. The resources used for pend and 
    post can be seen in the ``CANBus.c`` driver :ref:`documentation <CAN Driver>`.

Additional Considerations
    It also contains a :term:`queue <Queue>` for messages that are received on the CAN bus that has a depth of 10. 
    This queue cannot be accessed outside the file. If the queue exceeds it's limit, we will lose
    messages.

Contactor BSP
=============

Purpose
    The contactor BSP interfaces with the :term:`GPIO <GPIO>` pins connected to the contactor. These pins are used to control the contactor (PB0) and to check 
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
    Pack. It uses pins PC6, PC7, PB14, PB15 with the alternative function of :term:`Pulse-Width Modulation 
    <Pulse Width Modulation>` enabled for as many speeds as needed for a total of 4 fans. The fans 
    use PWM, so the speed can be reduced to save energy.

Usage
    It sets the speeds of individual fans and can also return the value of those speeds. The 
    ``BSP_Fans.h`` file has more information on how to use each function.

Additional Considerations
    None

I2C BSP: Manthan Upadhyaya
=================================

Purpose
    The :term:`I2C <I2C>` driver is based on polling and is used to communicate with the 
    :term:`EEPROM <EEPROM>` to log data. It requires pins PA8(SCL) and PC9(SDA) for the I2C3.

Usage
    There are multiple definitions of the same functions within this library. They are split into 2 
    categories, one for the RTOS and one for the Bare-Metal version of the BPS. There are ``#define`` that 
    encompass these functions that are set when compiled. The default ``#define`` is "RTOS" but it can be 
    changed to compile the Bare-Metal code if necessary. The ``BSP_I2C.h`` file has more information on how
    to use each function. The same function declarations are used regardless of whether it is RTOS or
    Bare-Metal.

Additional Considerations
    None

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
    
    ``CAN`` - The BPS is sending a :term:`CAN <CAN Bus>` message.
    
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

OS Header file: Sijin Woo
=========================

Purpose
    The BSP is meant to be compatible with the Baremetal and RTOS version of the BPS. In order to accomplish
    this, ``BSP_OS.h`` was written in order to separate the two code bases in our library.
Usage
    The ``struct`` of type ``bsp_os_t`` must be defined and both pend and post functions must be created for both
    versions of the BPS. The RTOS version of the BPS will have the pend and post functions initialized to
    pend and post the shared resource of that library. The baremetal version will have the pend and post 
    functions remain empty. There are multiple ways these can be used. Then ``#ifdef`` will surround both 
    initializations of this struct. If we compile with ``RTOS``, then the RTOS functions will be used. If 
    we compile with ``BAREMETAL``, then the baremetal functions will be used. For example,

    .. code-block:: c

        // LTC6811.c
        bsp_os_t spi_os;
        OS_SEM MinionsIO_Sem4;
        void LTC6811_Pend(void) {
            CPU_TS ts;
            OS_ERR err;
            OSSemPend(&MinionsIO_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
        }
        void LTC6811_Post(void) {
            OS_ERR err;
            OSSemPost(&MinionsIO_Sem4, OS_OPT_POST_1, &err);
        }
        void LTC6811_Init(void) {
            spi_os.pend = LTC6811_Pend;
            spi_os.post = LTC6811_Post;
            BSP_SPI_Init(&spi_os);
        }

Additional Considerations
    There are other ways you can also use this library. The struct can be passed in to the initialization
    function of the init function. You could even choose to not use this struct and just ``#ifdef``
    everything in the library. This is up to the programmer's judgement to make the code look as neat
    as possible.

PLL BSP: Sijin Woo
==================================

Purpose
    The :term:`PLL <Phase Locked Loop>` is used to increase the clock speed of the STM32F413's clock. 
    Right now, it is set to change it from 16 MHz to 80 MHz.

Usage
    Calling the init function automatically sets the clock speed to 80 Mhz. In order to verify
    that it worked, you can call ``BSP_PLL_GetSystemClock()``.

Additional Considerations
    Increasing the clock speed does take more power. Although this is minor considering how much the
    entire BPS takes, it is worth noting. It should also be initialized before anything else that is 
    dependent on the clock speed (e.g UART).

SPI BSP: Clark Poon, Sijin Woo, and Sugam Arora
===============================================

Purpose
    :term:`SPI <SPI>` is used for communication between the uC and the minion LTC6811s as well as 
    communication between the uC and the AS8510 current sensor.

Usage
    Our car uses the ``SPI1`` and ``SPI3`` buses. The ``SPI1`` bus is for communication with the LTC6811s
    and the ``SPI3`` bus is for communication with the AS8510 current sensor. When calling the init function, 
    you must specify which port you intend to use (via the ``spi_port_t`` enum). The user can read/write 
    on whichever bus they have chosen. This BSP includes IRQ Handlers for both buses that post the ``MinionsASIC_Mutex``.

    ``SPI_Wait()`` has two different versions - one for the bare-metal BPS code and one for the RTOS code. 
    The BPS code is automatically compiled with ``#define RTOS``. If the user wants to use the bare-metal
    version of the code, the code must be compiled with the ``BAREMETAL`` parameter.

Additional Considerations
    The ``SPI1`` and ``SPI3`` IRQs are only included in the RTOS version of the code.

    If additonal SPI buses are needed, the user must modify the ``spi_port_t`` enum

Timer BSP: Sijin Woo
=================================

Purpose
    The purpose of this timer is to calculate the state of charge of the battery pack. This driver 
    uses Timer 2 as a countdown timer with a period of 0xFFFF-1 that reloads when it reaches 0.

Usage
    In order to use the timer, first you must call ``BSP_Timer_Init()`` and then call 
    ``BSP_Timer_Start()`` to start it. ``BSP_Timer_GetRunFreq()`` is used to find the frequency of
    the timer. You can use this alongside ``BSP_Timer_GetTicksElapsed()`` to see how much real time
    has passed since ``BSP_Timer_GetTicksElapsed()`` was called. 

Additional Considerations
    If we need another timer for another driver or application, we will need to have input parameters
    for the functions ``BSP_Timer_Start()``(to select which timer to start), ``BSP_Timer_GetRunFreq()``
    (to find the frequency of a given timer), & ``BSP_Timer_GetTicksElapsed()``. We could also change 
    the function names for each timer. Another thing to note is that one timer should not be used for
    multiple resources because if ``BSP_Timer_GetTicksElapsed()`` is called for one resource, it will
    interfere with the time passed for the second resource.

UART BSP: Manthan Upadhyaya
==================================

Purpose
    The :term:`UART <UART>` driver is interrupt driven and initializes 2 serial transmission lines(UART 2 and 3). 
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

Watchdog Timer BSP: Sijin Woo
=================================

Purpose 
    The purpose of this timer is to make sure that the BPS has not stalled while running. If we are
    in the Bare-Metal version of our code, that means the BPS is stuck in a loop somewhere and is not able
    to check the data given to it. If we are in the RTOS version of our code, that means the BPS is stuck
    in a thread (in deadlock).
Usage
    First you must initialize and start the timer by calling ``BSP_WDTimer_Init()`` and 
    ``BSP_WDTimer_Start()``. After the timer is started, it must be reset before it finishes
    counting down or else it will reset the system. In the Bare-Metal version of our system, the 
    timer is reset once every time the entire while loop runs through. In the RTOS version, each 
    thread sets a bit and when every bit is set, the timer resets.

Additional Considerations
    None
