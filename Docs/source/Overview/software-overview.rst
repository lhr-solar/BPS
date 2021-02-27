************************
Software Design Overview
************************

The software is responsible for 

- Periodically measuring the battery pack
- If the pack is unsafe, the shutdown sequence will run
- Balancing battery cells to keep the pack safe
- Periodically sending data to Telemetry

Software Flow 
=============

.. figure:: ../_static/software-flow.jpg
   :align: center


Software Stack 
==============

.. figure:: ../_static/software-stack.jpg
   :align: center

   

Board Support Package (BSP)
===========================

The BSP interfaces with peripherals and implements interrupt handlers.

Drivers
=======

Drivers interface with external hardware.
The software for the BPS has drivers for:

- CAN
- EEPROM
- LTC6811 (Voltage and Temperature)
- AS8510 (Amperes)

Real Time Operating System (RTOS)
=================================

The RTOS uses a scheduler to run multiple threads. 
Running these threads creates "psuedo-concurrency".

The RTOS ensures that safety critical tasks will not be blocked by non-safety
critical tasks.

We use the Micrium uC/OS-III RTOS. 

Applications (Apps)
===================

The BPS Apps consist of threads/tasks and functions that interpret measurements.
The following tasks make up the BPS' engine:

- Initialization
- Fault State
- Critical State
- Pet Watchdog
- Voltage/Temperature monitor
- Amperes monitor
- Battery Balancing
- Log Information
- CAN Message Consumer
- Command Line Interface (CLI)
- Idle  

Descriptions of what each task does can be found in the Tasks section.


