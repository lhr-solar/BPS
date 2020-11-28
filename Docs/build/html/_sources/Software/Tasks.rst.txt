********************************
Real Time Operating System Tasks
********************************

Fault State Task: Manthan Upadhyaya
===================================

The Fault State Task is called when a fault condition is set off in the BPS. 
These fault conditions can be found here(place link to fault conditions).

Process:
    1) All other tasks are not allowed to run unless they have are utilized sempahore. The other tasks will run until they post that semaphore and then they are prevented from running again. This occurs internally through the RTOS.

    2) The contactor is turned off.
    
    3) All the fans are set to maximum speed.
    
    4) The proper LED's are turned on and off. When the fault LED is turned on, the strobelight turns on.
    
    5) The fault condition is logged into the EEPROM.
    
    6) A message is sent along the CAN to the BPS display board to notify the driver that the BPS is tripped.
    
    7) The WatchDog timer is continually reset to prevent the BPS from going into fault again.

Amperes Task: Manthan Upadhyaya
===============================

The amperes mutex was added in this version of the BPS. It prevents multiple tasks from acessing the 
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
