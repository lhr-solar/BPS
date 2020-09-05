#include "BSP_Strobelight.h"
#include <stdint.h>
#include <stdlib.h>
#define STROBE_OFF 0
#define STROBE_PULSING 1

static const char* file = "BSP/Simulator/DataGeneration/Data/Strobe.csv";
int StrobeStatus = 0;

/**
 * @brief   Create text file if none exists (Simulator)
 * @param   None
 * @return  None
 */
void BSP_Strobe_Init(void) {
	FILE* fp = fopen(file, "w");
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
	fprintf(fp, "0");
	fclose(fp);
    flock(fno, LOCK_UN);
}

/**
 * @brief   Turns on Strobe (pulse once per second)
 * @param   None
 * @return  None
 */
void BSP_Strobe_On(void) {
    StrobeStatus = STROBE_PULSING;    //1 = pulsing
    FILE* fp = fopen(file, "w");
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%d", StrobeStatus);
    fclose(fp);
    flock(fno, LOCK_UN);
}

/**
 * @brief   Turns off Strobe
 * @param   None
 * @return  None
 */
void BSP_Strobe_Off(void) {
    StrobeStatus = STROBE_OFF;     //0 = off
    FILE* fp = fopen(file, "w");
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fprintf(fp, "%d", StrobeStatus);
    fclose(fp);
    flock(fno, LOCK_UN);
}
