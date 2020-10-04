Boards and sensors
==================

There are two temperature sensors for each battery module with a module minion board for each of the four rows.
Each board measures up to 12 battery modules and 16 temperature sensors, but is configured for eight modules and 
16 sensors by default.

The Amperes minion board is connected to the main power cable that powers the electrical loop using the :term:`Hall effect sensor`. 

Leader board
------------

The Leader board uses the STM32F413 microcontroller. The board requires seven connectors:

* One 2-pin power connector (+12V, PWRGND)
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