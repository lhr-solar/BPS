#include "../BSP/Inc/BSP_Strobelight.h"
#include <stdio.h>

static const char* file = "BSP/Simulator/DataGeneration/Data/Strobe.csv";
char a;
int  main() {
BSP_Strobe_Init();
BSP_Strobe_On();
FILE* fp = fopen(file, "r");
a = getc(fp);
fclose(fp);
printf("After Strobe_On, current status is %c", a);
printf(" ... ");

BSP_Strobe_Off();
fp = fopen(file, "r");
a = getc(fp);
fclose(fp);
printf("After Strobe_Off, current status is %c", a);

return(0);
}