#include "BSP_Fans.h"
#include "simulator_conf.h"
#include <unistd.h>
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
    fprintf(fp, "0,0,0,0"); 
    flock(fno, LOCK_UN);
    fclose(fp);
}

/**
 * @brief   Sets fan duty cycle
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 *          fan: fan number whose speed should be changed (1-4)
 * @return  None
 */
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t dutyCycle){
    //if input is outside of bounds
    char Fans[4];
    if (fan < 1 || fan > 4 || dutyCycle < 0 || dutyCycle > 8) return ERROR;
    FILE* fp = fopen(file, "r");        //Open file for reading
    int fno = fileno(fp);               //Lock
    flock(fno, LOCK_EX);    
    fgets(Fans, 4, fp);
    Fans[fan-1] = dutyCycle;
    fprintf(fp, Fans, dutyCycle); //Write dutyCycle and fan number
    flock(fno, LOCK_UN);                //Unlock
    fclose(fp);                         //Close file
    return SAFE;
}

/**
 * @brief   Get current duty cycle of fans
 * @param   fan number
 * @return  Current PWM duty cycle
 */
int BSP_Fans_GetDuty(uint8_t fan)
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