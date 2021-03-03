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
    The purpose of this board is to display messages from the :term:`CAN <CAN Bus>` interface on 
    the E-Ink display for debugging.

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

**Main**
++++++++

*What does this circuit do?*
    This circuit makes connections from the microcontroller to the E-Ink Display and logic 
    analyzer pins.  
*Why do we need it?*
    We need this circuit in order for debugging (:term:`CAN <CAN Bus>` messages on the E-Ink 
    display and logic analyzer pins).  
*List of Circuit Components*
    * STM32F413RHTx 
        * Description: Microcontroller 
        * Justification for selection of specific part: Standard component  
        * `Datasheet link <https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwiqg4WM6NXuAhXULc0KHcHbAzIQFjABegQIAhAC&url=https%3A%2F%2Fwww.st.com%2Fresource%2Fen%2Fdatasheet%2Fstm32f413rg.pdf&usg=AOvVaw3U9MV6EuhEcmJpsEYCrU52>`__
        * Associated passives/components:
            * Bypass capacitors 
            * 8 MHz External Crystal Oscillator: used to set the Phase Lock Loop (PLL) and the system core frequency (CPU processor speed) 
                * Using an external clock ensures for precision, which is important in a safety-critical system like the BPS.  
            * Reset button
            * :term:`SWD <SWD>` Programming Pins
    * Logic Analyzer Pins (2.54 mm headers) 
        * **Description:** The logic analyzer pins serve to easily test the board to check proper 
          functionality of the board.  
        * **Why is it necessary:** This allows for testing to be much easier since we can monitor 
          what occurs to these signal lines through these pins  
        * Justification for selection of specific part: Standard component 
        * Associated passives/components:  
            * SPIO Analyzer Pins 1x5: PA5, PA6, PA7, PA8, GND 
            * GEN Analyzer Pins 1x4: PA9, PA10, PA11, GND 
            * USART Analyzer Pins 1x3: PC6, PC7, GND 
            * CAN Analyzer Pins 1x3: PB12, PB13, GND 
    * E-Ink Display: 
        * **Description:** Display screen. 
        * **Why is it necessary:** CAN messages will be displayed on this screen.  
        * Justification for selection of specific part: This display doesn't consume much power. 
        * Associated passives/components: 
            * 2x20 Connector 
    * 2x20 Connector (2.54mm) 
        * **Description:** Connects the STM to the E-Ink display. 
        * **Why is it necessary:** Allows the E-Ink display to show CAN messages. 
        * Justification for selection of specific part: Standard component 
        * Associated passives/components: 
            * Buttons: PB0, PB1, PB2, PB3 
            * SPIO: PA5, PA6, PA7, PA8 
            * GEN: PA9, PA10, PA11 
    * Reset button 
        * **Description:** Resets the STM microcontroller. 
        * **Why is it necessary:** Used in case the board needs to be reset. 
        * Justification for selection of specific part: Standard component 

*List of Subsheet I/O*

* Power +5V: 
    * Input from power distribution subsheet 
* Power +3.3V: 
    * Input from power distribution subsheet 
* CAN TX: 
    * Input from CAN subsheet 
* CAN RX: 
    * Input from CAN subsheet 

.. figure:: ../_static/DisplaySch.png
    :align: center

    Display Board Subsheet Schematic

**CAN**
+++++++

*What does this circuit do?* 
    This circuit sets up the CAN interface between microcontrollers.
*Why do we need it?*
    We need this circuit in order for debugging messages to be transmitted between microcontrollers.  
*List of Circuit Components*
    * ADM3055E: 
        * Description: This is an isolated CAN physical layer transceiver with integrated isolated 
          DC to DC converters.  
        * Why is it necessary: It provides isolation between the CAN controller and the main bus.  
        * Justification for selection of specific part: Standard component 
        * `Datasheet link <https://www.analog.com/media/en/technical-documentation/data-sheets/ADM3055E-3057E.pdf>`__ 
        * Associated passives/components: 
            * Bypass capacitors 
    * CDSOT23-T24CAN: 
        * Description: This component provides ESD and surge protection for CAN transceivers.  
        * Why is it necessary: CAN bus protection. 
        * Justification for selection of specific part: Standard component 
        * `Datasheet link <https://www.bourns.com/docs/Product-Datasheets/CDSOT23-T24CAN.pdf>`__  
        * Associated passives/components: 
            * External resistors 

*List of Subsheet I/O*

* Power +5V: 
    * Input from power distribution subsheet 
* Power +3.3V: 
    * Input from power distribution subsheet 

.. figure:: ../_static/DisplayCANSch.png
    :align: center

    Display Board CAN Subsheet Schematic

**Power Distribution**
++++++++++++++++++++++

*What does this circuit do?*
    This circuit takes the +12V power line from the car power connector and takes it down to +5V 
    and +3.3V.  
*Why do we need it?*
    We need this circuit in order to have stable +5V and +3.3V power lines for the rest of the board.  
*List of Circuit Components*
    * PDS1-S12-S5: 
        * Description: DC-DC converter. 
        * Why is it necessary: Converts DC +12V to DC +5V.  
        * Justification for selection of specific part: Standard component 
        * `Datasheet link <https://www.mouser.com/datasheet/2/670/pds1_m-1311700.pdf>`__  
        * Associated passives/components: 
            * Bypass capacitors 
            * Inductor 
    * NCP1117: 
        * Description: Low-dropout voltage regulator. 
        * Why is it necessary: Uses +5V to produce an output voltage of +3V.  
        * Justification for selection of specific part: Standard component 
        * `Datasheet link <https://www.onsemi.com/pub/Collateral/NCP1117-D.PDF>`__
        * Associated passives/components: 
            * Bypass capacitors 

*List of Subsheet I/O*

    * Power +12V: 
        * Input from car power connector 
    * Power +5V: 
        * Output from PDS1-S12-S5 
    * Power +3.3V: 
        * Output from NCP1117 

.. figure:: ../_static/DisplayPwrSch.png
    :align: center

    Display Board Power Distribution Subsheet Schematic

Layout 
^^^^^^
**Dimensions: 56mm x 91.46 mm**

Requirements/Constraints:  
    * The e-Ink display acts as a shield and lays on the board, so components with a tall height 
      can’t be placed by the display 
    * The LEDs must be visible and not covered by the e-Ink display 
Design Choices:
    * The parts are organized in sections, with the power distribution system in the bottom left 
      and the CAN connections on the center-right side of the board.  
    * All the logic analyzer pin headers are now separate. They were formerly all in one bigger 
      pin header, but have now been split apart to make traces shorter and neater.  
    * The CAN connectors and car power connectors were placed on the south end of the board so 
      that the wires connecting to them could go through the arm mount.  

.. figure:: ../_static/DisplayLayout.png
    :align: center

    Display Board Layout

.. figure:: ../_static/DisplayRenderTop.png
    :align: center

    Display Board Render Top

.. figure:: ../_static/DisplayRenderBottom.png
    :align: center

    Display Board Render Bottom

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
