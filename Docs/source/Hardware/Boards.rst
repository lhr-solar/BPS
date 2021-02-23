********
Boards
********

Amperes Minion Board
=====================

The Amperes minion board is connected to the main power cable that powers the electrical loop using 
the :term:`Shunt Resistor`. 

.. figure:: ../_static/AS8510.png
    :align: center

    AS8510 Block Diagram 

Fan Board: Manthan Upadhyaya
=============================

Purpose
    The purpose of this board is to set the speed of the fans that will be used to cool the battery 
    pack. It recieves signals from the Master Board and sends them across a :term:`MOSFET <MOSFET>` 
    that switches the fan on or off.

Blocks
    Mosfets - Each mosfet is between the power provided by the Master Board and power provided to the
    fans. When the :term:`PWM <Pulse Width Modulation>` signal is set, it switches the fan on or off 
    at a certain duty cycle.

Connections
    6 Connector: 4 of these connections are the PWM signals sent by the Master board. The other 2
    are power and ground.
    
    2 Connector(4): These are the power and grounds each fan is connected to.

Additional Considerations
    None

Leader board
============

Connectors
^^^^^^^^^^
The Leader board uses the STM32F413 microcontroller. The board requires seven connectors:

* One 2-pin :ref:`power connector <power-connector>` (+12V, PWRGND)
* One 2-pin error light connector (+12V, PWRGND)
* One 4-pin contactor connector (+12V, PWRGND, aux1, aux2)
* One 4-pin Amperes board connector (+12V, PWRGND, IP, IM)
* One 4-pin CAN connector (isolated +5V, isolated GND, CAN high, CAN low)
* One 2-pin Minion connector (IP, IM)
* One 2x4-pin fan connector (4x +12V, 4x PWRGND)

.. figure:: ../_static/LeaderBoard.png
    :align: center

    Leader Board Block Diagram 

The STM32F413 requires bypass capacitors and uses an 8MHz clock. To program the STM32F413, pins are 
connected to a 1x4 header to use the :term:`SWD <SWD>` programming protocol. 

The reset button allows you to avoid powering the BPS off to reset it.

.. note::
    You can configure the system to reset every time you use the Keil IDE to program it. 

For more information, see the `STM32F413 datasheet <https://www.st.com/resource/en/reference_manual/dm00305666-stm32f413-423-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf>`__.

.. figure:: ../_static/leader-board-diagram.png
    :align: center

    STM32F413 bypass capacitors 
Clock
^^^^^
The system's :abbr:`RTC (Real Time Clock)` is sourced from a low-power crystal oscillator. The crystal 
is used to set the :term:`PLL <Phase Locked Loop>`, which sets the system core frequency (CPU clock 
speed). The controller supports a range of frequencies, but the default is 8MHz. 

The :abbr:`MCU (Microcontroller Unit)` has an alternate internal clock that saves power consumption 
at the cost of precision. Since this system is safety critical, the external 8MHz crystal was added. 

.. note::
    The internal and external clocks were not tested against each other.
Capacitors
^^^^^^^^^^
Capacitor values are set based on the crystal’s load capacitance and the capacitance of the whole 
board. The crystal and capacitor should be as physically close to the MCU as possible to avoid signal drift.

.. figure:: ../_static/crystal-capacitors.png
    :align: center

    STM32 crystal layout

.. _power-connector:

Power connector
^^^^^^^^^^^^^^^
The Leader board gets a 12V power supply. The voltage must be dropped to power the MCU and components. 
The total voltage required for all the components is +3.3V and +5V. 

DC-DC converter
^^^^^^^^^^^^^^^
An isolated DC-DC converter (RI3-1205S) converts the +12V input to +5V. The +12V input will be 
isolated from the +5V output. The Leader board must use the correct grounds to maintain isolation. 

A switching regulator (NCP1117) converts the +5V input to +3.3V. This does not need isolation because 
the +5V input and corresponding grounds are already isolated from the +12V line.

.. figure:: ../_static/leader-board-power-connector.png
    :align: center

    Leader board power distribution

Voltage & Temperature Minion Board
==================================

There are two temperature sensors for each battery module with a module minion board for each of the 
four rows. Each board measures up to 12 battery modules and 16 temperature sensors, but is configured 
for eight modules and 16 sensors by default. The temperature sensors are placed as inputs to a mux 
and the mux switches between all of them.

.. figure:: ../_static/LTC6811.png
    :align: center

    LTC6811 Block Diagram 
