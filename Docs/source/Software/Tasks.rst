********************************
Real Time Operating System Tasks
********************************

Fault State Task: Manthan Upadhyaya
===================================

Purpose
    The Fault State Task is called when a fault condition is set off in the BPS. These fault 
conditions can be found here(place link to fault conditions). 

Functionality:
    1) All other tasks are not allowed to run unless they have are utilized sempahore. The other tasks will run (periodically according to the scheduler) until they post that semaphore and then they are prevented from running again. This occurs internally through the RTOS.

    2) The contactor is turned off.
    
    3) All the fans are set to maximum speed.
    
    4) The proper LED's are turned on and off. When the fault LED is turned on, the strobelight turns on.
    
    5) The fault condition is logged into the EEPROM.
    
    6) A message is sent along the CAN to the BPS display board to notify the driver that the BPS is tripped.
    
    7) The WatchDog timer is continually reset to prevent the BPS from going into fault again.

Priority
    This task has the second highest priority when the Init task is running. However, after the init
task destroys itself, it has the highest priority.

Shared Resources
    It uses the Fault_Sem4 which is used to block the task from running until something sets it. It 
uses the VoltageBuffer_Sem4, TemperatureBuffer_Sem4, and AmperesIO_Sem4 to log data into the 
EEPROM.

Timing Requirements
    None

Yields
    It will yield only if it tries to collect data with a semaphore already pending. This will only
be at the start of the task after it turns off the contactor.

Additional Considerations
    Although the BPS goes into fault state when the battery is in danger, it also goes into fault 
state when there is an issue with the RTOS. Since the BPS must always run during the race, care 
must be taken to minimize the chances of this happening. 

Amperes Task: Manthan Upadhyaya
===============================

Purpose
    Monitor the current and call the Fault state task if it is dangerously high.

Functionality
    1) First it checks the current and if it is safe, posts the SafetyCheck_Sem4. This only occurs once.

    2) If the current is above 75A, it sets the Fault_Sem4.

    3) After every updated measurement, it sends the current data to the Can queue.

Priority
    This task is the 5th priority, under the VoltTemp task.

Shared Resources
    It uses the Fault_Sem4, SafetyCheck_Sem4, AmperesData_Mutex(when collecting data from the 
current sensor), and AmperesIO_Sem4.

Timing Requirements
    .....

Yields
    Never yields

Additional Considerations
    None

Critical State Task: Manthan Upadhyaya
======================================

Purpose
The Critical State Task initializes the BPS when it first turns on.

Functionality:
    1) It waits for the VoltTemp and Amperes task to post the SafetyCheck semaphore 4 times. One for voltage, one for temperature, one for current, and one for open wire.
    
    2) If all of these checks are safe, the task will send the All Clear message and the Contactor On message across the CAN line.
    
    3) The task will then destroy itself since it is no longer needed

Priority
    It's priority 2, underneath the fault state task. 

Shared Resources
    All it uses is the SafetyCheck_Sem4.

Timing Requirements
    None

Yields
    After it initializes, it destroys itself.

Additional Considerations
    None