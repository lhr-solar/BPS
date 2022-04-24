************
Application
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

Fault Semaphore
===============

When the :ref:`Fault State <Fault State Task: Manthan Upadhyaya>` runs, it uses a blocking pend on this semaphore. 
When a task notices a fault condition, this semphore will be posted and the fault state task will enter the BPS 
fault state.

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
