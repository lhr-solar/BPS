/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

/** PrintCANHelper.c
 * Some helper functions for mirroring CAN messages over UART.
 */

#include "PrintCANHelper.h"

// lookup table for byte-to-string conversion
static const char hex_lut[] = "0123456789ABCDEF";

/**
 * @brief convert a single byte to a 2-character hex string. not null terminated.
 * 
 * @param byte byte to convert
 * @param buf  buffer to store the hex string. must be at least 2 bytes long.
 */
static inline void byte_to_hexstr(uint8_t byte, char *buf) {
    buf[0] = hex_lut[(byte >> 4) & 0x0F];   // high nibble
    buf[1] = hex_lut[byte & 0x0F];          // low nibble
}

// lookup table of precalculated crc values for each possible byte
static const uint8_t crc8_SAE_J1850[256];

/**
 * @brief calculate 8-bit crc of data using SAE J1850 polynomial (0x1d).
 * 
 * @param data pointer to buffer
 * @param len  length of data in buffer (in bytes)
 * @return uint8_t crc result
 */
static uint8_t crc8(void *data, uint32_t len) {
    char *data_p = (char *)data;
    uint8_t crc = 0xFF;
    for (uint32_t i = 0; i < len; i++) {
        crc = crc8_SAE_J1850[crc ^ data_p[i]];
    }
    return ~crc;
}

/**
 * @brief Convert CANBus message into a human-readable space-separated string with a newline at the end
 * 
 * @param msg CANBus message to convert
 * @param buf buffer to store result in. Must be at least 28 bytes long.
 * @param validonly only print valid data in the CAN payload (otherwise print all 8 bytes)
 * @param crc print out crc at the end of the message
 * @return int length of message
 */
int CAN_ToHexString_HumanReadable(CANMSG_t *msg, void *buf, 
                                  bool validonly, bool crc) {
    char *buf_p = (char *)buf;

    // ID - 3 chars + 1 space
    buf_p[0] = hex_lut[(msg->id >> 8) & 0x0F];
    buf_p[1] = hex_lut[(msg->id >> 4) & 0x0F];
    buf_p[2] = hex_lut[msg->id & 0x0F];
    buf_p[3] = ' ';

    // idx - 2 chars + 1 space
    uint8_t idx = CanMetadataLUT[msg->id].idx_used ? msg->payload.idx : 0;
    byte_to_hexstr(idx, &buf_p[4]);
    buf_p[6] = ' ';

    // data - 2*len chars OR 16 chars
    int payload_len = CanMetadataLUT[msg->id].len - (CanMetadataLUT[msg->id].idx_used ? 1 : 0);
    int len = (validonly) ? payload_len : 8;
    for (int i = 0; i < len; i++) {
        byte_to_hexstr(msg->payload.data.bytes[i], &buf_p[7 + 2*i]);
    }
    
    // crc - 2 chars
    int crclen = (crc) ? 3 : 0; // includes extra space
    if (crc) { // only crc the actual can msg data
        buf_p[7 + 2*len] = ' ';
        byte_to_hexstr(crc8(buf_p, 7 + 2*len), &buf_p[8 + 2*len]);
    }

    // term
    int end = 7 + 2*len + crclen;
    buf_p[end] = '\n';
    buf_p[end + 1] = '\0';    // not really necessary, but just to be safe.

    return end + 1; // excludes null term
}

/**
 * @brief Convert CANBus message into a raw byte array. Array will start with 0xBE 0xEF as framing bytes.
 * 
 * @param msg CANBus message to convert
 * @param buf buffer to store result in. Must be at least 14 bytes long.
 * @param validonly only convert valid data in the CAN payload (otherwise convert all 8 bytes)
 * @param crc append 8-bit crc as last byte of message
 * @return int length of message
 */
int CAN_ToBytes(CANMSG_t *msg, void *buf, 
                bool validonly, bool crc) {
    char *buf_p = (char *)buf;
    static const uint8_t framing_bytes[2] = {0xBE, 0xEF};   // arbitrarily chosen to be rare in data

    memcpy(&buf_p[0], framing_bytes, 2);

    uint32_t id = msg->id;          // in case short-enum is used
    memcpy(&buf_p[2], &id, 2);      // 2 bytes for 11 bit ID

    buf_p[4] = CanMetadataLUT[msg->id].idx_used ? msg->payload.idx : 0;
    
    int payload_len = CanMetadataLUT[msg->id].len - (CanMetadataLUT[msg->id].idx_used ? 1 : 0);
    int len = (validonly) ? payload_len : 8;
    memcpy(&buf_p[5], msg->payload.data.bytes, len);

    if (crc) {
        buf_p[5 + len] = crc8(&buf_p[2], 3 + len);  // crc of ID, idx, and data
    }

    return 5 + len + (crc ? 1 : 0);
}




static const uint8_t crc8_SAE_J1850[256] = {
    0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53, 0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB,
    0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E, 0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76,
    0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9, 0xD4, 0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C,
    0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19, 0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1,
    0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40, 0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8,
    0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D, 0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65,
    0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7, 0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F,
    0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A, 0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2,
    0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75, 0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
    0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8, 0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50,
    0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2, 0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A,
    0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F, 0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7,
    0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66, 0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
    0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB, 0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43,
    0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1, 0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09,
    0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C, 0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4, 
};
