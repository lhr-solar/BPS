********************************
Real Time Operating System Tasks
********************************

Fault State Task: Manthan Upadhyaya
===================================

The Fault State Task is called when a fault condition is set off in the BPS. 
These fault conditions can be found here(place link to fault conditions).

Process:
    1) All other tasks are not allowed to run unless they have are utilized sempahore. The other tasks will run (periodically according to the scheduler) until they post that semaphore and then they are prevented from running again. This occurs internally through the RTOS.

    2) The contactor is turned off.
    
    3) All the fans are set to maximum speed.
    
    4) The proper LED's are turned on and off. When the fault LED is turned on, the strobelight turns on.
    
    5) The fault condition is logged into the EEPROM.
    
    6) A message is sent along the CAN to the BPS display board to notify the driver that the BPS is tripped.
    
    7) The WatchDog timer is continually reset to prevent the BPS from going into fault again.

Amperes Task: Manthan Upadhyaya
===============================

The amperes mutex was added in this version of the BPS. It prevents multiple tasks from accessing the 
global current variable at the same time for read and write operations. For example, if the LogInfo task
is reading the current at the same time as the Amperes Task is updating it, the LogInfo task could read the
wrong value.

Critical State Task: Manthan Upadhyaya
======================================

The Critical State Task initializes the BPS when it first turns on.

Process:
    1) It waits for the VoltTemp and Amperes task to post the SafetyCheck semaphore 4 times. One for voltage, one for temperature, one for current, and one for open wire.
    
    2) If all of these checks are safe, the task will send the All Clear message and the Contactor On message across the CAN line.
    
    3) The task will then destroy itself since it is no longer needed

CLI Task: Sugam Arora
=====================

Purpose
    The Command Line Interface prints metrics and information about the BPS depending on what command the user has entered, and is meant to be used to debug the BPS.

Functionality
    This task initializes the CLI and polls for a command to be sent via UART (through USB). Once the user has entered a command, it is handled and the appropriate information is displayed.

Priority
    The CLI task has priority level 9 because it is the least important task (besides the idle task).

Shared Resources
    This task may use the ``MinionsASIC_Mutex`` or the ``Voltage_Mutex`` depending on what command the user enters.

Timing Requirements
    None

Yields
    This task will wait until the user enters a command. This task will yield for the resources mentioned in the "Shared Resources" section for this task.

Additional Considerations
    For information on how to use the CLI and its list of valid commands, click on the CLI section in "Application" on this site.


Battery Balancing Task: Sugam Arora
===================================

Purpose
    The solar car is powered by lithium ion cells. If these cells are overcharged, they may experience thermal runaway. Running the battery with unbalanced cells may lead to its accelerated degredation. This task has been 
    implemented in an effort to maintain peak performance and health for the car's battery pack. If we did not have battery balancing and there was one module whith a high voltage, the other modules wouldn't get charged and
    the battery would have a lower state of charge overall.

Functionality
    This task sets any battery module with a voltage that is higher than the minimum voltage of all the modules + a charging tolerance 
    (subject to change) to discharge. Any modules that have a voltage that is equal to or less than the minimum voltage of all the 
    modules in the system will no longer discharge. 

Priority
    The battery balancing task has priority level 6, so it will not interrupt any monitoring tasks or any tasks that check if the BPS is running correctly.

Shared Resources
    This task will access the ``MinionsASIC_Mutex``, the ``WDog_Mutex``, and the ``Voltage_Mutex``.
Timing Requirements
    (To be determined)

Yields
    This task yields for the ``MinionsASIC_Mutex``, the ``Voltage_Mutex``, and the ``MinionsIO_Mutex``. 

Additional Considerations
    N/A

 
CAN Consumer Task: Sugam Arora
==============================

Purpose
    The BPS must send CAN messages to the rest of the system. 

Functionality
    This task gets the message that is next up from the CAN message queue and sends it on the CAN bus. 

Priority
    This task has priority level 7, so it will not interrupt any monitoring tasks or any tasks that check if the BPS is running correctly.

Shared Resources
    This task uses the ``CANBus_MsgQ`` queue.

Timing Requirements
    (To be determined)

Yields
    This task will yield until there is a message in the ``CANBus_MsgQ``. 

Additional Considerations
    For information about how the message payloads are structured can be found in the documentation for the CAN driver.
    
    All possible CAN messages that will be sent to the rest of the car's system by the BPS
    are listed on the `CAN Bus IDs spreadsheet <https://docs.google.com/spreadsheets/d/11YWoMVZw8BFr8kyO4DIz0g-aIU_vVa0d-WioSRq85TI/edit#gid=0>`_.