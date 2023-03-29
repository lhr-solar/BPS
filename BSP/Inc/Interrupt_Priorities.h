/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/* Interrupt_Priorities.h
 * File to define Preemptive and Sub Interrupt Priorities. Belongs in the BSP layer as all references to these values are within BSP files.
 * Preemption Priority - allows an ISR to be preempted (interrupted) by another interrupt of higher priority.
 * Subpriority - determines which of two, equal priority interrupts to start with
 * When updating subpriorities, make sure there are no alternative interrupts with equal priorities that need to be run in a certain order
 */

#ifndef INTERRUPT_PRIORITIES_H
#define INTERRUPT_PRIORITIES_H

#define USART2_Preempt_Prio    2
#define USART2_Sub_Prio        1
#define USART3_Preempt_Prio    2
#define USART3_Sub_Prio        0

#define CAN1_RX0_Preempt_Prio  1 
#define CAN1_RX0_Sub_Prio      1 
#define CAN1_TX_Preempt_Prio   1
#define CAN1_TX_Sub_Prio       0

#define SPI1_Preempt_Prio      0
#define SPI1_Sub_Prio          0
#define SPI3_Preempt_Prio      0
#define SPI3_Sub_Prio          1

#endif
