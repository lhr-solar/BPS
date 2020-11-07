#include "common.h"
#include "config.h"
#include <stdio.h>
#include "BSP_Fans.h"

int main()
{
    BSP_Fans_Init();
    printf("Fans.csv created\n");
    printf("%i\n", BSP_Fans_Set(2,7));
    printf("This is test 1\n");
    printf("The speed of fan 2 is %d\n", BSP_Fans_GetSpeed(2));
    printf("%i\n", BSP_Fans_Set(2,9));
    printf("This is test 2\n");
    printf("The speed of fan 2 is %d\n", BSP_Fans_GetSpeed(2));
    printf("%i\n", BSP_Fans_Set(5,7));
    BSP_Fans_Set(1,2);
    BSP_Fans_Set(2,3);
    BSP_Fans_Set(3,4);
    BSP_Fans_Set(4,5);
    printf("This is test 3\n");
    printf("The speed of fan 1 is %d\n", BSP_Fans_GetSpeed(1));
    printf("The speed of fan 2 is %d\n", BSP_Fans_GetSpeed(2));
    printf("The speed of fan 3 is %d\n", BSP_Fans_GetSpeed(3));
    printf("The speed of fan 4 is %d\n", BSP_Fans_GetSpeed(4));
    printf("%i\n", BSP_Fans_Set(5,9));
    printf("This is test 4\n");
    printf("The speed of fan 1 is %d\n", BSP_Fans_GetSpeed(1));
    printf("The speed of fan 2 is %d\n", BSP_Fans_GetSpeed(2));
    printf("The speed of fan 3 is %d\n", BSP_Fans_GetSpeed(3));
    printf("The speed of fan 4 is %d\n", BSP_Fans_GetSpeed(4));
    while(1);
}
