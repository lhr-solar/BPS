/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"

int main() {

	OSStart();
	
	Tasks_Init();
}

