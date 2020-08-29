#include "common.h"
#include "config.h"
#include <stdio.h>
#include "BSP_Fans.h"

int main()
{
    BSP_Fans_Init();
    printf("Fans.csv created");
    int x = 0;
   /* BSP_Fans_Set(70);
    printf("%i", BSP_Fans_GetDuty());
    BSP_Fans_Set(100);
    printf("%i", BSP_Fans_GetDuty());
    BSP_Fans_Set(30);
    printf("%i", BSP_Fans_GetDuty());
    BSP_Fans_Set(4);
    printf("%i", BSP_Fans_GetDuty());
    BSP_Fans_Off();
    printf("%i", BSP_Fans_GetDuty());*/
}