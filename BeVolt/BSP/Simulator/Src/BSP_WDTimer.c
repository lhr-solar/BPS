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
    // TODO: Initialize the watchdog timer but do not start it.
    //      Make the watchdog timer duration to be 1 seconds. (May change later)
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
bool BSP_WDTimer_DidSystemReset(void) {
    // TODO: Check if the watchdog timer made the uC reset previously.
    /* char resetValue;
    FILE* fp = fopen(file, "r+");
    if(!fp) {
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    fseek(fp, -3, SEEK_END);
    resetValue = fgetc(fp);
    if(resetValue == 'b'){
        fseek(fp, 5, SEEK_SET);
        fprintf(fp, "  ");   //clear the 'b' and the comma before it
        DidSystemReset = false; //reset flag
        return true;
    }
    
    flock(fno, LOCK_UN);
    fclose(fp); */
    if(DidSystemReset == true){
        DidSystemReset = false;      //clear flag
        return true;
    }else{
        return DidSystemReset;
    }
}

//checks if system was reset and prints the PID in case system was reset;
void PrintPID(void){
    char resetValue;
    FILE* fp = fopen(file, "r+");       //check if python script left system reset flag ('b')
    if(!fp) {
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    fseek(fp, -3, SEEK_END);
    resetValue = fgetc(fp);
    if(resetValue == 'b'){
        DidSystemReset = true;
    }else{
        DidSystemReset = false;
    }
    flock(fno, LOCK_UN);
    fclose(fp);
    
    fp = fopen(file, "w");        //print PID
    if(!fp) {
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%d", (int)getpid()); //call some function that gets PID of simulator.out and save in in the csv
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
    // TODO: Start the watchdog countdown
    PrintPID();
    FILE* fp = fopen(file, "w");
    if(!fp) {
        perror("WDTimer.csv");
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%x", 0xCCCC);
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
        FILE* fp = fopen(file, "w");
        if(!fp) {
            perror("WDTimer.csv");
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        
        fprintf(fp, "%x", 0xAAAA);
    
        flock(fno, LOCK_UN);
        fclose(fp);
    }
}



