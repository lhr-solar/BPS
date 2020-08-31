#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"

int main(void) {
    OS_ERR err;

    OSInit(&err);


    OSStart(&err);
}

void test(void *p_arg) {
    
}