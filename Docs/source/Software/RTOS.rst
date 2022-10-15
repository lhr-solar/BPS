**********************
BPS RTOS Wrapper Layer
**********************

RTOS
====
Currently, the BPS wrapper code is based around the `Micrium uC/OS-III RTOS <https://docs.silabs.com/micrium/latest/micrium-general-concepts/>`_
RTOS. We plan to migrate over to `FreeRTOS <https://www.freertos.org/>`_.

Wrapper Structure
=================

Semaphore Pend - ``RTOS_BPS_SemPend(BPS_OS_SEM* sem, BPS_OS_TICK tick, BPS_OS_OPT opt)``
    ``RTOS_BPS_SemPend()`` pends a semaphore, and requires three parameters:

    1) A pointer to a BPS_Semaphore object, ``BPS_OS_SEM* sem``. 
    2) A time to wait on the pend, ``BPS_OS_TICK tick``.
    3) A pend option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_SemPend()`` returns the current semaphore count, or 0 if unavailable.

Semaphore Post - ``RTOS_BPS_SemPost(BPS_OS_SEM *sem4, BPS_OS_OPT opt)``
    ``RTOS_BPS_SemPost()`` posts a semaphore, and requires two parameters:

    1) A pointer to a BPS_Semaphore object, ``BPS_OS_SEM* sem4``.
    2) A post option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_SemPost()`` returns the current semaphore count, or 0 upon error.

Mutex Create - ``RTOS_BPS_MutexCreate(BPS_OS_MUTEX *mut, char* name)``
    ``RTOS_BPS_MutexCreate()`` initializes a mutex object, and requires two parameters:

    1) A pointer to a BPS_Mutex object, ``BPS_OS_Mutex* mutex``.
    2) A string for the name of the mutex, ``char* name``.

    ``RTOS_BPS_MutexCreate()`` returns nothing.

Mutex Pend - ``RTOS_BPS_MutexPend(BPS_OS_MUTEX* mutex, BPS_OS_TICK timeout, BPS_OS_OPT opt)``
    ``RTOS_BPS_MutexPend()`` pends a mutex, and requires three parameters:

    1) A pointer to a BPS_Mutex object, ``BPS_OS_Mutex* mutex``.
    2) A time to wait on the pend, ``BPS_OS_TICK timeout``.
    3) A pend option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_MutexPend()`` returns nothing.

Mutex Post - ``RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options)``
    ``RTOS_BPS_MutexPost()`` posts a mutex, and requires two parameters:

    1) A pointer to a BPS_Mutex object, ``BPS_OS_Mutex* mutex``.
    2) A pend option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_MutexPost()`` returns nothing.

Task Create - ``RTOS_BPS_TaskCreate(BPS_OS_TCB* p_tcb, char* p_name, void* p_task, void* p_arg, uint8_t prio, BPS_CPU_STK* p_stk_base, uint64_t stk_size)``
    ``RTOS_BPS_TaskCreate()`` creates an RTOS task, and has several parameters. This function will be refactored upon moving to FreeRTOS.

    1) A pointer to a TCB for the task, ``BPS_OS_TCB* p_tcb``.
    2) A pointer to the name of the task, ``char* p_name``.
    3) A pointer to the function to call when the task executes, ``void* p_task``.
    4) A pointer to the arguments of the task function, ``void* p_arg``.
    5) Priority level of the task, ``uint8_t prio``.
    6) A pointer to the stack base, ``BPS_CPU_STK* p_stk_base``.
    7) The stack size, ``uint64_t stk_size``.

    ``RTOS_BPS_TaskCreate()`` returns nothing.