#include "BSP_WDTimer.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include "simulator_conf.h"

static const char* file = GET_CSV_PATH(WDTIMER_CSV_FILE);

enum{
    Start = 1, Reset = 2
};

static bool didSystemReset;
static bool hasStarted;
/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(void) {
    if(access(file, F_OK) != 0) {
        // File is created if it doesn't exist, otherwise this function does nothing at all
        FILE* fp = fopen(file, "w");
        if(!fp) {
            perror(WDTIMER_CSV_FILE);
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        fprintf(fp, "\n%d", (int)getpid()); //call some function that gets PID of simulator.out and save in in the csv
        flock(fno, LOCK_UN);
        fclose(fp);
        didSystemReset = false;
        hasStarted = false;
    }
    
    
}

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void){
    if(didSystemReset == true){
        didSystemReset = false;      //clear flag
        return true;
    }else{
        return didSystemReset;
    }
}

//checks if system reset flag
void CheckReset(void){
    char resetValue;
    FILE* fp = fopen(file, "r+");       //check if python script left system reset flag ('b')
    if(!fp) {
        perror(WDTIMER_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    fseek(fp, 2, SEEK_SET);
    resetValue = fgetc(fp);
    if(resetValue == 'b'){
        didSystemReset = true;
    }else{
        didSystemReset = false;
    }
    flock(fno, LOCK_UN);
    fclose(fp);
    hasStarted = false; 

    
    
}


/**
 * @brief   Start the watchdog timer countdown. Once this is started, the BSP_WDTimer_Reset
 *          must be called or else the system will reset.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Start(void) {
    CheckReset();
    FILE* fp = fopen(file, "w");
    if(!fp) {
        perror(WDTIMER_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%x\n%d", Start, (int)getpid());
    hasStarted = true;
    flock(fno, LOCK_UN);
    fclose(fp);
}

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void) {
    if(hasStarted == true){
        FILE* fp = fopen(file, "r+");
        if(!fp) {
            perror(WDTIMER_CSV_FILE);
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        
        fprintf(fp, "%x\n%d", Reset, 0);
    
        flock(fno, LOCK_UN);
        fclose(fp);
    }
}



