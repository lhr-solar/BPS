#include "common.h"
#include "config.h"
#include "BSP_Strobelight.h"

static const char* file = "BSP/Simulator/DataGeneration/Data/Strobe.csv";
char a;
int main(){
BSP_Strobe_Init();
BSP_Strobe_On();
FILE* fp = fopen(file, "r");
a = getc(file);
fclose(file);
printf("After Strobe_On, current status is " + a);

BSP_Strobe_Off();
fp = fopen(file, "r");
a = getc(file);
fclose(file);
printf("After Strobe_Off, current status is " + a);

}