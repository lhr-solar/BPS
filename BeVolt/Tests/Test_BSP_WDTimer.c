#include "common.h"
#include "config.h"
#include "BSP_WDTimer.h"

int main(){
    uint32_t delay = 150000000;
    uint32_t secs = 40;
    uint32_t resetCount = 3;
    BSP_WDTimer_Start();
    printf("pid should be saved, countdown started\n");
    secs = 40;
    while(secs){
        delay = 15000000;
        while(delay){
            delay--;
        }
        secs--;
    }
    
    while(1){

        while(resetCount){
            secs = 40;
            while(secs){
                delay = 150000000;
                while(delay){
                    delay--;
                }
                secs--;
            }
        
            BSP_WDTimer_Reset();
            printf("reset %d\n", resetCount);
            bool checkSysReset = BSP_WDTimer_DidSystemReset();
            printf("Has the system been reset (1 = yes, 0 = no): %d\n\n", checkSysReset);
            resetCount--;
        }
        resetCount = 3;
        printf("this process will be killed any second now...\n");
        while(resetCount){
            secs = 20;
            while(secs){
                delay = 150000000;
                while(delay){
                    delay--;
                }
                secs--;
            }
        }
        exit(0);
    } 
}
