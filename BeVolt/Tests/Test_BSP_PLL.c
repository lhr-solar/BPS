#include "common.h"
#include "config.h"
#include "BSP_PLL.h"


int main(void){
    BSP_PLL_Init();
    uint32_t test = BSP_PLL_GetSystemClock();
    printf("\r%d", test);
    
    exit(0);
}