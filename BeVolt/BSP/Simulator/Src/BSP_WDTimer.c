#include "BSP_WDTimer.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>


static const char* file = "BSP/Simulator/DataGeneration/Data/WDTimer.csv";

bool DidSystemReset;
bool HasStarted;
/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(void) {
    FILE* fp = fopen(file, "w");
    if(!fp) {
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%d", (int)getpid()); //call some function that gets PID of simulator.out and save in in the csv
    flock(fno, LOCK_UN);
    fclose(fp);
    DidSystemReset = false;
    HasStarted = false;
}

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void){
    if(DidSystemReset == true){
        DidSystemReset = false;      //clear flag
        return true;
    }else{
        return DidSystemReset;
    }
}

//checks if system reset flag
void CheckReset(void){
    char resetValue;
    FILE* fp = fopen(file, "r+");       //check if python script left system reset flag ('b')
    if(!fp) {
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    fseek(fp, 5, SEEK_SET);
    resetValue = fgetc(fp);
    if(resetValue == 'b'){
        DidSystemReset = true;
    }else{
        DidSystemReset = false;
    }
    flock(fno, LOCK_UN);
    fclose(fp);
    HasStarted = false; 

    
    
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
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%x\n%d", 0xCCCC, (int)getpid());
    HasStarted = true;
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
    // TODO: Reset the watchdog countdown
    //uint32_t *check;
    if(HasStarted == true){
        FILE* fp = fopen(file, "r+");
        if(!fp) {
            perror("WDTimer.csv");
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        
        fprintf(fp, "%x\n%d", 0xAAAA, 0);
    
        flock(fno, LOCK_UN);
        fclose(fp);
    }
}



