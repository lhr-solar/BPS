/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"

// Define your CAN IDs in one place using an X-Macro
#define CAN_FILTER_LIST \
    CAN_RECV_GEN(IO_STATE)           \
    CAN_RECV_GEN(MPPT_A_STATUS)      \
    CAN_RECV_GEN(MPPT_B_STATUS)      \
    CAN_RECV_GEN(CONTACTOR_SENSE)

// Generate the filter list array
#define CAN_RECV_GEN(id) id,
CANID_t CanFilterList[] = { CAN_FILTER_LIST };
#undef CAN_RECV_GEN

#define NUM_CAN_FILTERS (sizeof(CanFilterList) / sizeof(*CanFilterList))

// Generate the can_recv_entries array with id set and payload zeroed
#define CAN_RECV_GEN(can_id) { .id = can_id, .payload = {0} },
static CANMSG_t can_recv_entries[NUM_CAN_FILTERS] = { CAN_FILTER_LIST };
#undef CAN_RECV_GEN

/**
 * @brief Gets the latest recieved value for a specific CAN ID
 * @param recv: CANMSG_t struct to store the received message
 * @param id: the CAN ID to search for in the filter list
*/
ErrorStatus getLastCANBusValue(CANMSG_t recv, CANID_t id){
  for(uint8_t i = 0; i < NUM_CAN_FILTERS; i++) {
    if(can_recv_entries[i].id == id) {
      // copy the payload of can_recv_entries into the recv from the user
      memcpy(&recv.payload, &can_recv_entries[i].payload, sizeof(CANPayload_t));

      // Copy the ID over, not technically needed but good for consistency
      memcpy(&recv.id, &can_recv_entries[i].id, sizeof(CANPayload_t));

      return SUCCESS;
    }
    // The requested ID has not been recieved yet
    return ERROR;
  }
  return ERROR;
}

static inline void updateCANBusReceiveEntries(CANMSG_t msg) {
    for(uint8_t i = 0; i < NUM_CAN_FILTERS; i++) {
    // If the ID matches one of the entries, copy the newly recieved payload
    if(msg.id == can_recv_entries[i].id) {
      memcpy(&can_recv_entries[i].payload, &msg.payload, sizeof(CANPayload_t));
      break;
    }
  }
}

/**
 * @brief call CANBUS_read and receive data in id/buffer, store into a
 * queue so that other tasks can check can messages and do func based on that
*/
void Task_CANBusProducer(void *p_arg) {
    (void)p_arg;

    CANMSG_t CANMsg;

    CANbus_Init(
        (bool) p_arg, 
        false, 
        CanFilterList, 
        NUM_CAN_FILTERS);
    
    while(1) {
      // BLOCKING =====================
      // Wait for CAN Bus to have message
      CANbus_WaitToReceive(&CANMsg.id, &CANMsg.payload);
      
      // Once an ID is recieved, update the can_recv_entries with the latest value
      updateCANBusReceiveEntries(CANMsg);
    }
}