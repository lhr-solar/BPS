***************************
Board Support Package
***************************

CAN BSP: Chase Block & Sijin Woo
================================
Purpose
    The purpose of this code is to send and recieve CAN messages. It also contains a queue for 
    messages that are recieved on the CAN bus that has a depth of 10. This queue cannot be accessed
    outside the file.

Usage
    Function usage is declared in the BSP_CAN.h file. When using these functions in the RTOS, pend
    and post must occur before and after the functions are called.

Additional Considerations
    None