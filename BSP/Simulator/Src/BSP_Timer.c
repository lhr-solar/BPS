#include "BSP_Timer.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include "simulator_conf.h"

static const char* file = GET_CSV_PATH(TIMER_CSV_FILE);
static const char* PLLfile = GET_CSV_PATH(PLL_CSV_FILE);

static bool hasBeenStarted = false;

static uint32_t ticks;

static char init = 's';


/**
 * @brief   Initialize the timer for time measurements.
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void) {
    // File is created if it doesn't exist, otherwise this function does nothing at all
    if(access(file, F_OK) != 0) {   
        FILE* fp = fopen(file, "w");
        if(!fp) {
            perror(TIMER_CSV_FILE);
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        flock(fno, LOCK_UN);
        fclose(fp);
        ticks = 0;
    }
}

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start(void) {
    FILE* fp = fopen(file, "w");
    if(!fp){
        perror(TIMER_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%c", init);
    flock(fno, LOCK_UN);
    fclose(fp);
    hasBeenStarted = true;

}

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void) {
    char numberString[30];
    uint32_t number;
    uint32_t ret;
    char check;
    
    if(hasBeenStarted){
        FILE* fp = fopen(file, "r");
        if(!fp){
            perror(TIMER_CSV_FILE);
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        
        check = fgetc(fp);
        if(check != init){
            fseek(fp, 0, SEEK_SET);     //reset file pointer to the beginning of the file
            fgets(numberString, 30, fp);
            number = atoi(numberString);
            ret = (number - ticks) & 0x00FFFFFF;    //Account for overflow
            ticks = number;
        }
        flock(fno, LOCK_UN);
        fclose(fp);
    
        return ret;
    }else{
        return 0;
    }
}

/**
 * @brief   Gets the running frequency of the timer (time per tick)
 * @param   None
 * @return  frequency in Hz
 */
uint32_t BSP_Timer_GetRunFreq(void) {
    uint32_t currentFreq;
    char str[20];
    FILE* fp = fopen(PLLfile, "r");
    
    if(access(PLLfile, F_OK) != 0) {
        return 16000000;    //return default value if PLL csv hasn't been created
    }else{
        if(!fp) {
            perror(PLL_CSV_FILE);
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);   //same as BSP_PLL_GetSystemClock()
        flock(fno, LOCK_EX);
        fgets(str, 20, fp);
        flock(fno, LOCK_UN);
        fclose(fp);
        currentFreq = atoi(str);
        return currentFreq;  
    }
     
}
