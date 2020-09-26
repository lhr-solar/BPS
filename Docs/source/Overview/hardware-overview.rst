******************
Hardware Overview
******************

Battery compartment
===================

The battery compartment has two shelves of battery modules. Each shelf has two rows of modules for a total of four in the compartment.

There are 31 modules connected in series, with eight modules a row for three rows and seven modules in the fourth row.

.. note::
    Battery modules are removed if judges determine that the compartment is heavier than regulations allow.

.. figure:: ../_static/battery-box-xsection.png
    :align: center

    Cross-section of battery compartment

The empty cavity at the right end of the compartment houses the BPS components. 

Boards and sensors
==================

There are two temperature sensors for each battery module with a module minion board for each of the four rows.
Each board measures up to 12 battery modules and 16 temperature sensors, but is configured for eight modules and 
16 sensors by default.

The Amperes minion board is connected to the main power cable that powers the electrical loop using the :term:`Hall effect sensor`. 

Leader board
------------

The Leader board uses the STM32F413 microcontroller. The board requires nine connectors:

* One 2-pin power connector (+12V, PWRGND)
* One 2-pin error light connector (+, -)
* One 4-pin contactor connector (+, -, aux1, aux2)
* One 4-pin current board connector (+5V, GND, Analog signal 1, Analog signal 2)
* One 4-pin CAN connector (isolated1 +5V, isolated1 GND, CAN high, CAN low)
* One 2-pin Voltage connector (IP, IM)
* Two 2x4-pin fan connectors (+12V, PWRGND)

The STM32F413 requires bypass capacitors and uses an 8MHz clock. To program the STM32F413, pins are connected to a 1x4 header to use the :abbr:`SWD (Serial Wire Debug)` programming protocol. 

The reset button allows us to avoid powering the BPS off to reset it.

.. note::
    You can configure the system to reset every time you use the Keil IDE to program it. 

For more information, see the STM32F413 datasheet.

