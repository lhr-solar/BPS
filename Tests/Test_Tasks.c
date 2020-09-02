#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

OS_SEM SafetyCheck_Sem4;

void Task1(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;
    
    while(1) {
        for(int i = 0; i < 4; i++) {
        OSSemPend(&SafetyCheck_Sem4,
                    0,
                    OS_OPT_PEND_BLOCKING,
                    &ts,
                    &err);
        }
        
        printf("1\r\n");
    }
}

void Task2(void *p_arg) {
    (void)p_arg;
    
    OS_ERR err;

    while(1) {
        OSSemPost(&SafetyCheck_Sem4,
                    OS_OPT_POST_ALL,
                    &err);

        printf("2\r\n");
    }
}

int main(void) {
    OS_ERR err;

    OSInit(&err);

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0,
                &err);

    OSTaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                128,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);

    OSTaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                2,
                Task2_Stk,
                128,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);

    OSStart(&err);
}
