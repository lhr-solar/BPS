#include "BSP_Fans.h"
#include "simulator_conf.h"
#include <stdio.h>
#include <sys/file.h>

static const char* file = GET_CSV_PATH(FANS_CSV_FILE);

/**
 * @brief   Initialize all the GPIO pins connected to each fan
 * @param   None
 * @return  None
 */
void BSP_Fans_Init(void)
{
    // Creates file if none exists
    FILE* fp = fopen(file, "w+");
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%d", 0);
    flock(fno, LOCK_UN);
	fclose(fp);
}

/**
 * @brief   Sets fan duty cycle
 * @param   dutyCycle: int for duty cycle amount in range 0-100
 * @return  None
 */
void BSP_Fans_Set(int dutyCycle)
{
    FILE* fp = fopen(file, "w");        //Open file for writing
    int fno = fileno(fp);               //Lock
    flock(fno, LOCK_EX);                
    fprintf(fp, "%d", dutyCycle);       //Write dutyCycle
    flock(fno, LOCK_UN);                //Unlock
    fclose(fp);                         //Close file
}

/**
 * @brief   Turns fans off
 * @param   None
 * @return  None
 */
void BSP_Fans_Off(void)
{
    FILE* fp = fopen(file, "w");        //Open file for writing
    int fno = fileno(fp);               //Lock
    flock(fno, LOCK_EX);
    fprintf(fp, "%d", 0);               //Write 0
    flock(fno, LOCK_UN);                //Unlock
    fclose(fp);                         //Close file
}

/**
 * @brief   Get current duty cycle of fans
 * @param   None
 * @return  Current PWM duty cycle
 */
int BSP_Fans_GetDuty(void)
{
    int state;
    FILE* fp = fopen(file, "r");        //Open file for reading
    int fno = fileno(fp);               //Lock
    flock(fno, LOCK_EX);
    fscanf(fp, "%d", &state);           //Read state
    flock(fno, LOCK_UN);                //Unlock
    fclose(fp);                         //Close file
    return state;                       
}