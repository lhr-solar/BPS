#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "os.h"
#include "Tasks.h"
#include <stdio.h>
#include <time.h>
#include "Simulator.h"
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];
OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];
void Task1(void *p_arg) {
    OS_ERR err;


    time_t now = time(NULL);
    while(1) {
        printf("%ld",time(NULL));
        printf(", %d\n",Simulator_getCurrent());
        Simulator_Log(LOG, "Logged \n");
        OSTimeDly(100, OS_OPT_TIME_DLY,&err);

        
        if(time(NULL) - now >= 12) {exit(0);}
        //OSTaskDel(&Task1_TCB,&err);
    }
    
}
void Task2(void *p_arg) {
    OS_ERR err;
    OS_CPU_SysTickInit();
    OSTaskDel(&Task1_TCB,&err);
}
int main(int argc, char **argv) {
    Simulator_Init(argv[1]);

    OS_ERR err;
    //CPU_Init();
    OSInit(&err);
    OSTaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                2,
                Task1_Stk,
                16,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);
    assertOSError(err);
    OSTaskCreate(&Task2_TCB,				// TCB
                "Task 2",	// Task Name (String)
                Task1,				// Task function pointer
                (void *)0,				// Task function args
                1,			// Priority
                CriticalState_Stk,				// Stack
                WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
                TASK_CRITICAL_STATE_STACK_SIZE,		// Stack size
                0,						// Queue size (not needed)
                10,						// Time quanta (time slice) 10 ticks
                (void *)0,				// Extension pointer (not needed)
                OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
                &err);					// return err code
    assertOSError(err);
    OSStart(&err);
}