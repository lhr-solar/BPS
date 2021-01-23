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

AS8510 Driver
=============

Purpose
    This driver is used to obtain data from the AS8510 current sensor chip.

Usage
    In order to use these functions, ``AS8510_Init()`` must be called first. The rest of the 
    functions are used to read and write to the registers in the chip. In order to read the current
    you must call ``AS8510_GetCurrent()`` which returns the value of the current as an ``int16_t``.

Additional Considerations
    Communication with this chip is done with SPI.

Battery Balancing Driver
========================

Purpose
    The Battery Balancing Driver is used to balance charging/discharging amongst all the modules in our system. Battery Balancing is a process that prevents thermal 
    runaway in the car's battery pack, and also allows the battery to maintain peak performance and health.


Usage
    ``Balancing_Balance`` is the only function from this module that you can use in other files. This function performs the complete
    battery balancing process - any modules that have a voltage that is greater than the lowest module voltage + a tolerance limit will 
    be set to discharge, and all other modules will remain charging.

Additional Considerations
    The functions in this module send instructions to the LTC6811 minions over SPI.
    
