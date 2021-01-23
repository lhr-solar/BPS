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

CAN Driver
===========

Purpose
    This driver is meant to send and recieve CAN messages. This allows us to talk with other systems
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