#include "BSP_PLL.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/file.h>
#include "simulator_conf.h"

static char* file = GET_CSV_PATH(PLL_CSV_FILE);

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void) {
    FILE* fp = fopen(file, "w");
    if(!fp) {
        perror(PLL_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    
    fprintf(fp, "%d", 80000000); 
    flock(fno, LOCK_UN);
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
    if(!fp) {
        perror(PLL_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fgets(str, 20, fp);
    flock(fno, LOCK_UN);
    fclose(fp);
    currentFreq = atoi(str);
    return currentFreq;   
}
