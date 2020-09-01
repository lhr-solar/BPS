#include "BSP_Fans.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>
#define MAX_SPEED    8
#define NUM_FANS     4
#define ARRAY_SIZE  (NUM_FANS * 3)

static const char* file = GET_CSV_PATH(FANS_CSV_FILE);
static char Fans[ARRAY_SIZE];

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
    fprintf(fp, "00,00,00,00"); 
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
    fgets(Fans, ARRAY_SIZE, fp); //read from file
    flock(fno, LOCK_UN); //Unlock
    fclose(fp); //Close file
    fp = fopen(file, "w"); //Open file for writing
    //convert integer to string and store in Fans array
    sprintf(&Fans[3 * (fan-1)], "0%d", dutyCycle);
    Fans[(3 * fan) - 1] = ','; //itoa might make last character newline so have to replace with comma
    fprintf(fp, "%s", Fans); //Write dutyCycle and fan number
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
    fgets(Fans, ARRAY_SIZE, fp); //read from file
    flock(fno, LOCK_UN); //Unlock
    fclose(fp); //Close file
    Fans[(3 * fan) - 1] = '\n'; //place new line for atoi function
    int x = atoi(&Fans[3 * (fan - 1)]); 
    return atoi(&Fans[3 * (fan - 1)]); //return integer value converted from string                     
}