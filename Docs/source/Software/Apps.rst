************
Application
************


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
- **error [error type]: prints error related to the given subsytem

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




