********************************
Real Time Operating System Tasks
********************************

Fault State Task: Manthan Upadhyaya
===================================

Purpose
    The Fault State Task is called when a fault condition is set off in the BPS. These fault 
    conditions can be found here(place link to fault conditions). 

Functionality:
    1) All other tasks are prevented from running. This occurs internally through the RTOS.

    2) The contactor is turned off.
    
    3) All the fans are set to maximum speed.
    
    4) The proper LED's are turned on and off. When the fault LED is turned on, the strobelight turns on.
    
    5) The fault condition is logged into the EEPROM.
    
    6) A message is sent along the CAN to the BPS display board to notify the driver that the BPS is tripped.
    
    7) The WatchDog timer is continually reset to prevent the BPS from going into fault again.

Priority
    This task has the second highest priority (1) when the Init task is running. However, after the 
    init task destroys itself, it has the highest priority. It will never be interrupted because
    we do not require any other monitoring to occur if we already know that a fault condition has 
    occured.

Shared Resources
    It uses the Fault_Sem4 which is used to block the task from running until something sets it.

Timing Requirements
    First thing that occurs is Contactor shuts off. So the amount of time for a context switch.

Yields
    It will yield when waiting for a fault. After that it will never yield.

Additional Considerations
    Although the BPS goes into fault state when the battery is in danger, it also goes into fault 
    state when there is an issue with the RTOS. Since the BPS must always run during the race, care 
    must be taken to minimize the chances of this happening. It also goes into a fault state when 
    the hard fault handler is called.

Amperes Task: Manthan Upadhyaya
===============================

Purpose
    Monitor the total current through the battery pack and call the Fault state task if it 
    is dangerously high.

Functionality
    1) First it checks the current and if it is safe, signals the critical state task. This only occurs once.

    2) If the current is above 75A, it signals the fault state task.

    3) After every updated measurement, it sends the current data to the CAN queue.

Priority
    This task is the 5th priority, under the VoltTemp task. This is due to professional advice the
    BPS team was given stating and the fact that the VoltTemp task monitors 2 potentially dangerous
    conditions while this task only monitors 1.

Shared Resources
    It uses the ``Fault_Sem4``, ``SafetyCheck_Sem4``, ``AmperesData_Mutex``(when collecting data from the 
    current sensor), and ``AmperesIO_Sem4``.

Timing Requirements
    TBD

Yields
    It yields when it signals the critical state task that the current is safe, when it detects a
    fault, when it tries to use a shared resource, and when it sends an SPI message.

Additional Considerations
    None

Critical State Task: Manthan Upadhyaya
======================================

Purpose
    The Critical State Task initializes the BPS when it first turns on.

Functionality:
    1) It waits for the VoltTemp and Amperes task to post the SafetyCheck semaphore 4 times. One for voltage, one for temperature, one for current, and one for open wire.
    
    2) If all of these checks are safe, the task will send the All Clear message and the Contactor On message across the CAN line. It will also turn the contactor on.
    
    3) The task will then destroy itself since it is no longer needed

Priority
    It's priority 2, underneath the fault state task. This is because if a fault occurs during the 
    critical state task, the fault task must be called.

Shared Resources
    All it uses is the ``SafetyCheck_Sem4``.

Timing Requirements
    Runs once at BPS startup.

Yields
    While initializing, it yields to other tasks to let them check their specific fault conditions.
    After initializing, it destroys itself and yields to the next highest priority task.

Additional Considerations
    None