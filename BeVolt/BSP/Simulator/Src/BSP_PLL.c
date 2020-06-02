#include "BSP_PLL.h"
#include <stdint.h>
#include <stdlib.h>

static char* file = "BSP/Simulator/DataGeneration/Data/PLL.csv";

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void) {

    FILE* fp = fopen(file, "w");
    fprintf(fp, "%d", 16000000);
	fclose(fp);
}

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void) {
    uint32_t currentFreq;
    char str[20];
    FILE* fp = fopen(file, "r");
    fgets(str, 20, fp);
    fclose(fp);
    currentFreq = atoi(str);
    return currentFreq;   
}
