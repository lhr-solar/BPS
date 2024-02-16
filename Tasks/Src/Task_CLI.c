/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

#include "Tasks.h"
#include "RTOS_BPS.h"
#include "CLI.h"

void Task_CLI(void *p_arg) {
    (void)p_arg;

    while(1) {
    // To do: make while loop only runs when you've inputted a command into the terminal
        CLI_Init();
        char* StartString = "-----------Start-----------\n";
        char* EndString   = "-------------End-----------\n";
        while(1){
            printf("%s\n", StartString);
            CLI_Voltage();
            CLI_Temperature();
            CLI_Contactor();
            CLI_Fans();
            CLI_FAULT();
            printf("%s\n", EndString);
        }
    // To do: fix this
    RTOS_BPS_DelayMs(100);

    }
}