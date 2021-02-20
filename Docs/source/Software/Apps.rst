************
Application
************

.. _CLI-app:


Main
====

The main function is the entry point into the Battery Protection System. 

It first checks whether the BPS has had to reset in the previous run using ``BPS_WDTimer_DidSystemRest()``. If it has, then the program will then enter a    fault state using ``EnterFaultState()``. If the BPS was not previously reset, then main will continue with the rest of the initialization.
        
``OSInit()`` initializes the operating system. ``AsserOSError()`` checks whether there are any errors in the RTOS functions. If there are no errors          ``OSTaskCreate()`` points the program to ``Task_Init()`` which executes the rest of the BPS startup.


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

| Reads and writes to the CANbus from the BPS
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

| Reads and writes to the EEPROM.
| **Argument(s)**: 

- **none**: prints EEPROM status
- ***reset**: resets the EEPROM
- **error** [error type]: prints error related to the given subsytem

"ADC"
#####

| Displays raw ADC data.
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

- **none**: prints voltage, current, temperature, contactor, and state of charge data (in the same format as using each command separately)




**Note**: " * " denotes a command that alters the state of the BPS. Only use these commands if you know the consequences 

**Examples of Valid CLI Commands**:

``voltage module 5`` - prints module 5's voltage

``watchdog error`` - prints most recent watchdog errors

``temperature module 6 1`` - prints temperature of sensor 1 on module 6


Charge
======

Purpose
    The Charge App is used to keep track of the state of charge of the battery.

Usage
    ``Charge_Init()`` must be called before calling any of the other Charge functions. Descriptions of the other functions can be found in Charge.h.

Additional Considerations
    None.

Temperature
===========

Purpose
    The BPS must monitor the battery pack's temperature to avoid any hazardous conditions for the car and the driver.
    Each battery module has 2 temperature sensors.

Usage
    This file uses a global ``int32_t ModuleTemperatures[][]`` 2D array that contains each sensor's temperature. 
    ``Temperature_UpdateAllMeasurements()`` should be called to update the values of this array.
    
    ``Temperature_Init()`` must be called before using any other temperature functions. This function will create the ``TemperatureBuffer_Mutex``.
    This function will return an ``ErrorStatus`` indicating its success/failure.


Additional Considerations
    None

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
    UART to be initialized, since it uses printf().

Amps
=====

Purpose
    This app is used to check the current through the battery pack.

Usage
    In order to call any functions in this app ``Amps_Init()`` must be called first. One thing to note
    is that if you want to read the current, you must call ``Amps_UpdateMeasurements()`` first and 
    then call ``Amps_GetReading()``.

Additional Considerations
    To make it compatible with both Bare-Metal and RTOS, some functions meant for RTOS are left empty
    so when it is compiled for Bare-Metal, it will not execute those functions. These functions are
    meant for pending and posting when running in the RTOS.

**********************
Mutexes and Semaphores
**********************

Voltage Mutex
=============

Mutually excludes accesses to the Voltage buffer in the Voltage.c library. 

MinionsIO Semaphore
===================

Whenever the LTC driver is calling the SPI function, there's going to be some delay until the SPI transfer is complete. During that delay, we should be 
executing some other task so once the SPI transfer starts, the system should start waiting for this semaphore. 
Whenever the SPI transfer is complete, we must signal this semaphore so we need to have an ISR that calls the signal semaphore function whenever a transfer is complete. This ISR is in the BSP SPI module.

Fifo Header file: Chase Block
=============================

Purpose
    Many of our BSP packages use FIFO's to send and receive data. In the old version of the BPS, a FIFO
    was created in each package along with it's driver. This header file does all that so we have a 
    defined FIFO "class" in our library.

Usage
    More detailed usage instructions are in lines 8-15 of ``fifo.h``. The function names for all of the FIFO
    drivers will be ``fifo_name_function_name`` where ``function_name`` can be anything in lines 69-75
    of this file.

Additional Considerations
    None
