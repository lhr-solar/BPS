/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_PLL.h"
#include "Charge.h"

#define CHECK_CONTACTOR_DELAY 200

// ~ 5 second timer
#define CONTROLS_HEARTBEAT_COUNT (1000/CHECK_CONTACTOR_DELAY) * 5

// ~ 5 second timer for waiting to turn on the array Contactor
#define ARRAY_PRECHARGE_DELAY (1000/CHECK_CONTACTOR_DELAY) * 5

static volatile bool ARRAY_CONTACTOR_ON = false;
static volatile bool array_precharge_complete = false;

typedef enum {
    MPPT_A = 0,
    MPPT_B = 1,
    NUM_MPPTs
} MPPT_t;

typedef enum {
    DISABLED,
    ENABLED,
    UNKNOWN
} boost_status_t;

static boost_status_t mppt_boost_status[NUM_MPPTs] = {UNKNOWN, UNKNOWN};


static void sendBoostEnable(MPPT_t mppt, bool enable){
    CANMSG_t msg = {.id = (mppt == MPPT_A) ? MPPT_A_BOOST_ENABLE : MPPT_B_BOOST_ENABLE};
    msg.payload.data.b = enable;
    CAN_TransmitQueue_Post(msg);
}

// TODO: REMOVE THE STATIC ARRAY PRECHARGE TIMER
static uint8_t array_precharge_tim = 0;
static void updateArrayContactorState(bool state){
    ARRAY_CONTACTOR_ON = state;
    if(state){
        Contactor_On(ARRAY_CONTACTOR);
    } else {
        Contactor_Off(ARRAY_CONTACTOR);
        array_precharge_tim = 0;
        array_precharge_complete = false;
    }
}

void Task_CheckContactor(void *p_arg) {
    (void)p_arg;
    
    // buffers for CAN messages
    // static to avoid stack allocation
    static CANMSG_t all_clear =             {.id = BPS_ALL_CLEAR, .payload.data.b = 1};
    static CANMSG_t contactor_state =       {.id = BPS_CONTACTOR_STATE};

    static CANMSG_t recv;

    BSP_PLL_DelayMs(30); // delay is needed for pull up resistor to stabilize before we check for contactor state
    // If a contactor is on before we turn it on in this task, it may have failed and welded closed
    if (Contactor_GetState(HVHIGH_CONTACTOR) || Contactor_GetState(HVLOW_CONTACTOR)) {
        Fault_BitMap |= Fault_ESTOP;
        EnterFaultState();
    }

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for (uint8_t i = 0; i < NUM_FAULT_POINTS; i++){
        RTOS_BPS_SemPend(&SafetyCheck_Sem4,OS_OPT_PEND_BLOCKING);
    }

    // Initialize state of charge from measured open-circuit voltage before we close the contactors
    Charge_Init();

    // Turn Contactor On
    Contactor_On(HVHIGH_CONTACTOR);
    Contactor_On(HVLOW_CONTACTOR);

    // number of times we've failed to recieve Controls IO_State message
    uint32_t controls_no_msg = 0;

    array_precharge_tim = 0;

    while (1) {
        // delay of 200ms
        // controls IO_STATE message every ~250ms, so we need to check for the 
        // message more frequently to ensure our CAN queue doesn't fill. 
        // 200ms is chosen because it's a neat number and should be fast enough. 
        RTOS_BPS_DelayMs(CHECK_CONTACTOR_DELAY);

        // Transmit that BPS is safe
        // Assumes that BPS is safe at this point
        // this task will stop sending BPS all clear if we enter a fault state
        CAN_TransmitQueue_Post(all_clear);

        // fault if the contactor is open -- this should only happen if ESTOP is hit
        if (Contactor_GetState(HVHIGH_CONTACTOR) != true) {
            Fault_BitMap |= Fault_ESTOP;
            EnterFaultState();
        }

        // Turn on/off array contactor based on what we receive from controls or the MPPT
        // if we get to this point and there's no message we try again ~200ms later
        ErrorStatus status = CAN_ReceiveQueue_Pend(&recv); // non-blocking

        // update the MPPT boost status
        if(status == SUCCESS && (recv.id == MPPT_A_STATUS || recv.id == MPPT_B_STATUS)){
            MPPT_t mppt = (recv.id == MPPT_A_STATUS) ? MPPT_A : MPPT_B;
            mppt_boost_status[mppt] = recv.payload.data.bytes[2] & 0x1 ? ENABLED : DISABLED;
        }
        if(status == SUCCESS){
            if(recv.id == IO_STATE){
                controls_no_msg = 0;
                // bit 1 of the payload is the array contactor state
                uint8_t array_ign_state = (recv.payload.data.bytes[2]) & 0x1;
                if(!array_ign_state){
                    // now need to wait for MPPT to send that boosting is disabled
                    updateArrayContactorState(false);
                }
                // if the array contactor is not already on and the MPPT is disabled, enable array based on controls
                else if(array_ign_state && !ARRAY_CONTACTOR_ON && mppt_boost_status[MPPT_B] == DISABLED){
                    updateArrayContactorState(true);
                }
                // otherwise leave the array contactor state as is
            }
        }
        else {
            controls_no_msg++;
        }
        // controls doesn't send a message for 5 seconds
        // TOOD: make this turn the contactor off instead of on, this is just for testing without Controls
        if(controls_no_msg >= CONTROLS_HEARTBEAT_COUNT && !ARRAY_CONTACTOR_ON && mppt_boost_status[MPPT_B] == DISABLED){
            updateArrayContactorState(true);
        }

        bool array_contactor_state = Contactor_GetState(ARRAY_CONTACTOR);

        //Send BPS contactor state via CAN
        contactor_state.payload.data.b = (Contactor_GetState(HVHIGH_CONTACTOR) << 2) |
                                         (Contactor_GetState(HVLOW_CONTACTOR) << 1) |
                                          array_contactor_state;
        CAN_TransmitQueue_Post(contactor_state);
    

        // TOOD: implement reading Active precharge can message for seeing if ready
        // for now we just wait 5 seconds after array contactor is on
        array_precharge_tim = array_contactor_state ? (array_precharge_tim+1) : 0;
        if(array_precharge_tim >= ARRAY_PRECHARGE_DELAY && array_contactor_state){
            // latches to true to avoid possible underflow problems
            array_precharge_complete = true;
        }

        sendBoostEnable(MPPT_A, array_contactor_state && array_precharge_complete);
        sendBoostEnable(MPPT_B, array_contactor_state && array_precharge_complete);
    }
}