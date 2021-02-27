********
Boards
********

There are two temperature sensors for each battery module with a module minion board for each of the four rows.
Each board measures up to 12 battery modules and 16 temperature sensors, but is configured for eight modules and 
16 sensors by default.

The Amperes minion board is connected to the main power cable that powers the electrical loop using the :term:`Shunt Resistor`. 

Display Board
=============

Overview
^^^^^^^^
`GitHub Link <https://github.com/lhr-solar/BPS-DisplayPCB>`__

`BOM Link <https://www.mouser.com/ProjectManager/ProjectDetail.aspx?State=EDIT&ProjectGUID=11b675c5-b15b-46bd-a790-bb450819a4d3>`__ 

Brief Description/Purpose:
    The purpose of this board is to display messages from the CAN interface on the E-Ink display 
    for debugging.

Pertinent Regulations
^^^^^^^^^^^^^^^^^^^^^
========== ============================================== ===============================================
Regulation Description of Regulation                      How Regulation is Met

8.3.B.2    | All supplemental batteries must have at a    The "SUPP CHECK" (Supplemental Check) LED
           | minimum Passive Protection for under voltage 
           | where charging occurs remote to the solar 
           | vehicle unless they are primary cells. 
           | Active Protection is required if charging is 
           | within the solar vehicle. No Secondary 
           | Lithium battery types shall be used for the 
           | Supplemental Battery unless the Supplemental 
           | Battery is powering a commercially procured 
           | component such as a cell phone or laptop and 
           | the Supplemental Battery was intended for 
           | this purpose.
========== ============================================== ===============================================

Context
^^^^^^^
**Location of the Board:** On a movable arm mount attached to the south end of the board. Mounted near the driver.  

List of I/O and Connections: 
    [List of all the inputs and outputs from this board and where it is connected to. 
    List all power signals first]
    
    * [Signal/Power] 
        * [Input/Output/Bidirectional] [from/to/with] [Board/Hardware/etc.] 
    * Power GNDPWR 
        * Input from car power connector 
    * Power +12V 
        * Input from car power connector 
    * Power +5V 
        * Output from power distribution subsheet 
    * Power +3.3V 
        * Output from power distribution subsheet 

Schematic
^^^^^^^^^
*What does this circuit do?*
    This circuit makes connections from the microcontroller to the E-Ink Display and logic 
    analyzer pins.  
*Why do we need it?*
    We need this circuit in order for debugging (CAN messages on the E-Ink display and logic 
    analyzer pins).  
*List of Circuit Components*
    * STM32F413RHTx 
        * Description: Microcontroller 
        * Justification for selection of specific part: Standard component  
        * `Datasheet link <https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwiqg4WM6NXuAhXULc0KHcHbAzIQFjABegQIAhAC&url=https%3A%2F%2Fwww.st.com%2Fresource%2Fen%2Fdatasheet%2Fstm32f413rg.pdf&usg=AOvVaw3U9MV6EuhEcmJpsEYCrU52>`__
        * Associated passives/components:
            * Bypass capacitors 
            * 8 MHz External Crystal Oscillator: used to set the Phase Lock Loop (PLL) and the 
            system core frequency (CPU processor speed) 
                * Using an external clock ensures for precision, which is important in a safety-critical system like the BPS.  
            * Reset button
            * :term:`SWD <SWD>` Programming Pins
    * BUK9M34-100EX(4) 
        * Description: a MOSFET that will switch the fans off and on 
        * Why is it necessary: so the fans can turn off and on 
        * Justification for selection of specific part: this part is used over others because of 
          Q101 compliant, its suitable for thermally demanding environments, and true logic gate 
          with VGS(th) rating of greather than 0.5V at 175°C 
        * Datasheet https://assets.nexperia.com/documents/data-sheet/BUK9M34-100E.pdf 
        * Associated passives/components:  
            * 4 different diodes, Molex_MicroFit3.0_1x2xP3.00mm_PolarizingPeg_Vertical and the 
              LDRBDConn above. 

*List of Subsheet I/O*

* Power +12 V 
    * Input from BPS Leader Board 
* Power GNDPWR 
    * Input from BPS Leader Board 
* FAN 1 
    * Input from BPS Leader Board 
* FAN 2 
    * Input from BPS Leader Board 
* FAN 3 
    * Input from BPS Leader Board 
* Fan 4 
    * Input from BPS Leader Board 

.. figure:: ../_static/FanBrdSch.png
    :align: center

    Fan Board Schematic

**Dimensions: 45.00mm by 34.50mm**

Requirements/Constraints:  
    We chose mosfet body diodes to be used as the transistor to prevent voltage spikes from entering 
    into the master board (since the fan speeds can change and that could cause the spikes).
    
.. figure:: ../_static/FanBrdLayout.png
    :align: center

    Fan Board Layout

.. figure:: ../_static/FanBrdRender.png
    :align: center

    Fan Board Render

Leader board
============

The Leader board uses the STM32F413 microcontroller. The board requires seven connectors:

* One 2-pin :ref:`power connector <power-connector>` (+12V, PWRGND)
* One 2-pin error light connector (+12V, PWRGND)
* One 4-pin contactor connector (+12V, PWRGND, aux1, aux2)
* One 4-pin Amperes board connector (+12V, PWRGND, IP, IM)
* One 4-pin CAN connector (isolated +5V, isolated GND, CAN high, CAN low)
* One 2-pin Minion connector (IP, IM)
* One 2x4-pin fan connector (4x +12V, 4x PWRGND)

The STM32F413 requires bypass capacitors and uses an 8MHz clock. To program the STM32F413, pins are connected to a 1x4 header to use the :abbr:`SWD (Serial Wire Debug)` programming protocol. 

The reset button allows you to avoid powering the BPS off to reset it.

.. note::
    You can configure the system to reset every time you use the Keil IDE to program it. 

For more information, see the `STM32F413 datasheet <https://www.st.com/resource/en/reference_manual/dm00305666-stm32f413-423-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf>`__.

.. figure:: ../_static/leader-board-diagram.png
    :align: center

    STM32F413 bypass capacitors 

The system's :abbr:`RTC (Real Time Clock)` is sourced from a low-power crystal oscillator. The crystal is used to set the :abbr:`PLL (Phase Lock Loop)`, which 
sets the system core frequency (CPU clock speed). The controller supports a range of frequencies, but the default is 8MHz. 

The :abbr:`MCU (Microcontroller Unit)` has an alternate internal clock that saves power consumption at the cost of precision. 
Since this system is safety critical, the external 8MHz crystal was added. 

.. note::
    The internal and external clocks were not tested against each other.

Capacitor values are set based on the crystal’s load capacitance and the capacitance of the whole board. 
The crystal and capacitor should be as physically close to the MCU as possible to avoid signal drift.

.. figure:: ../_static/crystal-capacitors.png
    :align: center

    STM32 crystal layout

.. _power-connector:

Power connector
^^^^^^^^^^^^^^^
The Leader board gets a 12V power supply. The voltage must be dropped to power the MCU and components. The total voltage required for all the components is +3.3V and +5V. 

An isolated DC-DC converter (RI3-1205S) converts the +12V input to +5V. The +12V input will be isolated from the +5V output. The Leader board
must use the correct grounds to maintain isolation. 

A switching regulator (NCP1117) converts the +5V input to +3.3V. This does not need isolation because the +5V input and corresponding grounds are already isolated from the +12V line.

.. figure:: ../_static/leader-board-power-connector.png
    :align: center

    Leader board power distribution

Fan Board: Manthan Upadhyaya
=============================

Purpose
    The purpose of this board is to set the speed of the fans that will be used to cool the battery 
    pack. It recieves signals from the Master Board and sends them across a MOSFET that switches the 
    fan on or off.

Blocks
    Mosfets - Each mosfet is between the power provided by the Master Board and power provided to the
    fans. When the PWM signal is set, it switches the fan on or off at a certain duty cycle.

Connections
    6 Connector: 4 of these connections are the PWM signals sent by the Master board. The other 2
    are power and ground.
    
    2 Connector(4): These are the power and grounds each fan is connected to.

Additional Considerations
    None
