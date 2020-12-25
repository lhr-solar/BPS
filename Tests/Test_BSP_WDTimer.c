/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_WDTimer.h"

int main(){
    uint32_t delay = 150000000;
    uint32_t time = 40;
    uint32_t resetCount = 3;        //for testing WDTimer resets, these delays will let BSP_Reset() be called before the WDTimer finishes counting down 
    BSP_WDTimer_Init();
    BSP_WDTimer_Start();
    printf("pid should be saved, countdown started\n");
    time = 40;
    while(time){
        delay = 15000000;
        while(delay){
            delay--;
        }
        time--;
    }
    
    while(1){

        while(resetCount){
            time = 40;
            while(time){
                delay = 150000000;
                while(delay){
                    delay--;
                }
                time--;
            }
        
            BSP_WDTimer_Reset();
            printf("reset %d\n", resetCount);
            bool checkSysReset = BSP_WDTimer_DidSystemReset();
            printf("Has the system been reset (1 = yes, 0 = no): %d\n\n", checkSysReset);
            resetCount--;
        }
        resetCount = 3;
        printf("this process will be killed any second now...\n"); 
        while(resetCount){                                                      //this delay is longer than the WDTimer countdown, so this process should be killed before the delay finishes
            time = 20;
            while(time){
                delay = 150000000;
                while(delay){
                    delay--;
                }
                time--;
            }
        }
        exit(0);
    } 
}
/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_WDTimer.h"

int main(){
    uint32_t delay = 150000000;
    uint32_t time = 40;
    uint32_t resetCount = 3;        //for testing WDTimer resets, these delays will let BSP_Reset() be called before the WDTimer finishes counting down 
    BSP_WDTimer_Init();
    BSP_WDTimer_Start();
    printf("pid should be saved, countdown started\n");
    time = 40;
    while(time){
        delay = 15000000;
        while(delay){
            delay--;
        }
        time--;
    }
    
    while(1){

        while(resetCount){
            time = 40;
            while(time){
                delay = 150000000;
                while(delay){
                    delay--;
                }
                time--;
            }
        
            BSP_WDTimer_Reset();
            printf("reset %d\n", resetCount);
            bool checkSysReset = BSP_WDTimer_DidSystemReset();
            printf("Has the system been reset (1 = yes, 0 = no): %d\n\n", checkSysReset);
            resetCount--;
        }
        resetCount = 3;
        printf("this process will be killed any second now...\n"); 
        while(resetCount){                                                      //this delay is longer than the WDTimer countdown, so this process should be killed before the delay finishes
            time = 20;
            while(time){
                delay = 150000000;
                while(delay){
                    delay--;
                }
                time--;
            }
        }
        exit(0);
    } 
}
