********
Boards
********

**NOTE**: All BOM's for all boards are located in the repository for that board.
**NOTE**: All boards are located in the battery box unless otherwise mentioned.

Amperes Minion Board
=====================

The Amperes minion board is connected to the main power cable that powers the electrical loop using 
the :term:`Shunt Resistor`. It sits on top of the Shunt Resistor board as a hat PCB. 

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-AmperesPCB/tree/master>`__

Brief Description/Purpose:
   This board monitors the current through the HV line.  

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
=========== ============================================== =================================================
Regulation  Description of Regulation                      How Regulation is Met

**8.3**     | All batteries must have protection circuitry | This board monitors the current/temperature 
            | appropriate for the battery technology used. | of the battery and sends both measurements 
            | Proof is required at scrutineering that the  | back to the BPS Leader. Then, the BPS system
            | protection System is functional and meet     | determines if these measurements are within 
            | manufacturer's specifications... All         | normal range and if not, will take 
            | protection circuitry should be contained in  | apropriate action.
            | the battery enclosure per Reg 8.4. 

**8.3.A.5** | System in which measurements are constantly  | This board sends current and temperature    
            | monitored and where actions are taken        | measurement back to the BPS leader 
            | immediately without operator intervention    | periodically. This helps the BPS Leader board 
            | to open the Main Power Switch should a       | determine when a Battery protection Fault 
            | battery Protection Fault occur. Any          | occurs and take appropriate actions.
            | manual clearing process is required by the   
            | driver with the vehicle not in motion and    
            | only after faults have been verified clear   
            | by the protection system.        
=========== ============================================== =================================================

Details
^^^^^^^
**List of I/O and Connections:**
    
    * 4 Pin Molex Microfit: From BPS Leader
        * 12V
        * IP (IP and IM should be twisted pair)
        * IM
        * GNDPWR
    * 5 Pin Header: Shunt Board
        * ETS: Used in case thermistor is needed to account for changes in resistance of shunt resistor
        * RSHH: Differential High-Side reading of Shunt Voltage
        * RSHL: Differential Low-Side reading of Shunt Voltage
        * GND
        * 3V3: Unused
    * 5 Pin Header: Shunt Board
        * NC: No connect to hold jumper when current is not being tested
        * SHUNT: Tied to RSHH
        * TEST: Connect jumper between SHUNT and TEST when wanting to test current
        * V+: Apply voltage to V+ that matches resistor divider on Shunt Board to test current
        * V-: Applying Positive voltage here will make current negative

Requirements/Constraints:  
    * The HX1188FNL needs to be isolated, as in it cannot be placed on a GND or Power layer. 
      So a keep-out area was used around the HX1188FNL.    
    * Connectors must be placed on opposite sides of the board. This will allow all the connections to fit on the board.   

Design Choices:    
    * The board was kept at a small size to conserve space  
    
Fan Board
=========

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-FanPCB>`__

Brief Description/Purpose:
    The purpose of this board is to set the speed of the fans that will be used to cool the battery 
    pack. It receives signals from the Leader Board and sends them across a :term:`MOSFET <MOSFET>` that switches 
    the fan on or off. This board can also be used to interface with other high current driving peripherals.

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
========== ============================================== ===============================================
Regulation Description of Regulation                      How Regulation is Met

**8.4.D**  | Battery enclosures may be equipped with a    | The BPS fan board is powered by the battery 
           | forced ventilation system. Such ventilation  | system and in the event of a battery protection 
           | systems must pull exhaust to the exterior of | fault it's powered by the supplemental battery.
           | the solar car and must be directly connected 
           | to the exterior of the vehicle away from any 
           | airstream that may reach the driver. The 
           | ventilation system shall be powered by the 
           | battery system. In the event of a Battery 
           | Protection Fault, provisions should be made 
           | to power this fan from the Supplemental 
           | battery.
========== ============================================== ===============================================

Details
^^^^^^^
**List of I/O and Connections:**
    
    * 6 Pin Molex Microfit: From BPS Leader
        * 12V
        * FAN1
        * FAN2
        * FAN3
        * FAN4
        * GNDPWR
    * 2 Pin Molex Microfit: To Peripherals (x12)
        * PWR
        * GND

Requirements/Constraints:  
    * We chose flyback diodes to be used to prevent voltage spikes from entering into the leader 
      board (since the pulsing fans can cause the spikes).

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

Microcontroller
^^^^^^^^^^^^^^^
The :term:`STM32F413 <STM>` requires bypass capacitors and uses an 8MHz clock. To program the STM32F413, pins are 
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

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-MinionPCB>`__

Brief Description/Purpose: 
    There are temperature sensors within the battery pack attached to each module (the number of which could 
    based on the design). Each board measures up to 12 battery modules and 16 temperature sensors. The 
    temperature sensors are placed as inputs to a mux and the mux switches between all of them.

.. figure:: ../_static/LTC6811.png
    :align: center

    LTC6811 Block Diagram 

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
===========  ============================================== ===========================================================
Regulation   Description of Regulation                      How Regulation is Met

**8.3.A.5**  | System in which measurements are constantly  | The Minion board constantly takes voltage and temperature 
             | monitored and where actions are taken        | measurements of the battery modules and transmits data
             | immediately without operator intervention to | back to the leader board.
             | open the Main Power Switch should a battery  
             | Protection Fault occur. 
===========  ============================================== ===========================================================

Details
^^^^^^^
**List of I/O and Connections:**
    
    * 2 Pin Molex Microfit: To/From BPS Leader/Minion Board (x2)
        * IP (IP and IM should be twisted pair)
        * IM
    * 7 Pin Molex Microfit: From Scrutineering Boards (x2)
        * NC
        * Voltages (x12)
        * GND
    * 9 Pin Header: From Minion Shield Board (x2)  
        * 5V
        * Temperatures (x16)
        * GND

Requirements/Constraints:  
    * Connectors to the battery modules are placed vertically on the right to make use the 
      connections sequential and more intuitive. 
    * Communication connections to the Leader Board are placed on the right side opposite the battery connections.
Design Choices:
    * The MUXs and connectors to the temperature sensors were placed on the Minion Shield board to keep the board 
      smaller and more compact 
    * LTC6811 was placed at the center of the board to minimize distance to the outlying components. 

BPS Minion Shield
=================

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-MinionShieldPCB.git>`__

Brief Description/Purpose:
    The Minion Shield board is a shield for the Module Minion board. The board connects 16 temperature 
    sensors to the Minion board, saving space on the actual Minion board.   

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
=========== ============================================== =================================================
Regulation  Description of Regulation                      How Regulation is Met

**8.3**     | All batteries must have protection circuitry | This board connects to the temperature sensors,
            | appropriate for the battery technology used. | which will check for the fault conditions that
            | Proof is required at scrutineering that the  | concern temperature (Max charging temperature 
            | protection System is functional and meet     | of 45 C and discharging temperature bounds of 
            | manufacturer's specifications... All         | 60 C).
            | protection circuitry should be contained in
            | the battery enclosure per Reg 8.4. 

**8.3.A.5** | System in which measurements are constantly  | This board connects to the temperature 
            | monitored and where actions are taken        | senors, which take temperature measurements 
            | immediately without operator intervention    | and allow the Module Minion board to 
            | to open the Main Power Switch should a       | constantly monitor them without 
            | battery Protection Fault occur. Any          | operator intervention
            | manual clearing process is required by the   
            | driver with the vehicle not in motion and    
            | only after faults have been verified clear   
            | by the protection system.        
=========== ============================================== =================================================

Details
^^^^^^^
**Location of the Board:** The board is stacked on the Module Minion board.  

**List of I/O and Connections:**
    
    * Power +5 V 
        * Input from Module Minion board 
    * GND
        * Input from Module Minion board 
    * TempSens (1-16) 
        * input from a temperature sensor (using `LMT87 <https://www.ti.com/lit/ds/symlink/lmt87.pdf?ts=1642971325896&ref_url=https%253A%252F%252Fwww.google.com%252F>`_
        * output to the Module Minion board 
   
Requirements/Constraints:  
    * The male pin headers are on the top and bottom edges of the board 
      so that they can properly connect/stack on the Module Minion board.  
    * The board curves in on the left because the battery stack on the Module Minion board occupies that area.  

Design Choices:    
    * Filtering circuitry added to reduce need for filtering software in code.
    * There is a power LED in the bottom right of the board. 

BPS Shunt Resistor Board
========================

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-ShuntResistorPCB>`__

Brief Description/Purpose:
    This board holds the shunt resistor that allows the BPS Amperes Board to measure current and temperature. 
    It also contains circuitry to test the Amperes Board since large current cannot always be run through 
    shunt resistor.

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
=========== ============================================== =================================================
Regulation  Description of Regulation                      How Regulation is Met

**8.3**     | All batteries must have protection circuitry | This board helps in monitoring the current/
            | appropriate for the battery technology used. | temperature of the battery. Then, the BPS 
            | Proof is required at scrutineering that the  | system determines if these measurements are 
            | protection System is functional and meet     | within normal range and if not, will take 
            | manufacturer's specifications... All         | appropriate action. 
            | protection circuitry should be contained in
            | the battery enclosure per Reg 8.4. 

**8.3.A.5** | System in which measurements are constantly  | This board sends current and temperature 
            | monitored and where actions are taken        | quantities to be measured to the BPS Amperes
            | immediately without operator intervention    | board constantly. Once measured, this 
            | to open the Main Power Switch should a       | information is sent to the BPS Leader board 
            | battery Protection Fault occur. Any          | and helps determine when a Battery protection
            | protection faults will latch such that a     | on fault occurs. 
            | manual clearing process is required by the   
            | driver with the vehicle not in motion and    
            | only after faults have been verified clear   
            | by the protection system.        
=========== ============================================== =================================================

Details
^^^^^^^^
**List of I/O and Connections:**
    * 5 Pin Header: Amperes Board
        * ETS: Used in case thermistor is needed to account for changes in resistance of shunt resistor
        * RSHH: Differential High-Side reading of Shunt Voltage
        * RSHL: Differential Low-Side reading of Shunt Voltage
        * GND
        * 3V3: Unused
    * 5 Pin Header: Amperes Board
        * NC: No connect to hold jumper when current is not being tested
        * SHUNT: Tied to RSHH
        * TEST: Connect jumper between SHUNT and TEST when wanting to test current
        * V+: Apply voltage to V+ that matches resistor divider on Shunt Board to test current
        * V-: Applying Positive voltage here will make current negative
    
Requirements/Constraints:  
    * This board is required to fit in the molded enclosure on the shunt resistor.
      This molded enclosure is 40.1 ± 0.2 mm by 35.1 ± 0.2 mm (in inches, it is 1.580 ± 0.009 in by 1.380 ± 0.008 in).  
    * The headers and mounting holes on this board must align with that of the molded enclosure of the shunt resistor.  

Design Choices:    
    * This board had dimensions just slightly smaller than that of the molded enclosure on the shunt resistor. 
      This prevented the board from moving around too much within the molded enclosure and made placement of 
      circuit parts easier.  

Additional Considerations:
    * The shunt resistor must be oriented such that a positive current corresponds to discharging the battery.

BPS Scrutineering Board
=======================

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-ScrutineeringPCB>`__

Brief Description/Purpose:
    This board will be used during the scrutineering process for the American Solar Challenge race. 
    It will be used to demonstrate that the BPS is functional by proving that the BPS executes the 
    appropriate measures in an overvoltage and undervoltage state. The battery modules will be connected to the  
    BPS Minion Boards through this board. The board will be connected to an external power supply, which 
    will be used to simulate a normal, overvoltage, and undervoltage state. This board will induce an overvoltage 
    and undervoltage state to trigger the BPS without affecting the batteries. It will also contain fuses to prevent
    the Minion boards from being damaged in the event that the testing is done improperly.

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
=========== ============================================== =================================================
Regulation  Description of Regulation                      How Regulation is Met

**8.3**     | Protection circuitry: proof is required at   | This board is intended to help the BPS pass 
            | scrutineering that the protection system is  | scrutineering. It is supposed to help in the 
            | functional and meets manufacturer's          | testing procedures to demonstrate that the 
            | specifications. Testing procedures will be   | BPS system is function and meets any 
            | provided, and the protection system design   | specifications
            | should allow for such testing.  
=========== ============================================== =================================================

Context
^^^^^^^
**List of I/O and Connections:**
    
    * Up to 14 Battery Voltages 
        * Input from the Battery Pack
    * Up to 14 Minion Board Connections 
        * Output to the BPS Minion Board
     
Requirements/Constraints:  
    * The board should be space-efficient, intuitive, and easy to use. The parts should be 
      appropriately labelled.     
