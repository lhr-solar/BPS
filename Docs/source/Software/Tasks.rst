********************************
Real Time Operating System Tasks
********************************

Amperes Task: Manthan Upadhyaya
===============================

Purpose
    Monitor the total current through the battery pack and call the Fault state task if it 
    is dangerously high.

Functionality
    1) First it checks the current and if it is safe, signals the check contactor task. This only occurs once.

    2) If the current is above 75A, it calls the fault state function.

    3) After every updated measurement, it sends the current data to the CAN queue and updates the BPS's State of Charge.

Priority
    This task is the 5th priority under the VoltTemp task. This is due to professional advice the
    BPS team was given. Also the VoltTemp task monitors 2 potentially dangerous conditions while 
    this task only monitors 1.

Shared Resources
    It uses the ``Fault_Sem4``, ``SafetyCheck_Sem4``, ``AmperesData_Mutex``(when collecting data from the 
    current sensor), and ``AmperesIO_Sem4``.

Timing Requirements
    According to Dr. Judy Jeevarajan, current through the Lithium-ion battery pack needs to be monitored at least once every 
    couple minutes to maintain safety and accuracy. For scrutineering and testing purposes, this task is monitored more 
    frequently at once every 100 milliseconds. 

Yields
    It yields when it signals the check contactor task that the current is safe, when it detects a
    fault, when it tries to use a shared resource, and when it sends an :term:`SPI <SPI>` message.

Additional Considerations
    None

Battery Balancing Task: Sugam Arora
===================================

Purpose
    The solar car is powered by lithium ion cells. If these cells are overcharged, they may experience 
    thermal runaway. Running the battery with unbalanced cells may lead to its accelerated degredation. 
    This task has been implemented in an effort to maintain peak performance and health for the car's 
    battery pack. If we did not have battery balancing and there was one module with a high voltage, 
    the other modules wouldn't get charged and the battery would have a lower state of charge overall.

Functionality
    This task sets any battery module with a voltage that is higher than the minimum voltage of all 
    the modules + a charging tolerance (subject to change) to discharge. Any modules that have a 
    voltage that is equal to or less than the minimum voltage of all the modules in the system will no longer discharge. 

Priority
    The battery balancing task has priority level 6, so it will not interrupt any monitoring tasks 
    or any tasks that check if the BPS is running correctly.

Shared Resources
    This task will access the ``MinionsASIC_Mutex``, the ``WDog_Mutex``, and the ``Voltage_Mutex``.

Timing Requirements
    According to Dr. Judy Jeevarajan, Lithium-ion batteries need to be monitored at least once every couple minutes to 
    maintain safety and accuracy. For scrutineering and testing purposes, this task is monitored more frequently at once 
    every 100 milliseconds. 

Yields
    This task yields for the ``MinionsASIC_Mutex``, the ``Voltage_Mutex``, and the ``MinionsIO_Mutex``. 

Additional Considerations
    None

CAN Consumer Task: Sugam Arora
==============================

Purpose
    The BPS must send :term:`CAN <CAN Bus>` messages to the rest of the system. 

Functionality
    This task gets the message that is next up from the CAN message queue and sends it on the CAN bus. 

Priority
    This task has priority level 7, so it will not interrupt any monitoring tasks or any tasks that 
    check if the BPS is running correctly.

Shared Resources
    This task uses the ``CANBus_MsgQ`` queue.

Timing Requirements
    This task is not safety critical and no safety critical tasks depend on this task, so there is no set timing requirement. 
    This task runs whenever there is a message in the CAN message queue as long as no higher priority tasks are running at 
    the time. 

Yields
    This task will yield until there is a message in the ``CANBus_MsgQ``. 

Additional Considerations
    For information about how the message payloads are structured can be found in the documentation for the CAN driver.
    
    All possible CAN messages that will be sent to the rest of the car's system by the BPS
    are listed on the `CAN Bus IDs spreadsheet 
    <https://docs.google.com/spreadsheets/d/11YWoMVZw8BFr8kyO4DIz0g-aIU_vVa0d-WioSRq85TI/edit#gid=0>`_.

CLI Task: Sugam Arora
=====================

Purpose
    The Command Line Interface prints metrics and information about the BPS depending on what command 
    the user has entered, and is meant to be used to debug the BPS.

Functionality
    This task initializes the CLI and polls for a command to be sent via UART (through USB). Once the 
    user has entered a command, it is handled and the appropriate information is displayed.

Priority
    The CLI task has priority level 9 because it is the least important task (besides the idle task).

Shared Resources
    This task may use the ``MinionsASIC_Mutex`` or the ``Voltage_Mutex`` depending on what command the user enters.

Timing Requirements
    None

Yields
    This task will wait until the user enters a command. This task will yield for the resources mentioned 
    in the "Shared Resources" section for this task.

Additional Considerations
    For information on how to use the CLI and its list of valid commands, click on :ref:`CLI section<CLI-app>`.

Check Contactor Task: Manthan Upadhyaya
=======================================

Purpose
    The Check Contactor Task initializes the BPS when it first turns on.

Functionality:
    1) It waits for the Check Contactor, VoltTemp and Amperes task to post the SafetyCheck :term:`semaphore <Semaphore>` 5 times. One for voltage, one for temperature, one for current, one for open wire, and one for welded contactors.
    2) It creates the Pet Watchdog Task after the checks are completed (since the checks take longer than the watchdog timer).
    3) If all of these checks are safe, the task will send the All Clear message and the Contactor On message across the CAN line. It will also turn the contactor on.
    4) Finally, the task enters a while-loop where it triggers a fault if the contactor is open.

Priority
    It's priority 2, underneath the fault state task. This is because if a fault occurs during the 
    check contactor task, the fault task must be called.

Shared Resources
    All it uses is the ``SafetyCheck_Sem4``.

Timing Requirements
    None

Yields
    While initializing, it yields to other tasks to let them check their specific fault conditions.
    Faults if it detects a welded contactor at init or an open contactor during operation.

Additional Considerations
    None

Idle Task
=========

Purpose
    The scheduler always needs to have an available task to run. The purpose of this task is for it 
    to run whenever the scheduler cannot schedule anything else.

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
    When modifying the idle task, it is important to not introduce any functionality that may affect 
    other tasks. For example, the idle task should not pend any mutexes, since this could block more important tasks from 
    running.

Initialization Task: Alex Koo
===============================

Purpose
    Create tasks by calling ``OSTaskCreate()`` and provide argument specifying to RTOS how the task will be managed.
    The order of the semaphore Initialization matters because if the fault semaphore isn't initialized first and a fault is 
    called we do not go into the fault state. Initialization Task also creates :term:`Semaphores <Semaphore>` which are used 
    when a task wants exclusive access to a resource, needs to synchronize its activities with an ISR or a task, or is 
    waiting until an event occurs.

Functionality:
    1) The TCB, Task name, Task function argument, Priority, Stack, Watermark limit for debugging, Stack size, Queue size, Time quanta, 
    2) Extension pointer, Options, Return err code is provided
    
    3) Defines the priority level for the tasks
    
    4) Specifies the size of the task's stack in bytes

Priority
    The initialization has the highest priority. 

Timing Requirements
    The task deletes itself after running.

Yields
   There is no yield.

Log Info Task
=============

Purpose
   This task logs the state of charge into the EEPROM every 3 seconds.

Functionality
   The log info task runs an infinite loop. Inside the loop, it sends percentage of charge left in 
   the battery pack to the EEPROM using ``EEPROM_LogData()`` and is then delayed by ``OSTimeDly()`` 
   every 3 seconds. 

Priority
   This task has priority 8, so it will not interrupt any monitoring tasks or any tasks that check 
   if the BPS is running correctly. It will also have a lower priority than sending :term:`CAN <CAN Bus>` messages.

Shared Resources
   The log info task uses battery state of charge data and the :term:`EEPROM <EEPROM>`, which is also 
   shared by the Fault State Task and CLI. 

Timing Requirements
   There is a time requirement of logging into the EEPROM every 3 seconds. Writing too often to the 
   EEPROM will exceed the EEPROM's limited (4 million) erase/write cycles, which causes the EEPROM 
   to malfunction. Thus, writing every 3 seconds will update the EEPROM accurately enough and stay 
   within the EEPROM's erase/write cycles.   
 
Yields
   The log info task yields when ``OSTimeDly()`` is called and when the EEPROM is initialized and written to. 

Additional Considerations
   None.

Pet WatchDog Task: Harshitha Gorla & Clark Poon
===============================================

Purpose
    The purpose of this task is to recognize if the BPS RTOS has stalled. If it is stuck somewhere
    in the code, the car must shut down.

Functionality
    This task checks the ``WDog_BitMap`` variable to see if the 3 LSB are set by the temperature,
    voltage, balancing, and current tasks. If these tasks ran and set those bits, that means that the 
    BPS is functional and the timer is reset. If those bits are not set, the timer will keep running
    and a reset the BPS if it reaches 0. The task can be called multiple times before
    the timer resets.

Priority
    This task is priority 3. It is above the voltage, temperature, balancing, and current monitoring tasks
    because if they run before the WatchDog timer is reset, the BPS will fault even if it is 
    working correctly. We also do not want this task to be blocked by other periodic threads.

Shared Resources
    The ``WDog_Mutex`` is read by this task and written to by the VoltTemp, Amperes, and
    BatteryBalancing tasks.

Timing Requirements
    This task is set to run every 400 milliseconds.

Yields
    It doesn't yield.

Additional Considerations
    If we add more tasks (or split up tasks such as voltage and temperature) and want to have the 
    watchdog timer look over them, we can add more bits to the timer and just check if they are set. This task
    is created in the Check Contactor Task because it takes longer than the watchdog time to intialize all the tasks.

Voltage Temperature Monitor Task: Sijin Woo
===========================================

Purpose
    The BPS must make sure that the battery pack's voltage, temperature, and open wires have safe 
    values in order to protect the car and the driver. If any battery module has a temperature between 
    45 and 60 degrees Celsius, the car can continue running safely but it should not be charged.
    

Functionality
    This task will check all voltage, temperature, and open wire values and sends voltage and 
    temperature values on the :term:`CAN <CAN Bus>` bus. This task also sends a suggestion to not 
    charge the battery when any module has a temperature between 45 and 60 degrees Celsius.
    
    If the state of the open wires or the battery pack's voltage/temperature is unsafe, then the 
    fault state task will be signaled. As each of the three (open wires, battery voltage, and battery 
    temperature) are deemed safe, this task signals to turn the contactor on once.

Priority
    This task has priority level 4, so it will not interrupt the fault state, check contactor, and watchdog tasks.

Shared Resources
    This task uses the ``CANBus_MsgQ`` queue, the ``Fault_Sem4``, and the ``SafetyCheck_Sem4``. 
    
    This task also pends the ``WDog_Mutex`` and the ``MinionsASIC_Mutex``. Measurement data is sent 
    on the ``SPI1`` port (this port is also used by the Battery Balancing Task).

Timing Requirements
    According to Dr. Judy Jeevarajan, voltage and tempterature of Lithium-ion batteries need to be monitored at least once 
    every couple minutes to maintain safety and accuracy. For scrutineering and testing purposes, this task is updated more 
    frequently at once every 100 milliseconds.

Yields
    Since this task checks all voltage and temperature values, it will wait for the ``Voltage_Mutex`` and the ``TemperatureBuffer_Mutex``
    to be available. 
    
    This task will also yield whenever it sends SPI messages to the LTC6811 minions.

Additional Considerations
    As of Fall 2021, the open wire functionality of this task has been removed since it takes too much time, and the normal voltage checks 
    usually catch open wires.
    
