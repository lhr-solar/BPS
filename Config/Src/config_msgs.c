/**
 * This is a C file because we will include it in compilation,
 * so all these messages are only executed once.
 * 
 * The general format is as follows:
 * 
 * #ifdef COMPILER_DEFINE_HERE
 * #pragma message ("XYZ WAS SPECIFIED BY COMPILER")
 * #endif
 * 
 * config.h will also have similar formatting.
 */

#ifdef CUSTOM_COMMAND_SIZE
#pragma message ("Custom command size set!")
#endif

#ifdef CUSTOM_MIN_VOLT_CHARGE
#pragma message ("Custom minimum voltage charge limit set!")
#endif

#ifdef CUSTOM_MIN_VOLT
#pragma message ("Custom minimum voltage set!")
#endif

#ifdef CUSTOM_MAX_VOLT
#pragma message ("Custom maximum voltage set!")
#endif

#ifdef CUSTOM_CHARGE_DIS_VOLT
#pragma message ("Custom charge disable voltage set!")
#endif

#ifdef CUSTOM_MAX_DISCHARGE_TEMP
#pragma message ("Custom maximum discharge temperature set!")
#endif

#ifdef CUSTOM_MAX_CHARGE_TEMP
#pragma message ("Custom maximum charge temperature set!")
#endif

#ifdef CUSTOM_MAX_CURRENT
#pragma message ("Custom maximum current set!")
#endif

#ifdef CUSTOM_MAX_CHARGE_CURRENT
#pragma message ("Custom charge current set!")
#endif

#ifdef CUSTOM_BALANCE_TOLERANCE_START
#pragma message ("Custom balance tolerance start set!")
#endif

#ifdef CUSTOM_BALANCE_TOLERANCE_STOP
#pragma message ("Custom balance tolerance stop set!")
#endif

// I don't think the other #defines in config.h should be editable