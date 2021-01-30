********************************
Real Time Operating System Tasks
********************************

Fault State Task: Manthan Upadhyaya
===================================

Purpose
    The Fault State Task is called when a fault condition is set off in the BPS. These fault 
    conditions can be found here(place link to fault conditions). 

Functionality:
    1) All other tasks are prevented from running. This is because this is the highest priority task.

    2) The contactor is turned off.
    
    3) All the fans are set to maximum speed.
    
    4) The proper LED's are turned on and off. When the fault LED is turned on, the strobelight turns on.
    
    5) The fault condition is logged into the EEPROM.
    
    6) A message is sent along the CAN bus to the BPS display board to notify the driver that the BPS is tripped.
    
    7) The WatchDog timer is continually reset to prevent the BPS from going into fault again.

Priority
    This task has the second highest priority (1) when the Init task is running. However, after the 
    init task destroys itself, it has the highest priority. It will never be interrupted because
    we do not require any other monitoring to occur if we already know that a fault condition has 
    occured.

Shared Resources
    It uses the ``Fault_Sem4`` which is used to block the task from running until something sets it.

Timing Requirements
    The contactor must be shut off as soon as possible after a fault is detected.

Yields
    It will yield when waiting for a fault. After a fault is detected, it will never yield.

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
    This task is the 5th priority under the VoltTemp task. This is due to professional advice the
    BPS team was given. Also the VoltTemp task monitors 2 potentially dangerous conditions while 
    this task only monitors 1.

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
    None

Yields
    While initializing, it yields to other tasks to let them check their specific fault conditions.
    After initializing, it destroys itself and yields to the next highest priority task.

Additional Considerations
    None

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
    For information on how to use the CLI and its list of valid commands, click on :ref:`CLI section<CLI-app>`.


Battery Balancing Task: Sugam Arora
===================================

Purpose
    The solar car is powered by lithium ion cells. If these cells are overcharged, they may experience thermal runaway. Running the battery with unbalanced cells may lead to its accelerated degredation. This task has been 
    implemented in an effort to maintain peak performance and health for the car's battery pack. If we did not have battery balancing and there was one module with a high voltage, the other modules wouldn't get charged and
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
    None
 
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

Idle Task
=========

Purpose
    The scheduler always needs to have an available task to run. The purpose of this task is for it to run whenever the scheduler cannot schedule anything else.

Functionality
    The idle task runs an empty infinite loop for as long as it is scheduled to run.

Priority
    The idle task has the lowest priority in the system (10), so it will not run unless all other tasks are blocked.

Shared Resources
    The idle task does not use any shared resources.

Timing Requirements
    The idle task does not have any timing requirements.

Yields
    The idle task never yields.

Additional Considerations
    When modifying the idle task, it is important to not introduce any functionality that may affect other tasks. For example, the idle task should not pend 
    any mutexes, since this could block more important tasks from running.

Voltage Temperature Monitor Task: Sijin Woo
==============================

Purpose
    The BPS must make sure that the battery pack's voltage, temperature, and open wires have safe values in order to protect the car and the driver. 
    If any battery module has a temperature between 45 and 60 degrees Celsius, the car can continue running safely but it should not be charged.
    

Functionality
    This task will check all voltage, temperature, and open wire values and sends voltage and temperature values on the CAN bus.
    This task also sends a suggestion to not charge the battery when any module has a temperature between 45 and 60 degrees Celsius.

Priority
    This task has priority level 4, so it will not interrupt the fault state, critical state, and watchdog tasks.

Shared Resources
    This task uses the ``CANBus_MsgQ`` queue, the ``Fault_Sem4``, and the ``SafetyCheck_Sem4``.
    
    This task also pends the ``WDog_Mutex``.

Timing Requirements
    (To be determined)

Yields
    Since this task checks all voltage and temperature values, it will wait for the ``Voltage_Mutex`` and the ``TemperatureBuffer_Mutex``
    to be available. 

Additional Considerations
    None
