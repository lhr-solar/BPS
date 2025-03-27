
/* Copyright (c) 2018-2025 UT Longhorn Racing Solar */
/**
 * @file CANMetaData.c
 * @brief CAN MetaData Definitions
 * 
 * Defines all data types used to send CAN messages between all the embedded systems within the car.
 * @copyright Copyright (c) 2023 UT Longhorn Racing Solar
 * 
 */

 #include "CANMetaData.h"

 /**
  * @brief Lookup table to simplify user-defined packet structs. Contains fields that are always the same for every message of a given ID.
  *        Indexed by CANID_t values. Any changes or additions must be made in parallel with changes made to the CANID_t enum in CANbus.h
  */
 const struct CANLUTEntry CanMetadataLUT[LARGEST_CAN_ID] = {
     [DASH_KILL_SWITCH]                 {.idx_used = 0, .len = 1},
     [BPS_TRIP]                         {.idx_used = 0, .len = 1},
     [ANY_SYSTEM_FAILURES]              {.idx_used = 0, .len = 1},
     [IGNITION]                         {.idx_used = 0, .len = 1},
     [ANY_SYSTEM_SHUTOFF]               {.idx_used = 0, .len = 1},
     [BPS_ALL_CLEAR]                    {.idx_used = 0, .len = 1},
     [BPS_CONTACTOR_STATE]              {.idx_used = 0, .len = 1},
     [CURRENT_DATA]                     {.idx_used = 0, .len = 4},
     [VOLTAGE_DATA_ARRAY]               {.idx_used = 1, .len = 5},
     [TEMPERATURE_DATA_ARRAY]           {.idx_used = 1, .len = 5},
     [STATE_OF_CHARGE_DATA]             {.idx_used = 0, .len = 4},
     [WDOG_TRIGGERED]                   {.idx_used = 0, .len = 1},
     [CAN_ERROR]                        {.idx_used = 0, .len = 1},
     [BPS_COMMAND_MSG]                  {.idx_used = 0, .len = 8},
     [SUPPLEMENTAL_VOLTAGE]             {.idx_used = 0, .len = 2},
     [CHARGING_ENABLED]                 {.idx_used = 0, .len = 1},
     [VOLTAGE_SUMMARY]                  {.idx_used = 0, .len = 8},
     [TEMPERATURE_SUMMARY]              {.idx_used = 0, .len = 8},
     [MPPT_A_BOOST_ENABLE]              {.idx_used = 0, .len = 1},
     [MPPT_B_BOOST_ENABLE]              {.idx_used = 0, .len = 1},
     [MOTOR_CONTROLLER_IDENTIFICATION]  {.idx_used = 0, .len = 8},
     [MOTOR_STATUS]                     {.idx_used = 0, .len = 8},
     [MOTOR_CONTROLLER_BUS]             {.idx_used = 0, .len = 8},
     [VELOCITY]                         {.idx_used = 0, .len = 8},
     [MOTOR_CONTROLLER_PHASE_CURRENT]   {.idx_used = 0, .len = 8},
     [MOTOR_VOLTAGE_VECTOR]             {.idx_used = 0, .len = 8},
     [MOTOR_CURRENT_VECTOR]             {.idx_used = 0, .len = 8},
     [MOTOR_BACKEMF]                    {.idx_used = 0, .len = 8},
     [LOW_VOLTAGE_RAIL_MEASUREMENT]     {.idx_used = 0, .len = 8},
     [DSP_VOLTAGE_RAIL_MEASUREMENT]     {.idx_used = 0, .len = 8},
     [RESERVED]                         {.idx_used = 0, .len = 8},
     [MOTOR_TEMPERATURE]                {.idx_used = 0, .len = 8},
     [DSP_BOARD_TEMPERATURE]            {.idx_used = 0, .len = 8},
     [ODOMETER__BUS_AMP_HOURS]          {.idx_used = 0, .len = 8},
     [SLIP_SPEED_MEASUREMENT]           {.idx_used = 0, .len = 8},
     [CONTACTOR_SENSE]                  {.idx_used = 0, .len = 2},
     [PRECHARGE_TIMEOUT_]               {.idx_used = 0, .len = 1},
     [CONTROL_MODE]                     {.idx_used = 0, .len = 1},
     [IO_STATE]                         {.idx_used = 0, .len = 3},
     [MOTOR_PRECHARGE_ENABLE]           {.idx_used = 0, .len = 1}
 };
 