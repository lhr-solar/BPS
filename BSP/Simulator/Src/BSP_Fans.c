#include "BSP_Fans.h"
#include "simulator_conf.h"

static const char* file = GET_CSV_PATH(FANS_CSV_FILE);

//Duty cycle for fans
int fanDuty = 0;

//Toggle for fans
int fanReg = 0;


/**
 * @brief   Initialize all the GPIO pins connected to each fan
 * @param   None
 * @return  None
 */
void BSP_Fans_Init(void)
{
    // Creates file if none exists
	FILE* fp = fopen(file, "w+");
    fprintf(fp, "0\n0");
	fclose(fp);
}

/**
 * @brief   Sets fan duty cycle
 * @param   dutyCycle: int for duty cycle amount in range 0-100
 * @return  None
 */
void BSP_Fans_Set(int dutyCycle)
{
    fanDuty = dutyCycle;
    BSP_Fans_Write();
}

/**
 * @brief   Turns fans on at currently set duty cycle
 * @param   None
 * @return  None
 */
void BSP_Fans_On(void)
{
    fanReg = 1;
    BSP_Fans_Write();
}

/**
 * @brief   Turns fans off
 * @param   None
 * @return  None
 */
void BSP_Fans_Off(void)
{
    fanReg = 0;
    BSP_Fans_Write();
}

/**
 * @brief   Get current duty cycle of fans
 * @param   None
 * @return  Current PWM duty cycle if fans
 */
int BSP_Fans_GetDuty(void)
{
    return fanDuty;
}

/**
 * @brief   Get current status of fans
 * @param   None
 * @return  1 if fans are on, 0 otherwise
 */
int BSP_Fans_GetStatus(void)
{
    return fanReg;
}

/**
 * @brief   Write files for fan simulation
 * @param   None
 * @return  None
 */
void BSP_Fans_Write(void)
{
    FILE* fp = fopen(file, "w+");
    fprintf(fp, "%d\n", fanReg);
    fprintf(fp, "%d", fanDuty);
	fclose(fp);
}