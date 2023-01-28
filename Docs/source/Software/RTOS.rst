**************
BPS RTOS Layer
**************

RTOS
====
The BPS RTOS layer serves as a wrapper between the core RTOS and the rest of the BPS system. 
It is an abstraction layer between the RTOS and the Task and Apps layers of the BPS code.

When migrating RTOSes, these functions will be modified. The parameters likely will not change much,
but the inner function calls to the RTOS will change, and its parameters will change as well. Please
reference the respective RTOS documents when migrating.

BPS RTOS Structure
==================

Semaphore Pend - ``RTOS_BPS_SemPend(BPS_OS_SEM* sem, BPS_OS_TICK tick, BPS_OS_OPT opt)``
    The ``SemPend`` function is a wrapper for pending a semaphore, which returns either the current counter
    of the specified semaphore, or 0 if the semaphore is unavailable. 

    Pending the semaphore decrements its count.

Semaphore Post - ``RTOS_BPS_SemPost(BPS_OS_SEM *sem4, BPS_OS_OPT opt)``
    The ``SemPost`` function is a wrapper for posting a semaphore, which returns either the current counter
    of the specified semaphore, or 0 if the semaphore is unavailable.

    Posting the semaphore increments its count.

Mutex Create - ``RTOS_BPS_MutexCreate(BPS_OS_MUTEX *mut, char* name)``
    The ``RTOS_BPS_MutexCreate`` function initializes the specified mutex object.
    A `mutex <https://en.wikipedia.org/wiki/Mutual_exclusion>`_ is a mutually exclusive lock on some resource.
    Mutexes utilize their respective ``MutexPend`` and ``MutexPost`` functions to await a resource being available,
    or take control of a resource in the program, ensuring that multiple threads don't write to a value at the same time.

Mutex Pend - ``RTOS_BPS_MutexPend(BPS_OS_MUTEX* mutex, BPS_OS_OPT opt)``
    The ``RTOS_BPS_MutexPend`` function pends a specified mutex object. Pending a mutex
    takes control of the mutex, and allows you to write to the resource as needed. When done with the 
    resource, a call to ``MutexPost`` should be made, informing the RTOS that you are releasing the
    lock on the mutex, marking it as available again.

Mutex Post - ``RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options)``
    The ``RTOS_BPS_MutexPost`` function posts a specified mutex object. Posting a mutex releases
    the mutex lock on the resource, marking it as available to the RTOS. Once you relinquish this mutex,
    you SHOULD NOT write to its associated resource until you successfully ``Pend`` the resource again.

Task Create - ``RTOS_BPS_TaskCreate(BPS_OS_TCB *p_tcb, char *p_name, void *p_task, void *p_arg, uint8_t prio, BPS_CPU_STK *p_stk_base, uint64_t stk_size)``
    The ``RTOS_BPS_TaskCreate`` function, as the name implies, creates a Task for the RTOS. 
    `**RTOS Tasks** <https://www.digikey.com/en/maker/projects/introduction-to-rtos-solution-to-part-3-task-scheduling/8fbb9e0b0eed4279a2dd698f02ce125f>`_  are a powerful feature of using the RTOS.
    Tasks are executed in sequential order depending on their priorities; if tasks have the same priority, then 
    they are executed in a round-robin fashion. Tasks allow us to run specific code at specific times or intervals
    on the RTOS. Tasks can (and should!) make use of other RTOS functions to ensure safe resource access.

