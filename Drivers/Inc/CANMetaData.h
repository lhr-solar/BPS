
/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/**
 * @file CANMetaData.h
 * @brief CAN MetaData Definitions
 * 
 * Defines all data types used to send CAN messages between all the embedded systems within the car.
 * 
 */

 #ifndef CANMETADATA_H
 #define CANMETADATA_H
 
 #include <stdint.h>
 
 /**
  * CAN Message structure copied from BPS
  */
 
 // Enum for ID's of all messages that can be sent across CAN bus
 // IDs must be listed in strictly increasing order!
 typedef enum {
     DASH_KILL_SWITCH                 = 0x001,
     BPS_TRIP                         = 0x002,
     ANY_SYSTEM_FAILURES              = 0x003,
     IGNITION                         = 0x004,
     ANY_SYSTEM_SHUTOFF               = 0x005,
     BPS_ALL_CLEAR                    = 0x101,
     BPS_CONTACTOR_STATE              = 0x102,
     CURRENT_DATA                     = 0x103,
     VOLTAGE_DATA_ARRAY               = 0x104,
     TEMPERATURE_DATA_ARRAY           = 0x105,
     STATE_OF_CHARGE_DATA             = 0x106,
     WDOG_TRIGGERED                   = 0x107,
     CAN_ERROR                        = 0x108,
     BPS_COMMAND_MSG                  = 0x109,
     SUPPLEMENTAL_VOLTAGE             = 0x10B,
     CHARGING_ENABLED                 = 0x10C,
     VOLTAGE_SUMMARY                  = 0x10D,
     TEMPERATURE_SUMMARY              = 0x10E,
     MPPT_A_BOOST_ENABLE              = 0x209,
     MPPT_B_BOOST_ENABLE              = 0x219,
     MOTOR_CONTROLLER_IDENTIFICATION  = 0x240,
     MOTOR_STATUS                     = 0x241,
     MOTOR_CONTROLLER_BUS             = 0x242,
     VELOCITY                         = 0x243,
     MOTOR_CONTROLLER_PHASE_CURRENT   = 0x244,
     MOTOR_VOLTAGE_VECTOR             = 0x245,
     MOTOR_CURRENT_VECTOR             = 0x246,
     MOTOR_BACKEMF                    = 0x247,
     LOW_VOLTAGE_RAIL_MEASUREMENT     = 0x248,
     DSP_VOLTAGE_RAIL_MEASUREMENT     = 0x249,
     RESERVED                         = 0x24A,
     MOTOR_TEMPERATURE                = 0x24B,
     DSP_BOARD_TEMPERATURE            = 0x24C,
     ODOMETER__BUS_AMP_HOURS          = 0x24E,
     SLIP_SPEED_MEASUREMENT           = 0x257,
     CONTACTOR_SENSE                  = 0x400,
     PRECHARGE_TIMEOUT_               = 0x401,
     CONTROL_MODE                     = 0x580,
     IO_STATE                         = 0x581,
     MOTOR_PRECHARGE_ENABLE           = 0x582,
     LARGEST_CAN_ID
 } CANID_t;
 
 // Union of data that can be sent across CAN bus. Only one field must be filled out
 typedef union {
     uint8_t b;
     uint16_t h;
     uint32_t w;
     float f;
     uint8_t bytes[8];   // use when data matches no normal datatype
 } CANData_t;
 
 /**
  * @note    idx is only used when an array of data needs to be sent.
  * @note    data is a union so only one of the fields should be filled out or bad stuff will happen.
  */
 typedef struct {
     uint8_t idx;
     CANData_t data;
 } CANPayload_t;
 
 // This data type is used to push messages onto the queue
 typedef struct {
     CANID_t id;
     CANPayload_t payload;
 } CANMSG_t;
 
 // Used to format the fields in the CAN metadata lookup table
 
 struct CANLUTEntry {uint8_t idx_used : 1; uint8_t len : 7;};
 
 extern const struct CANLUTEntry CanMetadataLUT[LARGEST_CAN_ID];
 
 #endif
 