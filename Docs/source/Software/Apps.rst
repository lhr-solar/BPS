************
Applications
************

Amps
=====

Purpose
    This app is used to check the current through the battery pack.

Usage
    In order to call any functions in this app ``Amps_Init()`` must be called first. One thing to note
    is that if you want to read the current, you must call ``Amps_UpdateMeasurements()`` first and 
    then call ``Amps_GetReading()``. ``Amps_Calibrate()`` should be run on startup because it 
    was observed that when the BPS is initially powered, there is a delay before the LTC2315 
    can be calibrated properly. This is not observed on pressing the reset button.

Additional Considerations
    To make it compatible with both Bare-Metal and RTOS, some functions meant for RTOS are left empty
    so when it is compiled for Bare-Metal, it will not execute those functions. These functions are
    meant for pending and posting when running in the RTOS.

Charge
======

Purpose
    The Charge App is used to keep track of the state of charge of the battery.

Usage
    ``Charge_Init()`` must be called before calling any of the other Charge functions. Descriptions of the other functions 
    can be found in ``Charge.h``.

Additional Considerations
    None.

.. _CLI-app:

Command Line Interface (CLI)
============================

This interface is meant to be used for debugging and monitoring the BPS. 
A list of all valid CLI commands is included below.

"Help"
######

| Displays a help menu with all available commands.
| **Argument(s)**: N/A

"Voltage"
#########

| Displays voltage levels (both module-specific and total voltage) in Volts and the safety of the battery.
| **Argument(s)**: 

- **none**: prints every module's voltage
- **module [module #]**: prints specific module's voltage
- **total**: prints total battery pack's voltage
- **safety**: prints safety status

"Current"
#########

| Displays current readings as measured by the current sensors in Amps.
| **Argument(s)**: 

- **none**: prints current data
- **safety**: prints safety status
- **charging**: prints if pack is charging or discharging

"Temperature"
#############

| Displays temperature readings as measured by the temperature sensors (2 per battery module) in degrees C.
| **Argument(s)**: 

- **none**: prints temperature of 31 modules
- **all**: prints temperature of 62 temperature sensors
- **module [module #]**: prints temperature of specific battery module
- **module [module #] [sensor #]**: prints temperature of a specific sensor on a module
- **total**: prints average temperature of the battery pack

"LTC"/"Register"
################

| Displays register information of the LTC6811 chip.
| **Argument(s)**: 

- **none**: prints all relevant register information from the ``cell_asic`` structure


"Contactor"/"Switch"
####################

| Displays current state of the contactor.
| **Argument(s)**: 

- **none**: prints current status of the contactor (enabled/disabled)

"Charge"
########

| Displays state of charge measurements/calculations.
| **Argument(s)**: 

- **none**: prints charge percentage of the battery
- ***reset**: resets the accumulator
- ***set [value]**: sets the accumulator to the argument value

"LED"/"Lights"
##############

| Tests the status LEDs on the master board.
| **Argument(s)**: 

- **none**: prints the state of the error light
- ***test**: runs a short test that flashes each status LED in sequence
- ***[LED type] [on/off]**: sets the state of a specific LED

"CAN"/ "Canbus"
###############

| Reads and writes to the CAN bus from the BPS
| **Argument(s)**: 

- **read**: prints messages until halted by escape sequence
- ***write [id name] [data in hex]**: writes a specific message to CAN

"Watchdog"
##########

| Displays watchdog data.
| **Argument(s)**: 

- **none**: prints watchdog status
- **error**: prints most recent errors from the watchdog

"EEPROM"
########

| Reads and writes to the :term:`EEPROM <EEPROM>`.
| **Argument(s)**: 

- **none**: prints EEPROM status
- ***reset**: resets the EEPROM
- **error** [error type]: prints error related to the given subsytem

"ADC"
#####

| Displays raw :term:`ADC <Analog to Digital Converter>` data.
| **Argument(s)**: 

- **none**: prints raw data from the ADC at the current time

"Critical"/"Abort"
##################

| Shuts down the contactor.
| **Argument(s)**: 

- ***none**: prompts the user for confirmation, then turns off the contactor upon confirmation.

"Openwire"
##########

| Displays open wire status.
| **Argument(s)**:

-**none**: prints open wire data for all wires in the system

"All"
#####

| Displays all relevant data to the BPS.
| **Argument(s)**: 

- **none**: prints voltage, current, temperature, contactor, and state of charge data (in the same format as using each 
  command separately)




**Note**: " * " denotes a command that alters the state of the BPS. Only use these commands if you know the consequences 

**Examples of Valid CLI Commands**:

``voltage module 5`` - prints module 5's voltage

``watchdog error`` - prints most recent watchdog errors

``temperature module 6 1`` - prints temperature of sensor 1 on module 6

Fifo Header file: Chase Block
=============================

Purpose
    Many of our BSP packages use FIFO's to send and receive data. In the old version of the BPS, a FIFO
    was created in each package along with it's driver. This header file does all that so we have a 
    defined :term:`FIFO <Queue>` "class" in our library.
    
Usage
    More detailed usage instructions are in lines 8-15 of ``fifo.h``. The function names for all of the FIFO
    drivers will be ``fifo_name_function_name`` where ``function_name`` can be anything in lines 69-75
    of this file. The ``#define`` for the fifo types must be declared before you write ``#include fifo.h``.

Additional Considerations
    None

Main
====

The main function is the entry point into the Battery Protection System. 

It first checks whether the BPS has had to reset in the previous run using ``BPS_WDTimer_DidSystemRest()``. If it has, then 
the program will then enter a    fault state using ``EnterFaultState()``. If the BPS was not previously reset, then main will 
continue with the rest of the initialization.
        
``OSInit()`` initializes the operating system. ``AsserOSError()`` checks whether there are any errors in the RTOS functions. 
If there are no errors ``OSTaskCreate()`` points the program to ``Task_Init()`` which executes the rest of the BPS startup.

Contactor
=============

Purpose
    The contactor BSP interfaces with the :term:`GPIO <GPIO>` pins connected to the contactor. These pins are used to control the 
    contactor (PB0) and to check the state of the contactor (PB1).

Usage
    The peripheral should be initialized by calling ``Contactor_Init()`` before any of the other functions are called. See 
    ``Contactor.h`` for details on individual functions. The AUX pin we have connected to the contactor is negative 
    logic so ``Contactor_GetState()`` reads negative logic for the state of the contactors.

Additional Considerations
    The contactor is one of the most important safety features in the vehicle, so the contactor should only be turned on (closed) 
    if the BPS is sure that the battery pack is in a safe state. There are 2 pins controlling the contactors. While the layout of which
    contactors are controlled by what will change, calling ``Contactor_On()`` or ``Contactor_Off()`` with the ``#define`` listed in
    "enum Contactors_t" will control that specific contactor.

Fans
====

Purpose
    The Fans driver is used to control the speed of the fans depending on the temperature of the Battery
    Pack. It uses pins PC6, PC7, PB14, PB15 with the alternative function of :term:`Pulse-Width Modulation 
    <Pulse Width Modulation>` enabled for as many speeds as needed for a total of 4 fans. The fans 
    use PWM, so the speed can be reduced to save energy.

Usage
    It sets the speeds of individual fans and can also return the value of those speeds. The 
    ``Fans.h`` file has more information on how to use each function.

Additional Considerations
    None

Temperature
===========

Purpose
    The BPS must monitor the battery pack's temperature to avoid any hazardous conditions for the car and the driver.
    Each battery module has 2 temperature sensors.

Usage
    This file uses a global ``int32_t temperatures[][]`` 2D array that contains each sensor's temperature. 
    ``Temperature_UpdateAllMeasurements()`` should be called to update the values of this array.
    
    ``Temperature_Init()`` must be called before using any other temperature functions. This function will create the 
    ``TemperatureBuffer_Mutex``. This function will return an ``ErrorStatus`` indicating its success/failure.

Additional Considerations
    ``TEMP_ERR_OUT_BOUNDS`` was chosen to be it's current value because it is an impossibly high temperature that is returned
    as an error code. The temperature sensors can never read this value using the LMT87, it will cause an OVOLT fault, and is
    a good debugging tool. ``delay_u()`` was used due to timing issues with the LTC. ``Temperature_ChannelConfig`` is called
    twice whenever channel 1 of a mux is initalized due to the start up time of enabling the mux from a powered off state.
    

Voltage
========

Purpose
    This App is used to keep track of the voltages and open wires of all the modules in the 
    battery pack.

Usage
    ``Voltage_Init()`` must be called before calling any other Voltage functions. All of the voltage data
    used by the functions is based on data collected by the function ``Voltage_UpdateMeasurements()``
    so it might be necessary to call ``Voltage_UpdateMeasurements()`` before those functions.
    Descriptions of other functions are located in Voltage.h.

Additional Considerations
    The Open Wire functions all directly contact the LTC. ``Voltage_OpenWireSummary()`` requires 
    UART to be initialized, since it uses printf(). Also, when using the RTOS version of the BPS, 
    communication with the LTC should also be protected by the mutex ``MinionsASIC_Mutex``. This 
    includes calling the function ``wakeup_sleep()`` along with all the other communication functions.
    Finally, ``Voltage_OpenWire()`` is modified upon the fact that the last LTC module only has 7 
    batteries connected to it. If changes to hardware are made, this function will require changes as well.

Fault State
===========

Purpose
    The Fault State is called when a fault condition is set off in the BPS. These fault 
    conditions can be found :ref:`here <Design Requirements>`. Entry into the fault state
    is achieved through a simple function call, ``EnterFaultState()``.

Functionality:
    1) The contactors are turned off.
    
    2) All the fans are set to maximum speed.
    
    3) The proper LED's are turned on and off. When the fault LED is turned on, the strobelight turns on.
    
    4) The fault condition is logged into the EEPROM.
    
    5) A message is sent along the CAN bus to the BPS display board to notify the driver that the BPS is tripped.
   
    6) The WatchDog timer is continually reset to prevent the BPS from going into fault again.

Common Fault types:
    a. Undervoltage
        An undervoltage fault is signaled by the "UVOLT" light on the BPS leader board.
        This means that at least one of our battery modules is not providing enough voltage,
        the limit being specified in ``config.h``. 
    b. Overvoltage
        An overvoltage fault is signaled by the "OVOLT" light on the BPS leader board.
        This means that at least one of our battery modules is exceeding the maximum
        voltage limit specified in ``config.h``.
    c. Overtemperature
        An overtemperature fault is signaled by the "OTEMP" light on the BPS leader board.
        This means that at least one of the battery modules' temperature is exceeding the
        maximum temperature we consider safe.
    d. Overcurrent
        An overcurrent fault is signaled by the "OCURR" light on the BPS leader board.
        This means that the current measured coming out of the battery pack is exceeding
        our maximum operating current.
    e. Watchdog
        A watchdog fault is signaled by the "WDOG" light on the BPS leader board.
        The watchdog fault is tripped whenever the watchdog isn't reset before
        it finishes counting down. Typically this means that the BPS is in an infinite loop
        somewhere, and could be an RTOS issue (deadlock) or a section of bad code.
    f. Other
        "Other" is a bit tricky to identify the source of. Typically, the BPS will experience
        an "OTHER" fault due to a CRC failure, where the BPS and expected minion boards fail
        their CRC check for communicating to each other. This is seen commonly when accidentally
        running the full BPS code but only testing a specific feature, without all expected
        peripherals and boards plugged in.

.. note::
    This list is NOT comprehensive, and only lists some of the more common faults seen
    when testing and debugging. For the full list of fault states, please see ``FaultState.c``.

Shared Resources
    The Fault State makes heavy use of the the ``Fault_BitMap`` variable. This variable is set by the 
    other tasks so the Fault task does not have to call other functions to find out what caused the fault. 
    The variable used to set ``Fault_BitMap`` is ``enum Fault_Set``. The description of this enum is in 
    the file ``tasks.h``.
    The variable ``Fault_Flag`` is also used by some functions to bypass OS functions in the case of
    a fault. If the variable is set to 1, functions such as ``OS_SemPend`` & ``OS_SemPost`` are skipped.

Timing Requirements
    The contactor must be shut off as soon as possible after a fault is detected.

Additional Considerations
    Although the BPS goes into fault state when the battery is in danger, it also goes into fault 
    state when there is an issue with the RTOS. Since the BPS must always run during the race, care 
    must be taken to minimize the chances of this happening. It also goes into a fault state when 
    the hard fault handler is called.

**********************
Mutexes and Semaphores
**********************

MinionsIO Semaphore
===================

Whenever the LTC driver is calling the :term:`SPI <SPI>` function, there's going to be some delay until the SPI 
transfer is complete.  During that delay, we should be executing some other task so once the SPI transfer starts, 
the system should start waiting for this :term:`semaphore <Semaphore>`. Whenever the SPI transfer is complete, we
must signal this semaphore so we need to have an :term:`ISR <ISR>` that calls the signal semaphore function 
whenever a transfer is complete. This ISR is in the BSP SPI module.

Safety Check Semaphore
======================

When the :ref:`Critical State <Critical state Task: Manthan Upadhyaya>` runs, it checks to see if the Amperes, 
Voltage, Open Wire, and Temperature are safe. This process is done by the Critical State Task using a blocking 
Pend function on the Safety Check Semaphore. The task uses a blocking pend (wont move past the function until the 
semaphore is pended ``NUM_FAULT_POINTS`` amount of times).

Amperes IO Semaphore
====================

Whenever the Amperes Driver is calling the :term:`SPI` fucntion, there is going to be some delay until the SPI
transfer is complete. The Semaphore is pended during that time and other tasks are allowed to execute. Once the
transfer is complete, an ISR posts the semaphore.

CAN FIFO Semaphore
==================

Used in `CAN_Queue.c` to flag that there is data in the CAN FIFO. Posted by tasks that place data 
in the CAN FIFO and pended when data is removed from the FIFO.

CAN Mailbox Semaphore
=====================

There are 3 hardware mailboxes that can hold CAN messages that will be sent along the CAN line. This semaphore is 
initialized to a value of 3 (number of empty mailboxes). When the semaphore is posted, the count decreases (one 
mailbox becomes full). When the semaphore is pended, the count increases (one mailbox becomes empty). This means 
that the semaphore will keep track of how many CAN mailboxes are free. If we pend this semaphore before sending a 
CAN message and post it after sending a CAN message, we can make sure the code will block until a mailbox is free 
if we try to send a CAN message when no mailboxes are free.

CAN FIFO Mutex
==============

Mutually excludes access to CAN FIFO in `CAN_Queue.c`.

Amperes Data Mutex
==================

Mutually excluded data to Amperes data in `Amps.c` library.

Watchdog Mutex
==============

This mutex excludes access to the ``WDog_Bitmap`` variable that makes sure that the 
:ref:`Voltage/Temperature monitor <Voltage Temperature Monitor Task: Sijin Woo>` and 
:ref:`Amperes monitor <Amperes Task: Manthan Upadhyaya>` are not stuck.

Voltage Mutex
=============

Mutually excludes accesses to the Voltage buffer in the Voltage.c library. 

MinionsASIC_Mutex
=================

This mutex excludes access to the ``Minions`` struct array. This struct holds voltage and temperature data that 
is taken from the LTC6811.
