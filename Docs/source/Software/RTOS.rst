******************************
BPS Real Time Operating System
******************************

RTOS
====
Currently, the BPS wrapper code is based around the `Micrium uC/OS-III RTOS <https://docs.silabs.com/micrium/latest/micrium-general-concepts/>`_
RTOS. We plan to migrate over to `FreeRTOS <https://www.freertos.org/>`_.

Wrapper Structure
=================

Semaphore Pend
    ``RTOS_BPS_SemPend()`` pends a semaphore, and requires three parameters:
    1. A pointer to a BPS_Semaphore object, ``BPS_OS_SEM* sem``. 
    2. A time to wait on the pend, ``BPS_OS_TICK tick``.
    3. A pend option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_SemPend()`` returns the current semaphore count, or 0 if unavailable.

Semaphore Post
    ``RTOS_BPS_SemPost()`` posts a semaphore, and requires two parameters:
    1. A pointer to a BPS_Semaphore object, ``BPS_OS_SEM* sem4``.
    2. A post option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_SemPost()`` returns the current semaphore count, or 0 upon error.

Mutex Create
    ``RTOS_BPS_MutexCreate()`` initializes a mutex object, and requires two parameters:
    1. A pointer to a BPS_Mutex object, ``BPS_OS_Mutex* mutex``.
    2. A string for the name of the mutex, in the form of ``char*``.

    ``RTOS_BPS_MutexCreate()`` returns nothing.

Mutex Pend
    ``RTOS_BPS_MutexPend()`` pends a mutex, and requires three parameters:
    1. A pointer to a BPS_Mutex object, ``BPS_OS_Mutex* mutex``.
    2. A time to wait on the pend, ``BPS_OS_TICK timeout``.
    3. A pend option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_MutexPend()`` returns nothing.

Mutex Post
    ``RTOS_BPS_MutexPost()`` posts a mutex, and requires two parameters:
    1. A pointer to a BPS_Mutex object, ``BPS_OS_Mutex* mutex``.
    3. A pend option, ``BPS_OS_OPT opt``.

    ``RTOS_BPS_MutexPost()`` returns nothing.