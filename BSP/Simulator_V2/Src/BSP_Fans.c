#include "BSP_Fans.h"
#include "Simulator.h"

#define NUM_FANS 4

static bool initialized = false;
static uint32_t speed[NUM_FANS] = {0,0,0,0};

/**
 * @brief   Initialize all the GPIO pins connected to each fan
 * @param   None
 * @return  None
 */
void BSP_Fans_Init(void)
{
    initialized = true;
    Simulator_log("Initialized Fans\n", 17);
}

/**
 * @brief   Sets fan duty cycle
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 *          fan: fan number whose speed should be changed (1-4)
 * @return  None
 */
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t dutyCycle){
    if (fan > 4 || fan < 1) return ERROR;

    if (initialized) {
        speed[fan - 1] = dutyCycle;
        char str[30];
        sprintf(str, "set fan %d to %d\n", fan, dutyCycle);
        Simulator_log(str, strlen(str));
        return SUCCESS;
    } else {
        Simulator_log("Hard Fault: used fan before initialization\n", 43);
        exit(-1);   // TODO: maybe actually enter a fault state here
    }
}

/**
 * @brief   Get current duty cycle of fans
 * @param   fan number
 * @return  Current PWM duty cycle
 */
int BSP_Fans_GetSpeed(uint8_t fan){
    return speed[fan - 1];                   
}


/**
 * @brief   Sets fan duty cycle for all fans
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 * @return  ErrorStatus
 */
ErrorStatus BSP_Fans_SetAll(uint32_t speed) {
    ErrorStatus result = SUCCESS;
    for (uint8_t i = 1; i <= 4; i++){
        ErrorStatus e = BSP_Fans_Set(i, TOPSPEED);
        if (e != SUCCESS) {
            result = e;
        }
    }
    return result;
}