#include "BSP_Fans.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>
#include "config.h"
#define MAX_SPEED    8

static const char* file = GET_CSV_PATH(FANS_CSV_FILE);
static int Fans[NUM_FANS];

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
    if (fan < 1 || fan > NUM_FANS) return ERROR;
    if (dutyCycle < 0 || dutyCycle > MAX_SPEED) return ERROR;
    FILE* fp = fopen(file, "r"); //Open file for reading
    int fno = fileno(fp);  //Lock
    flock(fno, LOCK_EX);  
    fscanf(fp, "%d,%d,%d,%d", &(Fans[0]), &(Fans[1]), &(Fans[2]), &(Fans[3])); //read file
    flock(fno, LOCK_UN); //Unlock
    fclose(fp); //Close file
    fp = fopen(file, "w"); //Open file for writing
    Fans[fan - 1] = dutyCycle; //write to array
    fprintf(fp, "%d,%d,%d,%d", Fans[0], Fans[1], Fans[2], Fans[3]); //write to file
    flock(fno, LOCK_UN); //Unlock
    fclose(fp); //Close file
    return SUCCESS;
}

/**
 * @brief   Get current duty cycle of fans
 * @param   fan number
 * @return  Current PWM duty cycle
 */
int BSP_Fans_GetSpeed(uint8_t fan){
    FILE* fp = fopen(file, "r"); //Open file for reading
    int fno = fileno(fp); //Lock
    flock(fno, LOCK_EX);
    fscanf(fp, "%d,%d,%d,%d", &(Fans[0]), &(Fans[1]), &(Fans[2]), &(Fans[3])); //read file
    flock(fno, LOCK_UN); //Unlock
    fclose(fp); //Close file
    return Fans[fan - 1];                   
}
