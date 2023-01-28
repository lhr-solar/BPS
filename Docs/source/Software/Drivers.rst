*******
Drivers
*******

LTC2315 Driver
==============

Purpose
    This driver is used to obtain data from the LTC2315 current sensor chip.

Usage
    In order to use these functions, ``LTC2315_Init()`` must be called first. The rest of the  functions are used to read 
    and write to the registers in the chip. In order to read the current you must call ``LTC2315_GetCurrent()`` which returns 
    the value of the current as an ``int16_t``. ``LTC2315_Calibrate()`` should be run on startup because it 
    was observed that when the BPS is initially powered, there is a delay before the LTC2315 can be calibrated properly. 
    This is not observed on pressing the reset button.

Additional Considerations
    Communication with this chip is done with :term:`SPI <SPI>`. The scheduler is locked whenever communicating with this
    chip to prevent another task from interrupting communications with it.

Battery Balancing Driver
========================

Purpose
    The Battery Balancing Driver is used to balance charging/discharging amongst all the modules in our 
    system. Battery Balancing is a process that prevents thermal runaway in the car's battery pack, 
    and also allows the battery to maintain peak performance and health.

Usage
    ``Balancing_Balance()`` is the only function from this module that you can use in other files. 
    This function performs the complete battery balancing process - any modules that have a voltage 
    that is greater than the lowest module voltage + a tolerance limit will be set to discharge, and 
    all other modules will remain charging.

Additional Considerations
    The functions in this module send instructions to the LTC6811 minions over :term:`SPI <SPI>`.

CAN Driver
===========

Purpose
    This driver is meant to send and recieve :term:`CAN <CAN Bus>` messages. This allows us to talk with other systems
    in the car.

Usage
    The header file contains data types that are used to send CAN messages. Each ``CANMSG_t`` should 
    be sent with a ``CANID_t`` and ``CANPayload_t`` initialized in a struct. ``CANPayload_t`` should
    contain the data (``CANData_t``) and id of the value if it is in an array. If it isn't in an array
    ``idx`` should be 0. ``CANData_t`` should only have the value of the data type being sent 
    filled into the union. For example, if the data is a float, only fill in ``CANData.f`` and 
    nothing else. There are two types of sending and recieving functions one can use. One is 
    blocking and one is non-blocking. 
    
    If the non-blocking functions are used:
    
    ``CANbus_Send()`` will throw an error if the sending mailbox if full of messages.
    
    ``CANbus_Receive()`` will throw an error if there is not a message to be received.
    
    If the blocking functions are used:
    
    ``CANbus_WaitToReceive()`` will wait for a message to be received.
    
    ``CANbus_BlockAndSend()`` will wait for the mailbox to be available for the next message.
    
    All of these functions return an ErrorStatus data type if there was an error or not.

Additional Considerations
    Right now, this driver is optimized for the RTOS version of the BPS. It is not guaranteed that
    it would work in the Bare Metal version of our code.

M24128 Driver
=============

Purpose
    The :term:`EEPROM <EEPROM>` Driver is used to read and write the the external EEPROM on the leader 
    board. We are using the EEPROM to store the state of charge of the battery for coulomb counting purposes.
    by storing the state of charge at reset, we can know the state of charge when it starts up again.

Usage
    ``M24128_Init()`` must be called before any of the other EEPROM functions can be used. 
    ``M24128_Write()`` or ``M24128_Read()`` are just wrappers for the ``BSP_I2C`` functions. 

Additional Considerations
    The EEPROM is a non-volatile memory chip, so any previous data on the EEPROM will remain remain. 
    The EEPROM is rated for 4 million write cycles, so we should try use less write cycles than this 
    over the lifetime of the BPS. It takes around 5 milliseconds to write to the EEPROM, so multiple 
    writes should not be attempted within the same 5 ms time period, or some of the writes may fail.

LTC6811 Driver
==============

Purpose
    The LTC6811 Driver is a library of functions to be used by the uC that instruct the LTC6811 minions via :term:`SPI <SPI>`.

Usage
    ``LTC6811_Init()`` must be called before using any other functions from this library. This function 
    will set the Pend/Post functions for the ``spi_os`` global variable (this variable is of the 
    ``bsp_os_t`` type). If the user compiles this code with the ``BAREMETAL`` parameter, the Pend/Post 
    functions will do nothing. Otherwise, they will pend/post the ``MinionsASIC_Mutex``. This function 
    will also create the ``MinionsASIC_Mutex`` if it hasn't already been created by the time ``LTC6811_Init()``
    was called. 

    This library includes wrappers for all LTC6811 functions that return PEC (Packet Error Code) 
    values. If too many packets are invalid in a row, the BPS will trip. The wrappers are the functions 
    that end with ``_safe()``. The functions that have wrappers are:
    ``LTC6811_rdcv()``,
    ``LTC6811_rdaux()``,
    ``LTC6811_rdstat()``,
    ``LTC6811_rdcfg()``,
    ``LTC6811_rdpwm()``,
    and ``LTC6811_rdcomm()``.

    (Ex: wrapper for ``LTC6811_rdcv()`` is ``LTC6811_rdcv_safe()``)

Additional Considerations
    Most of this module is provided by Analog Devices, but the code that LHR Solar members have written is marked in the file.
