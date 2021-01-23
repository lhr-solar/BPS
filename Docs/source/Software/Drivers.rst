*******
Drivers
*******

EEPROM Driver
=============

Purpose
    The EEPROM Driver is used to read and write the the external EEPROM on the leader board. We are using the EEPROM to store the state of charge of the battery
    and the cause of any faults that cause the BPS to trip. We are using an EEPROM to store this information so it will survive a BPS reset.

Usage
    ``EEPROM_Init()`` must be called before any of the other EEPROM functions can be used. ``EEPROM_Load()`` or ``EEPROM_Reset()`` must be called before 
    reading or writing data to the EEPROM. 

Additional Considerations
    The EEPROM is a non-volatile memory chip, so any previous data on the EEPROM will remain unless ``EEPROM_Reset()`` is called. The EEPROM is rated for
    4 million write cycles, so we should try use less write cycles than this over the lifetime of the BPS. It takes around 5 milliseconds to write to the 
    EEPROM, so multiple writes should not be attempted within the same 5 ms time period, or some of the writes may fail.

LTC6811 Driver
==============

Purpose
    The LTC6811 Driver is a library of functions to be used by the uC that instruct the LTC6811 minions via SPI.

Usage
    ``LTC6811_Init()`` must be called before using any other functions from this library. This function will set the Pend/Post functions for the ``spi_os`` global
    variable (this variable is of the ``bsp_os_t`` type). If the user compiles this code with the ``BAREMETAL`` parameter, the Pend/Post functions will do nothing. 
    Otherwise, they will pend/post the ``MinionsASIC_Mutex``. This function will also create the ``MinionsASIC_Mutex`` if it hasn't already been created by the time ``LTC6811_Init()``
    was called. 

Additional Considerations
    Most of this module is provided by Analog Devices, but the code that LHR Solar members have written is marked in the file.
