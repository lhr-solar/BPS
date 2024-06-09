/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

/** PrintCANHelper.h
 * Some helper functions for mirroring CAN messages over UART.
 */

#ifndef PRINT_CAN_HELPER_H
#define PRINT_CAN_HELPER_H

#include "common.h"
#include "CANMetaData.h"

int CAN_ToHexString_HumanReadable(CANMSG_t *msg, void *buf, bool validonly, bool crc);
int CAN_ToBytes(CANMSG_t *msg, void *buf, bool validonly, bool crc);

#endif // PRINT_CAN_HELPER_H
