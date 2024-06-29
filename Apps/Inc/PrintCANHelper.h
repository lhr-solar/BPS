/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

/** PrintCANHelper.h
 * Some helper functions for mirroring CAN messages over UART.
 */

#ifndef PRINT_CAN_HELPER_H
#define PRINT_CAN_HELPER_H

#include "common.h"
#include "CANMetaData.h"

/**
 * @brief Convert CANBus message into a human-readable space-separated string with a newline at the end
 * 
 * @param msg CANBus message to convert
 * @param buf buffer to store result in. Must be at least 28 bytes long.
 * @param validonly only print valid data in the CAN payload (otherwise print all 8 bytes)
 * @param crc print out crc at the end of the message
 * @return int length of message
 */
int CAN_ToHexString_HumanReadable(CANMSG_t *msg, void *buf, bool validonly, bool crc);

/**
 * @brief Convert CANBus message into a raw byte array. Array will start with 0xBE 0xEF as framing bytes.
 * 
 * @param msg CANBus message to convert
 * @param buf buffer to store result in. Must be at least 14 bytes long.
 * @param validonly only convert valid data in the CAN payload (otherwise convert all 8 bytes)
 * @param crc append 8-bit crc as last byte of message
 * @return int length of message
 */
int CAN_ToBytes(CANMSG_t *msg, void *buf, bool validonly, bool crc);

#endif // PRINT_CAN_HELPER_H
