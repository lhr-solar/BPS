/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/* Interrupt_Priorities.h
 * File to define Preemptive and Sub Interrupt Priorities. Belongs in the BSP layer as all references to these values are within BSP files.
 * Preemption Priority - allows an ISR to be preempted (interrupted) by another interrupt of higher priority.
 * Subpriority - determines which of two, equal priority interrupts to start with
 * When updating subpriorities, make sure there are no alternative interrupts with equal priorities that need to be run in a certain order
 */

#ifndef INTERRUPT_PRIORITIES_H
#define INTERRUPT_PRIORITIES_H

#define USART2_PREEMPT_PRIO    2
#define USART2_USB_PRIO        1
#define USART3_PREEMPT_PRIO    2
#define USART3_SUB_PRIO        0

#define CAN1_RX0_PREEMPT_PRIO  1 
#define CAN1_RX0_SUB_PRIO      1 
#define CAN1_TX_PREEMPT_PRIO   1
#define CAN1_TX_SUB_PRIO       0

#define SPI1_PREEMPT_PRIO      0
#define SPI1_SUB_PRIO          0
#define SPI3_PREEMPT_PRIO      0
#define SPI3_SUB_PRIO          1

#endif
