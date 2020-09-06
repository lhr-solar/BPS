/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"



int main() {
	
	OS_ERR err;

	Tasks_Init();

	OSStart(&err);

	// Should not get here or else there is an error
}

