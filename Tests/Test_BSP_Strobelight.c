#include "BSP_Strobelight.h"
#include <stdio.h>
#include "BSP_WDTimer.h"


int  main(){
    char strobeIn;
    BSP_WDTimer_Init();

    BSP_Strobe_Init();
    printf("Strobe Control : Enter 'p' for pulsing or 'o' for off\n");
    while(1){
        strobeIn = getchar();
        if(strobeIn==('p')){
            BSP_Strobe_On();
            printf("Strobe = Pulsing\n>> ");
        }
        if(strobeIn=='o'){
            BSP_Strobe_Off();
            printf("Strobe = Off\n>> ");
        }
    }
    return(0);
}

